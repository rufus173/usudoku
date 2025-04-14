#include "ncurses_interface.h"
#include "sudoku.h"
#include <ncurses.h>
#include <stdlib.h>
#include <stdio.h>
#include <locale.h>

#define SUDOKU_WIDTH 20
#define SUDOKU_HEIGHT 11
#define DIALOGUE_HEIGHT 5

#define MIN_WIDTH SUDOKU_WIDTH
#define MIN_HEIGHT (SUDOKU_HEIGHT+DIALOGUE_HEIGHT)

/*
+-----------+ the top half if the actual board,
|           | and the bottom is dialogue for errors
| sudoku    | or hints.
| board     |
|           | this whole structure is centred
|           | in the middle of the terminal
+-----------+
| dialogue  |
| window    |
+-----------+
*/

int ncurses_sudoku(sudoku_t *sudoku){
	//====== initialisation of ncurses ======
	setlocale(LC_ALL,"");
	initscr();
	//====== check if it can fit in the terminal ======
	if (LINES < MIN_HEIGHT || COLS < MIN_WIDTH){
		printw("Terminal to small:\nlines %d - requires %d\ncolumns %d - requires %d\n",LINES,MIN_HEIGHT,COLS,MIN_WIDTH);
		getch();
		endwin();
		return EXIT_FAILURE;
	}
	//====== create windows ======
	WINDOW *sudoku_window = newwin(SUDOKU_HEIGHT,SUDOKU_WIDTH,(LINES/2)-(MIN_HEIGHT/2),(COLS/2)-(MIN_WIDTH/2));
	WINDOW *dialogue_window = newwin(DIALOGUE_HEIGHT,SUDOKU_WIDTH,(LINES/2)-(MIN_HEIGHT/2)+SUDOKU_HEIGHT,(COLS/2)-(MIN_WIDTH/2));
	box(sudoku_window,0,0);
	box(dialogue_window,0,0);
	refresh();
	wrefresh(sudoku_window);
	wrefresh(dialogue_window);

	getch();

	//====== stop ncurses ======
	endwin();
	return EXIT_SUCCESS;
}
