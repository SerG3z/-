/*
 * Разработать подпрограмму на языке Си, выполняющую поиск в строке наиболее
 * часто встречающегося символа, и заменяющую его на пробел.
 */

#include <stdio.h>
 
char find_max_Sym(char *string)
{   
    //Массив, хранящий число вхождений символов
    size_t cnt[256] = {};
    //Заполнение массива
    while(*string) {
        cnt[(int)(*(string++))]++;
    }

    size_t max = 0;
    //Поиск наиболее часто встретившегося символа
    for (size_t i = 1; i < 256; i++) {
        if (cnt[i] > cnt[max]) {
            max = i;
        }
    }
    return (char)max;
}

int main(int argc, char const *argv[])
{
    char string[256];

	printf("Input test string: ");
	fgets(string, 256, stdin);

	char sym = find_max_Sym(string);

	for (int i = 0; i < 256; i++) {
    	if (string[i] == sym) {
    		string[i] = '*';
    	}
    }

	printf("Result: %s\n", string);

	return 0;
}