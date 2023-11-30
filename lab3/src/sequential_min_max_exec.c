#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main()
{
	// Создаем новый процесс с помощью fork()
	pid_t pid = fork();

	if (pid == -1)
	{
		// Если произошла ошибка при создании процесса, выводим сообщение об ошибке
		perror("fork");
		return 1;
	}
	else if (pid == 0)
	{
		// Код, выполняемый в дочернем процессе
		// Исполняемый файл и аргументы для передачи в sequential_min_max
		char *args[] = {"sequential_min_max", NULL};

		// Запускаем sequential_min_max с помощью execvp
		execvp("sequential_min_max", args);
		
		return 1;
	}
	
	return 0;
}