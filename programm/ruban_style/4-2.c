#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main(int argc, char **argv)
{
	if (argc != 2) return -1;

	int value = atoi(argv[1]);

	int mas[100];
	int i, n = 0, k, factor = 2;

	for (i = 0; i < 100 && value != 1; ++i, ++n) {
		while (value % factor) ++factor;
		value /= factor;
		mas[i] = factor;
		printf("val: %d\n", value);
	}

	for (i = 0; i < n; ++i)
		printf("%d ", mas[i]);
	printf("\n");

	return 0;
}
