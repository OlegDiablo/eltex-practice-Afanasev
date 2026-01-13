#include "DL_List.h"

Dlist* initDList(){

    Dlist* list = malloc(sizeof(Node));
    list->head = NULL;
    list->tail = NULL;
    return list;

}


Node* initNode(Contact* value){

    Node* node = malloc(sizeof(Node));
    node->data = value;
    node->index = 0;
    node->next = NULL;
    node->prev = NULL;
    return node;

}


int checkPriority(char value){

    if((value >= 'a' && value <= 'z') || (value >= 'A' && value <= 'Z')) {
        return value - 'a' + 1;
    }
    if(value == 32 || value == 0) {return 0;}

    return -1;
}


int dListPush(Dlist* list, Contact* value){

    Node* node = initNode(value);
    if(value->surname[0] == 32 || checkPriority(value->surname[0]) == -1) {
        return 1;
    }

    if(list->head == NULL) {
        list->head = node; 
        list->tail = node;
        return 0; 
    }
    else {

        Node* tmp = list->head; 
        int ind = -1;
        while(tmp != NULL) {
            for(int i = 0; i < (int)strlen(value->surname); i++) {

                if(checkPriority(value->surname[i]) == -2) {
                    return 1;
                }

                if(checkPriority(value->surname[i]) >= checkPriority(tmp->data->surname[i])) {
                    break;
                }
                if(checkPriority(value->surname[i]) < checkPriority(tmp->data->surname[i]) 
                || (checkPriority(value->surname[i]) == 0 && checkPriority(tmp->data->surname[i]) != 0)) {
                    ind = tmp->index;
                    break;
                }

            }
            tmp = tmp->next;   

        }
        
        tmp = list->head;
        while(1) {
            if(ind == -1) {
                node->index = list->tail->index + 1;
                node->next = list->tail->next;
                node->prev = list->tail;
                list->tail->next = node;
                list->tail = node;
                break;               
            }
            else if(tmp->index == ind) {
                if(list->head == tmp) {
                    list->head = node;
                }
                else {
                    node->prev = tmp->prev;
                    tmp->prev->next = node;
                }
                node->index = tmp->index;
                node->next = tmp;
                tmp->prev = node;


                for(int j = ind + 1; tmp != NULL; j++) {

                    tmp->index = j;
                    tmp = tmp->next;

                }
                break;
            }
            else {
                tmp = tmp->next;     
            }
        }
        return 0;
        
    }

}


Contact* dListPop(Dlist* list, int ind){
    if (list->head == NULL) {
        return NULL;
    }
    Contact *res;

    if (list->head == list->tail) {
        
        res = list->head->data;
        list->head = NULL;
        list->tail = NULL;
        return res;
    }

    Node* tmp = list->head;
    Node* current = tmp->next;

    if(list->head->index == ind) {

        res = list->head->data;
        list->head = current;
        current->prev = NULL;
        return res;
    }

    
    while(tmp->index != ind - 1 && tmp != NULL) {
        tmp = tmp->next;
    }

    res = tmp->next->data;
    tmp->next = tmp->next->next;
    if(tmp->next != NULL) {
        tmp->next->prev = tmp;
    }

    for(int i = ind; current != NULL; i++) {
        current->index = i;
        current = current->next;
    }

    return res;
}


void dListPrint(Dlist* list){

    Node* tmp = list->head;

    if(tmp == NULL) {
        printf("List is empty\n");
    }
    else {
        while (tmp != NULL) {

            printf("\nContact %d\n", tmp->index + 1);
            printContact(tmp->data);
            tmp = tmp->next;
        
        }
    }

}


int writeFile(Dlist* contacts) {

    int fd = open("contacts_book.dat", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if(fd == -1) {
        printf("Error with opening file\n");
        return 1;
    }

    if(contacts->head == NULL) {
        close(fd);
        return 1;
    }
    while(contacts->head != NULL) {
        ssize_t bytes_written = write(fd, contacts->head->data, sizeof(Contact));
        if(bytes_written != sizeof(Contact)) {
            printf("Error writing contacts to file\n");
            close(fd);
            return 1;
        }
        contacts->head = contacts->head->next;   

    }

    close(fd);
    return 0;
}


int readFile(Dlist* contacts) {

    int fd = open("contacts_book.dat", O_RDONLY | O_CREAT, 0644);
    if(fd == -1) {
        printf("Error reading file\n");
        return 1;
    }
    ssize_t bytes_read = -1;
    while (bytes_read != 0) {
        
        Contact* contact = malloc(sizeof(Contact));
        bytes_read = read(fd, contact, sizeof(Contact));
        if(bytes_read == sizeof(Contact)) {
            int err = dListPush(contacts, contact);
            if(err == 1) {
                printf("Error pushing to list\n");
                free(contact);
                close(fd);
                return 1;
            } 
        }
        else if(bytes_read != 0){
            free(contact);
            break;
        }

    }

    close(fd);
    return 0;
}