#ifndef _SUDOKU_H
#define _SUDOKU_H

#include <stddef.h>

//====== types and structs ======
typedef struct sudoku_t {
	int array[9][9];
} sudoku_t;

//====== prototypes ======
void shuffle_int(int *array,size_t len);


#endif
