#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>
#include "contacts.h"

typedef struct Node{
    int index;
    Contact* data;
    struct Node* next;
    struct Node* prev; 
} Node;

typedef struct Dlist{
    Node* head;
    Node* tail;
}Dlist;

Dlist* initDList();
Node* initNode();
int dListPush(Dlist* list, Contact* value);
Contact* dListPop(Dlist* list, int ind);
void dListPrint(Dlist* list);
int writeFile(Dlist* contacts);
int readFile(Dlist* contacts);