#include "contacts.h"
#include <stdlib.h>
#include <stdio.h>

void insertName(Contact* contact, char* value) {

    contact->name = value;

}


void insertSurname(Contact* contact, char* value) {

    contact->surname = value;

}


void insertPatronymic(Contact* contact, char* value) {

    contact->patronymic = value;

}


void insertPhoneNumber(Contact* contact, char* value) {

    contact->phone_number = value;

}


void insertPosition(Contact* contact, char* value) {

    contact->position = value;

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
    value[n] = '*';

    return value;

}


void addParameters() {

    

}
