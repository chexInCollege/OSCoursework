#ifndef FUNCTIONS_QUEUE_INCLUDED
#define FUNCTIONS_QUEUE_INCLUDED

typedef struct _node {
    struct _node *next;
    char *text;
    int lineNumber;
} node;

typedef struct {
    node *head;
} queue;

queue* newQueue();
node* newNode();
void appendQueue(queue* q, node* n);

#endif