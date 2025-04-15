#include <stdio.h>
#include <stdlib.h>
#include "sudoku.h"
#include "json_sudoku.h"
#include "ncurses_interface.h"
#include <time.h> 
#include <getopt.h>

void print_help();

int main(int argc, char **argv){
	//====== initial parameters ======
	unsigned long random_seed = time(NULL);
	enum {OUTPUT_JSON,OUTPUT_PLAIN,OUTPUT_NCURSES} output_format = OUTPUT_NCURSES;

	//====== read arguments ======
	static struct option long_options[] = {
		//'h' and 'w'  means it returns the same as its short counterpart
		{"json", no_argument,0,'j'}, 
		{"plain", no_argument,0,'p'}, 
		{"seed", required_argument,0,'s'},
		{"help", no_argument,0,'h'}
	};
	int option_index = 0;
	for (;;){
		int result = getopt_long(argc,argv,"jhps:",long_options,&option_index);
		if (result == -1) break; //end of args
		switch (result){
			case 'p':
				output_format = OUTPUT_PLAIN;
				break;
			case 'j':
				output_format = OUTPUT_JSON;
				break;
			case 's':
				random_seed = (unsigned long)strtol(optarg,NULL,10);
				break;
			case 'h':
				print_help();
				return 0;
			default:
				fprintf(stderr,"Invalaid arguments provided.\n");
				break;
		}
	}

	//====== initalise our board ======
	sudoku_t sudoku;
	sudoku_init(&sudoku);

	//set the seed
	srandom(random_seed);
	//generate and shuffle
	generate_sudoku(&sudoku);
	shuffle_sudoku(&sudoku);
	
	//====== output the result how the user wants it ======
	switch(output_format){
	case OUTPUT_JSON:
		char *json_represenation = sudoku_to_json_string(&sudoku);
		printf("%s\n",json_represenation);
		free(json_represenation);
		return 0;
	case OUTPUT_PLAIN:
		printf("%s\n",sudoku__str__(&sudoku));
		return 0;
	case OUTPUT_NCURSES:
		return ncurses_sudoku(&sudoku);
	}
}

void print_help(){
	printf("options:\n-j, --json : output the sudoku as a json string to stdout\n-h, --help : display the help text\n-s <seed>, --seed <seed> : set the seed\n-p, --plain : output in plaintext format\n");
}
