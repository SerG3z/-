/*
 * Разработать подпрограмму на языке Си, выполняющую поиск конца
 * предложения в строке (символ "точка"), в первом слове следующего предложения 
 * необходимо выполнить замену первой строчной буквы на прописную. Между словами
 * количество пробелов может быть любым
 */

#include <stdio.h>

/*
 An IC that contains a CPU may also contain memory, peripheral interfaces, 
 and other components of a computer. such integrated devices are variously called microcontrollers. 
 or systems on a chip (SoC). 
*/

void replace_fst_sign(char *str)
{
	for (int i = 0; i < 256; i++) {	
		if (str[i] == '.') {
			while (1) {
				i++;
				if (str[i] == ' ') {
					continue;
				} else if (str[i] == '\n') {
					return;
				} else {
					break;
				}	
			}
			char c = str[i];
			if (c >= 'a' && c <= 'z') {
    			c = (c - 'a' + 'A');
    			str[i] = c;
   			}
		}
		// if end of the string
		if (str[i] == '\n') {
			break;
		}
	}
}

int main(int argc, char const *argv[])
{
	char string[256];

	printf("Input test string: ");
	fgets(string, 256, stdin);

	replace_fst_sign(string);

	printf("Result: %s\n", string);
	return 0;
}