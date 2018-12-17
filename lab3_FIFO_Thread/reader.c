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

void * read_thread();

int main(int argc, char * argv[])
{
	pthread_t rthread;
	buffer = (char *)calloc(BUFFER, sizeof(char));
	mkfifo("./fifo_file", 0777);
	fd = open("./fifo_file", O_RDONLY);
	
	pthread_create(&rthread, NULL, read_thread, NULL);
	
	char c;
	scanf("%c", &c);
	if (c)
	{
		pthread_cancel(rthread);
		close(fd);
		exit(EXIT_SUCCESS);
		return 0;
	}	
}

void * read_thread()
{
	while(1)
	{
		if (read(fd, buffer, BUFFER) <= 0)
		{
			printf("Чтение FIFO невозможно!!!");
			exit(0);
		}
		
		printf("Другой процесс передает нам... %s\n", buffer);
		sleep(1);		
	}
}











