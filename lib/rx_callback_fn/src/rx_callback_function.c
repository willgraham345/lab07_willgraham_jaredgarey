#include "rx_callback_function.h"

void rx_callback_function(const struct gpio_dt_spec *mock_led, struct zcan_frame *frame, void *arg)
{
    // ... do something with the frame ...
    printk("Rx Callback Function Hit\n");
    switch (frame->data[0])
    {
        case 1:
            // set gpio pin
            printk("Hitting Case 1");
            gpio_pin_set(mock_led->port, mock_led->pin, SET_LED);
            break;
        case 2:
            // set gpio pin
            printk("Hitting Case 2");
            gpio_pin_set(mock_led->port, mock_led->pin, RESET_LED);
            break;
    }
}