#define __DELAY_BACKWARD_COMPATIBLE__  
#include <avr/io.h>
#include <stdlib.h>
#include <util/delay.h>
#include <LED.h>

#define F_CPU 16000000UL  // Set CPU clock frequency
#define LED_PORT PORTB     // Set LED port
#define LED_DDR DDRB       // Set LED data direction register
#define LED_1 PB2          // Set LED 1 pin
#define LED_2 PB3          // Set LED 2 pin
#define LED_3 PB4          // Set LED 3 pin
#define LED_4 PB5          // Set LED 4 pin

int main(void)
{
    LED_DDR |= (1 << LED_1) | (1 << LED_2) | (1 << LED_3) | (1 << LED_4);  // Set LED pins as outputs
    srand(0);  // Seed random number generator with 0

    while (1) {
        int led = rand() % 4;  // Generate random number between 0 and 3
        printf("led: %d\n", led);
        int delay_time = rand() % 900 + 100;  // Generate random delay between 100 and 1000 ms
        printf("delay time: %d\n", delay_time);

        switch (led) {
            case 0:
                LED_PORT |= (1 << LED_1);  // Turn on LED 1
                _delay_ms(delay_time);     // Wait for delay_time
                LED_PORT &= ~(1 << LED_1); // Turn off LED 1
                break;
            case 1:
                LED_PORT |= (1 << LED_2);  // Turn on LED 2
                _delay_ms(delay_time);     // Wait for delay_time
                LED_PORT &= ~(1 << LED_2); // Turn off LED 2
                break;
            case 2:
                LED_PORT |= (1 << LED_3);  // Turn on LED 3
                _delay_ms(delay_time);     // Wait for delay_time
                LED_PORT &= ~(1 << LED_3); // Turn off LED 3
                break;
            case 3:
                LED_PORT |= (1 << LED_4);  // Turn on LED 4
                _delay_ms(delay_time);     // Wait for delay_time
                LED_PORT &= ~(1 << LED_4); // Turn off LED 4
                break;
        }
    }

    return 0;
}
