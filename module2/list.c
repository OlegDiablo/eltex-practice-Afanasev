#include <stdio.h>

typedef struct {

    char data;
    Node* next;

}Node;

typedef struct {

    Node* head;
    Node* tail;

}List;

List* listPush(char* value, int n) {

    Node* node;
    node->data = value[0];
    node->next = NULL;
    List list = {node, node};

    for(int i = 1; i < n; i++) {

        Node* cur;
        cur->data = value[i];
        cur->next = NULL;
        list.tail->next = cur;
        list.tail = cur;
    
    }

}

void listPrint(List list) {



}

int main() {

    char value[5] = "1234";
    List list = *listPush(value, 5);

}