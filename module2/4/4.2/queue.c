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

    int data = -2;
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
                    data = cur->data;
                    queue->head = cur->next;
                    cur = queue->head;
                    if(queue->head == NULL) {
                        queue->tail = NULL;
                    }
                }
                else {
                    printf("Deleted %d with priority %d\n", cur->data, cur->priority);
                    data = cur->data;
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

    return data;
}

int queuePopAbovePriority(Queue* queue, int del_priority){

    int data = -2;
    if(queue->head == NULL) {
        return -1;
    }
    else {
        Node* cur = queue->head;

        if(cur->priority >= del_priority) {
            data = cur->data;
            cur = cur->next;
            queue->head = cur;
        }
        else {
            while(cur->next != NULL && cur->next->priority < del_priority) {
                cur = cur->next;       
            }

            printf("Deleted %d with priority %d\n", cur->next->data, cur->next->priority);
            data = cur->next->data;
            cur->next = cur->next->next; 
            if(cur->next == NULL) {
                queue->tail = cur;
            }
            if(cur == queue->head && cur->priority >= del_priority && cur == queue->tail) {
                data = cur->next->data;
                cur = cur->next;
                queue->head = cur;
                queue->tail = cur;
            }
        }
    }

    return data;
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
        del_priority[i] = priority;

    }
    
    queuePrint(queue);

    printf("Deleting last element\n");
    if(queuePop(queue) == -1) {
        printf("Queue is empty\n");
    }
    queuePrint(queue);

    int i = rand()%6;
    printf("Deleting element with priority %d\n", del_priority[i]);
    int data = queuePopByPriority(queue, del_priority[i]);
    if(data == -1) {
        printf("Queue is empty\n");
    }
    else if(data == -2) {
        printf("No value with priority %d\n", i);
    }
    else {
        printf("Deleted value: %d\n", data);
    }
    queuePrint(queue);

    int j = rand()%6;
    printf("Deleting elements with priority above %d\n", del_priority[j]);

    data = queuePopAbovePriority(queue, del_priority[j]);
    if(data == -1) {
        printf("Queue is empty\n");
    }
    else {
        printf("Deleted value: %d\n", data);
    }
    queuePrint(queue);

    free(queue);

    return 0;
}