#include <usart.h>
#include <button.h>
#include <avr/io.h>
#include <display.h>
#include <util/delay.h>
#include <avr/interrupt.h>

// Global variables
#define MULTIPLE 250
// Enter here your global variables
uint8_t counter = 0;
uint8_t minutes = 0;
uint8_t seconds = 0;
uint8_t button_pushed = 0;
void initTimer()
{
    // enter your code
    // TCCR2A |= _BV(WGM21);| _BV(WGM20);
    TCCR2A |= _BV(WGM22) | _BV(WGM20);  // the WAVE FORM and by consequence the Mode of Operation
    TIMSK2 |= _BV(OCIE2A);  // enable OCRA interrupt
    sei();
}
void startTimer()
{
    // enter your code
    TCCR2B |= _BV(CS22) | _BV(CS20);  // setting the prescaler
}
void stopTimer()
{
    // enter your code
    TCCR2B &= ~(_BV(CS22) | _BV(CS20));  //
}
void tick()
{
    // enter your code
    if (seconds == 60)
    {
        if (minutes == 60)
        {
            minutes = 0;
        }
        else
        {
            minutes += 1;
        }
        seconds = 0;
    }
}
void writeTimeAndWait(uint8_t minuten, uint8_t seconden, int delay)
{
    // enter your code
    for (uint8_t i = 0; i < delay; i++)
    {
        writeNumberToSegment(0, minuten / 10 % 10);
        _delay_ms(5);
        writeNumberToSegment(1, minuten % 10);
        _delay_ms(5);
        writeNumberToSegment(2, seconden / 10 % 10);
        _delay_ms(5);
        writeNumberToSegment(3, seconden % 10);
        _delay_ms(5);
    }
}
// This ISR runs every 4 ms
ISR(TIMER2_COMPA_vect)
{
    counter++;
    if ((counter + 1) % MULTIPLE == 0)
    {
        tick();
        seconds += 1;
    }
    PORTD &= ~(_BV(PD1) | _BV(PD5));
}


ISR(PCINT1_vect)
{
    if (bit_is_clear(BUTTON_PIN, BUTTON1) || bit_is_clear(BUTTON_PIN, BUTTON2) || bit_is_clear(BUTTON_PIN, BUTTON3))
    {
        _delay_ms(100);
        if (bit_is_clear(BUTTON_PIN, BUTTON1))
        {
            button_pushed = 1;
        }
        else if (bit_is_clear(PINC, PC2))
        {
            button_pushed = 0;
        }
        else if (bit_is_clear(PINC, PC3))
        {
            seconds = 0;
            minutes = 0;
        }
    }
}


int main()
{
    initTimer();
    initUSART();
    initDisplay();
    enableBTNinterrupt();
    for (uint8_t i = 0; i < 4; i++)
    {
        enableButton(i);
    }

    printf("Start the stopwatch by pressing button S1, stop by pressing button S2, and reset with S3\n");
    while (1)
    {
        if (button_pushed == 1)
        {
            startTimer();
        }
        if (button_pushed == 0)
        {
            stopTimer();
        }
        writeTimeAndWait(minutes, seconds, 4);
    }
    return 0;
}
