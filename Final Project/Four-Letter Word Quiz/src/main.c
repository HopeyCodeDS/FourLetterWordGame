#define __DELAY_BACKWARD_COMPATIBLE__
#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>
#include <display.h>
#include <usart.h>
#include <time.h>
#include <LED.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <buzzer.h>
#include <button.h>


#define MAX_COUNTER_VALUE 65535
#define PRESCALER 1024
#define BUTTON_PORT PORTC
#define BUTTON_PIN PINC
#define LED_DDR DDRB
#define BUTTON_DDR DDRC
#define BUTTON1 PC1
#define BUTTON2 PC2
#define BUTTON3 PC3


// Arrays of words for each category
const char animalWords[20][4] = {"Duck", "Lion", "Bear", "Frog", "Goat", "Fish", "Bird", "Deer", "Crab", "Worm",
                           "Seal", "Wolf", "Puma", "Mule", "Shad", "Moth", "Swan", "Orca", "Toad", "Lamb"};
const char humanWords[20][4] = {"John", "Mary", "Kate", "Alex", "Mark", "Luis", "Anna", "Paul", "Sara", "Adam",
                          "Lisa", "Eric", "Mike", "Lily", "Jake", "Tina", "Rose", "Juan", "Kyle", "Emma"};
const char countryWords[20][4] = {"Demk", "Keny", "Germ", "Ital", "Fran", "Cand", "Aust", "Japn", "Norw", "Russ",
                             "Peru", "Cuba", "Finl", "Cong", "Port", "Tanz", "Fiji", "Turk", "Nigr", "Belg"};
const char thingWords[20][4] = {"Book", "Belt", "Desk", "Lamp", "Fork", "Shoe", "Bell", "Coin", "Ring", "Bike",
                            "Sand", "Vest", "Bulb", "Ball", "Comb", "Dice", "Flag", "Milk", "Acid", "Soap"};                                                       

// General Game Information
void gameInformation()
{
  printString("\nThe Four-Letter Word Quiz is a word guessing game where players\nchoose a category and try to guess a 4-letter word.\nThe game uses a 4-digit display shield and buttons for input.\nPlayers set letters and aim to complete the word within a minute.\n\n\n");
  printString("Success is rewarded with a happy tune, while incorrect guesses prompt a buzzer sound.\nThe game tracks attempts, category information, and elapsed time.\n\n");
  printString("Keys: \n");
  printString("\t\t\t\tButton S1(First button) ------ To display all categories and guess the missing letter\n\t\t\t\tButton S2(Second button) ------ To select/confirm categories\n\t\t\t\tButton S3(Third button) ------ To confirm the guessed word\n");
  printString("The categories are:\n");
}

// Global variables
const char arrayOfLetters[] = {'A','B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'};                                                         
int categoryCount = 0;
int alphabetCounter = 0;
bool isGamePlayed = true;
int isWordSet = 1;
int isWordSelected = 0;
int rightOrderedLetters[] = {1,0,1,1};
int selectedLetterIndex = 0;
int wordIndex = 0;
int optionLetter = 0;
int minutesElapsed = 0;
char wordArray[4][1] = {" "," "," "," "};
int indexOfArray = 0;



// Struct to hold puzzle information
struct PUZZLE
{
  char* category[10];
  char word[4][1];
  int attempts;
  int time;
};

struct PUZZLE currentPuzzle;
struct PUZZLE* puzzlePtr = &currentPuzzle;


// Function prototypes
void gameInformation();
void displayCategories(int categoryCounter);
void displayWord(char* word);
void checkIfWordGuessed();
void appendLetterToWordArray(volatile char* letter);
void initTimer();
void endTimer();
void setupInterrupts();
void playLongLowTone();
void playHappyTune();


// Display the categories
void displayCategories(int categoryCounter)
{
  if(categoryCounter == 1)
  {
    printf("Animal\n");

  }
  else if(categoryCounter == 2)
  {
    printf("Human\n");
  }
  else if(categoryCounter == 3){
    printf("Country\n");
  }
  else{
    printf("Things\n");
  }
}

// Display the word Array
void displayWord(char* word)
{
  writeStringAndWait(word,100);
}

