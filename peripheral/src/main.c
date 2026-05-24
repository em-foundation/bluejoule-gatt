/* main.c - BlueJoule BLE GATT-RW v1 benchmark peripheral */

#include <stdint.h>
#include <stddef.h>

#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>

#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/bluetooth/conn.h>
#include <zephyr/bluetooth/gap.h>
#include <zephyr/bluetooth/gatt.h>
#include <zephyr/bluetooth/hci.h>
#include <zephyr/bluetooth/uuid.h>

#include <zephyr/drivers/gpio.h>

static const struct gpio_dt_spec bj_mark = {
    .port = DEVICE_DT_GET(DT_NODELABEL(gpio0)),
    .pin = 4,
    .dt_flags = GPIO_ACTIVE_LOW
};

#define DEVICE_NAME CONFIG_BT_DEVICE_NAME
#define DEVICE_NAME_LEN (sizeof(DEVICE_NAME) - 1)

#define BT_UUID_BJ_SVC_VAL \
    BT_UUID_128_ENCODE(0x0000b100, 0x0000, 0x1000, 0x8000, 0x00805f9b34fb)

#define BT_UUID_BJ_STATUS_CHR_VAL \
    BT_UUID_128_ENCODE(0x0000b101, 0x0000, 0x1000, 0x8000, 0x00805f9b34fb)

#define BT_UUID_BJ_COMMAND_CHR_VAL \
    BT_UUID_128_ENCODE(0x0000b102, 0x0000, 0x1000, 0x8000, 0x00805f9b34fb)

static struct bt_uuid_128 bj_svc_uuid = BT_UUID_INIT_128(BT_UUID_BJ_SVC_VAL);
static struct bt_uuid_128 bj_status_chr_uuid = BT_UUID_INIT_128(BT_UUID_BJ_STATUS_CHR_VAL);
static struct bt_uuid_128 bj_command_chr_uuid = BT_UUID_INIT_128(BT_UUID_BJ_COMMAND_CHR_VAL);

static uint8_t bj_status_value = 0x80;
static uint8_t bj_command_value;

static const struct bt_data ad[] = {
    BT_DATA_BYTES(BT_DATA_FLAGS, BT_LE_AD_GENERAL | BT_LE_AD_NO_BREDR),
    BT_DATA_BYTES(BT_DATA_UUID128_ALL, BT_UUID_BJ_SVC_VAL),
};

static const struct bt_data sd[] = {
    BT_DATA(BT_DATA_NAME_COMPLETE, DEVICE_NAME, DEVICE_NAME_LEN),
};

/* Advertise on channel 37 only, which makes Nordic Sniffer captures easier. */
static const struct bt_le_adv_param adv_param =
    BT_LE_ADV_PARAM_INIT(BT_LE_ADV_OPT_CONN |
                         BT_LE_ADV_OPT_DISABLE_CHAN_38 |
                         BT_LE_ADV_OPT_DISABLE_CHAN_39,
                         BT_GAP_ADV_FAST_INT_MIN_1,
                         BT_GAP_ADV_FAST_INT_MAX_1,
                         NULL);

static void bj_mark_pulse(void)
{
    gpio_pin_set_dt(&bj_mark, 1);
    k_busy_wait(10);
    gpio_pin_set_dt(&bj_mark, 0);
}

static ssize_t read_status_chr(struct bt_conn *conn,
                               const struct bt_gatt_attr *attr,
                               void *buf,
                               uint16_t len,
                               uint16_t offset)
{
    const uint8_t *value = attr->user_data;

    return bt_gatt_attr_read(conn, attr, buf, len, offset, value, sizeof(*value));
}

static ssize_t write_command_chr(struct bt_conn *conn,
                                 const struct bt_gatt_attr *attr,
                                 const void *buf,
                                 uint16_t len,
                                 uint16_t offset,
                                 uint8_t flags)
{
    if (offset != 0) {
        return BT_GATT_ERR(BT_ATT_ERR_INVALID_OFFSET);
    }

    if (len > 0) {
        bj_command_value = ((const uint8_t *)buf)[0];
        bj_status_value = bj_command_value | 0x80;
    }

    return len;
}

BT_GATT_SERVICE_DEFINE(bj_svc,
    BT_GATT_PRIMARY_SERVICE(&bj_svc_uuid.uuid),

    BT_GATT_CHARACTERISTIC(&bj_status_chr_uuid.uuid,
                           BT_GATT_CHRC_READ,
                           BT_GATT_PERM_READ,
                           read_status_chr, NULL, &bj_status_value),

    BT_GATT_CHARACTERISTIC(&bj_command_chr_uuid.uuid,
                           BT_GATT_CHRC_WRITE | BT_GATT_CHRC_WRITE_WITHOUT_RESP,
                           BT_GATT_PERM_WRITE,
                           NULL, write_command_chr, NULL),
);

static void connected(struct bt_conn *conn, uint8_t err)
{
    if (err) {
        printk("Connection failed, err 0x%02x %s\n", err, bt_hci_err_to_str(err));
        return;
    }

    bj_mark_pulse();
}

static void disconnected(struct bt_conn *conn, uint8_t reason)
{
    bj_mark_pulse();

    printk("Disconnected, reason 0x%02x %s, command 0x%02x, status 0x%02x\n",
           reason, bt_hci_err_to_str(reason), bj_command_value, bj_status_value);
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

    if (!device_is_ready(bj_mark.port)) {
        printk("BJ marker GPIO not ready\n");
        return 0;
    }

    err = gpio_pin_configure_dt(&bj_mark, GPIO_OUTPUT_INACTIVE);
    if (err) {
        printk("BJ marker GPIO config failed (err %d)\n", err);
        return 0;
    }

    err = bt_le_adv_start(&adv_param, ad, ARRAY_SIZE(ad), sd, ARRAY_SIZE(sd));
    if (err) {
        printk("Advertising failed to start (err %d)\n", err);
        return 0;
    }

    printk("Advertising successfully started as %s\n", DEVICE_NAME);

    while (1) {
        k_sleep(K_FOREVER);
    }

    return 0;
}
