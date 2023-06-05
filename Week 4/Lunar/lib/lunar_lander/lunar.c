#include <stdio.h>
#include <button.h>
#include <buzzer.h>
#include <lunar.h>

//FREQUENCIES OF THE NOTES
#define C5 523.250
#define D5 587.330
#define E5 659.250
#define F5 698.460
#define G5 783.990
#define A5 880.00
#define B5 987.770
#define C6 1046.500


void printLunderLanderData(LUNDER_LANDER *lunar_lander)
{

    printf("Current Speed\n");
    for (int i = 0; i < lunar_lander->length; i++)
    {
        printf("%d ", lunar_lander->current_speed[i]);
    }
    printf("\nDistance\n");
    for (int i = 0; i < lunar_lander->length; i++)
    {
        printf("%d ", lunar_lander->distance[i]);
    }
    printf("\nFuel Reserve\n");
    for (int i = 0; i < lunar_lander->length; i++)
    {
        printf("%d ", lunar_lander->fuelReserve[i]);
    }
    printf("\n");
}

uint8_t crashed()
{
    printf("Ow, You have crashed !!\n");

    uint32_t frequencies[] = {C5, D5, D5, C6};
    for (int i = 0; i < 4; i++)
    {
        enableBuzzer();
        playTone(frequencies[i], DURATION);
    }
    printLunderLanderData(lunar_lander);
  loop=0;
    return loop;
}

uint8_t safe_landing()
{
    printf("Congratulations, You have safe landed !!\n");
    uint32_t frequencies[] = {C5, D5, E5, F5, G5, A5, B5, C6};
    for (int i = 0; i < 8; i++)
    {
        enableBuzzer();
        playTone(frequencies[i], DURATION);
    }
    printLunderLanderData(lunar_lander);
    loop=0;
    return loop;
}

void recalculate_newSituation(int *currentSpeed, float *gravity, int *burst, int *distance, int *fuelReserve, LUNDER_LANDER *lunar_lander)
{
    *burst = burst_duration;
    if (*burst > 50)
    {
        *burst = 50;
    }
    *currentSpeed += (*gravity - *burst) / 5;
    *distance -= *currentSpeed;
    *fuelReserve -= *burst;

    lunar_lander->current_speed[lunar_lander->length] = *currentSpeed;
    lunar_lander->distance[lunar_lander->length] = *distance;
    lunar_lander->fuelReserve[lunar_lander->length] = *fuelReserve;
}

void tick()
{
    if (seconds == 60)
    {
        seconds = 0;
        minutes += 1;
    }
}

void showParameters(int *currentSpeed, int *current_fuelReserve, int *current_distance)
{
    uint8_t fuel_tank = (*current_fuelReserve / 375);
    writeNumber(*current_distance);
    if (fuel_tank == 4)
    {
        lightUpAllLeds();
    }
    else
    {
        lightDownAllLeds();
        for (uint8_t i = 0; i < fuel_tank; i++)
        {
            lightUpLed(i);
        }
    }
    if ((*current_distance <= 3) && *currentSpeed <= 5)
    {
        safe_landing();
    }
    else if ((*current_distance <= 3 && *currentSpeed > 5) || (*current_distance > 3 && *current_fuelReserve <= 0))
    {
        crashed();
    }
}