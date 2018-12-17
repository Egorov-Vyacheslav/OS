#include <time.h>
#include <error.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/types.h>

#define BUFFER 1024

struct sembuf lock = { 0, -1, SEM_UNDO };
struct sembuf unlock = { 0, 1, SEM_UNDO };

int main(int argc, char * argv[])
{
	key_t key = 3972;

    // СОЗДАНИЕ РАЗДЕЛЯЕМОЙ ПАМЯТИ
    int shm = shmget(key, BUFFER, IPC_CREAT | 0666);
	char * buffer = shmat(shm, NULL, 0);

	int sem = semget(key, 1, 0666);

	while(1)
	{
		semop(sem, &lock, 1);
		printf("Locked!\n");

		printf("%s\n", buffer);

		semop(sem, &unlock, 1);
		printf("Unlocked!\n");
		sleep(1);
	}

}
