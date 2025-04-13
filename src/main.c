#include <stdio.h>
#include <stdlib.h>
#include "sudoku.h"
#include <time.h> 

int main(int argc, char **argv){
	//initalise our board
	sudoku_t sudoku;
	sudoku_init(&sudoku);
	generate_sudoku(&sudoku);

	//set the seed
	srandom(time(NULL));
	
	//sudoku.array[0][0] = 1;
	//sudoku.array[8][8] = 9;
	
	printf("%s\n",sudoku__str__(&sudoku));

	return 0;
}
