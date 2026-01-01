#include <pthread.h>
#include "../tlpi_lib/tlpi_hdr.h"

static void* threadFunc(void *args)
{
    char *s = (char*)args;
    printf("%s", s);
    return (void*)strlen(s);
}

int main(int argc, char* argv[])
{
    pthread_t thr;
    pthread_attr_t attr;
    int s;

    s = pthread_attr_init(&attr);
    if(s != 0)
        errExitEN(s, "pthread_attr_init");
    
    s = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    if(s != 0)
        errExitEN(s, "pthread_attr_setdetachstate");
    
    s = pthread_create(&thr, &attr, threadFunc, /*(void*)1*/"Hello World\n");
    if(s != 0)
        errExitEN(s, "pthread_crete");
    
    s = pthread_attr_destroy(&attr);
    if(s != 0)
        errExitEN(s, "pthread_attr_destroy");
    
    return 0;
}