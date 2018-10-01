/**
 * A Program to control fans via PWM.
 *
 * This Program let the fan run a defined amount of time (1h) per day. The activation should be accured on the day time.
 * The speed of the fan can be controlled via potentiometer, also the fan can be completly turned of via a switch.
 * Aditionally the status and some extra informations are shown on a connected display.
 * Via a sensor the temperature status of the transistor can be monitored / recognized and the fan will slowed down or
 * turned of if the maximum temperature is reached.
 *
 * @author Igor Martens
 * @since 01.10.2018
 */

#define F_CPU 8000000

#include <avr/io.h>
#include <avr/interrupt.h>

void setup() {
}

int main(void) {
    setup();

    while(1) {
    }

    return 0;
}
