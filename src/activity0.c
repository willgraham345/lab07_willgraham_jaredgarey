#include <zephyr.h>
#include <kernel.h>
#include <sys/printk.h>

#include "send_msg_via_can.h"

//Define MACROS
#define SET_LED 1
#define RESET_LED 0
#define LED_MSG_ID 0x01
#define COUNTER_MSG_ID 0x00
#define ID1 0x123
#define ID2 0x234
#define SLEEP_TIME_LEN 1000
#define RX_THREAD_STACK_SIZE 512
#define RX_THREAD_PRIORITY 2

//Define Globals
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


void main(void)
{
    // Create a new frame
    struct zcan_frame frame;
    bool is_blocking = true;
}