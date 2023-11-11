#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "queue.h"

typedef struct {
    int id;
} packet;

 
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

    textQueue = newQueue();
    appendQueue(textQueue, newNode(1, "word word word 1"));
    appendQueue(textQueue, newNode(2, "word word word 2"));

    textQueue->head->lineNumber = 5;

    printf("\n%d\n\n", textQueue->head->next->lineNumber);

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