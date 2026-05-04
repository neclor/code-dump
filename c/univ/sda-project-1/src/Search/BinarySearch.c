#include <stdlib.h>

#include "BinarySearch.h"


size_t binarySearchLow(void *array, size_t length, void *key, int (*compare)(const void *array, size_t i, void *key)) {
	if (!compare || length < 2) return 0;

	size_t result = 0;
	size_t left = 0;
	size_t right = length;

	while (left < right) {
		size_t middle = left + (right - left) / 2;

		int comp = compare(array, middle, key);
		if (comp < 0) {
			left = middle + 1;

		} else {
			right = middle;
			if (comp == 0) {
				result = middle;
			}
		}
	}
	return result;
}

size_t binarySearchHigh(void *array, size_t length, void *key, int (*compare)(const void *array, size_t i, void *key)) {
	if (!compare || length < 2) return 0;

	size_t result = 0;
	size_t left = 0;
	size_t right = length;

	while (left < right) {
		size_t middle = left + (right - left) / 2;

		int comp = compare(array, middle, key);
		if (comp > 0) {
			right = middle;

		} else {
			left = middle + 1;
			if (comp == 0) {
				result = middle;
			}
		}
	}
	return result;
}
