#include <avr/io.h>
#include <usart.h>
#define __DELAY_BACKWARD_COMPATIBLE__
#include <util/delay.h>

// DURATION IS IN MILLISECONDS
#define DURATION 150

// FREQUENCIES OF THE NOTES
#define DO 523.250
#define RE 587.330
#define MI 659.250
#define FA 698.460
#define SO 783.990
#define LA 880.00
#define SI 987.770
#define DOO 1046.500

void enableBuzzer()
{
  DDRD |= _BV(PD3); // (thesame??) ( 1 << PD3 ); //Buzzer is connected to PD3
}

void disableBuzzer()
{
    DDRD &= ( 0 << PD3 );
}

void playTone(float frequency, uint32_t duration)
{
  uint32_t periodInMicroSecs = (uint32_t)(1000000 / frequency);              // Calculate the period in microsecs from the freq
  uint32_t durationInMicroSecs = duration * 1000;                            // We express duration in microsecs
  for (uint32_t time = 0; time < durationInMicroSecs; time += periodInMicroSecs) // See tutorial on Music for more info!
  {
    PORTD &= ~(1 << PD3);         // turn the buzzer on
    _delay_us(periodInMicroSecs / 2); // Wait for the half of the period
    PORTD |= (1 << PD3);          // Turn the buzzer off
    _delay_us(periodInMicroSecs / 2); // Wait again for half of the period
  }
}