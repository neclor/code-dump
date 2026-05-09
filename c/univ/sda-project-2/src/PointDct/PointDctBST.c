#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <stdio.h>

#include "PointDct.h"
#include "List.h"
#include "Point.h"
#include "BST.h"

typedef struct PointValue_t {
	Point *point;
	void *value;
} PointValue;

struct PointDct_t {
	BST *bst;
	double xmin, xmax, ymin, ymax;
};

/* ======= Prototypes of static functions ======= */

/* ------------------------------------------------------------------------- *
 * Returns the morton code (or z-code) associated to the (x,y) coordinates
 * in argument. The x and y values are expected to be uint32_t integer between
 * 0 and UINT32_MAX.
 *
 * PARAMETERS
 * x           the x coordinate
 * y           the y coordinate
 *
 * RETURN
 * z           the morton code
 *
 * ------------------------------------------------------------------------- */
static uint64_t zEncode(uint32_t x, uint32_t y);
static uint64_t interleave8(uint8_t m, uint8_t n);

static int compareZ(void *a, void *b);
static uint64_t pointToZ(Point *p, double xmin, double xmax, double ymin, double ymax);

/* ======= Public functions ======= */

PointDct *pdctCreate(List *lpoints, List *Lvalues) {
	if (!lpoints || !Lvalues || lpoints->size == 0) return NULL;

	PointDct *pd = malloc(sizeof(PointDct));
	if (!pd) {
		fprintf(stderr, "pdctCreate: allocation error\n");
		exit(1);
	}

	LNode *n = lpoints->head;
	pd->xmin = ptGetx((Point *)n->value);
	pd->xmax = pd->xmin;
	pd->ymin = ptGety((Point *)n->value);
	pd->ymax = pd->ymin;
	for (n = n->next; n; n = n->next) {
		double x = ptGetx((Point *)n->value);
		double y = ptGety((Point *)n->value);
		if (x < pd->xmin) pd->xmin = x;
		if (x > pd->xmax) pd->xmax = x;
		if (y < pd->ymin) pd->ymin = y;
		if (y > pd->ymax) pd->ymax = y;
	}

	List *zList = listNew();
	List *pvList = listNew();
	if (!zList || !pvList) {
		fprintf(stderr, "pdctCreate: allocation error\n");
		exit(1);
	}

	LNode *pointNode = lpoints->head;
	LNode *valueNode = Lvalues->head;
	while (pointNode && valueNode) {
		uint64_t z = pointToZ((Point *)pointNode->value, pd->xmin, pd->xmax, pd->ymin, pd->ymax);
		listInsertLast(zList, (void *)(uintptr_t)z);

		PointValue *pv = malloc(sizeof(PointValue));
		if (!pv) {
			fprintf(stderr, "pdctCreate: allocation error\n");
			exit(1);
		}
		pv->point = pointNode->value;
		pv->value = valueNode->value;
		listInsertLast(pvList, pv);

		pointNode = pointNode->next;
		valueNode = valueNode->next;
	}

	pd->bst = bstOptimalBuild(compareZ, zList, pvList);
	listFree(zList, false);
	listFree(pvList, false);

	return pd;
}

void pdctFree(PointDct *pd) {
	if (!pd) return;
	bstFree(pd->bst, false, true);
	free(pd);
}

size_t pdctSize(PointDct *pd) { return bstSize(pd->bst); }

size_t pdctAverageNodeDepth(PointDct *pd) { return (size_t)bstAverageNodeDepth(pd->bst); }

size_t pdctHeight(PointDct *pd) { return bstHeight(pd->bst); }

void *pdctExactSearch(PointDct *pd, Point *p) {
	if (!pd || !p) return NULL;

	uint64_t z = pointToZ(p, pd->xmin, pd->xmax, pd->ymin, pd->ymax);

	List *candidates = bstRangeSearch(pd->bst, (void *)(uintptr_t)z, (void *)(uintptr_t)z);
	if (!candidates) return NULL;

	void *result = NULL;
	for (LNode *node = candidates->head; node; node = node->next) {
		PointValue *pv = node->value;
		if (ptCompare(pv->point, p) == 0) {
			result = pv->value;
			break;
		}
	}
	listFree(candidates, false);
	return result;
}

List *pdctBallSearch(PointDct *pd, Point *p, double r) {
	if (!pd || !p) return NULL;

	Point *pointA = ptNewFromXY(ptGetx(p) - r, ptGety(p) - r);
	Point *pointB = ptNewFromXY(ptGetx(p) + r, ptGety(p) + r);
	uint64_t zA = pointToZ(pointA, pd->xmin, pd->xmax, pd->ymin, pd->ymax);
	uint64_t zB = pointToZ(pointB, pd->xmin, pd->xmax, pd->ymin, pd->ymax);
	ptFree(pointA);
	ptFree(pointB);

	List *list = bstRangeSearch(pd->bst, (void *)(uintptr_t)zA, (void *)(uintptr_t)zB);
	if (!list) return NULL;

	double r2 = r * r;
	LNode *prev = NULL;
	LNode *node = list->head;
	while (node) {
		LNode *next = node->next;
		PointValue *pointValue = node->value;

		if (ptSqrDistance(p, pointValue->point) > r2) {
			if (!prev) {
				list->head = next;
			} else {
				prev->next = next;
			}

			if (node == list->last) {
				list->last = prev;
			}

			free(node);
			list->size--;
		} else {
			node->value = pointValue->value;
			prev = node;
		}

		node = next;
	}

	if (list->size == 0) {
		list->head = NULL;
		list->last = NULL;
	}

	return list;
}

/* ======= Static functions ======= */

static uint64_t zEncode(uint32_t x, uint32_t y) {
	uint64_t out = 0;
	for (uint8_t byte = 0; byte < 4; ++byte) {
		out |= interleave8((x >> (byte * 8)) & 0xFF, (y >> (byte * 8)) & 0xFF) << (byte * 16);
	}
	return out;
}

static uint64_t interleave8(uint8_t m, uint8_t n) {
	return (
		((m * 0x0101010101010101ULL & 0x8040201008040201ULL) * 0x0102040810204081ULL >> 49) & 0x5555
	) | (
		((n * 0x0101010101010101ULL & 0x8040201008040201ULL) * 0x0102040810204081ULL >> 48) & 0xAAAA
	);
}

static int compareZ(void *a, void *b) {
	uint64_t za = (uint64_t)(uintptr_t)a;
	uint64_t zb = (uint64_t)(uintptr_t)b;
	return za < zb ? -1 : za > zb ? 1 : 0;
}

static uint64_t pointToZ(Point *p, double xMin, double xMax, double yMin, double yMax) {
	double doubleX = (xMax > xMin) ? (ptGetx(p) - xMin) / (xMax - xMin) * UINT32_MAX : 0;
	double doubleY = (yMax > yMin) ? (ptGety(p) - yMin) / (yMax - yMin) * UINT32_MAX : 0;
	uint32_t intX = doubleX <= 0 ? 0 : doubleX >= UINT32_MAX ? UINT32_MAX : (uint32_t)doubleX;
	uint32_t intY = doubleY <= 0 ? 0 : doubleY >= UINT32_MAX ? UINT32_MAX : (uint32_t)doubleY;
	return zEncode(intX, intY);
}
