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

key_t get_key();

int main(int argc, char * argv[])
{
	key_t key = get_key();	

	// СОЗДАНИЕ РАЗДЕЛЯЕМОЙ ПАМЯТИ
	int shm = shmget(key, BUFFER, 0666);
	buffer = shmat(shm, NULL, 0);

	while(1)
	{
		printf("%s\n", buffer);
		sleep(1);
	}
}

key_t get_key() 
{
    int id = 5;
    key_t key = 0;
    
    fopen("key", "r");
    key = ftok("./key", id);
    return key;
}
