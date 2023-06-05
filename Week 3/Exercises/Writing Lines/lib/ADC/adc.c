#include <util/delay.h>
#include <avr/io.h>
#include <usart.h>
#include <avr/interrupt.h>


void initADC()
{
    ADMUX |= (1 << REFS0);                                // Set up of reference voltage. We choose 5V as reference.
    ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0); // Determine a sample rate by setting a division factor. Used division factor: 128
    ADCSRA |= (1 << ADEN);                                // Enable the ADC
}

void startADC()
{
    ADCSRA |= (1 << ADSC);
}

uint16_t readADCvalue()
{
    startADC();
    loop_until_bit_is_clear(ADCSRA, ADSC);
    return ADC;

}


// void initADC()
// {
//     ADMUX |= (1 << REFS0);                                // Set up the reference voltage. We choose 5V as the reference.
//     ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0); // Determine the sample rate by setting the division factor to 128.
//     ADCSRA |= (1 << ADEN);                                // Enable the ADC
//     ADCSRA |= (1 << ADATE);                               // Enable ADC Auto Triggering
//     ADCSRB = 0;                                                // Set ADC Auto Trigger Source to Free Running Mode (default)
//     ADCSRA |= (1 << ADSC);                                // Start the analog-to-digital conversion
// }