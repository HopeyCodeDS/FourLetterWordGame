#include <display.h>
#include <util/delay.h>
#include <stdio.h>

// #include <usart.h>

int main()
{
  initDisplay();
  
  char str[] ="book";

  writeStringAndWait(str,10000);
  return  0;
}