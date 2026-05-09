#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>

#include "BST.h"
#include "List.h"

typedef struct kvpair_t {
	void *key;
	void *value;
} KVpair;

typedef struct BNode_t BNode;
struct BNode_t {
	BNode *parent;
	BNode *left;
	BNode *right;
	void *key;
	void *value;
};

struct BST_t {
	BNode *root;
	size_t size;
	int (*compfn)(void *, void *);
};

/* ======= Prototypes of static functions ======= */

static void bstFreeRec(BNode *n, bool freeKey, bool freeValue);
static BNode *bnNew(void *key, void *value);
static size_t bstHeightRec(BNode *root);

static KVpair *bstListsToPairs(List *keys, List *values);
static void bstQuickSort(int comparison_fn_t(void *, void *), KVpair *pairs, size_t left, size_t right);
static void bstInsertMedians(BST *bst, KVpair *pairs, size_t left, size_t right);
static void bstRangeSearchRecursive(int comparison_fn_t(void *, void *), BNode *node, void *keymin, void *keymax, List *list);
static void depthRecursive(BNode *node, size_t depth, size_t *sum, size_t *n);

/* ======= Public functions ======= */

BST *bstNew(int comparison_fn_t(void *, void *)) {
	BST *bst = malloc(sizeof(BST));
	if (bst == NULL)
	{
		fprintf(stderr, "bestNew: allocation error");
		exit(1);
	}
	bst->root = NULL;
	bst->size = 0;
	bst->compfn = comparison_fn_t;
	return bst;
}

void bstFree(BST *bst, bool freeKey, bool freeValue) {
	bstFreeRec(bst->root, freeKey, freeValue);
	free(bst);
}

size_t bstSize(BST *bst) {
	return bst->size;
}

size_t bstHeight(BST *bst) {
	if (!bst->root) return 0;
	return bstHeightRec(bst->root) - 1;
}

bool bstInsert(BST *bst, void *key, void *value) {
	if (bst->root == NULL) {
		bst->root = bnNew(key, value);
		bst->size++;
		return true;
	}
	BNode *prev = NULL;
	BNode *n = bst->root;
	while (n != NULL) {
		prev = n;
		int cmp = bst->compfn(key, n->key);
		if (cmp <= 0)
		{
			n = n->left;
		}
		else if (cmp > 0)
		{
			n = n->right;
		}
	}
	BNode *new = bnNew(key, value);
	new->parent = prev;
	if (bst->compfn(key, prev->key) <= 0) {
		prev->left = new;
	} else {
		prev->right = new;
	}
	bst->size++;
	return true;
}

void *bstSearch(BST *bst, void *key) {
	BNode *n = bst->root;
	while (n != NULL) {
		int cmp = bst->compfn(key, n->key);
		if (cmp < 0) {
			n = n->left;
		} else if (cmp > 0) {
			n = n->right;
		} else {
			return n->value;
		}
	}
	return NULL;
}

BST *bstOptimalBuild(int comparison_fn_t(void *, void *), List *lkeys, List *lvalues) {
	if (!comparison_fn_t || !lkeys || !lvalues) return NULL;

	BST *bst = bstNew(comparison_fn_t);
	KVpair *pairs = bstListsToPairs(lkeys, lvalues);
	bstQuickSort(comparison_fn_t, pairs, 0, lkeys->size);
	bstInsertMedians(bst, pairs, 0, lkeys->size);

	free(pairs);

	return bst;
}

List *bstRangeSearch(BST *bst, void *keymin, void *keymax) {
	if (!bst) return NULL;

	List *list = listNew();
	if (!list) return NULL;

	bstRangeSearchRecursive(bst->compfn, bst->root, keymin, keymax, list);
	return list;
}

double bstAverageNodeDepth(BST *bst) {
	if (!bst || !bst->root) return 0;

	size_t sum = 0;
	size_t n = 0;
	depthRecursive(bst->root, 0, &sum, &n);
	if (n == 0) return 0;

	return (double)sum / (double)n;
}

