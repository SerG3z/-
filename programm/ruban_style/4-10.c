#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main(int argc, char **argv)
{
	char a[256], b[256];

	printf("input string: ");
	fgets(a, sizeof(a), stdin);
	a[strlen(a) - 1] = '\0';

	printf("input substring: ");
	fgets(b, sizeof(b), stdin);
	b[strlen(b) - 1] = '\0';

	printf("output: %s\n", strstr(a, b));

	return 0;
}
