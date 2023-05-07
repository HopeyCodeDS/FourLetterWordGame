#include <util/delay.h>
#include <avr/io.h>
#include <usart.h>

void pow2(int* var){
  *var = *var * *var;
}

int main()
{
  int var = 5;
  initUSART();
  //calculate power 2 -> write as a function pow2
  pow2(&var);
  printf("Power 2 of 5 is %d", var);
}

