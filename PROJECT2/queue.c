#include <stdlib.h>
#include <stdio.h>
#include "queue.h"

queue* newQueue() {
    queue *q = malloc(sizeof(queue));
    q -> head = NULL;
    return q;
}

node* newNode(int lineNo, char* str) {
    node *n = malloc(sizeof(node));
    n -> text = str;
    n -> lineNumber = lineNo;
    return n;
}

void appendQueue(queue* q, node* n) {
    // check if queue is empty
    if(q -> head == NULL) {
        // easy mode: add node as head
        q -> head = n;
    } else {
        // hard mode: find tail node and append to the end
        node *cNode = q -> head;

        while(cNode -> next) cNode = cNode -> next;

        cNode -> next = n;
    }
}