#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "sudoku.h"
char  *realloc_strcat(char *dest,char *src){
	size_t new_size = strlen(dest) + strlen(src) + 1;
	dest = realloc(dest,new_size);
	if (dest == NULL){
		return NULL;
	}
	//cant fail
	strcat(dest,src);
	return dest;
}
char *sudoku_to_json_string(sudoku_t *sudoku){
	char *return_buffer = strdup("{\"array\":[");
	for (int j = 0; j < 9; j++){
		return_buffer = realloc_strcat(return_buffer,"[");
		for (int i = 0; i < 9; i++){
			char array_element_str[24];
			snprintf(array_element_str,sizeof(array_element_str),"%d,",sudoku->array[j][i]);
			return_buffer = realloc_strcat(return_buffer,array_element_str);
		}
		//remove last comma
		return_buffer[strlen(return_buffer)-1] = '\0';
		return_buffer = realloc_strcat(return_buffer,"],");
	}
	//remove last comma
	return_buffer[strlen(return_buffer)-1] = '\0';
	return_buffer = realloc_strcat(return_buffer,"]}");
	return return_buffer;
}
