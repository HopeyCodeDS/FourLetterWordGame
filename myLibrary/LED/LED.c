#define __DELAY_BACKWARD_COMPATIBLE__
#include <util/delay.h>
#include <avr/io.h>
//#include "LED.h"

#define NUMBER_OF_LEDS 4 //Define is a "preprocessor directive". It ensures that every NUMBER_OF_LEDS will be replaced by 4 in the following code
#define LEDS 15

void enableLed ( int lednumber ) //C has no classes; functions can be included directly in the .c file.
{
    if ( lednumber < 0 || lednumber > NUMBER_OF_LEDS-1 ) return;
    DDRB |= ( 1 << ( PB2 + lednumber ));    //Check the tutorial "Writing to a Pin". We know from the documentation on
                                            //the multifunctional shield that the LEDs start at PB2
}

void lightUpLed ( int lednumber )    //Note: enabled LEDs light up immediately ( 0 = on )
{
    if ( lednumber < 0 || lednumber > NUMBER_OF_LEDS-1 ) return;
    PORTB &= ~( 1 << ( PB2 + lednumber ));  //Check the tutorial on "Bit Operations" to know what happens in this line.
}

void lightDownLed ( int lednumber )
{
    if ( lednumber < 0 || lednumber > 3 ) return;
    PORTB |= ( 1 << ( PB2 + lednumber ));   //Make sure you understand this line as well!
}

void enableMultipleLeds ( uint8_t leds)
{
    if ( leds < 0 || leds > LEDS ) return;
    DDRB |= ( leds << ( PB2 ));
}

void lightUpMultipleLeds ( uint8_t leds)
{
    if ( leds < 0 || leds > LEDS ) return;
    PORTB &= ~( leds << ( PB2 ));
}

void enableAllLeds ()
{
 int leds = 0b00001111;
 DDRB |= ( leds << ( PB2 ));
}

void lightUpAllLeds()
{
    int leds = 0b00001111;
    PORTB &= ~( leds << ( PB2 ));
}

void lightDownAllLeds()
{
    int leds = 0b00001111;
    PORTB |= ( leds << ( PB2 ));
}

void lightToggleOneLed(int lednumber)
{
    if ( lednumber < 0 || lednumber > NUMBER_OF_LEDS - 1 ) return;
    PORTB ^= ( 1 << ( PB2 + lednumber ));
}

/*
void dimLed(int lednumber, int percentage, int duration)
{
    if ( lednumber < 0 || lednumber > NUMBER_OF_LEDS - 1 ) return;
    PORTB &= ( 1 << ( PB2 + lednumber ));
    _delay_ms(100);
    PORTB |= ( 1 << ( PB2 + lednumber));
    _delay_ms(150);
     

}
*/

void dimLed(int lednumber, int percentage, int duration)
{
    int onTime = (percentage * duration) / 100; // Calculate on time in milliseconds
    int offTime = duration - onTime; // Calculate off time in milliseconds

    int cycles = onTime / 10; // Calculate the number of PWM cycles (10ms per cycle)
    int remainder = onTime % 10; // Calculate the remainder of the last cycle

    for (int i = 0; i < cycles; i++) {
        PORTB &= ~( 1 << ( PB2 + lednumber )); // Turn on the LED
        _delay_ms(5); // Wait for 5ms
        PORTB |= ( 1 << ( PB2 + lednumber )); // Turn off the LED
        _delay_ms(5); // Wait for 5ms
    }

    if (remainder > 0) { // If there is a remainder, do one more cycle
        PORTB &= ~( 1 << ( PB2 + lednumber )); // Turn on the LED
        _delay_ms(remainder); // Wait for the remainder of the on time
        PORTB |= ( 1 << ( PB2 + lednumber )); // Turn off the LED
        _delay_ms(offTime); // Wait for the off time
    }
}

void fadeInLed(int led, int duration)
{
    for (int i = 0; i <= 100; i++) {
        dimLed(led, i, duration/100); // Dim the LED to the current brightness level for 1% of the total duration
    }
}

void fadeOutLed(int led, int duration)
{
  for (int brightness = 255; brightness >= 0; brightness--)
  {
    //analogWrite(led, brightness); // Set the LED brightness

    // Configure Timer/Counter1 for PWM
    TCCR1A |= (1 << COM1A1) | (1 << WGM10);
    TCCR1B |= (1 << CS10) | (1 << WGM12);

    // Set the duty cycle for the PWM signal
    int dutyCycle = 128;  // Set the duty cycle to 50%
    OCR1A = dutyCycle;

    _delay_ms(duration / 255); // Wait for a fraction of the duration
  }
  PORTB |= ( 1 << ( PB2 + led )); // Turn off the LED
}



/*
int main()
{
    for ( int i = 0; i < 4; i++ )
    {
        enableLed(i);
    }
    while (1)
    {
        for ( int i = 0; i < 4; i++ )
        {
            lightUpLed(i);
            _delay_ms( 100 );
        }
        for ( int i = 0; i < 4; i++ )
        {
            lightDownLed(i);
            _delay_ms( 100 );
        }
    }
    return 0;
}
*/