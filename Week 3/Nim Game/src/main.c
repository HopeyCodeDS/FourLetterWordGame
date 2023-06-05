#include <util/delay.h>
#include <avr/io.h>
#include <usart.h>
#include <display.h>
#include <avr/interrupt.h>
#include <led.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUTTON_PORT PORTC
#define BUTTON_PIN PINC
#define BUTTON_DDR DDRC
#define BUTTON1 PC1
#define BUTTON2 PC2
#define BUTTON3 PC3

#define maxNumber 3
#define minNumber 1


const char COMPUTER = 'C';
const char PLAYER = 'P';

int availableNumber = 21;
int button_pushed = 0;
int confirm_C = 0;
char getNextPlayer;
int availableNumber;

void gameInformation()
{
  printString("Nim is a two player game. \nAt the start of this game there are 21 matches on a table\nA player takes at least 1 or a maximum of 3 matches\nand whoever takes the last match loses the game.\n This version is played with the computer !!! \n");
}

void displayNumberAvailable(int number)
{
  if (number < 1 || number > 21)
    return;
  writeNumberToSegment(2, (number / 10) % 10);
  writeNumberToSegment(3, number % 10);
}
void displayMatchSelected(int number)
{
  if (number < minNumber || number > maxNumber)
    return;

  writeNumberToSegment(0, number);
}
void initADC()
{
  ADMUX |= (1 << REFS0);
  ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
  ADCSRA |= (1 << ADEN);
}
char getStartingPlayer(int counter)
{
  srand(counter);
  char firstPlayer;
  char players[] = {COMPUTER, PLAYER};
  firstPlayer = players[rand() % 2];
  return firstPlayer;
}
char currentPlayer(char player)
{
  char nextPlayer;
  if (player == COMPUTER)
  {
    nextPlayer = PLAYER;
  }
  else if (player == PLAYER)
  {
    nextPlayer = COMPUTER;
  }
  return nextPlayer;
}

void _ai(int availableNumber, int numberOfMatchesSelectedByPlayer)
{
  printf("\n%c Turn !!! \n", COMPUTER);
  printf("Press button 2 to confirm that it's the computer turn !!! \n");
  int checkNumberOfTimesButtonTwoIsPressed = 0;
  int counter = 0;
  sei();
  while (confirm_C == 0)
  {
    writeCharToSegment(1, COMPUTER);
    _delay_ms(500);
    turnOffSegment(1);
    _delay_ms(200);
    counter++;
  }
  cli();

  confirm_C = 1;
  getNextPlayer = COMPUTER;
  while (1)
  {

    while (bit_is_clear(BUTTON_PIN, BUTTON2) == 0)
    {
      if (checkNumberOfTimesButtonTwoIsPressed == 0)
      {
        checkNumberOfTimesButtonTwoIsPressed++;
        printf("%c is playing !!! \n", COMPUTER);
      }
      else
      {
        int com_value = (availableNumber - 1) % (maxNumber + 1);
        com_value -= numberOfMatchesSelectedByPlayer;
        if (com_value <= 0)
        {
          com_value = (rand() % 3) + 1;
        }
        if (availableNumber - com_value <= 0)
        {
          availableNumber = 0;
          printf("%c took the last match. There are %d matches left. !!!  \n%c lost !!! \n", COMPUTER, availableNumber, COMPUTER);
          printf("%c won the game!",PLAYER);
          printf("Press Reset to play again !!! \n");
          while (1)
          {
            writeString("pwon");
          }
        }

        if (availableNumber - com_value > 0)
        {
          availableNumber -= com_value;
          printf("%c has selected %d matches. There are %d matches left. !!! \n", COMPUTER, com_value, availableNumber);
        }
        player_game(availableNumber);
      }
      displayNumberAvailable(availableNumber);
      writeCharToSegment(1, getNextPlayer);
    }
  }
}
void player_game(int availableNumber)
{
  uint16_t seed;
  printf("\n%c Turn !!! \n", PLAYER);
  printf("Use the potentiometer to generate a seed !!! \n");
  printf("Press Button 1 to confirm seed !!! \n");

  sei();
  while (button_pushed == 0)
  {
    ADCSRA |= (1 << ADSC);
    loop_until_bit_is_clear(ADCSRA, ADSC);
    seed = ADC;
    writeNumber(seed);
  }
  cli();
  printf("\nPress Button 1 again to start game !!! \n");
  printf("Use Button 1 and Button 3 to select a number between 1-3 !!! \n");
  printf("Press Button 2 to confirm the number !!! \n");

  int value = maxNumber;
  while (1)
  {
    while (bit_is_clear(BUTTON_PIN, BUTTON1) == 0)
    {
      button_pushed = 1;
      displayNumberAvailable(availableNumber);
      writeCharToSegment(1, getNextPlayer);
      displayMatchSelected(value);
      if (PLAYER)
      {
        getNextPlayer = PLAYER;

        if (bit_is_clear(BUTTON_PIN, BUTTON1) || bit_is_clear(BUTTON_PIN, BUTTON2) || bit_is_clear(BUTTON_PIN, BUTTON3))
        {
          _delay_ms(100);
          if (bit_is_clear(BUTTON_PIN, BUTTON3))
          {
            value++;
            if (value > 3)
            {
              value = 3;
            }
          }
          else if (bit_is_clear(BUTTON_PIN, BUTTON1))
          {
            value--;
            if (value < 1)
            {
              value = 1;
            }
          }
          else if (bit_is_clear(BUTTON_PIN, BUTTON2))
          {
            _delay_ms(10);
            if (bit_is_clear(BUTTON_PIN, BUTTON2))
            {
              availableNumber -= value;
              printf("%c has selected %d matches. There are %d matches left. !!! \n", PLAYER, value, availableNumber);

              if (availableNumber <= 0)
              {
                availableNumber = 0;
                printf("%c took the last match !!! \n%c lost !!! \n", PLAYER, PLAYER);
                printf("Press Reset to play again !!! \n");
                while (1)
                {
                  writeString("cwon");
                }
              }
              _ai(availableNumber, value);
            }
          }
        }
      }
    }
  }
}
ISR(PCINT1_vect)
{
  if (bit_is_clear(BUTTON_PIN, BUTTON1) || bit_is_clear(BUTTON_PIN, BUTTON2))
  {
    _delay_ms(100);
    if (bit_is_clear(BUTTON_PIN, BUTTON1))
    {
      button_pushed = 1;
    }
    _delay_ms(100);
    if (bit_is_clear(BUTTON_PIN, BUTTON2))
    {
      confirm_C = 1;
    }
  }
}
int main()
{
  initDisplay();
  initUSART();
  initADC();

  BUTTON_DDR &= ~_BV(BUTTON1) | _BV(BUTTON2);
  BUTTON_PORT |= _BV(BUTTON1) | _BV(BUTTON2);
  PCICR |= _BV(PCIE1);
  PCMSK1 |= _BV(BUTTON1) | _BV(BUTTON2);

  // game Title
  char gameTitle[] = "nim";
  writeStringAndWait(gameTitle, 1000);
  gameInformation();


  ADCSRA |= (1 << ADSC);
  loop_until_bit_is_clear(ADCSRA, ADSC);
  uint8_t seed = ADC;
  writeNumber(seed);

  char firstPlayer = getStartingPlayer(seed);
  availableNumber = availableNumber;

  if (firstPlayer == PLAYER)
  {
    player_game(availableNumber);
  }
  else if (firstPlayer == COMPUTER)
  {
    _ai(availableNumber, 0);
  }
  return 0;
}

