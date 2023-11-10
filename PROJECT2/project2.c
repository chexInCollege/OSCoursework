#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

typedef struct {
    int id;
} packet;

typedef struct _node {
    struct _node *next;
    char *text;
    int lineNumber;
} node;

typedef struct {
    node *head;
} queue;

void *threadRoutine(void *arg) {
    packet *info = (packet*) arg;

    printf("Thread %d :3\n", info->id);

    return NULL;
}

int main(int argc, char **argv) {
    if ( argc < 2 ) {
        fprintf(stderr, "usage: project2 <numConsumers>\n");
        exit(1); 
    }

    // initialize main thread vars
    int i;
    int numConsumers = atoi(argv[1]);
    pthread_t *threadList;
    queue *textQueue;
    node *firstNode;

    textQueue = malloc(sizeof(queue));
    firstNode = malloc(sizeof(node));
    textQueue->head = firstNode;




    threadList = malloc(numConsumers * sizeof(pthread_t));
    // packet    *threadInfo[numConsumers];
 
    for(i = 0; i < numConsumers; i++) {
        packet *info = malloc(sizeof(packet));
        info->id = i;
        pthread_create(&threadList[i], NULL, threadRoutine, info);
    } 



    // wait for all threads to terminate
    for(i = 0; i < numConsumers; i++) {
        pthread_join(threadList[i], NULL);
    }

}