#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "Terms.h"
#include "Sort.h"
#include "BinarySearch.h"
#include "AutoComplete.h"


struct AC_t {
	TermArray *ta;
};


static void termArraySwap(void *array, size_t i, size_t j);
static int termArrayCompareByText(const void *array, size_t i, size_t j);
static int termArrayCompareSearchByText(const void *array, size_t i, void *key);

static void termPtrsSwap(void *array, size_t i, size_t j);
static int termPtrsCompareByWeight(const void *array, size_t i, size_t j);


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

	Term **term_ptrs = malloc(word_count * sizeof(Term*));
	for (size_t i = 0; i < word_count; i++) {
		term_ptrs[i] = &terms[left + i];
	}
	sort(term_ptrs, word_count, termPtrsCompareByWeight, termPtrsSwap);

	for (size_t i = 0; i < l; i++) {
		results[i] = (*term_ptrs[word_count - 1 - i]).text;
	}

	free(term_ptrs);

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

static void termPtrsSwap(void *array, size_t i, size_t j) {
	if (!array) {
		fprintf(stderr, "termPtrsSwap: array is NULL.\n");
		exit(1);
	}
	Term **term_ptrs = (Term**)array;

	if (i == j) return;

	Term *tmp = term_ptrs[i];
	term_ptrs[i] = term_ptrs[j];
	term_ptrs[j] = tmp;
}

static int termPtrsCompareByWeight(const void *array, size_t i, size_t j) {
	if (!array) {
		fprintf(stderr, "termPtrsCompareByWeight: array is NULL.\n");
		exit(1);
	}
	const Term **term_ptrs = (const Term**)array;

	if (i == j) return 0;

	long w1 = (*term_ptrs[i]).weight;
	long w2 = (*term_ptrs[j]).weight;

	if (w1 < w2) return -1;
	if (w1 > w2) return 1;
	return 0;
}
