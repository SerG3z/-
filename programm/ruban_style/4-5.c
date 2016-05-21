#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main(int argc, char **argv)
{
	int i, f = 0, end = 0;
	char a[256], b[256];

	printf("input: ");
	fgets(a, sizeof(a), stdin);

	for (i = 0; i != strlen(a); ++i) {
		if (a[i] == '.') f = 1;
		if (f) {
			while (a[i] < 65 || a[i] > 90) {
				if (i == strlen(a)) {
					end = 1;
					break;
				}

				b[i] = a[i];
				++i;
			}

			if (end) break;

			char ch = a[i] + 32;
			b[i] = ch;
			f = 0;
		} else
			b[i] = a[i];
	}
	b[i] = '\0';

	printf("output: %s\n", b);

	return 0;
}
