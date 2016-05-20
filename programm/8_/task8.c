/*
 * Разработать подпрограмму на языке Си, удаляющую из строки комментарии
 * вида /*\ ... *\/. Игнорировать вложенные комментарии.
 */

#include <stdio.h>

void reverse_string(char *str)
{
	int comment_count = 0;

	int k = 0;
	char temp[256];

	for (int i = 0; i < 256; i++) {
		if ((str[i] == '/') && (str[i+1] == '*')){
			comment_count++;
			i++;
		} else if ((str[i] == '*') && (str[i+1] == '/')) {
			i++;
			comment_count--;
		} else if (comment_count == 0) {
			temp[k] = str[i];
			k++;
		} else if (str[i] == '\n') {
			temp[k] = '\0';
			break;
		}
	}

	printf("%s\n", temp);
}

int main(int argc, char const *argv[])
{
    char string[256];

	printf("Input test string: ");
	fgets(string, 256, stdin);

	reverse_string(string);

	printf("Result: %s\n", string);

	return 0;
}