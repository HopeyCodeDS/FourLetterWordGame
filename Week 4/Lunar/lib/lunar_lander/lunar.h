#define __DELAY_BACKWARD_COMPATIBLE__
#include <led.h>
#include <timer.h>
#include <button.h>
#include <stdlib.h>
#include <buzzer.h>
#include <display.h>
#include <potentio.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define DURATION 500

typedef struct
{
    int length;
    int *burst;
    int *distance;
    float gravity;
    int *fuelReserve;
    int *current_speed;

} LUNDER_LANDER;

LUNDER_LANDER *lunar_lander;

int counter;
int seconds;
int minutes;
int loop;
int burst;
int burst_duration;

void printLunderLanderData(LUNDER_LANDER *lunar_lander);
uint8_t crashed();
uint8_t safe_landing();
void recalculate_newSituation(int *currentSpeed, float *gravity, int *burst, int *distance, int *fuelReserve, LUNDER_LANDER *lunar_lander);
void tick();

void showParameters(int *currentSpeed, int *current_fuelReserve, int *current_distance);