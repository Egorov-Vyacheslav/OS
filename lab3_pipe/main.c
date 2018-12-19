#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

#define BUFFER 80

void write_time(char * buffer);

int main(int argc, char * argv[]) 
{
    int fd[2];
    pid_t pid;
    char * buffer = (char *)calloc(BUFFER, sizeof(char));

    pipe(fd);

    if ((pid = fork()) == 0) 
    {
        close(fd[0]);
        while(1) 
        {
            write_time(buffer);
            write(fd[1], buffer, BUFFER);
            sleep(1);
        }
        close(fd[1]);
    } 
    else 
    {
        close(fd[1]);
        while (read(fd[0], buffer, BUFFER))
        {
            printf("Receive from child process: %s\n", buffer);
        }
        close(fd[0]);
    }
}

void write_time(char * buffer) 
{
    time_t timer = time(0);
    struct tm local_time = *localtime(&timer);
    sprintf(buffer, "%.2d:%.2d:%.2d\n", local_time.tm_hour, local_time.tm_min, local_time.tm_sec);
}