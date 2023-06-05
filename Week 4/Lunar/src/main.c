#define __DELAY_BACKWARD_COMPATIBLE__
#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>
#include <display.h>
#include <usart.h>
#include <time.h>
#include <LED.h>
#include <string.h>
#include <ctype.h>
#include <lunar.h>
#include <button.h>

// #define DURATION 150

int currentSpeed = 100; // speed in m/s (meters per second)
float gravity = 1.622;  // acceleration in m/s²
int distance = 9999;    // distance to the lunar surface in meters - m
int fuelReserve = 1500; // litre
int arrlen = 0;

ISR(TIMER2_COMPA_vect)
{
  counter++;
  if ((counter + 1) % MULTIPLE == 0)
  {
    tick();
    seconds += 1;
    if (seconds % 1 == 0)
    {
      recalculate_newSituation(&currentSpeed, &gravity, &burst, &distance, &fuelReserve, lunar_lander);
      lunar_lander->length += 1;
    }
  }
}

ISR(PCINT1_vect)
{
  if (bit_is_clear(BUTTON_PIN, BUTTON2))
  {
    burst_duration += 1;
  }
}

int main()
{
  initADC();
  initUSART();
  initTimer();
  initButton();
  initDisplay();
  enableBTNinterrupt();
  lunar_lander = malloc(sizeof(LUNDER_LANDER));
  lunar_lander->burst = malloc(sizeof(int));
  lunar_lander->distance = malloc(sizeof(int));
  lunar_lander->fuelReserve = malloc(sizeof(int));
  lunar_lander->distance = malloc(sizeof(int));

  lunar_lander->length = 0;
  burst_duration = 0;
  sei();
  loop=1;
  while (loop)
  {
    startTimer();
    showParameters(&currentSpeed, &fuelReserve, &distance);
  }
  cli();
  free(lunar_lander->burst);
  free(lunar_lander->current_speed);
  free(lunar_lander->fuelReserve);
  free(lunar_lander->distance);
  return 0;
}