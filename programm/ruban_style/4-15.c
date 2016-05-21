#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Result { size_t pos; int isFound; };

struct Result make_result(size_t pos, int isFound)
{
	struct Result r;

	r.pos = pos;
	r.isFound = isFound;

	return r;
}

#define FOUND(i)	make_result(i, 1)
#define NOTFOUND(i)	make_result(i, 0)

char table[10][32] = {
	"Aaaab",
	"Aaaacde",
	"Bravo",
	"Helga",
	"Hooligan",
	"Predictable",
	"Romeo",
	"Xanax",
	"Zadnica",
	"Zoo"
};

struct Result binary_search(size_t n, char *b)
{
	size_t first = 0;
	size_t last = n;

	if (n == 0) {
		return NOTFOUND(0);
	} else if (strcmp(table[0], b) == 1) {
		return NOTFOUND(0);
	} else if (strcmp(table[n - 1], b) == -1) {
		return NOTFOUND(n);
	}

	while (first < last) {
		size_t mid = first + (last - first) / 2;

		if (strcmp(table[mid], b) == 1 || strcmp(table[mid], b) == 0)
			last = mid;
		else
			first = mid + 1;
	}

	if (strcmp(table[last], b) == 0) {
		return FOUND(last);
	} else {
		return NOTFOUND(last);
	}

}

int main(int argc, char **argv)
{
	char str[32];

	printf("Input string: ");
	fgets(str, sizeof(str), stdin);
	str[strlen(str) - 1] = '\0';
	printf("%s\n", str);

	struct Result r = binary_search(10, str);

	printf("%s at %zu\n", r.isFound ? "Found" : "Not found", r.pos);

	return 0;
}
