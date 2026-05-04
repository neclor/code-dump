#ifndef SORT_H_
#define SORT_H_


#include <stdlib.h>


/* ------------------------------------------------------------------------- *
 * Sort the array given as argument in increasing order. The array is direcly
 * modified.
 *
 * PARAMETERS
 * array              The array to sort
 * length             The size of the array
 * compare            A function that compare two elements in the array. Returns
 *                    resp. an integer <, =, or > 0 if the element at position i
 *                    in the array is lower, equal, or greater than the element
 *                    at position j
 * swap				  A function that swap elements at positions i and j in the
 *                    array.
 * ------------------------------------------------------------------------- */
void sort(
	void *array,
	size_t length,
	int (*compare)(const void *, size_t i, size_t j),
	void (*swap)(void *array, size_t i, size_t j)
);


#endif // SORT_H_
