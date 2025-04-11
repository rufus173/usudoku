CFLAGS=-g -Wall
LDFLAGS=-fsanitize=address

usudoku : src/sudoku.o src/main.o
	$(CC) -o $@ $^ $(LDFLAGS)
