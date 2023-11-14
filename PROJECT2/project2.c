// CSCI 340-01 - Project 2
// Alex Groves & Andreas Dilling
// A program which demonstrates thread synchronization via a multithreaded word counter.

#include <stdio.h>      // i/o
#include <stdlib.h>     // malloc
#include <string.h>     // strcpy
#include <pthread.h>    // pthreads
#include <unistd.h>     // usleep()
#include "queue.h"      // custom queue library

// A struct passed into each thread to facilitate communication with main()
typedef struct {
    int id;     // represents current thread ID
    queue* q;   // pointer to the main queue
    int wc;     // word count for current thread
} packet;

// setting to 1 will signal to threads that the queue is done filling up
int stop = 0;
 

void *threadRoutine(void *arg) {
    // initialize...
    node* poppedNode;               // used to hold nodes popped from the queue          
    packet *info = (packet*) arg;   // typecast arg to packet
    queue* q = info -> q;           // queue pointer
    int threadID = info -> id;
    int i;

    // node seeking routine
    while(!stop || getTailNode(q)) {
        // wait for an available node
        while(!getTailNode(q) && !stop) usleep(100);

        // lock the mutex to alter the queue
        pthread_mutex_lock(&(q->popLock));
        poppedNode = popNode(q);
        pthread_mutex_unlock(&(q->popLock));

        if(poppedNode) {
            // count the words for the found node
            if(poppedNode->length > 1) { // make sure the line has content
                info->wc++; // 1 for EoL

                i = 0;
                while(poppedNode->text[i] == ' ') i++; // skip initial spaces

                for(; i < poppedNode->length; i++) {
                    if(poppedNode->text[i] == ' ') {
                        info->wc++;
                        while(poppedNode->text[i+1] == ' ') i++; // skip consecutive spaces
                    }
                }
            }
            // sing the line
            printf("Thread %03d: %6d | %s", threadID, poppedNode->lineNumber, poppedNode->text);
            
            // for some reason, freeing poppedNode itself
            // without a long, slow usleep() is disastrous here
            // so I guess we just free the string..
            free(poppedNode->text);
            
        }
    }
    pthread_exit(NULL);
}


int main(int argc, char **argv) {
    // command check
    if ( argc < 2 ) {
        fprintf(stderr, "usage: project2 <numConsumers>\n");
        exit(1); 
    }
    
    // initialize main thread vars
    int numConsumers = atoi(argv[1]);
    char *line = NULL;
    queue *textQueue = newQueue();
    size_t len = 0;
    pthread_t *threadList;
    packet **packetList;
    size_t read;
    int currentLine, finalWordCount, i;

    // a couple arrays for our thread communication medium
    threadList = malloc(numConsumers * sizeof(pthread_t));
    packetList = malloc(numConsumers * sizeof(packet));
 
    // initialize data packets and consumer threads
    for(i = 0; i < numConsumers; i++) {
        packet *info = malloc(sizeof(packet));
        info -> id = i;
        info -> q = textQueue;
        info -> wc = 0;
        packetList[i] = info;
        pthread_create(&threadList[i], NULL, threadRoutine, info);
    } 

    // Start processing stdin
    currentLine = 1;
    while ((read = getline(&line, &len, stdin)) != -1) {
        // give every line its own heap home
        char* lineText = malloc((read+1) * sizeof(char));
        strcpy(lineText, line);

        // lock the mutex to alter the queue
        pthread_mutex_lock(&(textQueue->popLock));
        pushNode(textQueue, newNode(currentLine++, read, lineText));
        pthread_mutex_unlock(&(textQueue->popLock));
    }

    // all lines extracted; signal threads that we're done
    stop = 1; 

    // wait for all threads to terminate, collect their word counts
    finalWordCount = 0;
    for(i = 0; i < numConsumers; i++) {
        pthread_join(threadList[i], NULL);
        finalWordCount += packetList[i] -> wc;
        free(packetList[i]);
    }

    // final output
    printf("--------------------------------------------------");
    printf("\nFINAL COUNT:  %d  words in  %d  lines\n", finalWordCount, currentLine-1);

    // free memory ~
    free(textQueue);
    free(threadList);
    free(packetList);
}