#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int nod(int a, int b) { return (a ? nod(b % a, a) : b); }
int nok(int a, int b) { return a * b / nod(a, b); }

int main(int argc, char **argv)
{
	srand((unsigned)time(NULL));

	int i, j, res = 1;
	int mas[5] = { 24, 36, 144, 48, 72 };

	for (i = 0; i < 5; ++i)
		printf("%d ", mas[i]);

	for (i = 0; i < 5; ++i)
		res = nok(res, mas[i]);

	printf("res: %d\n", res);

	return 0;
}
