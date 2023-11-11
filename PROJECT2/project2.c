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


    free(info);
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
    appendNode(textQueue, newNode(1, "word word word 1"));
    appendNode(textQueue, newNode(2, "word word word 2"));
    appendNode(textQueue, newNode(3, "word word word 5"));
    appendNode(textQueue, newNode(4, "word word word 5"));

    printf("%d aaaa \n\n", popNode(textQueue) -> lineNumber);


    //printf("\n%d\n\n", textQueue -> head -> lineNumber);

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


    // free memory ~
    free(textQueue);
    free(threadList);

}