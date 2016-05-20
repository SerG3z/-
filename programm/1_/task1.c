#include <stdio.h>

/*
 * Разработать подпрограмму на языке Си, удаляющую лишние пробелы в строке
 * (между словами только один пробел).
 */

void erase_mspace(char *str)
{
	int 	cnt = 0;
	char 	temp[256];

	for (int i = 0; i < 256; i++) {	
		temp[cnt] = str[i];
		if (str[i] == ' ') {
			// skipping
			while (1) {
				if (str[i] == ' ') {
					i++;
				} else {
					--i;
					break;
				}	
			}
		}
		// if end of the string
		if (str[i] == '\n') {
			break;
		}
		cnt++;
	}
	temp[cnt] = '\0';
	
	cnt = 0;
	
	// copy result
	while (1) {
		str[cnt] = temp[cnt];
		if (temp[cnt] == '\0') {
			break;
		}
		cnt++;
	}
	str[cnt] = '\0';
}

int main(int argc, char const *argv[])
{
	char string[256];

	printf("Input test string: ");
	fgets(string, 256, stdin);

	erase_mspace(string);

	printf("Result: %s\n", string);
	return 0;
}