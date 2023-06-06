# Infrastructure 2 - Embedded System


#   Individual Project - Four Leter Word Quiz
	
	Four Letter Word Quiz is a word guessing game similar to "Hangman." However, it has some specific limitations and features. The game is designed to run on a device with a 4-display 	shield, which can only show 4 characters at a time. Therefore, the game focuses on 4-letter words.

	The objective of the game is to guess the correct 4-letter word within as few attempts as possible. The game starts with the user selecting a category: animal, human, country, or 	thing. Then, a riddle is presented to the user, where only the vowel of the word is shown, and the consonants are masked with underscores.

	The user interacts with the game using three buttons. They press button 1 to set the corresponding letter in the riddle. If they believe the letter is correct, they move on to the 	next letter. If a letter is set in each position and there is no input for 1 minute, the attempt is verified.

	If the word is guessed correctly, the display shows "YES!" accompanied by a happy tune, and the user is provided with information about the category and the word they guessed, as 	well as the number of attempts. If the word doesn't match exactly, the buzzer makes a long low tone, incorrect letters are revealed, and the guessing continues with an incremented 	attempt counter.

	The game requires the storage of at least 20 four-letter words for each category, which are randomly selected. A struct called "PUZZLE" is used to store the category, word, 	attempts, and time information. The game also includes a timer and associates interrupts with the three buttons for user interaction.



### Week 1 Exercises Done
* Libraries
* Dimmed LEDs
* LED Chaos
* Basic Exercises C

### Week 1 Projects Done
*    Morse Trainer


### Week 2 Exercises Done
* Libraries
* Pointers Practice
    * pointerPract_1
    * stringPract_2
* Timing and Chronology of the ISRs

### Week 2 Projects Done
*    Simon Says



### Week 3 xercises Done

*    Scrolling Numbers	
*    Alphabet on Display	
*    Potentiometer on Display	
*    Writing lines


### Week 3 Projects Done	

*    Nim	


### Week  4 Projects Done
	
*    My Synthesizer	
*    Wolfgang Arduino Mozart	
*    Stopwatch
	 	

### Week 4 Projects Done

*    Lunar Lander