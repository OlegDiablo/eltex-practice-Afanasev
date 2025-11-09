#include <stdio.h>
#include <time.h>
#include <stdlib.h>


typedef struct Node{
    int priority;
    int data;
    struct Node* next;
}Node;

typedef struct Queue{
    struct Node* head;
    struct Node* tail;
}Queue;


void queuePush(Queue* queue, int new_priority, int value){

    Node* node = malloc(sizeof(Node));
    node->data = value;
    node->priority = new_priority;
    node->next = NULL;

    if (queue->head == NULL) {
        queue->head = node;
        queue->tail = node;
        return;
    }
    
    if (new_priority < queue->head->priority) {
        node->next = queue->head;
        queue->head = node;
        return;
    }
    
    Node* current = queue->head;

    while (current->next != NULL && current->next->priority <= new_priority) {
        current = current->next;
    }
    
    node->next = current->next;
    current->next = node;
    
    if (node->next == NULL) {
        queue->tail = node;
    }
    
    printf("Added value %d with priority %d\n", value, new_priority);
}

int queuePop(Queue* queue){

    int value = 0;
    
    if (queue->head == NULL) {
        return -1;
    }
    else {
        value = queue->head->data;
        queue->head = queue->head->next;
        queue->tail = queue->tail->next;
    }

    printf("Deleted value %d\n", value);
    return 0; 
}


int queuePopByPriority(Queue* queue, int del_priority) {

    if(queue->head == NULL) {
        return -1;
    }
    else {
        
        Node* cur = queue->head;
        Node* prev = NULL;
        while(cur != NULL) {

            if(cur->priority == del_priority) {
                if(prev == NULL) {
                    printf("Deleted %d with priority %d\n", cur->data, cur->priority);
                    queue->head = cur->next;
                    cur = queue->head;
                    if(queue->head == NULL) {
                        queue->tail = NULL;
                    }
                }
                else {
                    printf("Deleted %d with priority %d\n", cur->data, cur->priority);
                    prev->next = cur->next;
                    cur = cur->next;
                    if(prev->next == NULL) {
                        queue->tail = prev;
                    }
                }

            }
            else { 
                prev = cur;
                cur = cur->next;
            }

        }
    }

    return 0;
}

int queuePopAbovePriority(Queue* queue, int del_priority){

    if(queue->head == NULL) {
        return -1;
    }
    else {
        Node* cur = queue->head;
        while(cur->next != NULL && cur->next->priority <= del_priority) {

            printf("Deleting %d with priority %d\n", cur->data, cur->priority);
            cur = cur->next;
            queue->head = cur;
        
        }
    }

    return 0;
}


void queuePrint(Queue* queue){

    Node* tmp = queue->head;
    if(tmp == NULL) {
        printf("Queue is empty\n");
    }
    else {
        printf("value | priority\n");
        while (tmp != NULL) {

            printf("%d | %d\n", tmp->data, tmp->priority);
            tmp = tmp->next;
        
        }
    }

}


int main(){

    srand(time(NULL));
    Queue* queue = malloc(sizeof(Queue));
    queue->head = NULL;
    queue->tail = NULL;
    int del_priority[6];

    printf("start\n");
    for(int i = 0; i < 6; i++) {
        int data = rand();
        int priority = rand() % 256;
        queuePush(queue, priority, data);
        queuePrint(queue);
        del_priority[i] = priority;

    }
    
    queuePrint(queue);

    printf("Deleting last element\n");
    if(queuePop(queue) == -1) {
        printf("Queue is empty\n");
    }
    queuePrint(queue);

    printf("Deleting elements with priority %d\n", del_priority[rand()%7]);
    if(queuePopByPriority(queue, del_priority[rand()%7]) == -1) {
        printf("Queue is empty\n");
    }
    queuePrint(queue);

    printf("Deleting elements with priority above %d\n", del_priority[rand()%7]);
    if(queuePopAbovePriority(queue, del_priority[rand()%7]) == -1) {
        printf("Queue is empty\n");
    }
    queuePrint(queue);

    free(queue);

    return 0;
}