#ifndef FUNCTIONS_QUEUE_INCLUDED
#define FUNCTIONS_QUEUE_INCLUDED

#include <pthread.h>

typedef struct _node {
    struct _node *next;
    char *text;
    int length;
    int lineNumber;
} node;

typedef struct {
    node *head;
    pthread_mutex_t popLock;
    pthread_mutex_t countLock;
    int wordCount;
} queue;

queue* newQueue();
node* newNode(int lineNo, int length, char* str);
node* getTailNode(queue* q);
node* popNode(queue* q);
void appendNode(queue* q, node* n);

#endif