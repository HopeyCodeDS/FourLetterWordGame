#include <avr/io.h>
#include "button.h"

void enableButton(int button)
{
    switch (button)
    {
        case 1:
            DDRD &= ~(1 << PD2); // Set PD2 as input (leftmost button)
            PORTD |= (1 << PD2); // Enable internal pull-up resistor
            break;
        case 2:
            DDRD &= ~(1 << PD3); // Set PD3 as input (second button from left)
            PORTD |= (1 << PD3); // Enable internal pull-up resistor
            break;
        case 3:
            DDRD &= ~(1 << PD4); // Set PD4 as input (third button from left)
            PORTD |= (1 << PD4); // Enable internal pull-up resistor
            break;
        case 4:
            DDRD &= ~(1 << PD5); // Set PD5 as input (rightmost button)
            PORTD |= (1 << PD5); // Enable internal pull-up resistor
            break;
        default:
            break;
    }
}

int buttonPushed(int button)
{
    switch (button)
    {
        case 1:
            return !(PIND & (1 << PD2)); // Check if PD2 is low (button pushed)
        case 2:
            return !(PIND & (1 << PD3)); // Check if PD3 is low (button pushed)
        case 3:
            return !(PIND & (1 << PD4)); // Check if PD4 is low (button pushed)
        case 4:
            return !(PIND & (1 << PD5)); // Check if PD5 is low (button pushed)
        default:
            return 0;
    }
}

int buttonReleased(int button)
{
    switch (button)
    {
        case 1:
            return (PIND & (1 << PD2)); // Check if PD2 is high (button released)
        case 2:
            return (PIND & (1 << PD3)); // Check if PD3 is high (button released)
        case 3:
            return (PIND & (1 << PD4)); // Check if PD4 is high (button released)
        case 4:
            return (PIND & (1 << PD5)); // Check if PD5 is high (button released)
        default:
            return 0;
    }
}
