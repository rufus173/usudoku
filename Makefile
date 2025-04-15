CFLAGS=-g -Wall
LDFLAGS=-fsanitize=address -lncursesw
CC=gcc

usudoku : src/sudoku.o src/main.o src/json_sudoku.o src/ncurses_interface.o
	$(CC) -o $@ $^ $(LDFLAGS)
