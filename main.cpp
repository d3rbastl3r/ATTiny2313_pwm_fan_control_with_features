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

#include "EightSegmentCtrl.h"

// value decoder for the eight segment display
uint8_t eightSegmentDecoder[21] = {
    0b00111111, // 0
    0b10111111, // 0.
    0b00000110, // 1
    0b10000110, // 1.
    0b01011011, // 2
    0b11011011, // 2.
    0b01001111, // 3
    0b11001111, // 3.
    0b01100110, // 4
    0b11100110, // 4.
    0b01101101, // 5
    0b11101101, // 5.
    0b01111101, // 6
    0b11111101, // 6.
    0b00000111, // 7
    0b10000111, // 7.
    0b01111111, // 8
    0b11111111, // 8.
    0b01100111, // 9
    0b11100111, // 9.
    0b10000000  // .
};

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
    OCR0A=180; // 128 = 50% duty cycle
}

void eightSegmentInit() {
    // Setup output ports
    DDRB |= (1<<DDB5);  // SER Port
    DDRB |= (1<<DDB4);  // SRCLK Port
    DDRB |= (1<<DDB3);  // RCLK Port
}

void setup() {
    pwmInit();
    eightSegmentInit();

    DDRB |= (1 << DDB0); // Setup the Output fon port B0
}

/**
 * Push the given byte to the register and finally execute latch.
 * The left bit will be pushed first.
 */
void pushByteAndLatch(uint8_t byte) {
    for (uint8_t i=0; i<8; ++i) {
        (byte & 128) ? PORTB |= (1 << PB5) : PORTB &= ~(1 << PB5);
        PORTB |= (1 << PB4);
        PORTB &= ~(1 << PB4);
        byte = byte << 1;
    }

    PORTB |= (1 << PB3);
    PORTB &= ~(1 << PB3);
}

/**
 * Set the data inside register to zero and latch
 * This we only need to save one port on the MCU.
 * If the MCU have enough ports SRCLR on the shift register can be used to clear data instead of this method.
 */
void clearRegister() {
    pushByteAndLatch(0b00000000);
}

uint8_t segmentTestCoun = 0;

int main(void) {
    setup();

    while(1) {
        if (segmentTestCoun == 21) segmentTestCoun = 0;
        else ++segmentTestCoun;
        pushByteAndLatch(eightSegmentDecoder[segmentTestCoun]);
        PORTB |= (1<<PB0);
		_delay_ms(500);

		PORTB &= ~(1<<PB0);
		_delay_ms(500);
    }

    return 0;
}
