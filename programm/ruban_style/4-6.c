#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main(int argc, char **argv)
{
	char a[256], b[256];
	char *val;
	int i, j;

	printf("input: ");
	fgets(a, sizeof(a), stdin);

	for (i = 0, j = 0; i != strlen(a); ++i, ++j) {
		if (a[i] > 47 && a[i] < 58) {
			int l = 0;
			val = (char *) malloc(sizeof(char) * 3);

			while (a[i] > 47 && a[i] < 58) {
				val[l] = a[i];
				++i; ++l;
			}

			if (l > 3 || atoi(val) > 177) {
				printf("Error!\n");
				return - 1;
			}

			long int oct = atoi(val);
			printf("octal: %ld\n", oct);

			int k = 0;
			long int dec = 0;

			while (oct) {
				dec += (oct % 10) * pow(8, k++);
				oct /= 10;
			}

			printf("decimal: %ld\n", dec);

			char ch = (char) dec;
			b[j++] = ch;
			b[j] = a[i];
		} else
			b[j] = a[i];
	}
	b[j] = '\0';

	printf("output: %s\n", b);

	return 0;
}
