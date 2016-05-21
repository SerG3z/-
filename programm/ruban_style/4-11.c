#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main(int argc, char **argv)
{
	srand((unsigned)time(NULL));
	int a[15];
	int pos = 0, cnt = 1, max = 0, i;

	for (i = 0; i < 15; ++i)
		a[i] = rand() % 9;

	for (i = 0; i < 15; ++i)
		printf("%d ", a[i]);
	printf("\n");

	for (i = 1; i < 15; ++i) {
		if (a[i] > a[i - 1])
			++cnt;
		else {
			if (cnt > max) {
				max = cnt;
				pos = i - cnt;
				printf("cnt: %d\n", cnt);
				printf("pos: %d\n", pos);
				cnt = 1;
			} else
				cnt = 1;
		}
	}

	printf("idx: %d\n", pos);

	return 0;
}
