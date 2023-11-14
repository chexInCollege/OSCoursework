#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include "queue.h"

// A struct passed into each thread to facilitate communication with main()
typedef struct {
    int id;
    queue* q;
    int wc;
} packet;

// setting to 1 will signal to threads that the queue is done filling up
int stop = 0;
 

void *threadRoutine(void *arg) {
    // initialize ..
    node* poppedNode;              
    packet *info = (packet*) arg;   // typecast arg to packet
    int i;
    int threadID = info -> id;
    queue* q = info -> q;

    // node seeking routine
    while(!stop || getTailNode(q)) {
        // Wait for an available node
        while(!getTailNode(q) && !stop) usleep(100);

        pthread_mutex_lock(&(q->popLock)); // lock for popNode()
        if(!(poppedNode = popNode(q))) { // no node available; skip
            pthread_mutex_unlock(&(q->popLock));
        } else {
            pthread_mutex_unlock(&(q->popLock));
            // count the words for the found node
            if(poppedNode->length > 1) {
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

        pthread_mutex_lock(&(textQueue->popLock));
        appendNode(textQueue, newNode(currentLine++, read, lineText));
        pthread_mutex_unlock(&(textQueue->popLock));
    }

    // all lines extracted; signal threads that we're done
    stop = 1; 
 

    finalWordCount = 0;

    // wait for all threads to terminate, collect their word counts
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