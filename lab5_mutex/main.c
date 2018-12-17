#include <time.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>

#define BUFFER 40

pthread_mutex_t mutex;
char * buffer;

void * thread_write();
void * thread_read();

int main(int argc, char * argv[])
{
	buffer = (char *)calloc(BUFFER, sizeof(char));

	pthread_t writer;
	pthread_t reader;

	// СОЗДАНИЕ МЬЮТЕКСА
	pthread_mutex_init(&mutex, NULL);

	// СОЗДАНИЕ ПОТОКОВ
	pthread_create(&writer, NULL, thread_write, NULL);
	sleep(5);
	pthread_create(&reader, NULL, thread_read, NULL);
	sleep(10);

	// УДАЛЕНИЕ ПОТОКОВ
	pthread_cancel(writer);
	pthread_cancel(reader);

	// УДАЛЕНИЕ МЬЮТЕКСА
	pthread_mutex_destroy(&mutex);
}

void * thread_write()
{
	time_t timer;
    struct  tm timeval;
    while(1) 
    {
        if (!pthread_mutex_lock(&mutex)) 
        {
            printf("Locked\n");

            timer = time(0);
            timeval = *localtime(&timer);
            sprintf(buffer, "%.2d:%.2d:%.2d", timeval.tm_hour, timeval.tm_min, timeval.tm_sec);
            sleep(1);

            printf("Unlocked\n");
            pthread_mutex_unlock(&mutex);
        }
	}
}

void * thread_read()
{
	while(1) 
	{
        if (!pthread_mutex_lock(&mutex)) 
        {
            printf("Locked\n");

            printf("Time: %s\n", buffer);

            printf("Unlocked\n");
            pthread_mutex_unlock(&mutex);
            sleep(5);
        }
	}
}