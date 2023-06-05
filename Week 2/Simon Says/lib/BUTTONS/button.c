#include <avr/io.h>
#include "button.h"
#include <util/delay.h>
#include <avr/interrupt.h>
#include <usart.h>
#include <stdlib.h>

#define BUTTON_PORT PORTC
#define BUTTON_PIN PINC
#define BUTTON_DDR DDRC
#define BUTTON1 PC1
#define BUTTON2 PC2
#define BUTTON3 PC3

void enableButton(int button)
{
    BUTTON_DDR &= ~(button << PC1);
}

int buttonPushed(int button)
{
    enableButton(button);
    return ((PINC & (button << PC1)) == 0);
}

int buttonReleased(int button)
{
    return !buttonPushed(button);
}

int isAnyClicked()
{
    return bit_is_clear(BUTTON_PIN, BUTTON1) || bit_is_clear(BUTTON_PIN, BUTTON2) || bit_is_clear(BUTTON_PIN, BUTTON3);
}

void initButton()
{
    BUTTON_DDR &= ~_BV(BUTTON1) | ~_BV(BUTTON2) | ~_BV(BUTTON3);
    BUTTON_PORT |= _BV(BUTTON1) | _BV(BUTTON2) | _BV(BUTTON3);
}

// enable interrupts on buttons
void enableBTNinterrupt()
{
    PCICR |= _BV(PCIE1);
    PCMSK1 |= _BV(BUTTON1) | _BV(BUTTON2) | _BV(BUTTON3);
}
/*
int buttonPushed(int button)
{
    switch (button)
    {
    case 1:
        return !(PINC & (1 << PC1)); // Check if PC1 is low (button pushed)
    case 2:
        return !(PINC & (1 << PC2)); // Check if PC2 is low (button pushed)
    case 3:
        return !(PINC & (1 << PC3)); // Check if PC3 is low (button pushed)
    default:
        return 0;
    }
}

int buttonReleased(int button)
{
    switch (button)
    {
    case 1:
        return (PINC & (1 << PC1)); // Check if PC1 is high (button released)
    case 2:
        return (PINC & (1 << PC2)); // Check if PC2 is high (button released)
    case 3:
        return (PINC & (1 << PC3)); // Check if PC3 is high (button released)

    default:
        return 0;
    }
}
*/