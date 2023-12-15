#include <zephyr.h>
#include <kernel.h>
#include <sys/printk.h>
#include "send_msg_via_can.h"
#include "rx_callback_function.h"


#define LED_MSG_ID 0x01
#define COUNTER_MSG_ID 0x00
#define ID1 0x123
#define ID2 0x234
#define SLEEP_TIME_LEN 1000
#define RX_THREAD_STACK_SIZE 512
#define RX_THREAD_PRIORITY 2

//Define Globals
struct gpio_dt_spec led = GPIO_DT_SPEC_GET_OR(DT_ALIAS(led0), gpios, {0});
const struct device *can_dev = DEVICE_DT_GET(DT_CHOSEN(zephyr_can_primary));
struct zcan_frame led_frame = 
{
    .id_type = CAN_STANDARD_IDENTIFIER,
    .rtr = CAN_DATAFRAME,
    .id = LED_MSG_ID,
    .dlc = 1U,
    .data = {RESET_LED}
};

struct zcan_frame counter_frame = 
{
    .id_type = CAN_EXTENDED_IDENTIFIER,
    .rtr = CAN_DATAFRAME,
    .id = COUNTER_MSG_ID,
    .dlc = 2U,
    .data = {0x00, 0x00}
};

const struct zcan_filter led_filter = {
        .id_type = CAN_STANDARD_IDENTIFIER,
        .rtr = CAN_DATAFRAME,
        .id = LED_MSG_ID,
        .rtr_mask = 1,
        .id_mask = CAN_STD_ID_MASK // set to look at all incoming frames
        //Combo of the id_type and id mask is used to match what you're looking for
        //Masking is bitwise operations to only print selections that you want from all available. AND operation of bits it doesn't want to look at (sets bits to 0). 
};

const struct zcan_filter counter_filter = {
        .id_type = CAN_EXTENDED_IDENTIFIER,
        .rtr = CAN_DATAFRAME,
        .id = COUNTER_MSG_ID,
        .rtr_mask = 1,
        .id_mask = CAN_EXT_ID_MASK
};

void activity0(void)
{

    // Create a new frame
    #ifdef CONFIG_LOOPBACK_MODE
    can_set_mode(&can_dev, CAN_LOOPBACK_MODE); //returns an int
    #endif

    struct zcan_frame frame;
    rx_callback_function(&led, &frame, NULL);

    int ret;
    ret = can_attach_isr(can_dev, rx_callback_function, NULL, &led_filter);

}

void activity1(void)
{

    // Send LED message
    send_msg_via_can(can_dev, &led_frame, false);

    // Send counter message
    send_msg_via_can(can_dev, &counter_frame, false);

    // Register callback for LED message
    can_attach_isr(can_dev, rx_callback_function, NULL, &led_filter);

    // Register callback for counter message
    can_attach_isr(can_dev, rx_callback_function, NULL, &counter_filter);

}

int main()
{
    // Redirect to other main functions
    #ifdef RUN_ACTIVITY0
    activity0();
    #endif

    #ifdef RUN_ACTIVITY1
    activity1();
    #endif

    return 0;
}