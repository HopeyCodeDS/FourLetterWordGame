#include <stdio.h>
#include <util/delay.h>
#include "usart.h"
#include "button.h"

int main(void) {
    // Initialize UART for serial communication
    initUSART();

    // Enable buttons
    enableButton(1); // Button 1 is connected to PB0
    enableButton(2); // Button 2 is connected to PB1

    while (1) {
        // Check if button 1 was pushed
        if (buttonPushed(1)) {
            printf("Button 1 was pushed!\n");
        }

        // Check if button 2 was released
        if (buttonReleased(2)) {
            printf("Button 2 was released!\n");
        }

        // Wait a little bit before checking the buttons again
        _delay_ms(100);
    }

    return 0;
}
