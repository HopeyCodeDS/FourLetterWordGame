#include <scrolling_numbers.h>
#include <util/delay.h>


void scrollingNumbersOne(int value)
{
    printf("\n");

    for (int i = 0; i < 10; i++)
    {
        if (value == 7900)
        {
            value = 7890;
        }
        else if (value == 9001)
        {
            value = 8901;
        }

        else if (value == 10012)
        {
            value = 9012;
        }

        writeNumberAndWait(value, 1000);
        printf("%d \n", value);

        value += 1111;
    }
}

void scrollingNumbersTwo(uint8_t firstValue, uint8_t secondValue, uint8_t thirdValue, uint8_t fourthValue)
{
    printf("\n");
    for (uint8_t i = 0; i < 10; i++)
    {
        if (firstValue == 10)
        {
            firstValue = 0;
        }
        else if (secondValue == 10)
        {
            secondValue = 0;
        }
        else if (thirdValue == 10)
        {
            thirdValue = 0;
        }
        else if (fourthValue == 10)
        {
            fourthValue = 0;
        }
        printf("%d %d %d %d\n", firstValue, secondValue, thirdValue, fourthValue);
        for (int i = 0; i < 1000; i++)
        {
            writeNumberToSegment(0, firstValue);
            writeNumberToSegment(1, secondValue);
            writeNumberToSegment(2, thirdValue);
            writeNumberToSegment(3, fourthValue);
        }
        firstValue++;
        secondValue++;
        thirdValue++;
        fourthValue++;
    }
}