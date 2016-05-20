/*
 * Разработать подпрограмму на языке Си, определяющую в массиве
 * максимальную длину последовательности расположенных подряд возрастающих
 * значений, и возвращающую индекс ее начала.
 */
#include <stdio.h>

int find_max_len(char *str, int *max_len)
{
	*max_len = 0;
	int len = 1;
	int start_ptr = 0;
	int ptr = 0;

	for (int i = 1; i < 256; i++) {
		if (str[i] == '\n') {

			if (len > *max_len) {
				*max_len = len;
				start_ptr = ptr;
			}

			break;
		}
		if (str[i-1] < str[i]) {
			len++;
		} else {
			if (len > *max_len) {
				*max_len = len;
				start_ptr = ptr;
			}
			len = 1;
			ptr = i;
		}
	}
	return start_ptr;
}

int main(int argc, char const *argv[])
{
    char string[256];
    int max_len;
    int index;

	printf("Input test string: ");
	fgets(string, 256, stdin);
	
	index = find_max_len(string, &max_len);

	printf("[debug] idx = %d\n",index);
	printf("[debug] len = %d\n",max_len);

	printf("result: ");
	if (max_len == 1) {
		printf("not found\n");
	} else {
		for (int i = 0; i < max_len; i++) {
			printf("%c", string[index]);
			index++;
		}
		printf("\n");
	}
	return 0;
}