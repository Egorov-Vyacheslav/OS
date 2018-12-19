#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/shm.h>
#include <sys/types.h>

#define COUNTER_MAX 9
#define BUFFER 11

int counter = 0;
char * buffer;

pthread_rwlock_t rwlock;

void * produce();
void * consume();

int main(int argc, char * argv[]) 
{
	key_t key = 3792;
    pthread_t producer;
    pthread_t consumer;

    int shm = shmget(key, BUFFER, IPC_CREAT | 0666);
	buffer = shmat(shm, buffer, 0);

    pthread_rwlock_init(&rwlock, NULL);

    pthread_create(&producer, NULL, produce, NULL);
    pthread_create(&consumer, NULL, consume, NULL);

    sleep(15);

    pthread_cancel(producer);
    pthread_cancel(consumer);

    pthread_rwlock_destroy(&rwlock);
}

void * consume() 
{
    while (1) 
    {
        if (counter >= COUNTER_MAX) 
        {
            pthread_rwlock_rdlock(&rwlock);

            for (; counter; counter--)
                buffer[counter] = ' ';

            printf("Empty!\n");
            pthread_rwlock_unlock(&rwlock);
        }
    }
}

void * produce() 
{
    while(1) 
    {
        if (counter < COUNTER_MAX) 
        {
            pthread_rwlock_wrlock(&rwlock);
            sleep(5);
            buffer[counter] = 'A';
            counter++;
            printf("Data: %d - %s\n", counter, buffer);
            if (counter >= COUNTER_MAX) 
            {
                printf("Full!\n");
            }
            pthread_rwlock_unlock(&rwlock);
        }
    }
}
