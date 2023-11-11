#include <stdlib.h>
#include "queue.h"

// creates a new queue with a null head
queue* newQueue() {
    queue *q = malloc(sizeof(queue));
    q -> head = NULL;
    return q;
}

// creates a new node with a line number and string
node* newNode(int lineNo, char* str) {
    node *n = malloc(sizeof(node));
    n -> text = str;
    n -> lineNumber = lineNo;
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
    node *oldHead = q -> head;
    node *newHead = oldHead -> next;
    q -> head = newHead;
    return oldHead;
}

// adds a new node to the end of a queue
void appendNode(queue* q, node* n) {
    if(q -> head == NULL)    // easy mode: add node as head
        q -> head = n;
    else                     // hard mode: find tail node and append to the end
        getTailNode(q) -> next = n;
}

