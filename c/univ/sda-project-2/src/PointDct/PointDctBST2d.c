#include <stdlib.h>
#include <stdio.h>

#include "PointDct.h"
#include "List.h"
#include "Point.h"


/* ======= Functions ======= */

static double getCoord(Point *p, size_t depth) {
	return depth % 2 == 0 ? ptGetx(p) : ptGety(p);
}

/* ======= Node2D ======= */

typedef struct Node2D_t {
	struct Node2D_t *left;
	struct Node2D_t *right;
	Point *point;
	void *value;
} Node2D;

/* ======= Node2D Functions prototypes ======= */

static Node2D *node2DNew(Point *point, void *value);
static void node2DFree(Node2D *node, bool freePoint, bool freeValue);

static size_t node2DHeightRecursive(Node2D *node);
static void node2DDepthRecursive(Node2D *node, size_t depth, size_t *sum, size_t *n);

static void *node2DSearchRecursive(Node2D *node, Point *p, size_t depth);
static void node2DBallSearchRecursive(Node2D *node, Point *center, double r, size_t depth, List *result);

/* ======= Node2D Functions ======= */

static Node2D *node2DNew(Point *point, void *value) {
	Node2D *n = malloc(sizeof(Node2D));
	if (!n) {
		fprintf(stderr, "node2DNew: allocation error\n");
		exit(1);
	}
	n->left = NULL;
	n->right = NULL;
	n->point = point;
	n->value = value;
	return n;
}

static void node2DFree(Node2D *node, bool freePoint, bool freeValue) {
	if (!node) return;
	node2DFree(node->left, freePoint, freeValue);
	node2DFree(node->right, freePoint, freeValue);

	if (freePoint) free(node->point);
	if (freeValue) free(node->value);

	free(node);
}

static size_t node2DHeightRecursive(Node2D *node) {
	if (!node) return 0;
	size_t l = node2DHeightRecursive(node->left);
	size_t r = node2DHeightRecursive(node->right);
	return (l > r ? l : r) + 1;
}

static void node2DDepthRecursive(Node2D *node, size_t depth, size_t *sum, size_t *n) {
	if (!node) return;
	*sum += depth;
	(*n)++;
	node2DDepthRecursive(node->left, depth + 1, sum, n);
	node2DDepthRecursive(node->right, depth + 1, sum, n);
}

static void *node2DSearchRecursive(Node2D *node, Point *p, size_t depth) {
	if (!node) return NULL;
	if (ptCompare(node->point, p) == 0) return node->value;
	if (getCoord(p, depth) <= getCoord(node->point, depth)) {
		return node2DSearchRecursive(node->left, p, depth + 1);
	} else {
		return node2DSearchRecursive(node->right, p, depth + 1);
	}
}

static void node2DBallSearchRecursive(Node2D *node, Point *center, double r, size_t depth, List *result) {
	if (!node) return;

	if (ptSqrDistance(node->point, center) <= r * r) {
		listInsertLast(result, node->value);
	}

	double dist = getCoord(center, depth) - getCoord(node->point, depth);
	double distSqr = dist * dist;

	if (dist <= 0) {
		node2DBallSearchRecursive(node->left, center, r, depth + 1, result);
		if (distSqr <= r * r) {
			node2DBallSearchRecursive(node->right, center, r, depth + 1, result);
		}
	} else {
		node2DBallSearchRecursive(node->right, center, r, depth + 1, result);
		if (distSqr <= r * r) {
			node2DBallSearchRecursive(node->left, center, r, depth + 1, result);
		}
	}
}

/* ======= BST2D ======= */

typedef struct BST2D_t {
	Node2D *root;
	size_t size;
} BST2D;

typedef struct PointValue_t {
	Point *point;
	void *value;
} PointValue;

/* ======= BST2D Functions prototypes ======= */

static BST2D *bst2DNew(void);
static void bst2DFree(BST2D *bst, bool freePoint, bool freeValue);

static size_t bst2DSize(BST2D *bst);
static size_t bst2DHeight(BST2D *bst);

static void *bst2DExactSearch(BST2D *bst, Point *p);
static List *bst2DBallSearch(BST2D *bst, Point *center, double r);

static BST2D *bst2DOptimalBuild(List *lpoints, List *lvalues);
static double bst2DAverageNodeDepth(BST2D *bst);

/* ======= BST2D Static functions prototypes ======= */

static Node2D *bst2DOptimalBuildRecursive(PointValue *array, size_t left, size_t right, size_t depth);
static void bst2DQuickselect(PointValue *array, size_t left, size_t right, size_t k, size_t depth);

/* ======= BST2D functions ======= */

