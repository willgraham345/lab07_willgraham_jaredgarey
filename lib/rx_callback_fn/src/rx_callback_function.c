#include "rx_callback_function.h"

void rx_callback_function(const struct gpio_dt_spec *led, struct zcan_frame *frame, void *arg)
{
    // ... do something with the frame ...
    printk("Rx Callback Function Hit\n");
    switch (frame->data[0])
    {
        case 1:
            // set gpio pin
            printk("Hitting Case 1");
            gpio_pin_set(led->port, led->pin, SET_LED);
            break;
        case 2:
            // set gpio pin
            printk("Hitting Case 2");
            gpio_pin_set(led->port, led->pin, RESET_LED);
            break;
    }
}