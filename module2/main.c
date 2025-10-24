#include <stdlib.h>
#include <stdio.h>
#include "contacts.h"

int main() {

    Contact *contact = malloc(sizeof(Contact)); 
    

    printf("Insert name: ");
    char* name = scanInput();
    insertName(contact, name);

    printf("Insert surname: ");
    char* surname = scanInput();
    insertSurname(contact, surname);

    printf("Insert patronymic: ");
    char* patronymic = scanInput();
    insertPatronymic(contact, patronymic);

    printf("Insert phone number: ");
    char* phone_number = scanInput();
    insertPhoneNumber(contact, phone_number);

    printf("Insert Position: ");
    char* position = scanInput();
    insertPosition(contact, position);

    return 0;
}