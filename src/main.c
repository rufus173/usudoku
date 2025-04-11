#include <stdio.h>
#include <stdlib.h>
#include "sudoku.h"
#include <time.h> 

int main(int argc, char **argv){
	sudoku_t sudoku;

	srandom(time(NULL));
	
	sudoku.array[0][0] = 1;
	int test[8] = {1,2,3,4,5,6,7,8};
	shuffle_int(test,8);
	for (int i = 0; i < 8; i++) printf("%d,",test[i]);
	printf("\n");

	return 0;
}
