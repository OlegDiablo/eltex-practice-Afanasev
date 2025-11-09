#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include "contacts.h"
#include "DL_List.h"

int main() {

    Contact contact[40]; 
    
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

                printf("Insert amount of phone numbers: ");
                char* input = calloc(1 ,sizeof(char));
                char symbol;
                int counter = 0;
                printf("Input command: ");

                while(scanf("%c", &symbol) == 1 && symbol != '\n'){
                    
                    if(isdigit(symbol) == 0) {
                        printf("Incorrect amount of phone numbers\n");
                        continue;
                    }
                    input[counter] = symbol;
                    counter++;
                    input = realloc(input, sizeof(char) * (counter+1));

                } 
                int num = atoi(input);
                for(int i = 0; i < num; i++) {

                    printf("Insert phone number %d: ", i);
                    char* phone_number = scanInput();
                    insertPhoneNumber(contact, phone_number, i);

                }


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