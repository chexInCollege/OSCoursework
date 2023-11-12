#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <time.h>
#include "queue.h"

typedef struct {
    int id;
    queue* q;
} packet;

int stop = 0;
 
void *threadRoutine(void *arg) {
    packet *info = (packet*) arg;
    int threadID = info -> id;
    queue* q = info -> q;
    //printf("Thread %d is waiting...\n", threadID);

    //printf("Thread %d :3\n", info->id);

    while(!stop || getTailNode(q)) {
        while(!getTailNode(q)) usleep(1000);
        //printf("WAITING...\n");
        //printf("Thread %d is waiting...\n", threadID);
        node* poppedNode = popNode(q);
        //printf("Thread %d got line %d! (%d)\n", threadID, poppedNode -> lineNumber, q -> count);
        //printf("Thread %d just got line %d\n", threadID, poppedNode -> lineNumber);        
    }
    

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
    queue *textQueue = newQueue();
    char *line = NULL;
    size_t len = 0;
    size_t read;
    int currentLine;

    printf("\n%d\n\n", textQueue -> lock);

    threadList = malloc(numConsumers * sizeof(pthread_t));
    // packet    *threadInfo[numConsumers];
 
    for(i = 0; i < numConsumers; i++) {
        packet *info = malloc(sizeof(packet));
        info -> id = i;
        info -> q = textQueue;
        pthread_create(&threadList[i], NULL, threadRoutine, info);
    } 


    // Start processing stdin
    currentLine = 1;
    while ((read = getline(&line, &len, stdin)) != -1) {
        node* lineNode = newNode(currentLine++, read, line);
        appendNode(textQueue, lineNode);
    }

    stop = 1;

    printf("\n\nmain finished\n\n");

    // wait for all threads to terminate
    for(i = 0; i < numConsumers; i++) {
        pthread_join(threadList[i], NULL);
    }

    printf("%d\n", textQueue -> count);


    // free memory ~
    free(textQueue);
    free(threadList);

}