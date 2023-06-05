#include <display.h>
#include <avr/io.h>
#include <util/delay.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>

#define SPACE 0xFF
#define DOT 0x7F

/* Segment byte maps for numbers 0 to 9 */
const uint8_t SEGMENT_MAP[] = {0xC0, 0xF9, 0xA4, 0xB0, 0x99,
                               0x92, 0x82, 0xF8, 0X80, 0X90};

/* Byte maps to select digit 1 to 4 */
const uint8_t SEGMENT_SELECT[] = {0xF1, 0xF2, 0xF4, 0xF8};

const uint8_t ARROWS_MAP[] = {0xFE, 0xBF, 0xF7};
const uint8_t SQUARE_MAP[] = {0x9C, 0xA3, 0xF7};


void initDisplay() {
  sbi(DDRD, LATCH_DIO);
  sbi(DDRD, CLK_DIO);
  sbi(DDRB, DATA_DIO);
}

// loop through seven segments of LED display and shift the correct bits in the
// data register
void shift(uint8_t val, uint8_t bitorder) {
  uint8_t bit;

  for (uint8_t i = 0; i < NUMBER_OF_SEGMENTS; i++) {
    if (bitorder == LSBFIRST) {
      bit = !!(val & (1 << i));
    } else {
      bit = !!(val & (1 << (7 - i)));
    }
    // write bit to register
    if (bit == HIGH)
      sbi(PORTB, DATA_DIO);
    else
      cbi(PORTB, DATA_DIO);

    // Trigger the clock pin so the display updates
    sbi(PORTD, CLK_DIO);
    cbi(PORTD, CLK_DIO);
  }
}

//Writes a digit to a certain segment. Segment 0 is the leftmost.
void writeNumberToSegment(uint8_t segment, uint8_t value) {
  cbi(PORTD, LATCH_DIO);
  shift(SEGMENT_MAP[value], MSBFIRST);
  shift(SEGMENT_SELECT[segment], MSBFIRST);
  sbi(PORTD, LATCH_DIO);
}

//Writes a number between 0 and 9999 to the display. To be used in a loop...
void writeNumber(int number) {
  if (number < 0 || number > 9999) return;
  writeNumberToSegment(0, number / 1000);
  writeNumberToSegment(1, (number / 100) % 10);
  writeNumberToSegment(2, (number / 10) % 10);
  writeNumberToSegment(3, number % 10);
}

//Writes a number between 0 and 9999 to the display and makes sure that it stays there a certain number of millisecs.
//Note: the timing is approximate; the amount of time writeNumberToSegment takes is not accounted for...
void writeNumberAndWait(int number, int delay) {
  if (number < 0 || number > 9999) return;
  for (int i = 0; i < delay / 20; i++) {
    writeNumberToSegment(0, number / 1000);
    _delay_ms(5);
    writeNumberToSegment(1, (number / 100) % 10);
    _delay_ms(5);
    writeNumberToSegment(2, (number / 10) % 10);
    _delay_ms(5);
    writeNumberToSegment(3, number % 10);
    _delay_ms(5);
  }
}

// Blanks a certain segment. Segment 0 is the leftmost.
void blankSegment(uint8_t segment)
{
  cbi(PORTD, LATCH_DIO);
  shift(0xFF, MSBFIRST);
  shift(SEGMENT_SELECT[segment], MSBFIRST);
  sbi(PORTD, LATCH_DIO);
}

const uint8_t ALPHABET_MAP[] = {0x88, 0x83, 0xC6, 0xA1, 0x86, 0x8E, 0xC2,
                                0x89, 0xCF, 0xE1, 0x8A, 0xC7, 0xEA, 0xC8,
                                0xC0, 0x8C, 0x4A, 0xCC, 0x92, 0x87, 0xC1,
                                0xC1, 0xD5, 0x89, 0x91, 0xA4};


