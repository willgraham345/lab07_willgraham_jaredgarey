#include <zephyr.h>
#include <unity.h>
#include <kernel.h>
#include <drivers/gpio.h>

#include "rx_callback_function.h"

// Define MACROS needed for testing
#define LED_MSG_ID 0x01
#define COUNTER_MSG_ID 0x00
#define ID1 0x123
#define ID2 0x234
#define SLEEP_TIME_LEN 1000

#define RX_THREAD_STACK_SIZE 512
#define RX_THREAD_PRIORITY 2



/////// Define global variables needed for testing ///////


// rx_thread variables
K_THREAD_STACK_DEFINE(rx_thread_stack, RX_THREAD_STACK_SIZE);
struct k_thread rx_thread_data;
CAN_DEFINE_MSGQ(counter_msgq, 2);
const struct device *can_dev = DEVICE_DT_GET(DT_CHOSEN(zephyr_can_primary));


// not sure if we need these
struct gpio_dt_spec led = GPIO_DT_SPEC_GET_OR(DT_ALIAS(led0), gpios, {0});



void setUp(void)
{
}

void tearDown(void)
{
    // This is run after EACH test
}


void rx_thread(can_dev, counter_filter, void *arg1, void *arg2, void *arg3)
{
    // initialize msg queue
	struct zcan_frame msg;

    // set up rx msg queue
	int filter_id = can_attach_msgq(can_dev, &counter_msgq, &counter_filter);
	printk("Counter filter id: %d\n", filter_id);

    while(1)
    {
		k_msgq_get(&counter_msgq, &msg, K_FOREVER);

        // check for message we're expecting
        if (msg.dlc != 2U) {
			printk("Wrong data length: %u\n", msg.dlc);
			continue;
		}

		printk("Counter received: %u\n",
		       sys_be16_to_cpu(UNALIGNED_GET((uint16_t *)&msg.data)));
    }
}

void test_rx_thread_setup(void)
{
    // set up rx thread
    RxThread = k_thread_create(&rx_thread_data, rx_thread_stack,
            K_THREAD_STACK_SIZEOF(rx_thread_stack),
            (k_thread_entry_t)rx_thread,
            NULL,
            NULL,
            NULL,
            RX_THREAD_PRIORITY,
            0,
            K_NO_WAIT);
}

void test_rx_callback_function_case_2(void)
{
}

void test_rx_callback_function_invalid_case(void)
{
}

int main(void)
{
    UNITY_BEGIN();
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

    RUN_TEST(test_rx_callback_function_case_1);
    RUN_TEST(test_rx_callback_function_case_2);
    RUN_TEST(test_rx_callback_function_invalid_case);

    return UNITY_END();
}