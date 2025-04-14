CFLAGS=-g -Wall
LDFLAGS=-fsanitize=address

usudoku : src/sudoku.o src/main.o src/json_sudoku.o
	$(CC) -o $@ $^ $(LDFLAGS)
