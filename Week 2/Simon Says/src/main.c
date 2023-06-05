#include <button.h>
#include <led.h>
#include <avr/interrupt.h>
#include <usart.h>
#include <stdlib.h>
#include <display.h>
#include <buzzer.h>

#define DELAY 500
#define BUTTON_PORT PORTC
#define BUTTON_PIN PINC
#define BUTTON_DDR DDRC
#define BUTTON1 PC1
#define BUTTON2 PC2
#define BUTTON3 PC3

// DURATION IS IN MILLISECONDS
#define DURATION 500

// FREQUENCIES OF THE NOTES
#define C5 523.250
#define D5 587.330
#define E5 659.250
#define F5 698.460
#define G5 783.990
#define A5 880.00
#define B5 987.770
#define C6 1046.500

// Exercise 1  Preparations to play
int button_pushed = 0;
ISR(PCINT1_vect)
{
    if (isAnyClicked())
    {
        if (bit_is_clear(BUTTON_PIN, BUTTON1))
        {
            button_pushed = 1;
        }
    }
}
// Exercise 2: Seeding the random generator
int counter = 0;

// Exercise 3: Creating the random series
void generatePuzzle(uint8_t arr[], uint8_t arrlen)
{
    int i = 0;
    while (i < arrlen)
    {
        int randm = (rand() % 3) + 1;
        arr[i] = randm;
        i++;
    }
}
void printPuzzle(uint8_t arr[], uint8_t arrlen)
{
    printf("[");
    for (uint8_t i = 0; i < arrlen; i++)
    {
        printf("%d ", arr[i]);
    }
    printf("]\n");
}

// Exercise 4: Playing the puzzle
void playPuzzle(uint8_t arr[], uint8_t arrlen)
{
    generatePuzzle(arr, arrlen);
    for (uint8_t i = 0; i < arrlen; i++)
    {
        lightUpLed(arr[i] - 1);
        _delay_ms(DELAY);
        lightDownLed(arr[i] - 1);
        _delay_ms(DELAY);
    }
}
// Exercise 5: Reading the input of the user
void readInput(uint8_t arr[], uint8_t arrlen)
{
    playPuzzle(arr, arrlen);
    int button_pushed;
    uint8_t i = 0;
    uint8_t userInput[arrlen];
    printf("Press %d buttons to match the leds !!! \n", arrlen);
    while (i < arrlen)
    {
        button_pushed = 0;

        while (button_pushed == 0)
        {
            if (isAnyClicked())
            {
                _delay_ms(100);
                if (bit_is_clear(BUTTON_PIN, BUTTON1))
                {
                    button_pushed = 1;
                    userInput[i] = 1;
                    _delay_ms(DELAY);
                }
                else if (bit_is_clear(BUTTON_PIN, BUTTON2))
                {
                    button_pushed = 1;
                    userInput[i] = 2;
                    _delay_ms(DELAY);
                }
                else if (bit_is_clear(BUTTON_PIN, BUTTON3))
                {
                    button_pushed = 1;
                    userInput[i] = 3;
                    _delay_ms(DELAY);
                }

                // checking if user input matches with the random generated led numbers
                if (arr[i] == userInput[i])
                {
                    printf("You pressed button %d, correct!!! \n", userInput[i]);

                    // correct (LED4 flashes a few times on and off)
                    for (size_t i = 0; i < 5; i++)
                    {
                        lightUpMultipleLeds(8);
                        _delay_ms(100);
                        lightDownMultipleLeds(8);
                        _delay_ms(100);
                    }
                }
                else
                {
                    printf("You pressed button %d, wrong!!! \n", userInput[i]);

                    // print the correct led pattern
                    printf("The correct pattern was: ");
                    printPuzzle(arr, arrlen);
                    printf("Press Reset Button to play again!!! \n");

                    // wrong! (all 4 LEDs will turn on).
                    lightUpMultipleLeds(15);
                    _delay_ms(100);
                    button_pushed = 0;
                }
            }
        }
        i++;
    }
}

int main()
{
    initUSART();
    initButton();
    initDisplay();
    enableBTNinterrupt();
    sei();

    while (button_pushed == 0)
    {
        ledflashes(8, 100);
        counter++;
    }
    cli();
    srand(counter);
    // printf("%d\n", counter);

    _delay_ms(DELAY);
    uint8_t arrlen = 10;
    uint8_t arr[arrlen];
    int level = 1;
    while (1)
    {
        printf("\n");
        printf("=====================\n");
        printf("|       Level %d     |\n", level);
        printf("=====================\n");

        writeNumber(level);
        readInput(arr, level);
        level++;
        if (level == 11)
        {
            printf("Congratulations, you are the Simon Master!!! \n");
            enableBuzzer();
            // uint32_t frequencies[] = {C5, D5, E5, F5, G5, A5, B5, C6}; // do-re-mi...
            uint32_t frequencies[] = {C6, B5, C6, B5, A5, G5, F5, E5, F5, E5, D5, C5}; // do-re-mi...

            for (int i = 0; i < 3; i++)
            {
                for (int note = 0; note < 12; note++)
                {
                    playTone(frequencies[note], DURATION);
                }
            }
            disableBuzzer();
            button_pushed = 0;
        }
    }
    return 0;
}