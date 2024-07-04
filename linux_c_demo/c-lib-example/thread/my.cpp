#include <arpa/inet.h>
#include <cstdlib>
#include <inttypes.h>
#include <iomanip>
#include <iostream>
#include <math.h>
#include <netdb.h>
#include <pthread.h>
#include <set>
#include <stdarg.h>
#include <string>
#include <unistd.h>
#include <vector>

static int gi;
static pthread_mutex_t mg = PTHREAD_MUTEX_INITIALIZER; 

void *threadfunc(void *arg)
{
    pthread_mutex_lock(&mg);
    gi++;
    printf("%s gi: %d\n", "sub thread ", gi);
    pthread_mutex_unlock(&mg);

}

int main(int argc, char *argv[])
{
    for (int i = 0; i < 10; i++)
    {
        pthread_t t;
        pthread_create(&t, 0, &threadfunc, (void*)&i);
        pthread_join(t, NULL);
    }
    printf("main thread 4\n");
    return 0;
}