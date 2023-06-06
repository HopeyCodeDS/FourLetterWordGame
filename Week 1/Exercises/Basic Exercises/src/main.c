#define __DELAY_BACKWARD_COMPATIBLE__
#include <util/delay.h>
#include <avr/io.h>
#include <stdio.h>
#include <stdlib.h>
#include <usart.h>
#include <LED.h>
#include <string.h>

#define LED_PORT PORTB
#define NUMBER_OF_LED 4


void getLedno(uint8_t lednum)
{

  if (lednum < 0 || lednum > NUMBER_OF_LED - 1)
      return;
  enableLed(lednum);
  lightUpLed(lednum);
  _delay_ms(1000);
  lightDownLed(lednum);
  _delay_ms(1000);
}

void noOfFlashes(uint8_t numberOfFlashes)
{
  for (uint8_t i = 0; i < numberOfFlashes; i++)
  {
    enableAllLeds(); 
    lightUpAllLeds();
    _delay_ms(200);
    lightDownAllLeds();
    _delay_ms(200);
  }
}

int main() {
  initUSART();

  while(1)
  {
      // 1. Write a program to light up the 4 LEDs of the shield consecutively
      for (uint8_t i = 0; i < 10; i++)
      {
          lightUpMultipleLeds(15);
          _delay_ms(500);
          lightDownMultipleLeds(15);
          _delay_ms(500);
      }

      // 2. Write a program to make one of the LEDs light exactly 10 times
      for(int i = 0; i < 10; i++)
      {
        
        enableLed(2);   // enable the led

        lightUpLed(2);
        _delay_ms(500);
        lightDownLed(2);
        _delay_ms(500);
      }

      // 3. Turn exercise 2 into a function, where the parameters are the LED number and the number of flashes. (add the function to your library)

      flashes(10,20);

      // 4. Write a program that makes an LED burn longer and longer (start with 10 ms and rise up to 1000 ms, incrementing with 50 ms in every iteration.
      for (int sec = 0; sec <= 1000; sec += 50)
      {
          enableLed(1);

          lightUpLed(1);
          _delay_ms(sec);
          lightDownLed(1);
          _delay_ms(sec);
      }

      // 5. Write a program that uses an array of 10 digits between 0 and 4. Walk through the array and light up the corresponding led.
      uint8_t digits[10];
      srand(0);
      for (uint8_t i = 0; i < 10; i++)
      {
          digits[i] = rand() % 4;
          printf("digits[%d] = %d\n", i, digits[i]);

          enableLed(digits[i]);

          lightUpLed(digits[i]);
          _delay_ms(500);
          lightDownLed(digits[i]);
          _delay_ms(500);

      }

      // 6. Write a program that creates an empty array and then fills it with the numbers from 10 to 1000 with a step value of 50 ms. 
      // Use this array to light up a led, just like in exercise 4.
      int ledSteps[10];
      for (int sec = 0; sec <= 1000; sec += 50)
      {
          enableLed(1);

          ledSteps[sec] = sec;
          printf("%d\n", ledSteps[sec]);

          lightUpLed(1);
          _delay_ms(sec);
          lightDownLed(1);
          _delay_ms(sec);
      }


      // 7. Write a program that makes the leds light up based on a string. The string contains a sequence of 
      // the letters a, b, c and d. An 'a' lights the first LED, 'b' the second and so on.

      char fourLetter[] = "abcd";
      for (int i = 0; i < NUMBER_OF_LED; i++)
      {
          switch (fourLetter[i])
          {
          case 'a':
              getLedno(0);
              break;
          case 'b':
              getLedno(1);
              break;
          case 'c':
              getLedno(2);

              break;
          case 'd':
              getLedno(3);
              break;
          default:
              break;
          }
      }

      // 8. Write a program that lights up the leds based on a string. But this time the string of letters is random. 
      // With the letter 'a' all 4 LEDs light up once, with a letter 'b' they light up twice, and so on.
      char alph[] = "abcd";
      char random_letters[] = "";
      for (int i = 0; i < NUMBER_OF_LED; i++)
      {
          random_letters[i] = alph[rand() % 3];
          printf("%c\n", random_letters[i]);
          switch (random_letters[i])
          {
          case 'a':
              noOfFlashes(1);
              break;
          case 'b':
              noOfFlashes(2);
              break;
          case 'c':
              noOfFlashes(3);
              break;
          case 'd':
              noOfFlashes(4);
              break;
          default:
              break;
          }
          _delay_ms(2000);
      }
  }
  return 0;
}

