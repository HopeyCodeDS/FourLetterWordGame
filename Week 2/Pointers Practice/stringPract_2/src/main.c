#include <stdio.h>
#include <avr/io.h>
#include <usart.h>
#include <string.h>

#define MAX 7
#define NAME_LEN 10

void printFirstLetters(char names[][NAME_LEN], int size) {
    printf("First letters: ");
    for (int i = 0; i < size; i++) {
        printf("%c ", names[i][0]);
    }
    printf("\n");
}

void printLastLetters(char names[][NAME_LEN], int size) {
    printf("Last letters: ");
    for (int i = 0; i < size; i++) {
        printf("%c ", names[i][strlen(names[i])-1]);
    }
    printf("\n");
}

char* searchShortestName(char names[][NAME_LEN], int size) {
    char* shortest = names[0];
    for (int i = 1; i < size; i++) {
        if (strcmp(names[i], shortest) < 0) {
            shortest = names[i];
        }
    }
    return shortest;
}

int main() {
    char names[MAX][NAME_LEN] = {"Moe", "Larry", "Curly", "Champ", "Opeyemi", "Ted", "Klay"};
    printFirstLetters(names, MAX);
    printLastLetters(names, MAX);
    char* smallest = searchShortestName(names, MAX);
    printf("Smallest name: %s\n", smallest);
    return 0;
}