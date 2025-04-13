#include "sudoku.h"
#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>

void shuffle_int(int *array,size_t len){
	//fisher-yates shuffle
	for (size_t i = 0; i < len; i++){ //decrease size of pool
		//pick an index to swap
		size_t swap_index = random()%(len-i);
		//swap
		int temp = array[len-1-i];
		array[len-1-i] = array[swap_index];
		array[swap_index] = temp;
	}
}

char int_to_char(int val){
	char out = val+48;
	return out;
}
void char_replace(char *str,char find,char replace){
	for (int i = 0; i < strlen(str); i++){
		if (str[i] == find) str[i] = replace;
	}
}

char *sudoku__str__(sudoku_t *sudoku){
	static char output[4000]; //probably big enough
	strcpy(output,"╔═══╤═══╤═══╦═══╤═══╤═══╦═══╤═══╤═══╗\n");

	//for each mini 3x3, then for each row in that 3x3, except the last one doesnt get a seperator.
	int x = 0;
	for (int i = 0; i < 3; i++){
		for (int j = 0; j < 3; j++){
			char line[150];
			//these are widechars so uhhhh yeah give me a break
			snprintf(
				line,sizeof(line),"║ %d │ %d │ %d ║ %d │ %d │ %d ║ %d │ %d │ %d ║\n",
				sudoku->array[x][0],
				sudoku->array[x][1],
				sudoku->array[x][2],
				sudoku->array[x][3],
				sudoku->array[x][4],
				sudoku->array[x][5],
				sudoku->array[x][6],
				sudoku->array[x][7],
				sudoku->array[x][8]
			);
			char_replace(line,'0',' ');
			strcat(output,line);
			//row seperators
			if (i == 2 && j == 2){
				//the bottom peice
				strcat(output,"╚═══╧═══╧═══╩═══╧═══╧═══╩═══╧═══╧═══╝\n");
			}else if (j == 2){
				//the last row in the 3x3 gets a thicker seperator but not the last last row
				strcat(output,"╠═══╪═══╪═══╬═══╪═══╪═══╬═══╪═══╪═══╣\n");
			}else{
				//seperator between rows
				strcat(output,"╟───┼───┼───╫───┼───┼───╫───┼───┼───╢\n");
			}
			x++;
		}
	}
	return output;
}

void sudoku_init(sudoku_t *sudoku){
	for (int i = 0; i < 9; i++) memset(sudoku->array[i],0,sizeof(int)*9);
}

int generate_sudoku(sudoku_t *sudoku){
	int x,y = 0;
	if(find_empty_square(&x,&y,sudoku)){
		//====== the grid still has empty squares ======
		//get possible values that fit in the x and y we picked
		int possible_values[9];
		size_t possible_values_length = get_possible_values(x,y,possible_values,sudoku);
		//shuffle the values so the generated sudoku is different every time
		shuffle_int(possible_values,possible_values_length);

		//try all the possible values
		for (int i = 0; i < possible_values_length; i++){
			int possible_value = possible_values[i];
			sudoku->array[x][y] = possible_value;

			//create a new branch and try solution
			//if solution is found, return immediately and preserve the state of the grid
			printf("%s\n",sudoku__str__(sudoku));
			if (generate_sudoku(sudoku)) return 1;
		}

		//====== no successfull solution on current branch ======
		//return the grid to where it was at the start of the function call if no solutions were found
		sudoku->array[x][y] = 0;
		return 0;
	}else{
		//====== the grid is full (sudoku complete) ======
		//base case where solution has been found
		return 1;
	}
}
int find_empty_square(int *x_return,int *y_return,sudoku_t *sudoku){
	for (int x = 0; x < 9; x++){
		for (int y = 0; y < 9; y++){
			if (sudoku->array[x][y] == 0){
				*x_return = x; *y_return = y;
				return 1;
			}
		}
	}
	return 0;
}
size_t get_possible_values(int x, int y, int possible_values_return[9],sudoku_t *sudoku){
	//basicly we commit a process of elimination to be left with all the possible values
	int possible_values[9] = {1,2,3,4,5,6,7,8,9};

	//====== row ======
	for (int x = 0; x < 9; x++){
		int value = sudoku->array[x][y];
		if (value != 0){
			possible_values[value-1] = 0;
		}
	}

	//====== column ======
	for (int y = 0; y < 9; y++){
		int value = sudoku->array[x][y];
		if (value != 0){
			possible_values[value-1] = 0;
		}
	}

	//====== square ======
	//this is just div because we are performing maths with ints so the result will be an int
	int square_x = (x/3);
	int square_y = (y/3);
	for (int x = square_x; x < square_x+3; x++){
		for (int y = square_y; y < square_y+3; y++){
			int value = sudoku->array[x][y];
			if (value != 0){
				possible_values[value-1] = 0;
			}
		}
	}

	//====== fill the return buffer ======
	size_t possible_values_return_length = 0;
	for (int i = 0; i < 9; i++){
		if (possible_values[i] != 0){
			possible_values_return[possible_values_return_length] = possible_values[i];
			possible_values_return_length++;
		}
	}
	return possible_values_return_length;
}
