#include <stdio.h>
#include <stdlib.h>
#include "sudoku.h"
#include "json_sudoku.h"
#include <time.h> 
#include <getopt.h>

void print_help();

int main(int argc, char **argv){
	//====== initial parameters ======
	unsigned long random_seed = time(NULL);
	int output_json = 0;

	//====== read arguments ======
	static struct option long_options[] = {
		//'h' and 'w'  means it returns the same as its short counterpart
		{"json", no_argument,0,'j'}, 
		{"seed", required_argument,0,'s'},
		{"help", no_argument,0,'h'}
	};
	int option_index = 0;
	for (;;){
		int result = getopt_long(argc,argv,"js:",long_options,&option_index);
		if (result == -1) break; //end of args
		switch (result){
			case 'j':
				output_json = 1;
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
	
	//====== if json requested, output and quit ======
	if (output_json == 1){	
		char *json_represenation = sudoku_to_json_string(&sudoku);
		printf("%s\n",json_represenation);
		free(json_represenation);
		return 0;
	}



	//sudoku.array[0][0] = 1;
	//sudoku.array[8][8] = 9;
	
	printf("%s\n",sudoku__str__(&sudoku));

	return 0;
}

void print_help(){
}
