#include <stdio.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
#include <stddef.h>
#include <stdlib.h>
#include <sys/stat.h>

#define SOURCE_FILE "source.txt"
#define TARGET_FILE "target.txt"

#define FILE 100000000 //100mb
#define BUFFER 10000000

int fd_set_blocking(int fd, int blocking)
{
	int flags = fcntl(fd, F_GETFL, 0);
	if (flags == -1)
		return 0;
	if(blocking)
		flags &= ~O_NONBLOCK;
	else flags |= O_NONBLOCK;
	return fcntl(fd, F_SETFL, flags) != -1;
}

int generate()
{
	int fd;
	char * buffer = (char*)calloc(BUFFER, sizeof(char));

	for (int i = 0; i < BUFFER; i++)	//generating buffer
		buffer[i] = '0' + i%10;

	if((fd=open(SOURCE_FILE, O_RDWR | O_TRUNC | O_CREAT | O_NONBLOCK, S_IWRITE|S_IREAD)) < 0)
  {
		return -1;
	}
  else
  {
		printf("Starting generating file %s\n", SOURCE_FILE);
		for(int i = 0; i < FILE/BUFFER; i++)
    {
			write(fd, buffer, BUFFER);
			printf("%d/10 of file generated\n", i+1);
		}
	}
	printf("File %s generated\n", SOURCE_FILE);
	close(fd);
	return 0;
}


int source()
{
	fd_set rfds;
	fd_set wfds;

	FD_ZERO(&rfds);
	FD_ZERO(&wfds);

	struct timeval tv;

	int rfd = open(SOURCE_FILE, O_RDONLY | O_CREAT);
	int wfd = open(TARGET_FILE, O_WRONLY | O_TRUNC | O_CREAT, S_IWRITE|S_IREAD);

	if( wfd < 0 )
  {
		printf("Can't open file wfd\n");
		return -1;
	}
	if( rfd < 0 )
  {
    printf("Can't open file rfd\n");
    return -1;
  }

	fd_set_blocking(rfd, 0);
	fd_set_blocking(wfd, 0);

	FD_SET( rfd, &rfds );
	FD_SET( wfd, &wfds );

	int maxfd;
  if (rfd > wfd)
  {
    maxfd = rfd;
  }
  else
  {
    maxfd = wfd;
  }

	char * buffer = (char*)calloc(BUFFER, sizeof(char));

	tv.tv_sec = 1;
	tv.tv_usec = 0;

	int ret;
	int i = 0;
	ssize_t buf_amount;

	if( (ret = select(maxfd + 1, &wfds, &rfds, NULL, &tv)) == -1 )
  {
		return -1;
	}
  else if(ret == 0)
  {
		printf("1 sec wait. No data\n");
	}
  else
  {
		if( FD_ISSET( rfd, &rfds ) )
    {
			buf_amount = read(rfd, buffer, BUFFER);
			do
      {
				i++;
				write(wfd, buffer, buf_amount);
				buf_amount = read(rfd, buffer, BUFFER);
				printf("%d/10 of file copied\n", i);
			}while(buf_amount);
		}

		if( FD_ISSET( wfd, &wfds )) {}
	}

	close(rfd);
	close(wfd);
	printf("File %s copied\n", SOURCE_FILE);
	return(0);
}

int main(void)
{
	pid_t pid = fork();
	if(pid == 0)
  {
		sleep(1);
		if(source() == -1)
    {
			printf("Copying failed");
			exit(0);
		}
	}
	else
  {
		if(generate() == -1)
    {
			printf("Can't open %s", SOURCE_FILE);
			exit(0);
		}
	}
	return 0;
}
