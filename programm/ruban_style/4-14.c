#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main(int argc, char **argv)
{
	int a[100];
	int i, k, m, val;

	printf("input: ");
	scanf("%d", &val);

	for (i = 0, m = 2; i < 100 && m < val; ++m) {
		for (k = 0; k < i; ++k) {
			if (!(m % a[k]))
				break;
		}

		if (i == k)
			a[i++] = m;
	}
	int z = i;

	for (i = 0; i < z; ++i)
		printf("%d ", a[i]);

	return 0;
}
