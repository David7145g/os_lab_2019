#include "revert_string.h"

void RevertString(char *str)
{
	char *start = str;					  // указать на начало
	char *end = str + strlen(str) - 1; // указатель на конец строки

	// Обмен символами до сходимости указателей в середине строки
	while (start < end)
	{
		char temp = *start;
		*start = *end;
		*end = temp;

		start++;
		end--;
	}
}
