#include <zephyr.h>
#include <unity.h>
#include <kernel.h>
#include <sys/printk.h>

// Include lib files
#include "send_msg_via_can.c"


// Define MACROS
#define SET_LED 1
#define RESET_LED 0
#define LED_MSG_ID 0x01
#define COUNTER_MSG_ID 0x00
#define ID1 0x123
#define ID2 0x234
#define SLEEP_TIME_LEN 1000

#define RX_THREAD_STACK_SIZE 512
#define RX_THREAD_PRIORITY 2

// Define Globals
const struct device *can_dev = DEVICE_DT_GET(DT_CHOSEN(zephyr_can_primary));

void setUp(void)
{
    // This is run before EACH test
}

void tearDown(void)
{
    // This is run after EACH test
}

void test_send_msg_via_can_blocking(void)
{
    // Create a mock frame
    struct zcan_frame frame;
    bool is_blocking = true;

    // Call the function
    int ret;
    ret = send_msg_via_can(can_dev, &frame, is_blocking);

    TEST_ASSERT_EQUAL(0, ret);
}

void test_send_msg_via_can_non_blocking(void)
{
    // Create a mock frame
    struct zcan_frame frame;
    bool is_blocking = false;

    // Call the function
    int ret;
    ret = send_msg_via_can(can_dev, &frame, is_blocking);
    // CAN is blocking, so this test should fail. 
    TEST_ASSERT_NOT_EQUAL(0, ret);
}

int main(void)
{
    UNITY_BEGIN();


    struct zcan_frame led_frame = 
    {
        .id_type = CAN_STANDARD_IDENTIFIER,
        .rtr = CAN_DATAFRAME,
        .id = LED_MSG_ID, // Matches to identifier 0x123
        .dlc = 1, //length of the message
        .data = {1} // uint8_t payload
    };
    struct zcan_frame counter_frame = 
    {
        .id_type = CAN_EXTENDED_IDENTIFIER,
        .rtr = CAN_DATAFRAME,
        .id = COUNTER_MSG_ID, // Matches to identifier 0x123
        .dlc = 2, //length of the message
        .data = {0} // uint8_t payload
    };
    RUN_TEST(test_send_msg_via_can_blocking);
    RUN_TEST(test_send_msg_via_can_non_blocking);

    return UNITY_END();
}