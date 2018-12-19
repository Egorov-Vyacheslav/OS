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
	pthread_create(&reader, NULL, thread_read, NULL);
	
    // ВРЕМЯ РАБОТЫ ПРОГРАММЫ
    sleep(12); 

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
        pthread_mutex_lock(&mutex);
        printf("Locked write!\n");

        timer = time(0);
        timeval = *localtime(&timer);
        sprintf(buffer, "%.2d:%.2d:%.2d", timeval.tm_hour, timeval.tm_min, timeval.tm_sec);
 
        printf("Unlocked write!\n");
        pthread_mutex_unlock(&mutex);

        sleep(4);
	}
}

void * thread_read()
{
	while(1) 
	{
        pthread_mutex_lock(&mutex);
        printf("Locked read!\n");

        printf("Time: %s\n", buffer);

        printf("Unlocked read!\n");
        pthread_mutex_unlock(&mutex);
        sleep(2);
	}
}
