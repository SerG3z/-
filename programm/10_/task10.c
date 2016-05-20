/*
 * Разработать подпрограмму на языке Си, выполняющую поиск подстроки в
 * строке. Строка и подстрока заданы в массивах символов. Результат функции – указатель
 * на начало подстроки в строке или NULL.
 */

#include <stdio.h>

char *compare_str(char *main, char *substr)
{
	char *tmp_sub = substr;

	while (main) {
		if (*substr == '\n') {
			return tmp_sub;
		} else if (*main == '\n') {
			return NULL;
		} else if (*substr == *main) {
			substr++;
		} else {
			return NULL;
		}
		main++;
	}
	return NULL;
}

char *find_substr(char *main, char *substr)
{
	char *ptr = NULL;
	while (main) {
		if (*main == '\n') {
			break;
		}

		if (*substr == *main) {
			ptr = compare_str(main, substr);
			if (ptr) {
				return ptr;
			}
		}
		main++;
	}
	return ptr;
}

int main(int argc, char const *argv[])
{
    char string[256];
    char string1[256];

	printf("Input test string0: ");
	fgets(string, 256, stdin);
	
	printf("Input test string1: ");
	fgets(string1, 256, stdin);

	char *ptr = find_substr(string, string1);

	if (ptr) {
		printf("%s\n", ptr);
	} else {
		printf("not found\n");
	}
	return 0;
}