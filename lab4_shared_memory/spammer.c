#include <time.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <fcntl.h>

#define BUFFER 1024

char * buffer;

void time_spam(char * buffer);

int main()
{
	key_t key = 3972;

	// СОЗДАНИЕ РАЗДЕЛЯЕМОЙ ПАМЯТИ
	int shm = shmget(key, BUFFER, IPC_CREAT | 0666);
	buffer = shmat(shm, buffer, 0);

	time_spam(buffer);
}

void time_spam(char * buffer)
{
	struct tm timeval;
    while(1) 
    {
        time_t timer = time(0);
        timeval = *localtime(&timer);
        sprintf(buffer, "%.2d:%.2d:%.2d", timeval.tm_hour, timeval.tm_min, timeval.tm_sec);
		sleep(1);
	}
}