/*
void writeCharToSegment(uint8_t segment, char character) 
{

  if(isalpha(character) != 0)
  {
      int value = character;

    if(islower(character))
    {
      value = character - 32;
    }
    
    int index = value - 65;
    cbi(PORTD, LATCH_DIO);
    shift(ALPHABET_MAP[index], MSBFIRST);
    shift(SEGMENT_SELECT[segment], MSBFIRST);
    sbi(PORTD, LATCH_DIO);
  }
  else if(isalpha(character) == 0)
  {
    cbi(PORTD, LATCH_DIO);
    shift(SPACE, MSBFIRST);
    shift(SPACE, MSBFIRST);
    sbi(PORTD, LATCH_DIO);
  }
}
*/

void writeCharToSegment(uint8_t segment, char character)
{
  // Convert lowercase to uppercase
  if (character >= 'a' && character <= 'z')
  {
    character -= 32;
  }

  // Check if character is a letter from the alphabet
  /*For example, if the input character is 'D', its ASCII value is 68.
  Subtracting the ASCII value of 'A' (65) from 68 gives an index of 3.
  Therefore, the function retrieves the value at the 3rd index of the ALPHABET_MAP
  array and displays it on the segment specified by the input segment.*/
  uint8_t index = character - 'A';  //you do ASCII val of index - ASCII val of A (ASCII of A is 65)
  if (index > 25) //65-90 are the ASCII values of A-Z, so if the index is greater than 25, the character is a space
  {
    index = 26; // Space character
  }

  cbi(PORTD, LATCH_DIO); // set latch pin low allowing us to write to the registers
  shift(ALPHABET_MAP[index], MSBFIRST); // shift the bits out to the shift register, The shift function is called
  //with the value in the ALPHABET_MAP array at the index corresponding to the character, and MSBFIRST as the bit order.
  shift(SEGMENT_SELECT[segment], MSBFIRST); // shift the bits out to the shift register, The shift function is called again
  //with the value in the SEGMENT_SELECT array at the index corresponding to the segment to be displayed,
  //and MSBFIRST as the bit order.
  sbi(PORTD, LATCH_DIO); // set latch pin high to lock the register and send the data to the display

}


