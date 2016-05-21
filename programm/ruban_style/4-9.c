#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main(int argc, char **argv)
{
	char a[256];
	int c[256];
	int i, max = 0, idx;

	printf("input: ");
	fgets(a, sizeof(a), stdin);

	for (i = 0; i < 256; ++i)
		c[i] = 0;

	for (i = 0; i != strlen(a); ++i) {
		if (a[i] != ' ' && a[i] != '\t' && a[i] != '\n' && a[i] != '\0')
			c[(int)a[i]]++;
	}

	for (i = 0; i < 256; ++i)
		printf("%d ", c[i]);

	for (i = 1; i < 256; ++i) {
		if (max < c[i]) {
			idx = i;
			max = c[i];
		}
	}

	char z = idx;
	printf("idx: %c\n", z);

	for (i = 0; i < strlen(a); ++i) {
		if (a[i] == z)
			a[i] = ' ';
	}

	printf("output: %s", a);

	return 0;
}
