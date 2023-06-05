#include <util/delay.h>
#include <avr/io.h>
#include <usart.h>


#define MAX 5
#define SEVENFOLD(num) num*7

void printArray(int arr[])
{
    printf("Content of array: \n");
  for (int i = 0; i < MAX; i++)
    printf("Address: %p has value: %d\n", &arr[i], arr[i]);
}

void makeArray(int arr[])
{
  for (int i = 0; i < MAX; i++)
  {
    arr[i] = SEVENFOLD(i);
  }
}

int main()
{
  initUSART();
  int numbers[MAX] = {0};
  printArray(numbers);
  makeArray(numbers);
  printArray(numbers);
  return 0;
}
