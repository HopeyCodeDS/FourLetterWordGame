#ifndef _BUTTON_H_
#define _BUTTON_H_

// Enable the specified button for input
void enableButton(int button);

// Check if the button is currently pushed
int buttonPushed(int button);

// Check if the button is currently released
int buttonReleased(int button);

#endif
