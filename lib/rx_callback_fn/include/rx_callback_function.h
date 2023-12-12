#include <drivers/can.h>
#include <drivers/gpio.h>

//Define macros for sending messages via led
#define SET_LED 1
#define RESET_LED 0
#define LED_MSG_ID 0x01
#define COUNTER_MSG_ID 0x00
#define ID1 0x123
#define ID2 0x234
#define SLEEP_TIME_LEN 1000


#define SET_LED 1
#define RESET_LED 0
void rx_callback_function(const struct gpio_dt_spec *led, struct zcan_frame *frame, void *arg);
