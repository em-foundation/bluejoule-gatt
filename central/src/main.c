/* main.c - BlueJoule BLE GATT-RW v1 benchmark central */

#include <zephyr/types.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>

#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>

#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/bluetooth/hci.h>
#include <zephyr/bluetooth/conn.h>
#include <zephyr/bluetooth/gatt.h>
#include <zephyr/bluetooth/uuid.h>

#define BT_UUID_BJ_SVC_VAL \
    BT_UUID_128_ENCODE(0x0000b100, 0x0000, 0x1000, 0x8000, 0x00805f9b34fb)

#define BT_UUID_BJ_STATUS_CHR_VAL \
    BT_UUID_128_ENCODE(0x0000b101, 0x0000, 0x1000, 0x8000, 0x00805f9b34fb)

#define BT_UUID_BJ_COMMAND_CHR_VAL \
    BT_UUID_128_ENCODE(0x0000b102, 0x0000, 0x1000, 0x8000, 0x00805f9b34fb)

#define BJ_CONN_INTERVAL_MIN 6
#define BJ_CONN_INTERVAL_MAX 6
#define BJ_CONN_LATENCY 0
#define BJ_CONN_TIMEOUT 400

#define BJ_COMMAND_TEST_VALUE 0x04
#define BJ_STATUS_EXPECTED_VALUE 0x84

enum bench_phase {
    BENCH_DISC_PRIMARY,
    BENCH_DISC_CHARACTERISTIC,
    BENCH_WRITE_COMMAND,
    BENCH_READ_STATUS,
    BENCH_DONE,
};

static void start_scan(void);
static void start_discovery(void);
static void start_write_command(void);
static void start_read_status(void);
static void disconnect_now(void);

static uint8_t discover_func(struct bt_conn *conn, const struct bt_gatt_attr *attr,
                             struct bt_gatt_discover_params *params);
static uint8_t read_status_func(struct bt_conn *conn, uint8_t err,
                                struct bt_gatt_read_params *params,
                                const void *data, uint16_t length);
static void write_command_func(struct bt_conn *conn, uint8_t err,
                               struct bt_gatt_write_params *params);

static struct bt_conn *default_conn;
static struct bt_gatt_discover_params discover_params;
static struct bt_gatt_read_params read_params;
static struct bt_gatt_write_params write_params;

static bool done;
static bool passed;
static bool status_seen;
static enum bench_phase phase = BENCH_DISC_PRIMARY;

static struct bt_uuid_128 bj_svc_uuid = BT_UUID_INIT_128(BT_UUID_BJ_SVC_VAL);
static struct bt_uuid_128 bj_status_chr_uuid = BT_UUID_INIT_128(BT_UUID_BJ_STATUS_CHR_VAL);
static struct bt_uuid_128 bj_command_chr_uuid = BT_UUID_INIT_128(BT_UUID_BJ_COMMAND_CHR_VAL);

static const uint8_t bj_svc_ad_uuid[] = { BT_UUID_BJ_SVC_VAL };

static const struct bt_le_conn_param bj_conn_param = {
    .interval_min = BJ_CONN_INTERVAL_MIN,
    .interval_max = BJ_CONN_INTERVAL_MAX,
    .latency = BJ_CONN_LATENCY,
    .timeout = BJ_CONN_TIMEOUT,
};

static uint16_t bj_service_start;
static uint16_t bj_service_end;
static uint16_t bj_status_handle;
static uint16_t bj_command_handle;

static uint8_t bj_command_value[] = { BJ_COMMAND_TEST_VALUE };
static uint8_t bj_status_value;

static void reset_benchmark_state(void)
{
    phase = BENCH_DISC_PRIMARY;
    passed = false;
    status_seen = false;
    bj_status_value = 0;
    bj_service_start = 0;
    bj_service_end = 0;
    bj_status_handle = 0;
    bj_command_handle = 0;
}

static void disconnect_now(void)
{
    int err;

    if (!default_conn) {
        done = true;
        return;
    }

    err = bt_conn_disconnect(default_conn, BT_HCI_ERR_REMOTE_USER_TERM_CONN);
    if (err) {
        printk("Disconnect failed (err %d)\n", err);
        done = true;
    }
}