void writeFourLetterWord(char *str)
{
  writeCharToSegment(0, *str);
  writeCharToSegment(1, *(str + 1));
  writeCharToSegment(2, *(str + 2));
  writeCharToSegment(3, *(str + 3));
}
void writeStringAndWait(char* str, int delay)
{
  for (int i = 0; i < delay / 20; i++)
  {
    writeCharToSegment(0, str[0]);
    _delay_ms(5);
    writeCharToSegment(1, str[1]);
    _delay_ms(5);
    writeCharToSegment(2, str[2]);
    _delay_ms(5);
    writeCharToSegment(3, str[3]);
    _delay_ms(5);
    // cbi(PORTD, LATCH_DIO);
    // shift(SPACE, MSBFIRST);
    // shift(SEGMENT_SELECT[3], MSBFIRST);
    // sbi(PORTD, LATCH_DIO);
  }
}
void writeString(char* str)
{
  writeCharToSegment(0, str[0]);    
  writeCharToSegment(1, str[1]); 
  writeCharToSegment(2, str[2]);  
  writeCharToSegment(3, str[3]);  
}
// made purposely for the nim game.
// turns segment 1 off
/*
void turnOffSegment(int number)
{
  cbi(PORTD, LATCH_DIO);
  shift(SPACE, MSBFIRST);
  shift(SEGMENT_SELECT[number], MSBFIRST);
  sbi(PORTD, LATCH_DIO);
}
void displayArrow(uint8_t segment, uint8_t arrloc)
{
  cbi(PORTD, LATCH_DIO);
  shift(ARROWS_MAP[arrloc], MSBFIRST);
  shift(SEGMENT_SELECT[segment], MSBFIRST);
  sbi(PORTD, LATCH_DIO);
}
void writeDotToSegment(uint8_t segment)
{
  for (int i = 0; i < segment; i++)
  {
    cbi(PORTD, LATCH_DIO);
    shift(DOT, MSBFIRST);
    shift(SEGMENT_SELECT[i], MSBFIRST);
    sbi(PORTD, LATCH_DIO);
  }
}

// Define the port and pin connections for the display
#define DISPLAY_PORT PORTD
#define DISPLAY_DDR DDRD
#define BUTTON1 PC1
#define BUTTON2 PC2
#define BUTTON3 PC3

// Define the segment byte maps for clearing the display
// Modify these values based on your specific display module
const uint8_t SEGMENT_CLEAR_MAP[4] = {
  0xFF,  // Clear segment A
  0xFF,  // Clear segment B
  0xFF,  // Clear segment C
  0xFF   // Clear segment D
};

void clearDisplay() {
  // Set the display pins as output
  DISPLAY_DDR = 0xFF;

  // Write the segment byte maps to clear the display
  for (int i = 0; i < 4; i++) {
    DISPLAY_PORT = ~(1 << i);
    DISPLAY_PORT = SEGMENT_CLEAR_MAP[i];
  }

  // Disable all segments and turn off the display
  DISPLAY_PORT = 0xFF;
}

// Function to check if a character is a vowel
int isVowel(char c) {
  c = tolower(c);
  return (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u');
}

// Function to initialize the timer
// void initTimer() {
//   // Configure Timer1 for CTC mode
//   TCCR1B |= (1 << WGM12);

//   // Enable timer compare interrupt
//   TIMSK1 |= (1 << OCIE1A);

//   // Set the compare value for 1 second delay
//   OCR1A = 15624;

//   // Set the prescaler to 256 and start the timer
//   TCCR1B |= (1 << CS12);
// }

// Function to get the current segment based on the button pressed
uint8_t getCurrentSegment() {
  uint8_t segment = 0;

  if (buttonPushed(BUTTON1))
    segment = 1;
  else if (buttonPushed(BUTTON2))
    segment = 2;
  else if (buttonPushed(BUTTON3))
    segment = 3;

  return segment;
}

// Function to read the character from the current segment
char readCharFromSegment(uint8_t segment) {
  // Segment byte map for each digit
  const uint8_t SEGMENT_BYTE_MAP[4] = {
    0x7E,  // Segment A - 0b01111110
    0xDB,  // Segment B - 0b11011011
    0xE7,  // Segment C - 0b11100111
    0xFF   // Segment D - 0b11111111 (No Segment)
  };

  // Read the segment byte and check which character is displayed
  uint8_t segmentByte = SEGMENT_BYTE_MAP[segment];
  if (segmentByte == 0x7E)
    return 'A';
  else if (segmentByte == 0xDB)
    return 'B';
  else if (segmentByte == 0xE7)
    return 'C';
  else
    return '_';  // Placeholder for no character displayed
}




// void writeDotToSegmentAndWait(uint8_t segment, int delay)
// {
//   for (int i = 0; i < delay / 20; i++)
//   {
//     writeDotToSegment(0);
//     _delay_ms(5);
//     writeDotToSegment(1);
//     _delay_ms(5);
//     writeDotToSegment(2);
//     _delay_ms(5);
//     writeDotToSegment(3);
//     _delay_ms(5);
//   }
// }
*/


// #include "display.h"
// #include <avr/io.h>
// #include <util/delay.h>
// #include <string.h>



// /* Segment byte maps for numbers 0 to 9 */

// const uint8_t SEGMENT_MAP[] = {0xC0, 0xF9, 0xA4, 0xB0, 0x99,

//                                0x92, 0x82, 0xF8, 0X80, 0X90};




// /* Byte maps to select digit 1 to 4 */

// const uint8_t SEGMENT_SELECT[] = {0xF1, 0xF2, 0xF4, 0xF8};




// const uint8_t ALPHABET_MAP[] = {0x88, 0x83, 0xC6, 0xA1, 0x86, 0x8E, 0xC2,

//                                 0x89, 0xCF, 0xE1, 0x8A, 0xC7, 0xEA, 0xC8,

//                                 0xC0, 0x8C, 0x4A, 0xCC, 0x92, 0x87, 0xC1,

