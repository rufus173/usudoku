#include "ncurses_interface.h"
#include <string.h>
#include "sudoku.h"
#include <ncurses.h>
#include <stdlib.h>
#include <stdio.h>
#include <locale.h>
#include <curses.h>
#include <unistd.h>
#include <assert.h>

#define MIN(val1,val2) ((val1 < val2) ? (val1) : (val2))
#define MAX(val1,val2) ((val1 > val2) ? (val1) : (val2))
#define CLAMP(val,min,max) (MAX(MIN(val,max),min))

#define SPLASH_DIALOGUE "q: quit, backspace: delete number\narrows: move\nnumbers: enter number"

#define SUDOKU_WIDTH 37
#define SUDOKU_HEIGHT 19
#define DIALOGUE_HEIGHT 5

#define MIN_WIDTH SUDOKU_WIDTH
#define MIN_HEIGHT (SUDOKU_HEIGHT+DIALOGUE_HEIGHT)

enum {
	PAIR_DEFAULT = 1, 
	PAIR_USER_ENTERED = 2, //colours for the numbers the user enters
} pairs;

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
	noecho();
	keypad(stdscr,true);
	start_color();
	//====== custom colours ======
	init_pair(PAIR_DEFAULT,COLOR_WHITE,COLOR_BLACK);
	init_pair(PAIR_USER_ENTERED,COLOR_GREEN,COLOR_BLACK);
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
	box(dialogue_window,0,0);
	refresh();
	wrefresh(sudoku_window);
	wrefresh(dialogue_window);
	update_sudoku_window(sudoku_window,sudoku);

	//====== show help text ======
	update_dialogue_window(dialogue_window,SPLASH_DIALOGUE);

	int cursor_y = 0, cursor_x = 0;

	//update cursor initaly
	wmove(sudoku_window,cursor_y*2+1,cursor_x*4+2);
	wrefresh(sudoku_window);

	//====== mainloop ======
	for (;;){
		//====== get user input ======
		int input = getch();
		switch(input){
			case KEY_LEFT:
			cursor_x--;
			break;
			case KEY_RIGHT:
			cursor_x++;
			break;
			case KEY_UP:
			cursor_y--;
			break;
			case KEY_DOWN:
			cursor_y++;
			break;
			case KEY_BACKSPACE:
			sudoku->empty_squares_array[cursor_x][cursor_y] = 0;
			break;
			case 'q':
			goto stop;
			default:
			//number input
			if (input >= '1' && input <= '9'){
				//dont fill in squares that are pregenerated
				if (sudoku->array[cursor_x][cursor_y] != 0) break;
				sudoku->empty_squares_array[cursor_x][cursor_y] = input-48;
			}
			break;
		}
		//====== place the cursor back in the boundary if it has been moved out ======
		//use truncated division modulus to wrap cursor
		cursor_x = ((cursor_x % 9) + 9)%9;
		cursor_y = ((cursor_y % 9) + 9)%9;

		//====== update the screen ======
		//update the sudoku window
		update_sudoku_window(sudoku_window,sudoku);
		//move the cursor last
		wmove(sudoku_window,cursor_y*2+1,cursor_x*4+2);
		wrefresh(sudoku_window);
	}

	//====== stop ncurses ======
	stop:
	endwin();
	delwin(sudoku_window);
	delwin(dialogue_window);
	return EXIT_SUCCESS;
}

void update_sudoku_window(WINDOW *sudoku_window,sudoku_t *sudoku){
	char *sudoku_as_string = strdup(
"╔═══╤═══╤═══╦═══╤═══╤═══╦═══╤═══╤═══╗\n"
"║   │   │   ║   │   │   ║   │   │   ║\n"
"╟───┼───┼───╫───┼───┼───╫───┼───┼───╢\n"
"║   │   │   ║   │   │   ║   │   │   ║\n"
"╟───┼───┼───╫───┼───┼───╫───┼───┼───╢\n"
"║   │   │   ║   │   │   ║   │   │   ║\n"
"╠═══╪═══╪═══╬═══╪═══╪═══╬═══╪═══╪═══╣\n"
"║   │   │   ║   │   │   ║   │   │   ║\n"
"╟───┼───┼───╫───┼───┼───╫───┼───┼───╢\n"
"║   │   │   ║   │   │   ║   │   │   ║\n"
"╟───┼───┼───╫───┼───┼───╫───┼───┼───╢\n"
"║   │   │   ║   │   │   ║   │   │   ║\n"
"╠═══╪═══╪═══╬═══╪═══╪═══╬═══╪═══╪═══╣\n"
"║   │   │   ║   │   │   ║   │   │   ║\n"
"╟───┼───┼───╫───┼───┼───╫───┼───┼───╢\n"
"║   │   │   ║   │   │   ║   │   │   ║\n"
"╟───┼───┼───╫───┼───┼───╫───┼───┼───╢\n"
"║   │   │   ║   │   │   ║   │   │   ║\n"
"╚═══╧═══╧═══╩═══╧═══╧═══╩═══╧═══╧═══╝\n");
	//====== split and print each line individualy ======
	char *line;
	char *next_line = sudoku_as_string;
	for (int i = 0;next_line != NULL;){
		line = next_line;
		next_line = strchr(line,'\n');
		if (next_line != NULL){
			next_line[0] = '\0';
			next_line++;
		}
		mvwprintw(sudoku_window,i,0,"%s",line);
		i++;
	}
	//====== render pregenerated numbers ======
	for (int x = 0; x < 9; x++){
		for (int y = 0; y < 9; y++){
			if (sudoku->array[x][y] == 0) continue;
			mvwprintw(sudoku_window,y*2+1,x*4+2,"%c",sudoku->array[x][y]+48);
		}
	}
	//====== render users inputs ======
	for (int x = 0; x < 9; x++){
		for (int y = 0; y < 9; y++){
			if (sudoku->empty_squares_array[x][y] == 0) continue;
			int char_attributes = A_DIM | COLOR_PAIR(PAIR_USER_ENTERED);
			mvwaddch(sudoku_window,y*2+1,x*4+2,(sudoku->empty_squares_array[x][y]+48) | char_attributes);
			//====== highlight user input squares ======
		}
	}

	free(sudoku_as_string);

	wrefresh(sudoku_window);
}
void update_dialogue_window(WINDOW *dialogue_window,char *text){
	
	//this function assumes the window has a box drawn around it

	//====== split and print each line individualy ======
	char *text_copy = strdup(text);
	char *line;
	char *next_line = text_copy;
	for (int i = 1; next_line != NULL;){
		line = next_line;
		next_line = strchr(line,'\n');
		if (next_line != NULL){
			next_line[0] = '\0';
			next_line++;
		}
		//====== wrap text ======
		int window_width;
		int window_height;
		getmaxyx(dialogue_window,window_height,window_width);
		window_width-=2;
		window_height-=1;
		int x = 1;
		for (int line_index = 0; line[line_index] != '\0'; line_index++){
			//dont print outside of the window
			if (i >= window_height) break;
			mvwprintw(dialogue_window,i,x,"%c",line[line_index]);
			x++;
			if (x > window_width){
				x = 1;
				i++;
			}
		}
		i++;
	}
	free(text_copy);
	wrefresh(dialogue_window);
}
