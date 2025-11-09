#include "DL_List.h"


Dlist* initDList(){

    Dlist* list = malloc(sizeof(Node));
    list->head = NULL;
    list->tail = NULL;
    return list;

}

Node* initNode(Contact value){

    Node* node = malloc(sizeof(Node));
    node->data = value;
    node->index = 0;
    node->next = NULL;
    node->prev = NULL;
    return node;

}

void dListPush(Dlist* list, Contact value){

    Node* node = initNode(value);

    if(list->head == NULL) {
        list->head = node; 
        list->tail = node; 
    }
    else {
        node->index = list->tail->index + 1;
        node->prev = list->tail;
        list->tail->next = node;
        list->tail = node;
    }

}

int dListPop(Dlist* list, int ind){

    if (list->head == NULL) {
        return 1;
    }

    Node* tmp = list->head;
    while(tmp->index != ind - 1 && tmp != NULL) {
        tmp = tmp->next;
    }

    if(tmp == NULL || tmp->next == NULL) {
        return 2;
    }

    printf("Deleted %d with index %d\n", tmp->next->data, tmp->next->index);
    tmp->next = tmp->next->next;
    if(tmp->next != NULL) {
        tmp->next->prev = tmp;
    }

    Node* current = tmp->next;
    for(int i = ind; current != NULL; i++) {
        current->index = i;
        current = current->next;
    }

    return 0;
}


void dListPrint(Dlist* list){

    Node* tmp = list->head;
    if(tmp == NULL) {
        printf("List is empty\n");
    }
    else {
        printf("value | index\n");
        while (tmp != NULL) {

            printf("%d | %d\n", tmp->data, tmp->index);
            tmp = tmp->next;
        
        }
    }

}


int main(){

    Dlist* list = initDList();
    /*for(int i = 0; i < 6; i++) {
        int data = rand();
        dListPush(list, data);
    }*/
    dListPrint(list);
    dListPop(list, 3);
    dListPrint(list);

}