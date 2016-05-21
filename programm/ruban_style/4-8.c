#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main(int argc, char **argv)
{
	int i, j, l, cnt = 0;
	char a[256], b[256];

	printf("input: ");
	fgets(a, sizeof(a), stdin);

	for (i = 0, l = 0; i != strlen(a); ++i, ++l) {
		if (a[i] == '/' && a[i + 1] == '*') {
			cnt = 1;
			i += 2;
			for (j = i; j != strlen(a); ++j) {
				if ((a[j] == '/') && (a[j + 1] == '*')) {
					++cnt;
					j += 2;
				}
				else if ((a[j] == '*') && (a[j + 1] == '/')) {
					--cnt;
					j += 2;
				}

//				printf("cnt: %d\n", cnt);

				if (!cnt) {
					i = j;
					break;
				}
			}
		}

		b[l] = a[i];
	}

	b[l] = '\0';

	printf("output: %s\n", b);

	return 0;
}
