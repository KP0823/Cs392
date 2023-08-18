//Kush Parmar
//I pledge my honor that I have abided by the stevens honor system- Kush Parmar
#include "utils.h"
#include <stdlib.h>

/*
	You are free to use any data type you like in this file.
    However, other than the five functions declared in "utils.h",
    DO NOT create any other functions.
	
*/

/* takes two ints and compares their values, if a is smaller than b then return -1, if bigger than 1 , if equal then 0 */
int cmpr_int(void* a, void* b) {
	int * i_a= (int*) a;
	int * i_b= (int *) b;
	if ((* i_a)<(* i_b)){
		return -1;
	}
	else if ((* i_a)>(* i_b)){
		return 1;
	}
	else {
		return 0;
	}
}

/* takes two float and compares their values, if a is smaller than b then return -1, if bigger than 1 , if equal then 0 */
int cmpr_float(void* a,void* b) {
	float* f_a= (float*) a;
	float* f_b= (float*) b;
	if ((* f_a)<(* f_b)){
		return -1;
	}
	else if ((* f_a)>(* f_b)){
		return 1;
	}
	else {
		return 0;
	}
}
/*casts void pointer to int pointer, then prints out value*/
void print_int(void*a) {
	int* i_a= (int*) a;
	printf("%d", (*i_a));
}

/*causes void pointer to float pointer, then prints out value*/
void print_float(void*a) {
	float* f_a= (float*) a;
	printf("%f", (*f_a));
}

/*tries to open the file and read all the element from this file*/
void* read_array(char* filename, char* format, size_t* len) {
	size_t help=0;
	FILE* fp =fopen(filename, "r");
	if (fp == NULL) {
		fprintf(stderr, "File failed to open.\n");
		//exit(EXIT_FAILURE); not sure whether they were equal
		exit(1);
	}
	fclose(fp);
	fp =fopen(filename, "r");

	void* scam_pointer= (void*) malloc(sizeof(int));
	while(fscanf(fp,format, scam_pointer)!=-1){
		(*len)++;
	}
	fclose(fp);
	free(scam_pointer);
	fp =fopen(filename, "r");
	void* array =(void*) malloc(sizeof(int)*(*len));
	int array_index=0;
	while(fscanf(fp,format, array+array_index)!=-1){
		array_index+=sizeof(int);
	}
	fclose(fp);
	return array;
}