#include "contacts.h"
#include "DL_List.h"



Contact* initContact(char* name, char* surname, char* patronymic, char* postition){

    Contact* contact = malloc(sizeof(Contact));

    strcpy(contact->name, name);

    strcpy(contact->surname, surname);

    strcpy(contact->patronymic, patronymic);

    strcpy(contact->position, postition);
    contact->phone_number = ' ';
    contact->num_counter = 0;

    return contact;
}



void insertName(Contact* contact, char* value){

    strcpy(contact->name, value);

}


void insertSurname(Contact* contact, char* value){

    strcpy(contact->surname, value);


}


void insertPatronymic(Contact* contact, char* value){

    strcpy(contact->patronymic, value);


}


void insertPhoneNumber(Contact* contact, char* value, int counter){

    strcpy(contact->phone_number[counter], value);
    contact->num_counter = counter+1;

}


void insertPosition(Contact* contact, char* value){
    
    strcpy(contact->position, value);


}


void printString(char* data){

    for (int i = 0; i < (int)strlen(data); i++) {

        printf("%c", data[i]);
    
    }

}


char* scanInput(){
    
    char *value = calloc(1, sizeof(char));
    int n = 0;
    char input = ' ';
    value = realloc(value, sizeof(char) * (n+1));
    while (scanf("%c", &input) == 1 && input != '\n') {

        value[n] = input;
        n++;
        value = realloc(value, sizeof(char) * (n+1));

    }
    value[n] = '\0';

    return value;

}


char* scanChangeInput(){
    
    char *value = calloc(1, sizeof(char));
    int n = 0;
    char input = ' ';
    scanf("%c", &input);
    if(input == '\n') {
        return NULL;
    }
    value[n] = input;
    n++;
    while (scanf("%c", &input) == 1 && input != '\n') {

        value[n] = input;
        n++;
        value = realloc(value, sizeof(char) * (n+1));

    }
    value[n] = '\0';

    return value;

}


void printContact(Contact* contact){

    printf("Contact Name: ");
    printString(contact->name);

    printf("\nContact Surname: ");
    printString(contact->surname);

    printf("\nContact Patronymic: ");
    printString(contact->patronymic);

    printf("\nPnone numbers: ");
    for(int i = 0; i < contact->num_counter; i++) {

        if(i != 0) {printf(", ");}
        printString(contact->phone_number[i]);

    }

    printf("\nContact position: ");
    printString(contact->position);
    printf("\n");

}


void addParameters(){

    

}
