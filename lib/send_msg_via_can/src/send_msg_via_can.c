#include "send_msg_via_can.h"

void tx_callback_function(uint32_t ret_val, char *info)
{
    if (ret_val != 0) {
        printk("TX Callback: From %s \t Error Code: %d\n", info, ret_val);
    }
}

int send_msg_via_can(const struct device *can_dev, struct zcan_frame *frame, bool is_blocking)
{
    int ret;
    if (is_blocking) {
        ret = can_send(can_dev, frame, K_MSEC(100), NULL, NULL);
        if (ret!=0) {
            printk("can_send failed, error code: %d\n", ret);
        }
    }
    else {
        ret = 0;
        can_send(can_dev, frame, K_FOREVER, tx_callback_function, "LED change");
    }
    return ret;
}