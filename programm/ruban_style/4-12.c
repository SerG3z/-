#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define eps 6

int main(int argc, char **argv)
{
	int i = 1, j;
	double a;
	char s[10], res[10];

	printf("input: ");
	scanf("%lf", &a);

	int tmp = (int)(a * pow(10, eps));
	s[0] = (char)(fmod(tmp, 10) + '0');

	while ((int)(tmp /= 10)) {
		if (i == eps)
			s[i++] = '.';
		s[i++] = (char)(fmod(tmp, 10) + '0');
	}
	s[i] = '\0';

	for (i = strlen(s) - 1, j = 0; i >= 0; --i, ++j)
		res[j] = s[i];
	res[j] = '\0';

	printf("output: %s\n", s);
	printf("output: %s\n", res);

	return 0;
}
