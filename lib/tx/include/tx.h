// lib/tx/src/tx.c

#include <zephyr.h>
#include <sys/printk.h>

void tx_callback_function(uint32_t ret_val, char *info)
{
    if (ret_val != 0) {
        printk("TX Callback: From %s \t Error Code: %d\n", info, ret_val);
    }
}