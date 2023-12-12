#include <zephyr.h>
#include <kernel.h>
#include <sys/printk.h>
#include <device.h>
#include <drivers/gpio.h>
#include <sys/byteorder.h>

#include "send_msg_via_can.h"
// comment out the activity if we're doing activity 1 or 2
#define ACTIVITY_3 1


#define SET_LED 1
#define RESET_LED 0
#define LED_MSG_ID 0x01
#define COUNTER_MSG_ID 0x00
#define ID1 0x123
#define ID2 0x234
#define SLEEP_TIME_LEN 1000

#define RX_THREAD_STACK_SIZE 512
#define RX_THREAD_PRIORITY 2
K_THREAD_STACK_DEFINE(rx_thread_stack, RX_THREAD_STACK_SIZE);
CAN_DEFINE_MSGQ(counter_msgq, 2);

struct gpio_dt_spec led = GPIO_DT_SPEC_GET_OR(DT_ALIAS(led0), gpios, {0});
const struct device *can_dev = DEVICE_DT_GET(DT_CHOSEN(zephyr_can_primary));
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


struct zcan_work rx_work;
struct k_thread rx_thread_data;

void tx_callback_function(uint32_t ret_val, char *info)
{
    if (ret_val != 0) {
        printk("TX Callback: From %s \t Error Code: %d\n", info, ret_val);
    }
}


void rx_callback_function(struct zcan_frame *frame, void *arg)
{
    
    // ... do something with the frame ...
    printk("Rx Callback Function Hit\n");
    switch (frame->data[0])
    {
        case 1:
            // set gpio pin
            printk("Hitting Case 1");
            gpio_pin_set(led.port, led.pin, SET_LED);
            break;
        case 2:
            // set gpio pin
            printk("Hitting Case 2");
            gpio_pin_set(led.port, led.pin, RESET_LED);
            break;
    }
}

// void send_msg_via_can(struct zcan_frame *frame, bool is_blocking)
// {
//     int ret;
//     if (is_blocking) {
//         ret = can_send(can_dev, frame, K_MSEC(100), NULL, NULL);
//         if (ret!=0) {
//             printk("can_send failed, error code: %d\n", ret);
//         }
//     }
//     else {
//         can_send(can_dev, frame, K_FOREVER, tx_callback_function, "LED change");
//     }
// }

void rx_thread(void *arg1, void *arg2, void *arg3)
{
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

void main(void)
{
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
    
    // Set a CAN device to do loopback mode (Only set this in certain build environments)
    #ifdef CONFIG_LOOPBACK_MODE
    can_set_mode(&can_dev, CAN_LOOPBACK_MODE); //returns an int
    #endif
    
    int ret;
    k_tid_t rx_tid;

    // activity 1&2:
#ifndef ACTIVITY_3
    // Setting up callback function for filter ID
    ret = can_attach_isr(can_dev, rx_callback_function, NULL, &led_filter);
#else
    // activity 3:
    // Make rx workq to listen for LED CAN messages
    ret = can_attach_workq(can_dev, &k_sys_work_q, &rx_work, &rx_callback_function, NULL, &led_filter);
    if (ret == CAN_NO_FREE_FILTER) {
		printk("Error, no filter available!\n");
		return;
	}
#endif

    // Make an rx thread using a workq instead to receive the Counter CAN messages
    rx_tid = k_thread_create(&rx_thread_data, rx_thread_stack,
				 K_THREAD_STACK_SIZEOF(rx_thread_stack),
				 rx_thread, NULL, NULL, NULL,
				 RX_THREAD_PRIORITY, 0, K_NO_WAIT);
    

    // If statement configures gpio hardware pins
    if (led.port != NULL) {
		if (!device_is_ready(led.port)) {
			printk("LED: Device %s not ready.\n",
			       led.port->name);
			return;
		}
		ret = gpio_pin_configure_dt(&led, GPIO_OUTPUT_HIGH);
		if (ret < 0) {
			printk("Error setting LED pin to output mode [%d]",
			       ret);
			led.port = NULL;
		}
	}

    // Send (TX) both CAN messages
    while (1)
    {
        led_frame.data[0] ^= 1; //Toggle LED state each iteration
        printk("Sending LED message\n");
        int ret1;
        ret = send_msg_via_can(&led_frame, true);

        counter_frame.data[0] = counter_frame.data[0]++; // make the data look like other random messages
        printk("Sending counter message\n");
        int ret2;
        ret2 = send_msg_via_can(&counter_frame, false);
        k_sleep(K_MSEC(SLEEP_TIME_LEN));
    }
}

