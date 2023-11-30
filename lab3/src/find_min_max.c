#include "find_min_max.h"
#include <limits.h>

struct MinMax GetMinMax(int *array, unsigned int begin, unsigned int end)
{
	struct MinMax min_max;
	min_max.min = array[1];
	min_max.max = array[1];

	for (unsigned int i = begin; i < end; i++)
	{
		if (array[i] < min_max.min)
		{
			min_max.min = array[i];
		}
		if (array[i] > min_max.max)
		{
			min_max.max = array[i];
		}
	}

	return min_max;
}
