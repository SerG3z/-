/*
 * Разработать подпрограмму на языке Си, выполняющую поиск слова в строке,
 * начинающегося с самой младшей латинской буквы, и возвращающую указатель на его
 * начало.
 */
#include <stdio.h>

void sort_fst_sign(char *str)
{
	int fst_p = 0;
	int lst_p = 0;

	int k = 0;
	char *temp[256];

	temp[k] = &str[0]; // remember 1st sign of the 1st word
	k++;

	for (int i = 0; i < 256; i++) {
		// find space between words
		if (str[i] == ' ') {
			i++;
			temp[k] = &str[i]; // remember 1st sign of the word
			k++;
		} else if (str[i] == '\n') {
			break;
		}
	}

	// find min
	char *min_idx;
	char min_char = 'z';

	for (int i = 0; i < k; i++) {
		char c = *temp[i];
		if (c < min_char) {
			min_char = c;
			min_idx = temp[i];
		}
	}

	printf("Min word:\n");
	str = min_idx;
	while (1) {
		if ( (*str == ' ') || (*str == '\n') ) {
			printf("\n");
			break;
		}
		printf("%c", *str);
		str++;
	}
}

int main(int argc, char const *argv[])
{
    char string[256];

	printf("Input test string: ");
	fgets(string, 256, stdin);

	sort_fst_sign(string);

	//printf("Result: %s\n", string);

	return 0;
}