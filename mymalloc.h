/* Ben Tomasulo
 * CS0499
 * Project 3 
 * Header File */
 
//multiple inclusion guard
#ifndef MYMALLOC_H_
#define MYMALLOC_H_

//function declarations
void *my_nextfit_malloc(int size);
void my_free(void *ptr);

#endif