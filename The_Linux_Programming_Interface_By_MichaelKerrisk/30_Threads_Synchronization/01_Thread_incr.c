//Incorrectly increamenting a global variable from two threads
#include <pthread.h>
#include "../tlpi_lib/tlpi_hdr.h"

static int glob = 0;

static void* threadFunc(void *arg)
{
    int loops = *((int*)arg);
    int loc, j;

    for(j = 0; j < loops; j++)
    {
        loc = glob;
        loc++;
        glob = loc;
    }
    return NULL;
}

int main(int argc, char* argv[])
{
    pthread_t t1, t2;
    int loops, s;

    loops = (argc > 1) ? getInt(argv[1], GN_GT_0, "num_loops") : 10000000;

    s = pthread_create(&t1, NULL, threadFunc, &loops);
    if(s!=0)
        errExitEN(s, "pthread_create");
    
    s = pthread_create(&t2, NULL, threadFunc, &loops);
    if(s!=0)
        errExitEN(s, "pthread_create");
    
    s = pthread_join(t1, NULL);
    if(s != 0)
        errExitEN(s, "pthreaad_join");
    
    s = pthread_join(t2, NULL);
    if(s != 0)
        errExitEN(s, "pthread_join");
    
    printf("glob = %d \n", glob);
    exit(EXIT_SUCCESS);
}

/* 
gcc -std=c11 -Wall -Wextra -Wno-unused-parameter -Wno-attributes -Wno-format-truncation -pedantic -I../tlpi_lib 01_Thread_ince.c ../tlpi_lib/error_functions.c ../tlpi_lib/get_num.c -pthread -o 01_Thread_incr 
*/