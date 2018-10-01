/**
 * A Program to control fans via PWM on ATTiny2313.
 *
 * This Program let the fan run a defined amount of time (1h) per day. The activation should be accured on the day time.
 * The speed of the fan can be controlled via potentiometer, also the fan can be completly turned of via a switch.
 * Aditionally the status and some extra informations are shown on a connected display.
 * Via a sensor the temperature status of the transistor can be monitored / recognized and the fan will slowed down or
 * turned of if the maximum temperature is reached.
 *
 * Ports:
 * `-> PB2 (OC0A): PWM Output for fan speed
 *
 * @author Igor Martens
 * @since 01.10.2018
 */

#define F_CPU 8000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

/**
 * Initialize fast pwm on PB2 (OC0A)
 */
void pwmInit() {
    // Setup the output for PWM
    DDRB |= (1 << DDB2);

    // Set Timer/Counter0 prescaler to clock/1.
    // At 8MHz this is 8MHz.
    TCCR0B |= (1 << CS00);

    // Set to 'Fast PWM' mode
    TCCR0A |= (1 << WGM01) | (1 << WGM00);

    // Clear OC0A output on compare match, upwards counting.
    TCCR0A |= (1 << COM0A1);

    // If the value '128' is reached, the PWM signal will set to LOW
    OCR0A=128; // 128 = 50% duty cycle
}

void setup() {
    pwmInit();

    DDRB |= (1 << DDB0); // Setup the Output fon port B0
}

int main(void) {
    setup();

    while(1) {
        PORTB |= (1<<PB0);
		_delay_ms(500);

		PORTB &= ~(1<<PB0);
		_delay_ms(500);
    }

    return 0;
}