// Check the word that was guessed 
void checkIfWordGuessed()
{
  int isWordGuessed = 0;
  int letterCounter = 0;
  int resetIndex = 0;

  while(letterCounter < 4)
  {
    if(tolower(wordArray[letterCounter][0]) != tolower(puzzlePtr->word[letterCounter][0]))
    {
        wordArray[letterCounter][0] = ' ';
        resetIndex++;
        isWordGuessed = 1;

        if(resetIndex == 1)
        {
            wordIndex = letterCounter;
        }
    }
    else
    {
      rightOrderedLetters[letterCounter] = 0;
    }
    letterCounter++;
  }

  enableBuzzer(); // Called from the buzzer library
  if(isWordGuessed == 0)
  {
    playHappyTune();
    printf("\nCorrect!!!\n");
    printf("\n\nIn category %s the word ",puzzlePtr->category);
    for(int x = 0; x < 4; x++)
    {
      printf("%c",puzzlePtr->word[x][0]);
    }
    printf(" was guessed in %d attempts and a total time of %d minutes and %d seconds\n", puzzlePtr->attempts, minutesElapsed, puzzlePtr->time);
    printf("If you want to play again, press button S1 to show the categories and then you guess new word!\n");
    isWordSelected = 0;
    isWordSet = 1;

    // Free the allocated memory for the category
    free(puzzlePtr->category[10]);

    // Resetting the variables and arrays
    wordArray[0][0] = ' ';
    wordArray[1][0] = ' ';
    wordArray[2][0] = ' ';
    wordArray[3][0] = ' ';
    rightOrderedLetters[0] = 1;
    rightOrderedLetters[2] = 1;
    rightOrderedLetters[3] = 1;
    wordIndex = 0;
    categoryCount = 0;
    endTimer();
    initTimer();
  }
  else{
    playLongLowTone();
    puzzlePtr->attempts++;
  }
   alphabetCounter = 0;
}

// Set the letters in the word array and dynamically allocate memory space per category
void setSecretWord(int categoryCount)
{
  

  if(categoryCount == 1){             
    wordArray[1][0] = animalWords[indexOfArray][1];

    puzzlePtr->category[10] = malloc(strlen("ANIMAL"));
    strcpy(puzzlePtr->category,"ANIMAL");
                
    for(int x = 0; x < 4; x++)
    {
      puzzlePtr->word[x][0] = animalWords[indexOfArray][x];
    }

  }
  else if(categoryCount == 2){ 
    wordArray[1][0] = humanWords[indexOfArray][1];

    puzzlePtr->category[10] = malloc(strlen("HUMAN"));
    strcpy(puzzlePtr->category,"HUMAN");

    for(int x = 0; x < 4; x++)
    {
       puzzlePtr->word[x][0] = humanWords[indexOfArray][x];
    }
  }
  else if(categoryCount == 3){ 
    wordArray[1][0] = countryWords[indexOfArray][1];

    puzzlePtr->category[10] = malloc(strlen("COUNTRY"));
    strcpy(puzzlePtr->category,"COUNTRY");

    for(int x = 0; x < 4; x++)
    {
       puzzlePtr->word[x][0] = countryWords[indexOfArray][x];
    }
  }
  else{
    wordArray[1][0] = thingWords[indexOfArray][1];

    puzzlePtr->category[10] = malloc(strlen("THING"));
    strcpy(puzzlePtr->category,"THING");

    for(int x = 0; x < 4; x++)
    {
       puzzlePtr->word[x][0] = thingWords[indexOfArray][x];
    }
  }
}

// Function to add a word to the
void appendLetterToWordArray(volatile char* letter)
{
  *letter = arrayOfLetters[alphabetCounter];
  wordArray[wordIndex][0] = *letter;
  alphabetCounter = alphabetCounter + 1;
  

  if(alphabetCounter == 26)
  {
    alphabetCounter = 0;
  }
}


void initTimer()
{
    DDRB |= _BV(DDB5);//This is to enable PORTB5. This line sets the fifth pin (bit 5) of the DDRB register to 1, which configures it as an output pin.
    TCCR1B = ((1 << CS10) | ((1 << CS12))); //This line sets the Timer/Counter Control Register 1B (TCCR1B) to configure the timer settings. Setting the fast mode. 1024 as prescaler
    OCR1A = 15625; // 16000000 /1024... The OCR1A register is used to set the compare match value for Timer/Counter 1. 
    TIMSK1 = (1 << TOIE1); // this enables the timer 1 overflow interrupt.
    sei();  // Enable Global interrupt
}

void endTimer()
{
  PORTB |= TCCR1B = ((0 << CS10) | ((0 << CS12)));
}

// Configure the button interrupts
void setupInterrupts()
{
  PCICR |= _BV(PCIE1);        // Activate interrupt for port C

  PCMSK1 |= _BV(BUTTON1);     // Indicate button pins that activate the ISR
  PCMSK1 |= _BV(BUTTON2);
  PCMSK1 |= _BV(BUTTON3);

  sei();                      // Enable global interrupts
}


