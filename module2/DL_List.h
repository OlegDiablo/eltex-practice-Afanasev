#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "contacts.h"

typedef struct Node{
    int index;
    Contact data;
    struct Node* next;
    struct Node* prev; 
} Node;

typedef struct Dlist{
    Node* head;
    Node* tail;
}Dlist;

Dlist* initDList();
Node* initNode(Contact value);
void dListPush(Dlist* list, Contact value);
int dListPop(Dlist* list, int ind);
void dListPrint(Dlist* list);