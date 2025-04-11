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