// Define the interrupt service routine for TIMER1 overflow
ISR(TIMER1_OVF_vect)
{
    // Increment the time counter
    puzzlePtr->time++;

    // Check if the time has reached 59 seconds
    if(puzzlePtr->time == 59){
      puzzlePtr->time = 0; // Reset the time counter
      minutesElapsed++; // Increment the minutes
    }

    // Set the counter value for TIMER1
    TCNT1 = MAX_COUNTER_VALUE - (F_CPU / PRESCALER); // This is the counter for this timer. It goes from 0 to 255
    // Toggle the state of PORTB5 (bit 5 of PORTB)
    PORTB ^= _BV(PORTB5); //This light up PORTB5
}

void playHappyTune()
{
  // Define the frequency and duration of each note in the happy tune
  float notes[] = {523.25, 587.33, 659.25, 523.25, 523.25, 587.33, 659.25, 523.25, 659.25, 698.46, 659.25, 587.33, 523.25};

  // Define the duration of each note in milliseconds
  uint32_t durations[] = {500, 500, 500, 500, 500, 500, 1000, 500, 500, 500, 500, 500, 1000};

  // Play each note in the happy tune
  for (int i = 0; i < sizeof(notes) / sizeof(notes[0]); i++)
  {
    playTone(notes[i], durations[i]);
    _delay_ms(50); // Add a small delay between each note
  }
}

void playLongLowTone()
{
  float frequency = 100.0;      // Frequency of the low tone
  uint32_t duration = 3000;     // Duration of the low tone in milliseconds

  playTone(frequency, duration);
}

ISR( PCINT1_vect )
{
    if ( bit_is_clear( BUTTON_PIN, BUTTON1 ))
    {
        _delay_us( 100 );
        
        if ( bit_is_clear( BUTTON_PIN, BUTTON1 ))
        {

          if(isWordSelected == 0)
          {
            endTimer();
            srand((int)puzzlePtr->time);
            puzzlePtr->attempts = 1;
            categoryCount++;
            if(categoryCount == 5)
            {
              categoryCount = 1;
            }

          displayCategories(categoryCount);
          }
          else
          {
              optionLetter = 1;
              selectedLetterIndex = 1;
          }
        }       
    }

    if(bit_is_clear(BUTTON_PIN, BUTTON2))
    {
       _delay_us( 100 );

        if ( bit_is_clear( BUTTON_PIN, BUTTON2 ))
        {

          if(isWordSelected == 0 && isWordSet == 1)
          {
            initTimer();
            puzzlePtr->time = 0;
            minutesElapsed = 0;
            isWordSelected = 1;
          }
          else
          {
             printf("\nPress button 1 to see the letter. Press the button 3 to confirm\n");
          }
        }
    }


    if ( bit_is_clear( BUTTON_PIN, BUTTON3 ))
    {

      _delay_us( 100 );

      if ( bit_is_clear( BUTTON_PIN, BUTTON3 ) )
      {

        if(selectedLetterIndex ==1)
        {
          selectedLetterIndex = 0;
        
          if(wordIndex == 0  && rightOrderedLetters[2] == 1){
            wordIndex = 2;
          }
          else if(wordIndex == 0  && rightOrderedLetters[2] == 0 && rightOrderedLetters[3] == 1)
          {
            wordIndex = 3;
          }
          else if(wordIndex == 2 && rightOrderedLetters[3] == 1)
          {
            wordIndex = 3;
          }
          else{
            wordIndex = 4;
          }

        }
        else{
          printf("\nSelect a category.\n");
        }
        alphabetCounter = 0;
      }
    }
    
}


int main()
{
  initTimer();
  initUSART();
  initDisplay();
  initButton();
  setupInterrupts();
  puzzlePtr->attempts = 1;
  gameInformation();

  // Game about to begin!
  char playDisplay[] = "PLAY";
  writeStringAndWait(playDisplay, 4000);

  char categoryDisplay[] = "CAT?";
  writeStringAndWait(categoryDisplay, 6000);


  while(isGamePlayed){

    displayWord(wordArray);

    if(isWordSelected == 1 && isWordSet == 1)
    {
      isWordSet = 0;
      indexOfArray = rand()%20 + 1;

      printf("\n");

            
      setSecretWord(categoryCount);
      for(int x = 0; x < 4; x++)
      {
        printf(" %c",puzzlePtr->word[x][0]);
      }
      
    }

     if(optionLetter == 1)
     {
        char letter;
        appendLetterToWordArray( &letter );

        optionLetter = 0;
      
     }

     if(wordIndex == 4)
     {
       checkIfWordGuessed();
     }
  }
  return 0;
}
