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

#include <getopt.h>
#include <signal.h>

#include "find_min_max.h"
#include "utils.h"

int main(int argc, char **argv)
{
	int seed = -1;
	int array_size = -1;
	int pnum = -1;
	int timeout = -1;
	bool with_files = false;

	while (true)
	{
		int current_optind = optind ? optind : 1;

		static struct option options[] = {{"seed", required_argument, 0, 0},
													 {"array_size", required_argument, 0, 0},
													 {"pnum", required_argument, 0, 0},
													 {"timeout", required_argument, 0, 0},
													 {"by_files", no_argument, 0, 'f'},
													 {0, 0, 0, 0, 0}};

		int option_index = 0;
		int c = getopt_long(argc, argv, "f", options, &option_index);

		if (c == -1)
			break;

		switch (c)
		{
		case 0:
			switch (option_index)
			{
			case 0:
				seed = atoi(optarg);
				break;
			case 1:
				array_size = atoi(optarg);
				break;
			case 2:
				pnum = atoi(optarg);
				break;
			case 3:
				timeout = atoi(optarg);
				break;
			case 4:
				with_files = true;
				break;
			defalut:
				printf("Index %d is out of options\n", option_index);
			}
			break;
		case 'f':
			with_files = true;
			break;

		case '?':
			break;

		default:
			printf("getopt returned character code 0%o?\n", c);
		}
	}

	if (optind < argc)
	{
		printf("Has at least one no option argument\n");
		return 1;
	}

	if (seed == -1 || array_size == -1 || pnum == -1)
	{
		printf("Usage: %s --seed \"num\" --array_size \"num\" --pnum \"num\" [--timeout \"num\"]\n",
				 argv[0]);
		return 1;
	}

	int *array = malloc(sizeof(int) * array_size);
	GenerateArray(array, array_size, seed);
	int active_child_processes = 0;

	struct timeval start_time;
	gettimeofday(&start_time, NULL);

	int pipefd[2];
	if (with_files)
	{
		if (pipe(pipefd) == -1)
		{
			perror("pipe");
			exit(EXIT_FAILURE);
		}
	}

	int step = array_size / pnum;
	int min = INT_MAX;
	int max = INT_MIN;

	for (int i = 0; i < pnum; i++)
	{
		pid_t child_pid = fork();
		if (child_pid >= 0)
		{
			// successful fork
			active_child_processes += 1;
			if (child_pid == 0)
			{
				// child process

				struct timeval start_time;
				gettimeofday(&start_time, NULL);

				// Calculate the boundaries of the current step
				int start_index = i * step;
				int end_index = (i == pnum - 1) ? (array_size - 1) : (start_index + step - 1);

				// Find min and max in the current step
				struct MinMax mm = GetMinMax(array, start_index, end_index);
				printf("Process with pid: %d, min: %d, max: %d\n", getpid(), mm.min, mm.max);

				if (with_files)
				{
					FILE *file = fopen("temp.txt", "a");
					fwrite(&mm, sizeof(struct MinMax), 1, file);
					fclose(file);
				}
				else
				{
					write(pipefd[1], &mm, sizeof(struct MinMax));
				}

				return 0;
			}
		}
		else
		{
			printf("Fork failed!\n");
			return 1;
		}
	}

	if (timeout != -1)
	{
		// Wait for the specified timeout
		sleep(timeout);

		// Send SIGKILL to all child processes
		for (int i = 0; i < active_child_processes; i++)
		{
			kill(0, SIGKILL);
		}
	}

	while (active_child_processes > 0)
	{
		// Wait for any child process to finish
		pid_t child_pid = waitpid(-1, NULL, 0);
		active_child_processes -= 1;
	}

	if (with_files)
	{
		FILE *file = fopen("temp.txt", "r");
		struct MinMax mm;
		min = INT_MAX;
		max = INT_MIN;
		while (fread(&mm, sizeof(struct MinMax), 1, file) == 1)
		{
			if (mm.min < min)
				min = mm.min;
			if (mm.max > max)
				max = mm.max;
		}
		fclose(file);
	}
	else
	{
		struct MinMax mm;
		for (int i = 0; i < pnum; i++)
		{
			read(pipefd[0], &mm, sizeof(struct MinMax));
			if (mm.min < min)
				min = mm.min;
			if (mm.max > max)
				max = mm.max;
		}
	}

	struct timeval finish_time;
	gettimeofday(&finish_time, NULL);

	double elapsed_time = (finish_time.tv_sec - start_time.tv_sec) * 1000.0;
	elapsed_time += (finish_time.tv_usec - start_time.tv_usec) / 1000.0;

	free(array);

	printf("Min: %d\n", min);
	printf("Max: %d\n", max);
	printf("Elapsed time: %fms\n", elapsed_time);

	return 0;
}