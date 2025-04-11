#ifndef _SUDOKU_H
#define _SUDOKU_H

#include <stddef.h>

//====== types and structs ======
typedef struct sudoku_t {
	int array[9][9];
} sudoku_t;

//====== prototypes ======
void shuffle_int(int *array,size_t len);
char *sudoku__str__(sudoku_t *sudoku);
char int_to_char(int val);
void sudoku_init(sudoku_t *sudoku);
void char_replace(char *str,char find,char replace);


#endif
