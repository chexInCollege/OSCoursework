#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include "queue.h"

typedef struct {
    int id;
    queue* q;
} packet;

int stop = 0;
 
void *threadRoutine(void *arg) {
    node* poppedNode;
    packet *info = (packet*) arg;
    int i, wc;
    int threadID = info -> id;
    queue* q = info -> q;

    while(!stop || getTailNode(q)) {
        while(!getTailNode(q) && !stop) usleep(1000);
        pthread_mutex_lock(&(q->popLock));
        if((poppedNode = popNode(q))) {
            pthread_mutex_unlock(&(q->popLock));
            // process the newly acquired node
            //printf("Thread %d got line %d! (%d)\n", threadID, poppedNode -> lineNumber, q -> wordCount);
            
            wc = 1; // 1 for EoL

            for(i = 0; i < poppedNode->length; i++) {
                if(poppedNode->text[i] == ' ') {
                    wc++;
                }
            }
            //printf("\n");
            printf("Thread %d got line %d! (%d words)\n", threadID, poppedNode -> lineNumber, wc);
            //for(int i = 0; i < 100000; i++) {int x = i * i * i;}
            poppedNode->seen = 1;
            //free(poppedNode);
            usleep(100);
        } else {
            printf("Thread %d skipped cycle!\n", threadID);
            pthread_mutex_unlock(&(q->popLock));
        }
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
        char* lineText = malloc(read * sizeof(char)+1);
        strcpy(lineText, line);
        pthread_mutex_lock(&(textQueue->popLock));
        appendNode(textQueue, newNode(currentLine++, read, lineText));
        pthread_mutex_unlock(&(textQueue->popLock));
    }

    stop = 1; 
 
    printf("\n\nmain finished\n\n");

    // wait for all threads to terminate
    for(i = 0; i < numConsumers; i++) {
        pthread_join(threadList[i], NULL);
    }

    printf("\n%d\n", textQueue -> wordCount);


    // free memory ~
    free(textQueue);
    free(threadList);

}