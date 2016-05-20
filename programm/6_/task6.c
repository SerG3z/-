/*
 * Разработать подпрограмму на языке Си, заменяющую в строке принятое в Си
 * обозначение символа с заданным кодом в восьмеричной системе счисления на сам символ
 * в базовой кодировке ASCII. Например, ASCII код символа A – 4116 и 1018 тогда строка
 * “test101 and test_102” должна быть преобразована в строку “testA and test_B”.
 */

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

int conver(int octal)
{
	int decimal = 0;
    int i = 0;

    while (octal != 0) {
        decimal = decimal + (octal % 10) * pow(8,i++);
        octal = octal/10;
    }
    return decimal;
}

void transfer(char *str)
{
	char tmp[256];
	int j = 0;

	for (int i = 0; i < 256; i++) {	
		if ((str[i] >= 'a' && str[i] <= 'z') || (str[i] == ' ')) {
			tmp[j] = str[i];
			j++;
		} else {
			if (str[i] == '\n') {
				break;
			} else {
				char hex[256];
				int k = 0;

				while(1) {
					if (str[i] >= '0' && str[i] <= '9') {
						hex[k] = str[i];
						k++;
					} else if (str[i] == ' ') {
						hex[k] = '\0';
						tmp[j] = conver(atoi(hex));
						j++;
						tmp[j] = str[i];
						j++;
						break;
					} else if (str[i] == '\n') {
						i--;
						hex[k] = '\0';
						tmp[j] = conver(atoi(hex));
						j++;
						break;
					}
					i++;
				}
			}
		}
	}
	tmp[j] = '\0';

	for (int i = 0; i < 256; i++) {	
		str[i] = tmp[i];
		if (tmp[i] == '\0') {
			break;
		}
	}
}

int main(int argc, char const *argv[])
{
	char string[256];

	printf("Input test string: ");
	fgets(string, 256, stdin);
	
	transfer(string);
	
	printf("Result: %s\n", string);
	return 0;
}