Add the following configuration to prj.conf

Note that the CAN device is "CAN_1" on the discovery board.

## [Activity 0](https://github.com/uofu-emb/rtos/tree/main/labs/7.networking#activity-0)

This activity will establish sending and receiving messages.

~~1. Set the CAN device to loopback mode with `can_set_mode`~~
~~2. Create a function that sends a message periodically using `can_send`.~~
~~3. Register a callback with `can_attach_isr` to receive the messages~~
    ~~1. Set the filter to receive any messages.~~

Hint: follow the examples in the documentation.

## [Activity 1](https://github.com/uofu-emb/rtos/tree/main/labs/7.networking#activity-1)

This activity will deal with filtering for specific message types.

1. Create a function that sends two different types of messages.
2. Register a callback which filters only one type of message.

## [Activity 2](https://github.com/uofu-emb/rtos/tree/main/labs/7.networking#activity-2)

This activity will work with the CAN transceiver and connecting multiple boards.

1. Wire up CAN transceiver to your boards.
    1. Remember to disable loopback mode.
    2. RX is on PB8 and TX is on PB9.
2. Run the code from activity 1 and observe the electrical behavior of the bus line and the microcontroller lines.

## [Activity 3](https://github.com/uofu-emb/rtos/tree/main/labs/7.networking#activity-3)

This activity will observe bus contention from a babbling node. "In a Controller Area Network a babbling node is a node continuously (and usually erroneously) transmitting CAN frames with identical - often high - priority."

1. Setup a receiver using a worker queue.
2. On one board, broadcast messages with a high priority continously.
3. On the other board, broadcast a message with a lower priority periodically.
4. use oscilloscope to observe the access pattern.
5. Observe how often the lower priority message is actually transmitted and received.
6. Introduce a short busy wait delay into the babbling node, and slowly increase the duration until you observe the other messages consistently

## [Activity 3](https://github.com/uofu-emb/rtos/tree/main/labs/7.networking#activity-3-1)

This activity will work with real car data from a 2007 Toyota Yaris. Data used between control units on vehicles is usually proprietary. We'll need to try to reverse engineer what data is being sent.

These were found online from previous groups reverse engineering attempts.

[https://github.com/P1kachu/talking-with-cars/blob/master/notes/toyota-yaris.md](https://github.com/P1kachu/talking-with-cars/blob/master/notes/toyota-yaris.md)

[https://github.com/fabiobaltieri/toyothack/blob/master/toyothack.c](https://github.com/fabiobaltieri/toyothack/blob/master/toyothack.c)

1. Load the provided program onto one of your boards. This program will replay recorded messages from a vehicle can bus.
2. Try to decode one of the messages from the data.