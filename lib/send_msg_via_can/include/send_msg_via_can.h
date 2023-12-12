#include <drivers/can.h> 
#include <sys/printk.h>

void tx_callback_function(uint32_t ret_val, char *info);
void send_msg_via_can(const struct device *can_dev, struct zcan_frame *frame, bool is_blocking);