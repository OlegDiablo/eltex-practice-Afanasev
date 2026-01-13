#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct Contact{

    char name[20];
    char surname[20];
    char patronymic[20];
    char phone_number[5][12];
    int num_counter;
    char position[30];

}Contact;


Contact* initContact(char* name, char* surname, char* patronymic, char* postition);
void insertName(Contact*, char*);
void insertSurname(Contact*, char*);
void insertPatronymic(Contact*, char*);
void insertPhoneNumber(Contact*, char*, int);
void insertPosition(Contact*, char*);
char* scanInput();
char* scanChangeInput();
void printString(char* data);
void printContact(Contact*);
