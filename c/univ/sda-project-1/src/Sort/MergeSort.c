#include <stdlib.h>

#include "Sort.h"


static void merge_sort(size_t *indices, size_t *temp, size_t left, size_t right, const void *array, int (*compare)(const void *, size_t i, size_t j));
static void merge(size_t *indices, size_t *temp, size_t left, size_t middle, size_t right, const void *array, int (*compare)(const void *, size_t i, size_t j));
static void rearrange(void *array, size_t length, size_t *indices, void (*swap)(void *array, size_t i, size_t j));


void sort(
	void *array,
	size_t length,
	int (*compare)(const void *, size_t i, size_t j),
	void (*swap)(void *array, size_t i, size_t j)
) {
	if (!compare || !swap) return;

	size_t *indices = malloc(length * sizeof(size_t));
	size_t *temp = malloc(length * sizeof(size_t));
	if (!indices || !temp) {
		free(indices);
		free(temp);
		return;
	}

	for (size_t i = 0; i < length; i++) {
		indices[i] = i;
	}

	merge_sort(indices, temp, 0, length, array, compare);
	rearrange(array, length, indices, swap);

	free(indices);
	free(temp);
}


static void merge_sort(
	size_t *indices,
	size_t *temp,
	size_t left,
	size_t right,
	const void *array,
	int (*compare)(const void *, size_t i, size_t j)
) {
	if (right - left < 2) return;

	size_t middle = left + (right - left) / 2;

	merge_sort(indices, temp, left, middle, array, compare);
	merge_sort(indices, temp, middle, right, array, compare);

	merge(indices, temp, left, middle, right, array, compare);
}

static void merge(
	size_t *indices,
	size_t *temp,
	size_t left,
	size_t middle,
	size_t right,
	const void *array,
	int (*compare)(const void *, size_t i, size_t j)
) {
	size_t i = left;
	size_t j = middle;
	for (size_t k = left; i < middle || j < right; k++) {
		if (i >= middle) {
			temp[k] = indices[j];
			j++;

		} else if (j >= right) {
			temp[k] = indices[i];
			i++;

		} else if (compare(array, indices[i], indices[j]) <= 0) {
			temp[k] = indices[i];
			i++;

		} else {
			temp[k] = indices[j];
			j++;
		}
	}

	for (size_t l = left; l < right; l++) {
		indices[l] = temp[l];
	}
}

static void rearrange(
	void *array,
	size_t length,
	size_t *indices,
	void (*swap)(void *array, size_t i, size_t j)
) {
	size_t i = 0;
	while (i < length) {
		if (indices[i] == i) {
			i++;
		} else {
			size_t j = indices[i];
			swap(array, indices[i], indices[j]);

			size_t tmp = indices[i];
			indices[i] = indices[j];
			indices[j] = tmp;
		}
	}
}
