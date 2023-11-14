#include <stdlib.h>     // i/o
#include <pthread.h>    // pthread_mutex_init
#include "queue.h"      // queue+node struct stuff

// creates a new queue with a null head
queue* newQueue() {
    queue *q = malloc(sizeof(queue));
    q -> head = NULL;
    pthread_mutex_init(&(q -> popLock), NULL);
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
        oldHead = q -> head;
        node *newHead = oldHead ? oldHead -> next : NULL;
        q -> head = newHead ? newHead : NULL;
    }
    return oldHead;
}

// adds a new node to the end of a queue
void pushNode(queue* q, node* n) {
    if(q -> head == NULL)    // easy mode: add node as head
        q -> head = n;
    else                     // hard mode: find tail node and append to the end
        getTailNode(q) -> next = n;
}