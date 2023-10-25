#include <zephyr.h>
#include <kernel.h>
#include <sys/printk.h>
#include <device.h>
#include <drivers/can.h>
#include <drivers/gpio.h>
#include <sys/byteorder.h>

#define ID1 0x123
#define ID2 0x234

const struct zcan_filter my_filter = {
        .id_type = CAN_STANDARD_IDENTIFIER,
        .rtr = CAN_DATAFRAME,
        .id = 0x123,
        .rtr_mask = 1,
        .id_mask = CAN_STD_ID_MASK // set to look at all incoming frames
        //Combo of the id_type and id mask is used to match what you're looking for
        //Masking is bitwise operations to only print selections that you want from all available. AND operation of bits it doesn't want to look at (sets bits to 0). 
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

// }
// TODO: Define a GPIO device
// TODO: Define a CAN message



// Below are functions from zephyr docs
void rx_callback_function(struct zcan_frame *frame, void *arg)
{
    
    // ... do something with the frame ...
    printk("Rx Callback Function\n");
    switch (frame->data[0])
    {
        case 1:
            // set gpio pin
            break;
        case 2:
            // set gpio pin
            break;
    }
}



void main(void)
{
    can_dev = device_get_binding("CAN_0");

    const struct zcan_frame frame1 = 
    {
        .id_type = CAN_STANDARD_IDENTIFIER,
        .rtr = CAN_DATAFRAME,
        .id = ID1, // Matches to identifier 0x123
        .dlc = 8, //length of the message
        .data = {1,2,3,4,5,6,7,8} // uint8_t payload
    };
    const struct zcan_frame frame2 = 
    {
        .id_type = CAN_STANDARD_IDENTIFIER,
        .rtr = CAN_DATAFRAME,
        .id = ID2, // Matches to identifier 0x123
        .dlc = 8, //length of the message
        .data = {1,3,5,7,9,11,13,15} // uint8_t payload
    };
    
    // Set a CAN device to do loopback mode
    can_set_mode(&can_dev, CAN_LOOPBACK_MODE); //returns an int

    // Configure GPIO HW pins


    send_msg_via_can(&frame1);
    send_msg_via_can(&frame2);
    
    int filter_id = can_attach_isr(can_dev, rx_callback_function, NULL, &my_filter); // Placing NULL for the callback function for now

}