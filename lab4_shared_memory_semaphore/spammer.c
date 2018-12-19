#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/types.h>

#define BUFFER 1024

int sem;
char * buffer;
struct sembuf lock = { 0, -1, SEM_UNDO };
struct sembuf unlock = { 0, 1, SEM_UNDO };

void time_spam();
key_t get_key();

int main(int argc, char * argv[])
{
	key_t key = get_key();

    // СОЗДАНИЕ РАЗДЕЛЯЕМОЙ ПАМЯТИ
    int shm = shmget(key, BUFFER, IPC_CREAT | 0666);
	buffer = shmat(shm, buffer, 0);

	// СОЗДАНИЕ СЕМАФОРА
	sem = semget(key, 1, IPC_CREAT | 0666);
    struct sembuf init = {0, 1, SEM_UNDO};
	semop(sem, &init, 1);

    time_spam();
} 

void time_spam()
{
	struct tm timeval;
    for(int i = 0; i < 10; i++) 
    {
    	// ЗАКРЫТИЕ СЕМАФОРА
        semop(sem, &lock, 1);
        printf("Locked!\n");

        time_t timer = time(0);
        timeval = *localtime(&timer);
        sprintf(buffer, "%.2d:%.2d:%.2d", timeval.tm_hour, timeval.tm_min, timeval.tm_sec);

        // ОТКРЫТИЕ СЕМАФОРА
        semop(sem, &unlock, 1);
        printf("Unlocked!\n");

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