//                                 0xC1, 0xD5, 0x89, 0x91, 0xA4, 0xF7};




// void initDisplay() {

//   sbi(DDRD, LATCH_DIO);

//   sbi(DDRD, CLK_DIO);

//   sbi(DDRB, DATA_DIO);

// }




// // loop through seven segments of LED display and shift the correct bits in the

// // data register

// void shift(uint8_t val, uint8_t bitorder) {

//   uint8_t bit;




//   for (uint8_t i = 0; i < NUMBER_OF_SEGMENTS; i++) {

//     if (bitorder == LSBFIRST) {

//       bit = !!(val & (1 << i));

//     } else {

//       bit = !!(val & (1 << (7 - i)));

//     }

//     // write bit to register
//     if (bit == HIGH)

//       sbi(PORTB, DATA_DIO);
//     else
//       cbi(PORTB, DATA_DIO);

//     // Trigger the clock pin so the display updates
//     sbi(PORTD, CLK_DIO);
//     cbi(PORTD, CLK_DIO);

//   }

// }




// //Writes a digit to a certain segment. Segment 0 is the leftmost.

// void writeNumberToSegment(uint8_t segment, uint8_t value) {

//   cbi(PORTD, LATCH_DIO);

//   shift(SEGMENT_MAP[value], MSBFIRST);

//   shift(SEGMENT_SELECT[segment], MSBFIRST);

//   sbi(PORTD, LATCH_DIO);

// }




// //Writes a nuber between 0 and 9999 to the display. To be used in a loop...

// void writeNumber(int number) {
//   if (number < 0 || number > 9999) return;
//   writeNumberToSegment(0, number / 1000);
//   writeNumberToSegment(1, (number / 100) % 10);
//   writeNumberToSegment(2, (number / 10) % 10);
//   writeNumberToSegment(3, number % 10);
// }




// //Writes a number between 0 and 9999 to the display and makes sure that it stays there a certain number of millisecs.
// //Note: the timing is approximate; the amount of time writeNumberToSegment takes is not accounted for...
// void writeNumberAndWait(int number, int delay) {
//   if (number < 0 || number > 9999) return;
//   for (int i = 0; i < delay / 20; i++) {
//       writeNumberToSegment(0, number / 1000);
//       _delay_ms(1);
//       writeNumberToSegment(1, (number / 100) % 10);
//       _delay_ms(1);
//       writeNumberToSegment(2, (number / 10) % 10);
//       _delay_ms(1);
//       writeNumberToSegment(3, number % 10);
//       _delay_ms(1);
//   }

// }

// void writeCharToSegment(uint8_t segment, char letter) {
//   int value = letter;
//   int index = value -65;
//   if(97 <= value && value <= 122){
//       value = value - 32;
//   } else if(value == 95){
//     index = 26;
//   }
//   cbi(PORTD, LATCH_DIO);
//   shift(ALPHABET_MAP[index], MSBFIRST);
//   shift(SEGMENT_SELECT[segment], MSBFIRST);
//   sbi(PORTD, LATCH_DIO);
// }

// void writeString(char string[]) {
//   if(strlen(string) > 4) return;
//   for (int i = 0; i < strlen(string); i++)
//   {
//     int value = string[i];
//     if(97 <= value && value <= 122) string[i] = value - 32 + '0';
//   }
//   writeCharToSegment(0, string[0]);
//   writeCharToSegment(1, string[1]);
//   writeCharToSegment(2, string[2]);
//   writeCharToSegment(3, string[3]);
// }


// void writeStringAndWait(char string[], int delay) {
//   if(strlen(string) > 4) return;
//   for (int i = 0; i < delay / 20; i++) {
//     writeCharToSegment(0, string[0]);
//     _delay_ms(1);
//     writeCharToSegment(1, string[1]);
//     _delay_ms(1);
//     writeCharToSegment(2, string[2]);
//     _delay_ms(1);
//     writeCharToSegment(3, string[3]);
//     _delay_ms(1);
//   }

// }