/* WBA55 standby experiment. Keep the working STOP-only build separate. */
#include <stdint.h>
#include <limits.h>
#include <errno.h>

#include <zephyr/kernel.h>
#include <zephyr/init.h>
#include <zephyr/pm/policy.h>
#include <zephyr/sys/atomic.h>
#include <zephyr/sys/printk.h>
#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/bluetooth/hci.h>
#if defined(CONFIG_PRINTK)
#include <zephyr/bluetooth/hci.h>
#include <zephyr/bluetooth/conn.h>
#include <zephyr/sys/byteorder.h>

static struct bt_conn *tx_power_conn;
static void read_connection_tx_power(struct k_work *work)
{
    ARG_UNUSED(work);
    struct bt_conn *conn = tx_power_conn;
    uint16_t handle;
    struct net_buf *rsp = NULL;
    int err = bt_hci_get_conn_handle(conn, &handle);
    if (!err) {
        struct net_buf *cmd = bt_hci_cmd_alloc(K_NO_WAIT);
        if (!cmd) {
            err = -ENOBUFS;
        } else {
            struct bt_hci_cp_read_tx_power_level *cp = net_buf_add(cmd, sizeof(*cp));
            cp->handle = sys_cpu_to_le16(handle);
            cp->type = BT_TX_POWER_LEVEL_CURRENT;
            err = bt_hci_cmd_send_sync(BT_HCI_OP_READ_TX_POWER_LEVEL, cmd, &rsp);
        }
    }
    if (!err && rsp && rsp->len >= sizeof(struct bt_hci_rp_read_tx_power_level)) {
        const struct bt_hci_rp_read_tx_power_level *rp = (const void *)rsp->data;
        printk("WBA TX: connection status=%u current=%d dBm\n", rp->status, rp->tx_power_level);
    } else {
        printk("WBA TX: connection read failed err=%d\n", err);
    }
    if (rsp) net_buf_unref(rsp);
    bt_conn_unref(conn);
    tx_power_conn = NULL;
}
K_WORK_DEFINE(tx_power_work, read_connection_tx_power);

static void read_advertising_tx_power(void)
{
    struct net_buf *rsp = NULL;
    int err = bt_hci_cmd_send_sync(BT_HCI_OP_LE_READ_ADV_CHAN_TX_POWER, NULL, &rsp);
    if (!err && rsp && rsp->len >= sizeof(struct bt_hci_rp_le_read_chan_tx_power)) {
        const struct bt_hci_rp_le_read_chan_tx_power *rp = (const void *)rsp->data;
        printk("WBA TX: advertising status=%u current=%d dBm\n", rp->status, rp->tx_power_level);
    } else {
        printk("WBA TX: advertising read failed err=%d\n", err);
    }
    if (rsp) net_buf_unref(rsp);
}
#endif


static atomic_t controller_ready;
static atomic_t invalid_reported;
static atomic_t connection_standby_lock;
static void permit_standby(struct k_work *work)
{
    ARG_UNUSED(work);
    pm_policy_state_lock_put(PM_STATE_SUSPEND_TO_RAM, PM_ALL_SUBSTATES);
}
K_WORK_DELAYABLE_DEFINE(standby_start, permit_standby);
static int block_startup_standby(void)
{
    pm_policy_state_lock_get(PM_STATE_SUSPEND_TO_RAM, PM_ALL_SUBSTATES);
    return 0;
}
SYS_INIT(block_startup_standby, APPLICATION, 0);
int __real_bt_enable(bt_ready_cb_t cb);
int __wrap_bt_enable(bt_ready_cb_t cb)
{
    /* Benchmark uses synchronous bt_enable(NULL). */
    int err = __real_bt_enable(cb);
    if (err == 0 && cb == NULL) {
        /* ST ACI_HAL_SET_TX_POWER_LEVEL, PA_Level 0x19: nominal 0 dBm
         * (-0.3 dBm typical in the installed maximum-power table).
         * Submit via HCI so the driver serializes controller access. */
        printk("WBA TX: sending nominal 0 dBm request\n");
        struct net_buf *cmd = bt_hci_cmd_alloc(K_NO_WAIT);
        if (!cmd) { return -ENOBUFS; }
        net_buf_add_u8(cmd, 0);    /* En_High_Power: deprecated */
        net_buf_add_u8(cmd, 0x19); /* PA_Level, not signed dBm */
        err = bt_hci_cmd_send_sync(0xFC0F, cmd, NULL);
        printk("WBA TX: nominal 0 dBm request err=%d\n", err);
        if (err) { return err; }
        printk("WBA PM: connected standby %s\n",
               IS_ENABLED(BLUEJOULE_WBA55_CONNECTED_STANDBY) ? "ALLOWED" : "BLOCKED");
        atomic_set(&controller_ready, 1);
#if defined(CONFIG_PRINTK)
        read_advertising_tx_power();
#endif
        k_work_schedule(&standby_start, K_SECONDS(5));
    }
    return err;
}
#ifdef BLUEJOULE_WBA55_PM_TRACE
void wba_pm_trace_policy(int64_t ticks);
#endif
int64_t __real_pm_policy_next_custom_ticks(void);
int64_t __wrap_pm_policy_next_custom_ticks(void)
{
    if (!atomic_get(&controller_ready)) { return 0; }
    int64_t ticks = __real_pm_policy_next_custom_ticks();
#ifdef BLUEJOULE_WBA55_PM_TRACE
    wba_pm_trace_policy(ticks);
#endif
    /* Inhibit sleep on invalid values; never truncate a wide deadline. */
    if (ticks < -1 || ticks > INT32_MAX) {
        if (atomic_cas(&invalid_reported, 0, 1)) {
            printk("WBA55 PM: sleep blocked, invalid ticks hi=%08x lo=%08x\n",
                   (unsigned int)((uint64_t)ticks >> 32), (unsigned int)ticks);
        }
        return 0;
    }
    atomic_clear(&invalid_reported);
    return ticks;
}

