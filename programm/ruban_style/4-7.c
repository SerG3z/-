#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main(int argc, char **argv)
{
	char a[256];
	int c[256];
	int pos, i, f = 0, min = 9999;

	printf("input: ");
	fgets(a, sizeof(a), stdin);

	for (i = 0; i < 256; ++i)
		c[i] = 0;

	for (i = 0; i != strlen(a); ++i) {
		if (i == 0 || f) {
			int ai = (int) a[i];
			if (ai > 64 && ai < 91)
				c[i] = ai + 32;
			else
				c[i] = ai;
			f = 0;
		}

		if (a[i] == ' ') f = 1;
	}

	for (i = 0; i < 256; ++i)
		printf("%d ", c[i]);

	for (i = 0; i < 256; ++i) {
		if (min > c[i] && c[i]) {
			min = c[i];
			pos = i;
			printf("pos: %d\n", pos);
		}
	}

	printf("output: %s\n", a + pos);

	return 0;
}