static bool ad_parse_func(struct bt_data *data, void *user_data)
{
    bool *found = user_data;

    if (data->type != BT_DATA_UUID128_SOME &&
        data->type != BT_DATA_UUID128_ALL) {
        return true;
    }

    for (uint8_t i = 0; i + sizeof(bj_svc_ad_uuid) <= data->data_len;
         i += sizeof(bj_svc_ad_uuid)) {
        if (!memcmp(&data->data[i], bj_svc_ad_uuid, sizeof(bj_svc_ad_uuid))) {
            *found = true;
            return false;
        }
    }

    return true;
}

static bool adv_has_bj_service(struct net_buf_simple *ad)
{
    bool found = false;

    bt_data_parse(ad, ad_parse_func, &found);
    return found;
}

static void start_discovery(void)
{
    int err;

    (void)memset(&discover_params, 0, sizeof(discover_params));
    discover_params.func = discover_func;

    if (phase == BENCH_DISC_PRIMARY) {
        discover_params.uuid = &bj_svc_uuid.uuid;
        discover_params.type = BT_GATT_DISCOVER_PRIMARY;
        discover_params.start_handle = BT_ATT_FIRST_ATTRIBUTE_HANDLE;
        discover_params.end_handle = BT_ATT_LAST_ATTRIBUTE_HANDLE;
    } else if (phase == BENCH_DISC_CHARACTERISTIC) {
        discover_params.uuid = NULL;
        discover_params.type = BT_GATT_DISCOVER_CHARACTERISTIC;
        discover_params.start_handle = bj_service_start + 1;
        discover_params.end_handle = bj_service_end;
    } else {
        disconnect_now();
        return;
    }

    err = bt_gatt_discover(default_conn, &discover_params);
    if (err) {
        printk("GATT discovery failed to start (err %d)\n", err);
        disconnect_now();
    }
}

static uint8_t discover_func(struct bt_conn *conn, const struct bt_gatt_attr *attr,
                             struct bt_gatt_discover_params *params)
{
    if (!attr) {
        if (phase == BENCH_DISC_PRIMARY) {
            if (!bj_service_start || !bj_service_end) {
                printk("BJ primary service not found\n");
                disconnect_now();
                return BT_GATT_ITER_STOP;
            }

            phase = BENCH_DISC_CHARACTERISTIC;
            start_discovery();
            return BT_GATT_ITER_STOP;
        }

        if (phase == BENCH_DISC_CHARACTERISTIC) {
            if (!bj_status_handle || !bj_command_handle) {
                printk("BJ characteristics incomplete: status 0x%04x command 0x%04x\n",
                       bj_status_handle, bj_command_handle);
                disconnect_now();
                return BT_GATT_ITER_STOP;
            }

            phase = BENCH_WRITE_COMMAND;
            start_write_command();
            return BT_GATT_ITER_STOP;
        }

        disconnect_now();
        return BT_GATT_ITER_STOP;
    }

    if (phase == BENCH_DISC_PRIMARY) {
        struct bt_gatt_service_val *svc = attr->user_data;

        bj_service_start = attr->handle;
        bj_service_end = svc->end_handle;
        return BT_GATT_ITER_CONTINUE;
    }

    if (phase == BENCH_DISC_CHARACTERISTIC) {
        struct bt_gatt_chrc *chrc = attr->user_data;

        if (!bt_uuid_cmp(chrc->uuid, &bj_status_chr_uuid.uuid)) {
            bj_status_handle = chrc->value_handle;
        } else if (!bt_uuid_cmp(chrc->uuid, &bj_command_chr_uuid.uuid)) {
            bj_command_handle = chrc->value_handle;
        }
    }

    return BT_GATT_ITER_CONTINUE;
}

static void write_command_func(struct bt_conn *conn, uint8_t err,
                               struct bt_gatt_write_params *params)
{
    if (err) {
        printk("BJ command write failed (err 0x%02x)\n", err);
        disconnect_now();
        return;
    }

    phase = BENCH_READ_STATUS;
    start_read_status();
}

static void start_write_command(void)
{
    int err;

    (void)memset(&write_params, 0, sizeof(write_params));

    write_params.func = write_command_func;
    write_params.handle = bj_command_handle;
    write_params.offset = 0;
    write_params.data = bj_command_value;
    write_params.length = sizeof(bj_command_value);

    err = bt_gatt_write(default_conn, &write_params);
    if (err) {
        printk("BJ command write failed to start (err %d)\n", err);
        disconnect_now();
    }
}

