#include <zephyr.h>
#include <kernel.h>
#include <sys/printk.h>
#include <device.h>
#include <drivers/can.h>
#include <drivers/gpio.h>
#include <sys/byteorder.h>

struct zcan_frame frame = {
        .id_type = CAN_STANDARD_IDENTIFIER,
        .rtr = CAN_DATAFRAME,
        .id = 0x123, // Matches to identifier 0x123
        .dlc = 8, //length of the message
        .data = {1,2,3,4,5,6,7,8} // uint8_t payload
};


const struct zcan_filter my_filter = {
        .id_type = CAN_STANDARD_IDENTIFIER,
        .rtr = CAN_DATAFRAME,
        .id = 0x123,
        .rtr_mask = 1,
        .id_mask = CAN_STD_ID_MASK
        //Combo of the id_type and id mask is used to match what you're looking for
        //Masking is bitwise operations. AND operation of bits it doesn't want to look at (sets bits to 0). 
};

const struct device *can_dev;







void send_msg_via_can(struct zcan_frame *frame)
{
    int ret;
    ret = can_send(can_dev, frame, K_MSEC(100), NULL, NULL);
    if (ret!=0) {
        printk("can_send failed, error code: %d\n", ret);
    }

}
// TODO: Define a GPIO device
// TODO: Define a CAN message



// Below are functions from zephyr docs
void rx_callback_function(struct zcan_frame *frame, void *arg)
{
    int filter_id;

    // ... do something with the frame ...
}



void main(void)
{
    can_dev = device_get_binding("CAN_0");

    // TODO: Set a CAN device to do loopback mode
    can_set_mode(&can_dev, CAN_LOOPBACK_MODE); //returns an int

    send_msg_via_can(&frame);
    int filter_id;
    filter_id = can_attach_isr(can_dev, rx_callback_function, NULL, &my_filter); // Placing NULL for the callback function for now

}