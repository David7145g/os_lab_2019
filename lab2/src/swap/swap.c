#include "swap.h"

void Swap(char *left, char *right)
{
	char buf = *left; // Сохраняем значение, на которое указывает left
	*left = *right;	// Присваиваем значение, на которое указывает right, левому указателю
	*right = buf;		// Присваиваем сохраненное значение правому указателю
}