static BST2D *bst2DNew(void) {
	BST2D *bst2D = malloc(sizeof(BST2D));
	if (bst2D == NULL) {
		fprintf(stderr, "best2DNew: allocation error");
		exit(1);
	}
	bst2D->root = NULL;
	bst2D->size = 0;
	return bst2D;
}

static void bst2DFree(BST2D *bst, bool freePoint, bool freeValue) {
	if (!bst) return;
	node2DFree(bst->root, freePoint, freeValue);
	free(bst);
}

static size_t bst2DSize(BST2D *bst) {
	return bst ? bst->size : 0;
}

static size_t bst2DHeight(BST2D *bst) {
	if (!bst || !bst->root) return 0;
    return node2DHeightRecursive(bst->root) - 1;
}

static void *bst2DExactSearch(BST2D *bst, Point *p) {
	if (!bst || !p) return NULL;
	return node2DSearchRecursive(bst->root, p, 0);
}

static List *bst2DBallSearch(BST2D *bst, Point *center, double r) {
	if (!bst || !center) return NULL;

	List *result = listNew();
	if (!result) return NULL;

	node2DBallSearchRecursive(bst->root, center, r, 0, result);
	return result;
}

static BST2D *bst2DOptimalBuild(List *lpoints, List *lvalues) {
	if (!lpoints || !lvalues) return NULL;

	BST2D *bst = bst2DNew();
	bst->size = lpoints->size;

	PointValue *array = malloc(lpoints->size * sizeof(PointValue));
	if (!array) {
		fprintf(stderr, "bstListToArray: allocation error");
		exit(1);
	}

	LNode *pointNode = lpoints->head;
	LNode *valueNode = lvalues->head;
	for (size_t i = 0; pointNode && valueNode; i++) {
		array[i].point = pointNode->value;
		array[i].value = valueNode->value;

		pointNode = pointNode->next;
		valueNode = valueNode->next;
	}

	bst->root = bst2DOptimalBuildRecursive(array, 0, lpoints->size, 0);
	free(array);

	return bst;
}

static double bst2DAverageNodeDepth(BST2D *bst) {
	if (!bst || !bst->root) return 0;
	size_t sum = 0;
	size_t n = 0;
	node2DDepthRecursive(bst->root, 0, &sum, &n);
	return n > 0 ? (double)sum / (double)n : 0;
}

/* ======= BST2D Static functions ======= */

static Node2D *bst2DOptimalBuildRecursive(PointValue *array, size_t left, size_t right, size_t depth) {
	if (left >= right) return NULL;

	size_t median = left + (right - left) / 2;
	bst2DQuickselect(array, left, right, median, depth);

	Node2D *node = node2DNew(array[median].point, array[median].value);
	node->left = bst2DOptimalBuildRecursive(array, left, median, depth + 1);
	node->right = bst2DOptimalBuildRecursive(array, median + 1, right, depth + 1);

	return node;
}

static void bst2DQuickselect(PointValue *array, size_t left, size_t right, size_t k, size_t depth) {
	if (right - left < 2) return;

	size_t pivotIndex = left + (right - left) / 2;
	double pivot = getCoord(array[pivotIndex].point, depth);

	PointValue tmp = array[pivotIndex];
	array[pivotIndex] = array[right - 1];
	array[right - 1] = tmp;

	size_t lowerEqual = left;
	for (size_t i = left; i < right - 1; i++) {
		if (getCoord(array[i].point, depth) <= pivot) {
			tmp = array[lowerEqual];
			array[lowerEqual] = array[i];
			array[i] = tmp;

			lowerEqual++;
		}
	}

	tmp = array[lowerEqual];
	array[lowerEqual] = array[right - 1];
	array[right - 1] = tmp;

	if (lowerEqual == k) return;
	else if (lowerEqual < k) bst2DQuickselect(array, lowerEqual + 1, right, k, depth);
	else bst2DQuickselect(array, left, lowerEqual, k, depth);
}

/* ======= PointDct_t ======= */

typedef BST2D PointDct_t;

/* ======= Public functions ======= */

PointDct *pdctCreate(List *lpoints, List *lvalues) { return (PointDct *)bst2DOptimalBuild(lpoints, lvalues); }

void pdctFree(PointDct *pd) { bst2DFree((BST2D *)pd, false, false); }

size_t pdctSize(PointDct *pd) { return bst2DSize((BST2D *)pd); }

size_t pdctHeight(PointDct *pd) { return bst2DHeight((BST2D *)pd); }

size_t pdctAverageNodeDepth(PointDct *pd) { return (size_t)bst2DAverageNodeDepth((BST2D *)pd); }

void *pdctExactSearch(PointDct *pd, Point *p) { return bst2DExactSearch((BST2D *)pd, p); }

List *pdctBallSearch(PointDct *pd, Point *p, double r) { return bst2DBallSearch((BST2D *)pd, p, r); }
