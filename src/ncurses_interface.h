#ifndef _NCURSES_SUDOKU_INTERFACE
#define _NCURSES_SUDOKU_INTERFACE

#include "sudoku.h"
#include <ncurses.h>

int ncurses_sudoku(sudoku_t *sudoku);
void update_sudoku_window(WINDOW *sudoku_window,sudoku_t *sudoku);
void update_dialogue_window(WINDOW *dialogue_window,char *text);

#endif