/* ======= Static functions ======= */

static void bstFreeRec(BNode *n, bool freeKey, bool freeValue) {
	if (n == NULL)
		return;
	bstFreeRec(n->left, freeKey, freeValue);
	bstFreeRec(n->right, freeKey, freeValue);
	if (freeKey)
		free(n->key);
	if (freeValue)
		free(n->value);
	free(n);
}

static BNode *bnNew(void *key, void *value) {
	BNode *n = malloc(sizeof(BNode));
	if (n == NULL) {
		fprintf(stderr, "bnNew: allocation error\n");
		exit(1);
	}
	n->parent = NULL;
	n->left = NULL;
	n->right = NULL;
	n->key = key;
	n->value = value;
	return n;
}

static size_t bstHeightRec(BNode *root) {
	if (!root)
		return 0;

	size_t hleft = bstHeightRec(root->left);
	size_t hright = bstHeightRec(root->right);
	if (hleft > hright)
		return 1 + hleft;
	else
		return 1 + hright;
}

/* ======= My static functions ======= */

static KVpair *bstListsToPairs(List *keys, List *values) {
	KVpair *array = malloc(keys->size * sizeof(KVpair));
	if (!array) {
		fprintf(stderr, "bstListToArray: allocation error");
		exit(1);
	}

	LNode *keyNode = keys->head;
	LNode *valueNode = values->head;

	for (size_t i = 0; keyNode && valueNode; i++) {
		array[i].key = keyNode->value;
		array[i].value = valueNode->value;

		keyNode = keyNode->next;
		valueNode = valueNode->next;
	}

	return array;
}

static void bstQuickSort(int comparison_fn_t(void *, void *), KVpair *pairs, size_t left, size_t right) {
	if (right - left < 2) return;

	void *pivot = pairs[left + (right - left) / 2].key;
	size_t lower = left;
	size_t greater = right - 1;

	size_t i = left;
	while (i <= greater) {
		int cmp = comparison_fn_t(pairs[i].key, pivot);

		if (cmp < 0) {
			KVpair tmp = pairs[lower];
			pairs[lower] = pairs[i];
			pairs[i] = tmp;

			lower++;
			i++;
		} else if (cmp > 0) {
			KVpair tmp = pairs[greater];
			pairs[greater] = pairs[i];
			pairs[i] = tmp;

			greater--;
		} else {
			i++;
		}
	}

	bstQuickSort(comparison_fn_t, pairs, left, lower);
	bstQuickSort(comparison_fn_t, pairs, greater + 1, right);
}

static void bstInsertMedians(BST *bst, KVpair *pairs, size_t left, size_t right) {
	if (left >= right) return;

	size_t median = left + (right - left) / 2;
	KVpair medianPair = pairs[median];
	bstInsert(bst, medianPair.key, medianPair.value);

	bstInsertMedians(bst, pairs, left, median);
	bstInsertMedians(bst, pairs, median + 1, right);
}

static void bstRangeSearchRecursive(int comparison_fn_t(void *, void *), BNode *node, void *keymin, void *keymax, List *result) {
	if (!node) return;

	int cmpMin = comparison_fn_t(keymin, node->key);
	int cmpMax = comparison_fn_t(keymax, node->key);

	if (cmpMin <= 0) {
		bstRangeSearchRecursive(comparison_fn_t, node->left, keymin, keymax, result);
	}
	if (cmpMin <= 0 && cmpMax >= 0) {
		listInsertLast(result, node->value);
	}
	if (cmpMax > 0) {
		bstRangeSearchRecursive(comparison_fn_t, node->right, keymin, keymax, result);
	}
}

static void depthRecursive(BNode *node, size_t depth, size_t *sum, size_t *n) {
	if (!node) return;
	*sum += depth;
	(*n)++;
	depthRecursive(node->left, depth + 1, sum, n);
	depthRecursive(node->right, depth + 1, sum, n);
}
