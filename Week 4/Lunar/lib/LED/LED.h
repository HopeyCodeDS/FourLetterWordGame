#include <stdint.h>


// Function to enable one LED
void enableLed ( int lednumber );

// Function to light up a LED
void lightUpLed ( int lednumber );

// Function to switch off a LED
void lightDownLed ( int lednumber );

// Function to enable multiple LEDs
void enableMultipleLeds ( uint8_t leds);

// Function to light up multiple LEDs
void lightUpMultipleLeds ( uint8_t leds);

void lightDownMultipleLeds(uint8_t leds);

// Function to enable all LEDs
void enableAllLeds ();

// Function to light up all LEDs
void lightUpAllLeds();

// Function to switch off all LEDs
void lightDownAllLeds();

// Function to toggle a LED
void lightToggleOneLed(int lednumber);

// Function to dim a LED
void dimLed(int lednumber, int percentage, int duration);

// Function to Fade in a LED
void fadeInLed (int led, int duration);

// Function to Fade out a LED
void fadeOutLed (int led, int duration);

void dimAllLeds(int percentage, int duration);

void flashes(uint8_t leds, uint8_t numberOfFlashes);

void ledflashes(uint8_t leds, int delay);

void lightUpLedsAndWait(int delay);