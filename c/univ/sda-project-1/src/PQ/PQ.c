#include <stdlib.h>

#include "PQ.h"


struct PQ_t {
	size_t capacity;
	size_t count;
	void **heap;
	int (*compare)(const void *, const void *);
};


static void heapifyDown(PQ* pq);


PQ *pqCreate(size_t capacity, int (*compare)(const void *, const void *)) {
	if (!compare) return NULL;

	PQ *pq = malloc(sizeof(PQ));
	void *heap = malloc(capacity * sizeof(void *));
	if (!pq || !heap) {
		free(pq);
		free(heap);
		return NULL;
	}

	pq->capacity = capacity;
	pq->count = 0;
	pq->heap = heap;
	pq->compare = compare;

	return pq;
}

void pqFree(PQ* pq) {
	if (!pq) return;
	free(pq->heap);
	free(pq);
}

bool pqInsert(PQ* pq, void *obj) {
	if (!pq || pq->capacity == pq->count) return false;

	void **heap = pq->heap;
	heap[pq->count] = obj;

	size_t i = pq->count;
	size_t p = (i - 1) / 2;
	pq->count++;

	while (i > 0 && pq->compare(heap[i], heap[p]) > 0) {
		void *tmp = heap[p];
		heap[p] = heap[i];
		heap[i] = tmp;

		i = p;
		p = (i - 1) / 2;
	}
	return true;
}

void *pqGetMax(const PQ* pq) {
	if (!pq || pq->count == 0) return NULL;
	return pq->heap[0];
}

void *pqExtractMax(PQ* pq) {
	if (!pq || pq->count == 0) return NULL;

	void **heap = pq->heap;
	void *result = heap[0];
	heap[0] = heap[--pq->count];

	if (pq->count != 0) {
		heapifyDown(pq);
	}

	return result;
}

void pqReplaceMax(PQ *pq, void *obj) {
	if (!pq || pq->count == 0) return;

	pq->heap[0] = obj;
	heapifyDown(pq);
}

size_t pqSize(const PQ* pq) {
	if (!pq) return 0;
	return pq->count;
}

size_t pqCapacity(const PQ* pq) {
	if (!pq) return 0;
	return pq->capacity;
}


static void heapifyDown(PQ* pq) {
	void **heap = pq->heap;
	size_t count = pq->count;
	int (*compare)(const void *, const void *) = pq->compare;

	size_t i = 0;
	while (true) {
		size_t new = i;
		size_t left = 2 * new + 1;
		size_t right = 2 * new + 2;

		if (left < count && compare(heap[new], heap[left]) < 0) {
			new = left;
		}
		if (right < count && compare(heap[new], heap[right]) < 0) {
			new = right;
		}

		if (i == new) break;

		void *tmp = heap[i];
		heap[i] = heap[new];
		heap[new] = tmp;

		i = new;
	}
}
