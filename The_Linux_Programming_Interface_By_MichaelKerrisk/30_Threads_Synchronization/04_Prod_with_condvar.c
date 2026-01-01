/* Producer-consumer example that doesn't use a condition variable*/
#include <pthread.h>
#include <stdbool.h>
#include <time.h>
#include "../tlpi_lib/tlpi_hdr.h"

static pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

static int avail = 0;

static void* producer(void *arg)
{
    int cnt = atoi((char*)arg);
    for(int i = 0; i < cnt; i++)
    {
        sleep(1);
    
        int s = pthread_mutex_lock(&mtx);
        if(s != 0)
            errExitEN(s, "pthread_mutex_lock");
    
        avail++;

        s = pthread_mutex_unlock(&mtx);
        if(s != 0)
            errExitEN(s, "pthread_mutex_unlock");

        s = pthread_cond_signal(&cond); // Wake sleeping consumer
        if(s != 0)
            errExitEN(s, "pthread_cond_signal");
    }   
    return NULL;
}

int main(int argc, char* argv[])
{
    time_t t = time(NULL);
    int totRequired = 0;

    for(int j = 1 ; j < argc; j++)
    {
        totRequired += atoi(argv[j]);
        
        pthread_t tid;
        int s = pthread_create(&tid, NULL, producer, argv[j]);
        if(s != 0)
            errExitEN(s, "pthread_create");
    }

    /* Use polling loop to check for available units */
    int numConsumed = 0;
    bool done = false;

    for(;;)
    {
        int s = pthread_mutex_lock(&mtx);
        if(s != 0)
            errExitEN(s, "pthread_mutex_lock");

        while(avail == 0)
        {
            s = pthread_cond_wait(&cond, &mtx); // Wait for something to consume
            if (s != 0)
                errExitEN(s, "pthread_cond_wait");
        }
        
        while(avail > 0)
        {
            numConsumed++;
            avail--;
            printf("T = %ld: numConsumed = %d \n", (long)(time(NULL)-t), numConsumed);
            done = numConsumed >= totRequired;
        }

        s = pthread_mutex_unlock(&mtx);
        if(s != 0)
            errExitEN(s, "pthread_mutex_unlock");
        
        if(done)
            break;
    }
    exit(EXIT_SUCCESS);
}


/* 
gcc -std=c11 -Wall -Wextra -Wno-unused-parameter -Wno-attributes -Wno-format-truncation -pedantic -I../tlpi_lib 03_Prod_no_condvar.c ../tlpi_lib/error_functions.c ../tlpi_lib/get_num.c -pthread -o 03_Prod_no_condvar
*/