static uint8_t read_status_func(struct bt_conn *conn, uint8_t err,
                                struct bt_gatt_read_params *params,
                                const void *data, uint16_t length)
{
    if (err) {
        printk("BJ status read failed (err 0x%02x)\n", err);
        disconnect_now();
        return BT_GATT_ITER_STOP;
    }

    if (data) {
        if (length > 0) {
            const uint8_t *bytes = data;

            bj_status_value = bytes[0];
            status_seen = true;
        }

        return BT_GATT_ITER_CONTINUE;
    }

    passed = status_seen && bj_status_value == BJ_STATUS_EXPECTED_VALUE;
    phase = BENCH_DONE;
    disconnect_now();

    return BT_GATT_ITER_STOP;
}

static void start_read_status(void)
{
    int err;

    (void)memset(&read_params, 0, sizeof(read_params));

    read_params.func = read_status_func;
    read_params.handle_count = 1;
    read_params.single.handle = bj_status_handle;
    read_params.single.offset = 0;

    err = bt_gatt_read(default_conn, &read_params);
    if (err) {
        printk("BJ status read failed to start (err %d)\n", err);
        disconnect_now();
    }
}

static void device_found(const bt_addr_le_t *addr, int8_t rssi, uint8_t type,
                         struct net_buf_simple *ad)
{
    char addr_str[BT_ADDR_LE_STR_LEN];
    int err;

    if (default_conn || done) {
        return;
    }

    if (type != BT_GAP_ADV_TYPE_ADV_IND &&
        type != BT_GAP_ADV_TYPE_ADV_DIRECT_IND) {
        return;
    }

    if (!adv_has_bj_service(ad)) {
        return;
    }

    bt_addr_le_to_str(addr, addr_str, sizeof(addr_str));

    err = bt_le_scan_stop();
    if (err) {
        printk("Stop scan failed (err %d)\n", err);
        done = true;
        return;
    }

    err = bt_conn_le_create(addr, BT_CONN_LE_CREATE_CONN, &bj_conn_param, &default_conn);
    if (err) {
        printk("Create conn to %s failed (err %d)\n", addr_str, err);
        done = true;
    }
}

static void start_scan(void)
{
    int err;

    err = bt_le_scan_start(BT_LE_SCAN_PASSIVE, device_found);
    if (err) {
        printk("Scanning failed to start (err %d)\n", err);
        done = true;
        return;
    }

    printk("Scanning started\n");
}

static void connected(struct bt_conn *conn, uint8_t err)
{
    char addr[BT_ADDR_LE_STR_LEN];

    bt_addr_le_to_str(bt_conn_get_dst(conn), addr, sizeof(addr));

    if (err) {
        printk("Failed to connect to %s, err 0x%02x %s\n",
               addr, err, bt_hci_err_to_str(err));

        if (default_conn) {
            bt_conn_unref(default_conn);
            default_conn = NULL;
        }

        done = true;
        return;
    }

    if (conn != default_conn) {
        return;
    }

    reset_benchmark_state();
    start_discovery();
}

static void disconnected(struct bt_conn *conn, uint8_t reason)
{
    char addr[BT_ADDR_LE_STR_LEN];

    if (conn != default_conn) {
        return;
    }

    bt_addr_le_to_str(bt_conn_get_dst(conn), addr, sizeof(addr));

    printk("Disconnected: %s, reason 0x%02x %s\n",
           addr, reason, bt_hci_err_to_str(reason));
    printk("BJ result: %s, service 0x%04x-0x%04x, status 0x%04x, command 0x%04x, value 0x%02x\n",
           passed ? "PASS" : "FAIL",
           bj_service_start,
           bj_service_end,
           bj_status_handle,
           bj_command_handle,
           bj_status_value);

    bt_conn_unref(default_conn);
    default_conn = NULL;
    done = true;
}

BT_CONN_CB_DEFINE(conn_callbacks) = {
    .connected = connected,
    .disconnected = disconnected,
};

int main(void)
{
    int err;

    err = bt_enable(NULL);
    if (err) {
        printk("Bluetooth init failed (err %d)\n", err);
        return 0;
    }

    printk("Bluetooth initialized\n");

    start_scan();

    while (!done) {
        k_sleep(K_MSEC(100));
    }

    printk("Done\n");

    return 0;
}
