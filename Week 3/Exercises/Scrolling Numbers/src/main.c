#include <display.h>
#include <usart.h>
#include <util/delay.h>
#include <scrolling_numbers.h>


int main()
{
    initDisplay();
    initUSART();
    while (1)
    {
        scrollingNumbersOne(123);
        // scrollingNumbersTwo(0, 1, 2, 3);
        // scrollingNumbersTwo(2, 5, 4, 9); 
    }

    return 0;
}
