#include <stdlib.h>
#include <pthread.h>
#include "queue.h"

// creates a new queue with a null head
queue* newQueue() {
    queue *q = malloc(sizeof(queue));
    q -> head = NULL;
    q -> count = 0;
    pthread_mutex_init(&(q -> lock), NULL);
    return q;
}

// creates a new node with a line number and string
node* newNode(int lineNo, int length, char* str) {
    node *n = malloc(sizeof(node));
    n -> text = str;
    n -> next = NULL;
    n -> lineNumber = lineNo;
    n -> length = length;
    return n;
} 

// finds the last node in a queue
node* getTailNode(queue* q) {
    node *cNode = q -> head;

    if(cNode) 
        while(cNode -> next) 
            cNode = cNode -> next;
    
    return cNode;
}

// removes the front element from the queue and returns the popped node
node* popNode(queue* q) {
    node *oldHead = NULL;
    if(q -> head) {
        pthread_mutex_lock(&(q->lock));
        printf("mutex locked\n");
        oldHead = q -> head;
        //printf("got old head\n");
        node *newHead = oldHead -> next;
        //printf("mutex got new head\n");
        q -> head = newHead;
        //printf("set new head\n");
        pthread_mutex_unlock(&(q->lock));
        printf("mutex unlocked\n");
    }
    return oldHead;
}

// adds a new node to the end of a queue
void appendNode(queue* q, node* n) {
    if(q -> head == NULL)    // easy mode: add node as head
        q -> head = n;
    else                     // hard mode: find tail node and append to the end
        getTailNode(q) -> next = n;
}

