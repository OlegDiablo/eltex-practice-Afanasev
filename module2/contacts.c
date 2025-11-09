#include "contacts.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void insertName(Contact* contact, char* value) {

    contact->name = value;

}


void insertSurname(Contact* contact, char* value) {

    contact->surname = value;

}


void insertPatronymic(Contact* contact, char* value) {

    contact->patronymic = value;

}


void insertPhoneNumber(Contact* contact, char* value, int counter) {

    char** new_phones = realloc(contact->phone_number, sizeof(char*)*(counter+1));
    contact->phone_number = new_phones;
    contact->phone_number[counter] = malloc(strlen(value) + 1);
    strcpy(contact->phone_number[counter], value);

}


void insertPosition(Contact* contact, char* value) {

    contact->position = value;

}


void printString(char* data) {

    for (int i = 0; data[i] != '*'; i++) {

        printf("%c", data[i]);
    
    }
    printf(" ");
}


char* scanInput() {
    
    char *value = calloc(1, sizeof(char));
    int n = 1;
    char input = ' ';
    while (scanf("%c", &input) == 1 && input != '\n') {

        value[n-1] = input;
        n++;
        value = realloc(value, sizeof(char) * n);

    }
    value[n] = '\0';

    return value;

}


void printContact(Contact* contact){

    printString(contact->name);

    printString(contact->surname);

    printString(contact->patronymic);

    printString(contact->phone_number);

    printString(contact->position);
    printf("\n");

}


void addParameters() {

    

}