/* ST blob returns UINT64_MAX for no software timers; Zephyr's WBA
 * integration expects LL_DP_SLP_NO_WAKEUP, defined as UINT32_MAX.
 * Normalize only that exact sentinel, preserving all real deadlines. */
uint64_t __real_os_timer_get_earliest_time(void);
uint64_t __wrap_os_timer_get_earliest_time(void)
{
    uint64_t us = __real_os_timer_get_earliest_time();
    return us == UINT64_MAX ? (uint64_t)UINT32_MAX : us;
}

/* Serial-only timing evidence; PRINTK is disabled in the power image. */
#include <zephyr/bluetooth/conn.h>
int __real_bt_le_adv_start(const struct bt_le_adv_param *param,
                          const struct bt_data *ad, size_t ad_len,
                          const struct bt_data *sd, size_t sd_len);
int __wrap_bt_le_adv_start(const struct bt_le_adv_param *param,
                          const struct bt_data *ad, size_t ad_len,
                          const struct bt_data *sd, size_t sd_len)
{
    int64_t before = k_uptime_get();
    /* Advertising is outside the scored transaction. Fix the requested
     * interval at 30 ms so ST does not select 60 ms from a range.
     * The controller still applies the BLE advertising random delay. */
    struct bt_le_adv_param fixed_param = *param;
    fixed_param.interval_min = 0x0030; /* 48 * 0.625 ms = 30 ms */
    fixed_param.interval_max = 0x0030;
    int err = __real_bt_le_adv_start(&fixed_param, ad, ad_len, sd, sd_len);
    printk("WBA timing: ADV start uptime=%lld done=%lld err=%d\n",
           (long long)before, (long long)k_uptime_get(), err);
    return err;
}
#ifdef BLUEJOULE_WBA55_PM_TRACE
void wba_pm_trace_begin(void);
void wba_pm_trace_end(void);
#endif
static void timing_connected(struct bt_conn *conn, uint8_t err)
{
#ifdef BLUEJOULE_WBA55_PM_TRACE
    if (!err) wba_pm_trace_begin();
#endif
    if (!IS_ENABLED(BLUEJOULE_WBA55_CONNECTED_STANDBY) &&
        err == 0 && atomic_cas(&connection_standby_lock, 0, 1)) {
        pm_policy_state_lock_get(PM_STATE_SUSPEND_TO_RAM, PM_ALL_SUBSTATES);
    }
    ARG_UNUSED(conn);
#if defined(CONFIG_PRINTK)
    if (!err && !tx_power_conn) {
        tx_power_conn = bt_conn_ref(conn);
        k_work_submit(&tx_power_work);
    }
#endif
    printk("WBA timing: CONNECT uptime=%lld err=%u\n",
           (long long)k_uptime_get(), err);
}
static void timing_disconnected(struct bt_conn *conn, uint8_t reason)
{
#ifdef BLUEJOULE_WBA55_PM_TRACE
    wba_pm_trace_end();
#endif
    if (atomic_cas(&connection_standby_lock, 1, 0)) {
        pm_policy_state_lock_put(PM_STATE_SUSPEND_TO_RAM, PM_ALL_SUBSTATES);
    }
    ARG_UNUSED(conn);
    printk("WBA timing: DISCONNECT uptime=%lld reason=%u\n",
           (long long)k_uptime_get(), reason);
}
BT_CONN_CB_DEFINE(wba_timing_callbacks) = {
    .connected = timing_connected,
    .disconnected = timing_disconnected,
};
