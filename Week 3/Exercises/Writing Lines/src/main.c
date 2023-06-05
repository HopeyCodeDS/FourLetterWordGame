#include <util/delay.h>
#include <avr/io.h>
#include <usart.h>
#include <display.h>
#include <avr/interrupt.h>
#include <led.h>
#include <adc.h>
#include <string.h>


#define BUTTON_PORT PORTC
#define BUTTON_PIN PINC
#define BUTTON_DDR DDRC
#define BUTTON1 PC1
#define BUTTON2 PC2
#define BUTTON3 PC3

int memoryValue;
void writeLinesOnHeap(char sentence[])
{
  char *sentenceLoc;
  sentenceLoc = (char *)malloc(strlen(sentence) * sizeof(char));
  memoryValue += sizeof(sentenceLoc);
  strcpy(sentenceLoc, sentence);
  // free(sentenceLoc);
}



int main()
{
  initUSART();
  BUTTON_DDR &= ~_BV(BUTTON1);
  BUTTON_PORT |= _BV(BUTTON1);

  int count = 100;

  char sentence[] = "I am not allowed to speak in class";
  while (1)
  {
    for (int i = 1; i <= count; i++)
    {
      printf("%3d : %s\n", i, sentence);
      writeLinesOnHeap("\"I am not allowed to speak in class\" is on heap\n");
      printf("%d bytes are now occupied on heap\n", memoryValue);
    }
  }
  return 0;
}