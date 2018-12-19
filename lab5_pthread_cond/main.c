#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>

#define COUNTER_MAX 10

int counter = 0;
char * buffer;

pthread_mutex_t mutex;
pthread_cond_t condition;

void * produce();
void * consume();

int main(int argc, char * argv[])
{
    buffer = (char *)calloc(COUNTER_MAX, sizeof(char));
	pthread_t producer;
	pthread_t consumer;

	pthread_mutex_init(&mutex, NULL);
	pthread_cond_init(&condition, NULL);

	pthread_create(&consumer, NULL, consume, NULL);
	pthread_create(&producer, NULL, produce, NULL);

    sleep(50);

	pthread_cancel(producer);
	pthread_cancel(consumer);

	pthread_mutex_destroy(&mutex);
	pthread_cond_destroy(&condition);
}

void * produce() 
{
    while (1) 
    {
        pthread_mutex_lock(&mutex);
        while (counter < COUNTER_MAX) 
        {
            pthread_cond_wait(&condition, &mutex);
    	}
        printf("Read: - %s\n", buffer);
        while(counter)
        {
            buffer[--counter] = ' ';
        }
        printf("buffer clear! - %s\n", buffer);
        pthread_mutex_unlock(&mutex);
    }
}

void * consume() 
{
    while(1) 
    {
        pthread_mutex_lock(&mutex);
        sleep(1);
        buffer[counter] = 'X';
        counter++;
        printf("Write: - %s\n", buffer);
        if (counter > COUNTER_MAX) 
        {
            printf("Buffer full! - %s\n", buffer);
            pthread_cond_broadcast(&condition);
            pthread_mutex_unlock(&mutex);
            sleep(2);
        }
        pthread_mutex_unlock(&mutex);

    }
} 