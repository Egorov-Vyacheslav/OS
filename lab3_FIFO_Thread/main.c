#include <time.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/stat.h>
#include <sys/types.h>

#define BUFFER 100

int fd;
char * buffer;

void * write_thread();
void current_time(char * buffer);

int main(int argc, char * argv[])
{
	pthread_t wthread;
	buffer = (char *)calloc(BUFFER, sizeof(char));
	mkfifo("./fifo_file", 0777);
	fd = open("./fifo_file", O_WRONLY);
	
	pthread_create(&wthread, NULL, write_thread, NULL);
	
	while(1)
	{
		sleep(1);
	}
}

void current_time(char * buffer)
{
	time_t timer = time(0);
	struct tm ctime = *localtime(&timer);
	sprintf(buffer, "%.2d:%.2d:%.2d\n", ctime.tm_hour, ctime.tm_min, ctime.tm_sec);
}

void * write_thread()
{
	while(1)
	{
		current_time(buffer);
		write(fd, buffer, BUFFER);
		sleep(1);
	}
}
