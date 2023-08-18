//Kush Parmar
//I pledge my honor that I have abided by the stevens honor system- Kush Parmar
#include "insertion.h"


/*
	We emphasize this: because this is a generic implementation, in insertion.c you must not use if-else to
	discriminate different data types in your code. Your code must be able to deal with any data type possible.
	Therefore, we limit the usage of the following primitive types:
	- int or size_t: only allowed as loop iterators;
	- char or u_int8_t: can be used as you wish;
	- Types that are used in the declarations of iSort() and iPrint(): feel free to use them for their _original_
      purpose;
	- All other types are not allowed to appear in insertion.c: no exceptions, no matter if you actually used
	  them, or what purpose it is. Once it appears, you will receive 30% reduction of the homework.

	You are free to create helper functions only in insertion.c; you must declare and implement them in the
    .c file instead of the header files. Do not change any of the header files.
	
*/

/* essentail swap, idk why i named it switching*/
void switching(void *pa, void* pb , size_t width){

	char *_pa= (char*)pa;
	char *_pb= (char*)pb;
	char temp;
		
	for(int i =0; i <width; i++){
		temp=_pa[i];
		_pa[i]= _pb[i];
		_pb[i]=temp;
	}
} 

/*insertion sort......*/
void iSort(void* base, size_t nel, size_t width, int (*compare)(void*,void*)) {
	size_t total_index= nel*width;
	size_t i,j;
	for(i =width; i <total_index; i+=width){
		j=i;
		while(j>0 && (compare((base +j) ,(base +(j-width)) )==-1)){
			switching ((base+j), (base+(j-width)),width);
			j=j-width;
		}
	}	
}

/*goes through the array and prints each element then goes on to next line*/
void iPrint(void* base, size_t nel, size_t width, void (*print)(void*)) {
	 size_t total= nel*width;
	 for(int i = 0; i<total; i+=width){
		print(base+i);
		printf("\n");
	 }
	
}
