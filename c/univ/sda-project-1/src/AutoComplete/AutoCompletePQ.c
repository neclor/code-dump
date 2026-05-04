#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "Terms.h"
#include "Sort.h"
#include "BinarySearch.h"
#include "AutoComplete.h"
#include "PQ.h"


struct AC_t {
	TermArray *ta;
};


static void termArraySwap(void *array, size_t i, size_t j);
static int termArrayCompareByText(const void *array, size_t i, size_t j);
static int termArrayCompareSearchByText(const void *array, size_t i, void *key);

static int termCompareByNegativeWeight(const void *a, const void *b);


AC *acCreate(TermArray *ta) {
	AC *ac = malloc(sizeof(AC));
	if (!ac) return NULL;

	sort(ta, ta->length, termArrayCompareByText, termArraySwap);
	ac->ta = ta;

	return ac;
}

void acFree(AC *ac) {
	free(ac);
}

size_t acComplete(AC *ac, char *query, size_t k, char **results) {
	if (!ac || !query) return 0;

	TermArray *ta = ac->ta;
	size_t length = ta->length;
	Term *terms = ta->array;

	size_t q_len = strlen(query);

	size_t left = 0;
	size_t right = length - 1;
	if (q_len != 0) {
		left = binarySearchLow(ta, ta->length, query, termArrayCompareSearchByText);
		if (strncmp(ta->array[left].text, query, q_len) != 0) return 0;

		right = binarySearchHigh(ta, ta->length, query, termArrayCompareSearchByText);
	}
	size_t word_count = right + 1 - left;
	size_t l = (k < word_count) ? k : word_count;

	PQ *pq = pqCreate(l, termCompareByNegativeWeight);

	for (size_t i = left; i < right + 1; i++) {
		if (pqSize(pq) < l) {
			pqInsert(pq, &terms[i]);

		} else if (((Term*)pqGetMax(pq))->weight < terms[i].weight) {
			pqReplaceMax(pq, &terms[i]);
		}
	}

	for (size_t i = 0; i < l; i++) {
		results[l - 1 - i] = ((Term*)pqExtractMax(pq))->text;
	}

	pqFree(pq);

	return l;
}



static void termArraySwap(void *array, size_t i, size_t j) {
	if (!array) {
		fprintf(stderr, "termArraySwap: array is NULL.\n");
		exit(1);
	}
	TermArray *ta = (TermArray*)array;
	size_t length = ta->length;

	if (length <= i || length <= j) {
		fprintf(stderr, "termArraySwap: index out of bounds.\n");
		exit(1);
	}
	if (i == j) return;
	Term *terms = ta->array;

	Term tmp = terms[i];
	terms[i] = terms[j];
	terms[j] = tmp;
}

static int termArrayCompareByText(const void *array, size_t i, size_t j) {
	if (!array) {
		fprintf(stderr, "termArrayCompareByText: array is NULL.\n");
		exit(1);
	}
	const TermArray *ta = (TermArray*)array;
	size_t length = ta->length;

	if (length <= i || length <= j) {
		fprintf(stderr, "termArrayCompareByText: index out of bounds.\n");
		exit(1);
	}
	if (i == j) return 0;
	const Term *terms = ta->array;

	char *t1 = terms[i].text;
	char *t2 = terms[j].text;

	if (t1 == NULL && t2 == NULL) return 0;
	if (t1 == NULL) return -1;
	if (t2 == NULL) return 1;
	return strcmp(t1, t2);
}

static int termArrayCompareSearchByText(const void *array, size_t i, void *key) {
	if (!array || !key) {
		fprintf(stderr, "termArrayCompareSearchByText: array or key is NULL.\n");
		exit(1);
	}
	TermArray *ta = (TermArray*)array;
	size_t length = ta->length;

	if (length <= i) {
		fprintf(stderr, "termArrayCompareSearchByText: index out of bounds.\n");
		exit(1);
	}
	Term *terms = ta->array;
	const char *prefix = (char*)key;

	const char *text = terms[i].text;
	if (text == NULL) return -1;

	return strncmp(text, prefix, strlen(prefix));
}

static int termCompareByNegativeWeight(const void *a, const void *b) {
	if (!a || !b) {
		fprintf(stderr, "termCompareByWeight: term is NULL.\n");
		exit(1);
	}
	const Term *term_a = (Term*)a;
	const Term *term_b = (Term*)b;

	long w1 = -term_a->weight;
	long w2 = -term_b->weight;

	return (w1 < w2) ? -1 : ((w1 > w2) ? 1 : 0);
}
