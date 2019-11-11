#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

void *my_thread(void *argument)
{
    printf( "This thread is created with argument '%s' .\n", (char *) argument);
    sleep(5);
    return NULL;
}

int main()
{
    pthread_t thread_id;
    pthread_create( &thread_id, NULL, my_thread, (void *) "Testing");
    pthread_join(thread_id,NULL);
    printf("Done\n");
}
