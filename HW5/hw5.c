// Alex Groves
// Homework 5, CSCI 340-01

// NOTE: This code ensures each thread runs in order in cycles.
// This can slow the program down a lot, and if that is an issue,
// set WAIT_FOR_NEIGHBORS to 0 rather than 1.
#define WAIT_FOR_NEIGHBORS 1

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

// variables used between threads
char *myStr;                    // the string every thread shares
pthread_mutex_t mutex;         
int stringLength, numThreads;   // holds input args, set in main

int whoseTurn;                  // not required(?), but threads will cycle;
                                // honestly, this bit renders the mutex obsolete,
                                // but I'll keep it in anyways

// Code for each individual thread
void *threadRoutine(void *param) {
    // Collect info regarding thread ID
    int *id = (int *) param;
    char threadChar[2] = {((*id) % 26) + 65, '\0'}; // char based on thread ID

    for(;;) {
        // thread waits its turn
        while(whoseTurn != *id && strlen(myStr) < stringLength && WAIT_FOR_NEIGHBORS);

        // acquire the official mutex
        pthread_mutex_lock(&mutex);
        
        if(strlen(myStr) < stringLength) { // add new char
            // update the string
            strcat(myStr, threadChar);
            
            // pass access to its neighbor
            whoseTurn = (whoseTurn + 1) % numThreads;

            printf("Thread %d just added '%s'\n", *id, threadChar);
            
            // release the mutex in all cases
            pthread_mutex_unlock(&mutex);
        } else { // string is full! free the mutex and kill the thread
            pthread_mutex_unlock(&mutex);
            break;
        }
    }

    return NULL;
}

int main(int argc, char *argv[]) {
    int i;                  
    pthread_t *threadArray; // pointer to list of threads
    int *threadIds;         // holds thread IDs

    if ( argc < 3 ) {
        fprintf(stderr, "usage: hw5 <numThreads> <stringLength>\n");
        exit(1);
    }

    numThreads = atoi(argv[1]);
    stringLength = atoi(argv[2]);

    threadArray = malloc(numThreads * sizeof(pthread_t));   
    threadIds = malloc(numThreads * sizeof(int)); 

    myStr = malloc((stringLength+1) * sizeof(int));

    // fill array with threads
    for(i = 0; i < numThreads; i++) {
        threadIds[i] = i;
        pthread_create(&threadArray[i], NULL, threadRoutine, &threadIds[i]);
    }


    // wait for threads to be done
    for(i = 0; i < numThreads; i++) {
        pthread_join(threadArray[i], NULL);
    }

    // output the final string
    printf("FINAL STRING: \n'%s'\nMade by  %d  threads\n", myStr, numThreads);

    // free the mallocs
    free(threadArray); 
    free(threadIds);
    free(myStr);
}