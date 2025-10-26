#include <stdlib.h>
#include <stdio.h>
#include "contacts.h"

int main() {

    Contact *contact = malloc(sizeof(Contact)); 
    
    char command;

    while (1) {

        printf("Press:\n1 to add contact\n2 to change contact\n3 to delete contact\n0 to exit from program\n");
        scanf("%c", &command);
        char test;
        

        if (scanf("%c", &test) != 1 || test != '\n') {

            printf("Incorrect input\n");
            while(test != '\n') {scanf("%c", &test);}
            continue;

        }

        switch (command) {
            case '1':

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

                printContact(contact);
                break;

            case '2':

                break;

            case '3':

                break;

            case '0':

                return 0;


            default:
                
                printf("Incorrect input\n");
                continue;
                
        }

        
        
    }

    return 0;

}