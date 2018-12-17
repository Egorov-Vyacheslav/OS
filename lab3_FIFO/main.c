#include <time.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

#define BUFFER 100

void write_time(char *);

int main (int argc, char * argv[]) 
{
	int val;
    pid_t pid;
    char * buffer = (char *)calloc(BUFFER, sizeof(char));
    val = mkfifo("./fifo_file", 0777);

	if (val == 0)
	{
		printf("Done!\n");
	}		
	
    pid = fork();

    if (pid == 0) 
    {
        int fd = open("./fifo_file", O_WRONLY);
        write_time(buffer);
        write(fd, buffer, BUFFER);
        close(fd);
    } 
	else 
	{
        int fd = open("./fifo_file", O_RDONLY);
        read(fd, buffer, BUFFER);
        printf("Receive from other process: %s\n", buffer);
        close(fd);
    }
}

void write_time(char * str) 
{
    time_t timer = time(0);
    struct tm timeval = *localtime(&timer);

    sprintf(str, "[%.2d:%.2d:%.2d]\n", timeval.tm_hour, timeval.tm_min, timeval.tm_sec);
}