#include <stdlib.h>

#include "Sort.h"


static void quickSort(void *array,size_t left,size_t right,int (*compare)(const void *array,size_t i,size_t j),void (*swap)(void *array,size_t i,size_t j));


void sort(
	void *array,
	size_t length,
	int (*compare)(const void *array,size_t i,size_t j),
	void (*swap)(void *array,size_t i,size_t j)
) {
	if(!compare || !swap) return;

	quickSort(array,0,length,compare,swap);
}


static void quickSort(
	void *array,
	size_t left,
	size_t right,
	int (*compare)(const void *array,size_t i,size_t j),
	void (*swap)(void *array,size_t i,size_t j)
) {
	if(right - left < 2) return;

	size_t pivot = right - 1;
	size_t lower = left;
	size_t greater = pivot;

	size_t i = left;
	while(i < greater) {
		int cmp = compare(array,i,pivot);

		if(cmp < 0) {
			swap(array,lower++,i++);
		} else if(cmp > 0) {
			swap(array,i,--greater);
		} else {
			i++;
		}
	}
	swap(array,greater,pivot);
	quickSort(array,left,lower,compare,swap);
	quickSort(array,greater + 1,right,compare,swap);
}
