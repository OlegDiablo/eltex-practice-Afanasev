#include <ctype.h>
#include "contacts.h"
#include "DL_List.h"


Contact* searchContact(const Dlist* list, int ind){

    Node* tmp = list->head;
    while(tmp != NULL && tmp->index != ind) {
        tmp = tmp->next;
    }
    if(tmp->index == ind) {
        return tmp->data;
    }
    else {
        return NULL;
    }
    
}


int scanCounter(){
    char* input = calloc(1 ,sizeof(char));
    char symbol; 
    int counter = 0;
    scanf("%c", &symbol);
    if(symbol == '\n') {
        return -1;
    }
    else {
        if(isdigit(symbol) == 0) {
            printf("Incorrect amount of phone numbers\n");
            return -2;
        }
        input[counter] = symbol;
        counter++;
        input = realloc(input, sizeof(char) * (counter+1));
    }
    while(scanf("%c", &symbol) == 1 && symbol != '\n'){
        
        if(isdigit(symbol) == 0) {
            printf("Incorrect amount of phone numbers\n");
            return -2;
        }
        input[counter] = symbol;
        counter++;
        input = realloc(input, sizeof(char) * (counter+1));

    } 
    int num = atoi(input);
    free(input);
    return num;
}


int main() {

    Dlist* contacts_list = initDList(); 
    
    char command;

    while (1) {

        printf("Press:\n1 to add contact\n2 to delete contact\n3 to change contact\n4 to print contacts list\n0 to exit from program\n");
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

                printf("Insert surname: ");
                char* surname = scanInput();

                printf("Insert patronymic: ");
                char* patronymic = scanInput();

                printf("Insert position: ");
                char* position = scanInput();

                Contact* contact = initContact(name, surname, patronymic, position);
                
                printf("Insert amount of phone numbers: ");
                int ins_num = scanCounter();

                for(int i = 0; i < ins_num; i++) {

                    printf("Insert phone number %d: ", i+1);
                    char* phone_number = scanInput();
                    if(strlen(phone_number) != 11) {
                        printf("Incorrect lenth of phone number\n");
                        i--;
                        continue;
                    }
                    insertPhoneNumber(contact, phone_number, i);

                }

                printContact(contact);

                if(dListPush(contacts_list, contact) == 1) {
                    printf("Incorrect surname\n");
                }
                else {
                    printf("Contact added\n");
                }
                break;

            case '2':
                printf("Insert id of contact: ");
                int del_id = scanCounter();
                Contact* err = dListPop(contacts_list, del_id-1);
                if(err == NULL) {
                    printf("No contact with id = %d\n", del_id);
                } 
                else {
                    printf("Contact deleted\n");
                }
  

                break;

            case '3':
                printf("Insert id of contact you want to change: ");
                int ch_id = scanCounter();
                Contact* res = dListPop(contacts_list, ch_id-1);
                if(res == NULL) {
                    printf("No contact with id = %d", ch_id);
                    break;
                } 
                else {
                    printf("Insert new name (press Enter to skip): ");
                    char* name = scanChangeInput();
                    if(name != NULL) {
                        insertName(res, name);
                    }

                    printf("Insert new surname (press Enter to skip): ");
                    char* surname = scanChangeInput();
                    if(surname != NULL) {
                        insertSurname(res, surname);
                    }                   

                    printf("Insert new patronymic (press Enter to skip): ");
                    char* patronymic = scanChangeInput();
                    if(patronymic != NULL) {
                        insertPatronymic(res, patronymic);
                    }

                    printf("Insert new position (press Enter to skip): ");
                    char* position = scanChangeInput();
                    if(position != NULL) {
                        insertPosition(res, position);
                    }

                    printf("Insert new amount of phone numbers (press Enter to skip): ");
                    int ch_num = scanCounter();                   
                    if(ch_num != -1 && ch_num != -2) {
                        for(int i = 0; i < ch_num; i++) {
                            
                        printf("Insert phone number %d: ", i+1);
                            char* phone_number = scanInput();
                            if(strlen(phone_number) != 11) {
                                    printf("Incorrect lenth of phone number\n");
                                    i--;
                                    continue;
                            }
                            insertPhoneNumber(res, phone_number, i);

                        }
                    }  
                if(dListPush(contacts_list, res) == 1) {
                    printf("Incorrect surname\n");
                }
                else {
                    printf("Contact changed\n");
                }

                }

                break;

            case '4':

                dListPrint(contacts_list);
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