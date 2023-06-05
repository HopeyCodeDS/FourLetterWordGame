#define __DELAY_BACKWARD_COMPATIBLE__
#include <util/delay.h>
#include <string.h>
#include <usart.h>
#include <display.h>
#include <stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <potentio.h>
#include <button.h>
#include <buzzer.h>

/*
// FREQUENCIES OF THE NOTES
#define C5 523.250
#define D5 587.330
#define E5 659.250
#define F5 698.460
#define G5 783.990
#define A5 880.00
#define B5 987.770
#define C6 1046.500
*/

//FREQUENCIES OF THE NOTES
#define C5 523.250
#define D5 587.330
#define E5 659.250
#define F5 698.460
#define G4 391.995
#define G5 783.990
#define A5 880.00
#define B5 987.770
#define C6 1046.500
#define E4 329.628
#define D6 1174.660
#define E6 1318.510
#define F6 1396.910
#define G6 1567.980
#define A6 1760.00
#define B6 1975.530
#define C7 2093.000
#define A4 440.00
#define B4 493.88
#define AS4 466.16


uint32_t frequencies[] = {F5, E5, F5, E5, D5, C5, G4, E5, C5, D5, E5, F5, E5, F5, E5, D5, C5, G4, D5, C5, B4, D5, C5};
uint32_t durations[] = {1000, 500, 250};
int button_pushed = 0;

typedef struct
{
    float freq;
    uint16_t duration;
} NOTE;

typedef struct
{
    char *name;
    NOTE **notes;
    uint16_t length;
} SONG;

void playNote(NOTE *note)
{
    playTone(note->freq, note->duration);
}

SONG *generateSong(char *name, uint16_t length)
{

    SONG *song = malloc(sizeof(SONG));
    song->length = length;
    song->name = malloc(sizeof(name) + 1);
    strcpy(song->name, name);
    song->notes = malloc(length * sizeof(NOTE *));

    for (int i = 0; i < length; i++)
    {
        song->notes[i] = malloc(sizeof(NOTE));
        song->notes[i]->freq = frequencies[rand() % (sizeof(frequencies)) / 7];
        song->notes[i]->duration = durations[rand() % (sizeof(durations)) / 3];
    }
    return song;
}
void playSong(SONG *song)
{
    for (int i = 0; song->length; i++)
    {
        playNote(song->notes[i]);
    }
}
ISR(PCINT1_vect)
{
    if (isAnyClicked())
    {
        _delay_us(1000);
        if (isAnyClicked())
        {
            button_pushed = 1;
            printf("\nButton pushed \n");
        }
    }
}
int main()
{
    initUSART();
    enableBuzzer();
    enableButton(1);
    enableBTNinterrupt();
    int counter = 0;

    sei();
    while (button_pushed == 0)
    {

        printf("Button is NOT pressed \n");
        counter++;
    }
    cli();
    srand(counter);
    SONG *song = generateSong("Opeyemi", 4);
    NOTE *note;
    while (isAnyClicked())
    {
        if (sizeof(frequencies) / 2 == 0)
        {
            int i = 0;
            while (i < 4)
            {
                playNote(note);
                i++;
            }
        }
        else
        {
            for (int i = 0; i < 10; i++)
            {
                playSong(song);
            }
        }
    }
    return 0;
}
