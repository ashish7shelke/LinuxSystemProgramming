#include <pthread.h>
#include "../tlpi_lib/tlpi_hdr.h"

static void* threadFunc(void *arg)
{
    char *s = (char *)arg;
    printf("%s", s);
    return (void*) strlen(s);
}

int main(int argc, char* argv[])
{
    pthread_t t1;
    void *res;
    int s;

    s = pthread_create(&t1, NULL, threadFunc, "Hello World\n");
    if(s != 0)
        errExitEN(s, "pthread+create()");
    
    printf("Meassage from main()\n");

    s = pthread_join(t1, &res);
    if(s != 0)
        errExitEN(s, "pthread_join");

    printf("Thread returned %ld\n", (long)res);

    exit(EXIT_SUCCESS);
}

//gcc -std=c11 -Wall -Wextra -Wno-unused-parameter 
//-Wno-attributes -Wno-format-truncation -pedantic 
//-I../tlpi_lib simple_thread.c ../tlpi_lib/error_functions.c 
//../tlpi_lib/get_num.c -pthread -o simple_thread
