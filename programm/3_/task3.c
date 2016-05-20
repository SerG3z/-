#include <stdio.h>

//#include <wchar.h>

/*
 * Разработать подпрограмму на языке Си, "переворачивающую" все слова в
 * строке. (Например: "Жили были дед и баба" - "илиЖ илыб дед и абаб").
 */

void reverse_string(char *str)
{
	int fst_p = 0;
	int lst_p = 0;

	int k = 0;
	char temp[256];

	for (int i = 0; i < 256; i++) {
		if (str[i] == ' ') {
			lst_p = i; 
			/*
			for (int j = fst_p; j < lst_p; j++) {
				printf("%c", str[j]);
			}
			*/
			for (int j = lst_p-1; j >= fst_p; j--) {
				//printf("%c", str[j]);
				temp[k] = str[j];
				k++;
			}
			temp[k] = ' ';
			k++;
			fst_p = i + 1;
			//printf("\n");
		} else if (str[i] == '\n') {
			lst_p = i;
			/* 
			for (int j = fst_p; j < lst_p; j++) {
				printf("%c", str[j]);
			}
			*/
			for (int j = lst_p-1; j >= fst_p; j--) {
				//printf("%c", str[j]);
				temp[k] = str[j];
				k++;
			}
			temp[k] = '\0';
			//printf("\n");
			break;
		}
	}

	k = 0;
	// copy result
	while (1) {
		str[k] = temp[k];
		if (temp[k] == '\0') {
			break;
		}
		k++;
	}
	str[k] = '\0';
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