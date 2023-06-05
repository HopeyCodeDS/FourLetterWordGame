#define __DELAY_BACKWARD_COMPATIBLE__ 
#include <avr/io.h>
#include <avr/interrupt.h>
#include <potentio.h>
#include <util/delay.h>
#include <button.h>
#include <buzzer.h>
#include <display.h>

// DURATION IS IN MILLISECONDS
#define DURATION 500

//FREQUENCIES OF THE NOTES
#define C5 523.250
#define D5 587.330
#define E5 659.250
#define F5 698.460
#define G5 783.990
#define A5 880.00
#define B5 987.770
#define C6 1046.500

int playing = 0;
int counter = 0;

ISR(PCINT1_vect)
{
  // is any button clicked?
  if (isAnyClicked())
  {
    // debounce
    _delay_us(1000);

    if (isAnyClicked())
    {
      enableBuzzer();
      if (playing == 1)
      {
        playing = 0;
      }
      else
      {
        playing = 1;
      }
    }
  }
}

uint32_t frequencies[] = {C5, D5, E5, F5, G5, A5, B5, C6};

int main()
{

  initDisplay();
  initADC();
  initButton();
  enableBTNinterrupt();
  sei();
  uint16_t value;

  while (1)
  {
    startADC();
    // value = readADCvalue(value);
    value = ADC;
    writeNumberAndWait(value,500);

    for (int i = 0; i < frequencies[i]; i++)
    {
      while (playing == 1)
      {
       playTone(value, DURATION);
      }
    }
  }
  return 0;
}
