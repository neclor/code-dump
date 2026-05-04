#ifndef BS_H_
#define BS_H_


#include <stdlib.h>


/* ------------------------------------------------------------------------- *
 * Returns the the leftmost object in the array which is equal to the key
 * (according to the compare function). The array is supposed to be sorted.
 * When the key does not appear in the array, the function may return any
 * position in the array that does not contain the key.
 *
 * PARAMETERS
 * array              The array
 * length             The size of the array
 * key                The key to be searched in the array
 * compare            A function that compare an element in the array with the
 *                    the key. Returns resp. an integer <, =, or > 0 if the
 *                    element at position i in the array is lower, equal, or
 *                    greater than the third argument.
 * ------------------------------------------------------------------------- */
size_t binarySearchLow(void *array, size_t length, void *key, int (*compare)(const void *, size_t i, void *));

/* ------------------------------------------------------------------------- *
 * Returns the rightmost object in the array which is equal to the key
 * (according to the compare function). The array is supposed to be sorted.
 * When the key does not appear in the array, the function may return any
 * position in the array that does not contain the key.
 *
 * PARAMETERS
 * array              The array
 * length             The size of the array
 * key                The key to be searched in the array
 * compare            A function that compare an element in the array with the
 *                    the key. Returns resp. an integer <, =, or > 0 if the
 *                    element at position i in the array is lower, equal, or
 *                    greater than the third argument.
 * ------------------------------------------------------------------------- */
size_t binarySearchHigh(void *array, size_t length, void *key, int (*compare)(const void *, size_t i, void *));


#endif // BS_H_
