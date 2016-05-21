#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main(int argc, char **argv)
{
	int i, j;
	char buf[256], res[256];

	printf("input string: ");
	fgets(buf, sizeof(buf), stdin);

	int flag = 0;

	for (i = 0, j = 0; buf[i] != '\0'; ++i) {
		if (buf[i] == ' ') {
			flag++;
			if (flag < 2)
				res[j++] = ' ';
		}
		else {
			flag = 0;
			res[j++] = buf[i];
		}
	}
	res[j] = '\0';
	printf("result: %s", res);

	return 0;
}
