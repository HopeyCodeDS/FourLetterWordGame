#include <util/delay.h>
#include <avr/io.h>
#include <LED.h>



int main()
{
    //#include <LED.h> // Include the LED library

    int ledPin = 3; // Define the pin number for the LED

    enableLed(ledPin);

    // Test the dimLed function by dimming the LED to 50% brightness for 2 seconds
    dimLed(ledPin, 50, 2000);

    // Test the fadeInLed function by fading the LED in from 0 to 100% brightness over 5 seconds
    fadeInLed(ledPin, 5000);

    // Test the fadeOutLed function by fading the LED in from 0 to 100% brightness over 1 seconds
    fadeOutLed(ledPin, 1000);
    
    // delay for 8 seconds
    _delay_ms(8000);

    // Thereafter enable and light up the second LED
    enableLed(1);
    lightUpLed(1);

}
