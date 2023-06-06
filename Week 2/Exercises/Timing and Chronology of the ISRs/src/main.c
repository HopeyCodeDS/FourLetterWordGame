#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <LED.h>

#define LED_PORT PORTB
#define BUTTON_PORT PORTC
#define BUTTON_PIN PINC
#define LED_DDR DDRB
#define BUTTON_DDR DDRC
#define BUTTON1 PC1
#define BUTTON2 PC2
#define LED1 PB2
#define LED2 PB3
#define LED3 PB4
#define LED4 PB5
#define FLASH_TIME 4


/* First, review the code in the main function!
 * This ISR is called when Pin Change Interrupt 1 is triggered (PCINT1_vect)
 * This is the interrupt for PORTC, where the switches/buttons are connected... */
ISR( PCINT1_vect )
{
    // button 1 is pressed (bit is set to 0)?
    if ( bit_is_clear( BUTTON_PIN, BUTTON1 ))
    {
        //We wait 1000 microseconds and check again (debounce!)
        _delay_us( 5000 );
        // button 1 is pressed (bit is set to 0)?
        if ( bit_is_clear( BUTTON_PIN, BUTTON1 ))
        {
          if ( bit_is_set(LED_PORT, LED2) && bit_is_set (LED_PORT, LED3) )  // if led2 is off (bit is set to 1)
            {
                LED_PORT &= ~_BV( LED2 );       // turn led2 on (set bit to 0)
                LED_PORT &=~_BV( LED3 );        // turn led3 on (set bit to 0)
                _delay_us( 5000 );
            }
            else                                // else
            {
                LED_PORT |= _BV( LED2 );        // turn led2 off (set bit to 1)
                LED_PORT |= _BV( LED3 );        // turn led3 off (set bit to 1)
                _delay_us( 5000 );
            }
        }
    }

    // button 2 is pressed (bit is set to 0)?
    if ( bit_is_clear( BUTTON_PIN, BUTTON2 ))
    {
        for (int i = 0; i < FLASH_TIME; i++){
                 // flash for 7 times
            if ( bit_is_set(LED_PORT, LED2) )  // if led2 is off (bit is set to 1)
            {
                LED_PORT &= ~_BV( LED2 );       // turn led2 on (set bit to 0)
                _delay_ms( 500 );
            }
            else                                // else
            {
                LED_PORT |= _BV( LED2 );        // turn led2 off (set bit to 1)
                _delay_ms( 500 );
            }
        }
    }

}

int main()
{
    LED_DDR |= _BV( LED1 ) | _BV( LED2 ) | _BV( LED3 ) | _BV( LED4 );   // we'll use led1, led2, led3 and led4
    LED_PORT |= _BV( LED1 ) | _BV( LED2 ) | _BV( LED3 ) | _BV( LED4 );  // turn 3 leds off
    BUTTON_DDR &= ~_BV( BUTTON1 );          // we'll use button1
    BUTTON_DDR &= ~_BV( BUTTON2 );          // we'll use button2
    BUTTON_PORT |= _BV( BUTTON1 );          // enable pull-up
    BUTTON_PORT |= _BV( BUTTON2 );          // enable pull-up
    PCICR |= _BV( PCIE1 );  /* in Pin Change Interrupt Control Register: indicate
                             * which interrupt(s) you want to activate (PCIE0: port B,
                             * PCIE1: port C, PCIE2: port D) */
    
    PCMSK1 |= _BV( BUTTON1 );   /* In the corresponding Pin Change Mask Register: indicate
                                 * which pin(s) of that port activate the ISR. */
    PCMSK1 |= _BV( BUTTON2 );
    
    sei();  // Set Enable Interrupts --> activate the interrupt system globally.
    
    while(1)
    {
      LED_PORT &= ~_BV( LED3 );   // turn led3 on
      LED_PORT &= ~_BV( LED4 ); // turn led4 on
      _delay_ms( 2000 );
      LED_PORT |= _BV( LED3 );    // turn led3
      LED_PORT |= _BV( LED4 ); // turn led4
      _delay_ms( 2000 );

    }
    return 0;
}


/*
Answers to Question

Q1: Yes
Q2: Yes
Q3: Yes
*/