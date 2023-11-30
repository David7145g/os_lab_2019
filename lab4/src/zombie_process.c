#include <ctype.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>

#include <ctype.h>
#include <getopt.h>

int main()
{
	pid_t childPid = fork();

	if (childPid == -1)
	{
		perror("fork");
		return 1;
	}

	if (childPid == 0)
	{
		// Дочерний процесс
		printf("Дочерний процесс выполнил свою работу.\n");
		printf("PID дочернего процесса: %d\n", getpid());
	}
	else
	{
		// Родительский процесс
		printf("Родительский процесс ожидает завершения дочернего процесса.\n");
		sleep(2); // Даем дочернему процессу время завершиться
		printf("PID родительского процесса: %d\n", getpid());
		printf("Дочерний процесс стал зомби. Завершение родительского процесса.\n");
	}

	return 0;
}