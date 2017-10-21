#include <stdio.h>
#include <stdlib.h>

int main () {
	int length = 8;
	
	for (int i = 0; i < length; ++i) {
		for (int j = 0; j < length; ++j) {
			int num = (i*length) + (j+1);
			printf("%d", num);
			printf(" ");
			if ((num / 10) < 1) {
				printf(" ");
			}
		}
		printf("\n");
	}
}