#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main()
{
	pid_t pid;
	int code;
	pid = fork();
	
	switch(pid)
	{
		case -1:
			printf("Error!!!");
			exit(1);
		
		case 0:
			printf("ДОЧЕРНИЙ ПРОЦЕСС: это дочерний процесс!\n");
			printf("ДОЧЕРНИЙ ПРОЦЕСС: мой pid - %d\n", getpid());
			printf("ДОЧЕРНИЙ ПРОЦЕСС: pid моего родителя - %d\n", getppid());
			printf("ДОЧЕРНИЙ ПРОЦЕСС: введите возвращаемый код: ");
			scanf("%d", &code);
			printf("ДОЧЕРНИЙ ПРОЦЕСС: выход!\n");
			exit(code);
		
		default:
			printf("РОДИТЕЛИ: это родитель потомка!\n");
			printf("РОДИТЕЛИ: мой pid - %d\n", getpid());
			printf("РОДИТЕЛИ: pid моего потомка - %d\n", pid);
			printf("РОДИТЕЛИ: жду ... \n");
			wait(&code);
			printf("РОДИТЕЛИ: код возврата потомка - %d\n", WEXITSTATUS(code));
			printf("РОДИТЕЛИ: выход!");
	}
}