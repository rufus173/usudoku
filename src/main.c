#include <stdio.h>
#include <stdlib.h>
#include "sudoku.h"
#include "json_sudoku.h"
#include <time.h> 

int main(int argc, char **argv){
	//initalise our board
	sudoku_t sudoku;
	sudoku_init(&sudoku);

	//set the seed
	srandom(time(NULL));

	generate_sudoku(&sudoku);

	shuffle_sudoku(&sudoku);

	//sudoku.array[0][0] = 1;
	//sudoku.array[8][8] = 9;
	
	printf("%s\n",sudoku__str__(&sudoku));

	char *json_represenation = sudoku_to_json_string(&sudoku);
	printf("%s\n",json_represenation);
	free(json_represenation);

	return 0;
}
