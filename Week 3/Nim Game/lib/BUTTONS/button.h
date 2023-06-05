#define BUTTON_PORT PORTC
#define BUTTON_PIN PINC
#define BUTTON_DDR DDRC
#define BUTTON1 PC1
#define BUTTON2 PC2
#define BUTTON3 PC3


void enableButton(int button);
int buttonPushed(int button);
int buttonReleased(int button);
int isAnyClicked();
void initButton();
void enableBTNinterrupt();





/*
#ifndef _BUTTON_H_
#define _BUTTON_H_

// Enable the specified button for input
void enableButton(int button);

// Check if the button is currently pushed
int buttonPushed(int button);

// Check if the button is currently released
int buttonReleased(int button);

#endif
*/