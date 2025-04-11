#include "sudoku.h"
#include <stdio.h>
#include <stddef.h>
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
