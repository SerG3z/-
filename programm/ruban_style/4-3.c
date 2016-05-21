#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main(int argc, char **argv)
{
	char a[32], b[32];
	int i, j, len = 0;

	printf("input: ");
	fgets(a, sizeof(a), stdin);

	for (i = 0; a[i] != '\n'; ++i)
		len++;

	printf("len: %d\n", len);

	for (i = len - 1, j = 0; j != len; --i, ++j)
		b[j] = a[i];
	b[j] = '\0';

	printf("output: %s\n", b);

	return 0;
}
