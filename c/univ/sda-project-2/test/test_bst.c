#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "BST.h"
#include "List.h"

/* ======= Simple test framework ======= */

static int passed = 0;
static int failed = 0;

#define ASSERT(cond, msg) do { \
    if (cond) { printf("  [PASS] %s\n", msg); passed++; } \
    else       { printf("  [FAIL] %s  (line %d)\n", msg, __LINE__); failed++; } \
} while(0)

/* ======= Helpers ======= */

static int intcmp(void *a, void *b) {
    return *(int *)a - *(int *)b;
}

static int *mkint(int v) {
    int *p = malloc(sizeof(int));
    *p = v;
    return p;
}

static List *intlist(int *vals, int n) {
    List *l = listNew();
    for (int i = 0; i < n; i++)
        listInsertLast(l, mkint(vals[i]));
    return l;
}

/* ======= BST basic tests ======= */

static void test_empty_tree(void) {
    printf("\n[BST] Empty tree\n");
    BST *bst = bstNew(intcmp);
    ASSERT(bstSize(bst) == 0, "size is 0");
    ASSERT(bstHeight(bst) == 0, "height is 0");

    int k = 1;
    ASSERT(bstSearch(bst, &k) == NULL, "search returns NULL");

    int lo = 0, hi = 10;
    List *r = bstRangeSearch(bst, &lo, &hi);
    ASSERT(r != NULL, "rangeSearch returns non-NULL list");
    ASSERT(r->size == 0, "rangeSearch returns empty list");
    ASSERT(bstAverageNodeDepth(bst) == 0.0, "average depth is 0");
    listFree(r, false);
    bstFree(bst, false, false);
}

static void test_single_element(void) {
    printf("\n[BST] Single element\n");
    BST *bst = bstNew(intcmp);
    int *k = mkint(42);
    int *v = mkint(100);
    bstInsert(bst, k, v);
    ASSERT(bstSize(bst) == 1, "size is 1");
    ASSERT(bstHeight(bst) == 0, "height is 0");

    int sk = 42;
    ASSERT(bstSearch(bst, &sk) == v, "search finds value");

    int sk2 = 0;
    ASSERT(bstSearch(bst, &sk2) == NULL, "search non-existing returns NULL");
    ASSERT(bstAverageNodeDepth(bst) == 0.0, "average depth is 0");
    bstFree(bst, true, true);
}

static void test_insert_and_search(void) {
    printf("\n[BST] Insert and search\n");
    BST *bst = bstNew(intcmp);
    int keys[] = {5, 3, 7, 1, 4, 6, 8};
    int *vals[7];
    for (int i = 0; i < 7; i++) {
        vals[i] = mkint(keys[i] * 10);
        bstInsert(bst, mkint(keys[i]), vals[i]);
    }
    ASSERT(bstSize(bst) == 7, "size is 7");
    for (int i = 0; i < 7; i++) {
        int *found = bstSearch(bst, &keys[i]);
        ASSERT(found == vals[i], "search finds correct value");
    }

    int missing1 = 99, missing2 = 0;
    ASSERT(bstSearch(bst, &missing1) == NULL, "search non-existing returns NULL");
    ASSERT(bstSearch(bst, &missing2) == NULL, "search non-existing (0) returns NULL");
    bstFree(bst, true, true);
}

static void test_duplicates(void) {
    printf("\n[BST] Duplicate keys\n");
    BST *bst = bstNew(intcmp);
    int *k1 = mkint(5); int *v1 = mkint(1);
    int *k2 = mkint(5); int *v2 = mkint(2);
    int *k3 = mkint(5); int *v3 = mkint(3);
    bstInsert(bst, k1, v1);
    bstInsert(bst, k2, v2);
    bstInsert(bst, k3, v3);
    ASSERT(bstSize(bst) == 3, "size is 3 with duplicates");

    int sk = 5;
    void *found = bstSearch(bst, &sk);
    ASSERT(found != NULL, "search finds one of the duplicates");
    /* equal keys go left: bstSearch returns the root (first inserted) */
    ASSERT(found == v1, "search finds the root value for duplicate keys");
    bstFree(bst, true, true);
}

static void test_height(void) {
    printf("\n[BST] Height\n");
    BST *bst = bstNew(intcmp);
    /* inserting sorted → degenerates to linked list */
    for (int i = 1; i <= 7; i++)
        bstInsert(bst, mkint(i), mkint(i));
    ASSERT(bstHeight(bst) == 6, "height of degenerate tree (1..7) is 6");
    bstFree(bst, true, true);
}

/* ======= bstOptimalBuild tests ======= */

static void test_optimal_build(void) {
    printf("\n[BST] bstOptimalBuild\n");
    int ks[] = {1, 2, 3, 4, 5, 6, 7};
    int vs[] = {10, 20, 30, 40, 50, 60, 70};
    List *lk = intlist(ks, 7);
    List *lv = intlist(vs, 7);
    BST *bst = bstOptimalBuild(intcmp, lk, lv);
    ASSERT(bst != NULL, "bstOptimalBuild returns non-NULL");
    ASSERT(bstSize(bst) == 7, "size is 7");
    ASSERT(bstHeight(bst) <= 3, "height is minimal (<=3 for 7 elements)");
    for (int i = 0; i < 7; i++) {
        int *found = bstSearch(bst, &ks[i]);
        ASSERT(found != NULL && *found == vs[i], "search finds correct value after optimal build");
    }
    int missing = 99;
    ASSERT(bstSearch(bst, &missing) == NULL, "search non-existing returns NULL");
    listFree(lk, true);
    listFree(lv, true);
    bstFree(bst, false, false);
}

static void test_optimal_build_single(void) {
    printf("\n[BST] bstOptimalBuild single element\n");
    int k = 42, v = 99;
    List *lk = intlist(&k, 1);
    List *lv = intlist(&v, 1);
    BST *bst = bstOptimalBuild(intcmp, lk, lv);
    ASSERT(bst != NULL, "bstOptimalBuild single returns non-NULL");
    ASSERT(bstSize(bst) == 1, "size is 1");
    ASSERT(bstHeight(bst) == 0, "height is 0");
    ASSERT(bstAverageNodeDepth(bst) == 0.0, "average depth is 0 for single node");
    listFree(lk, true);
    listFree(lv, true);
    bstFree(bst, false, false);
}

static void test_optimal_build_depth(void) {
    printf("\n[BST] bstOptimalBuild average depth vs degenerate\n");
    /* 15 elements: optimal tree has avg depth 2.something, degenerate has 7 */
    int n = 15;
    List *lk = listNew(), *lv = listNew();
    BST *deg = bstNew(intcmp);
    for (int i = 1; i <= n; i++) {
        listInsertLast(lk, mkint(i));
        listInsertLast(lv, mkint(i * 10));
        bstInsert(deg, mkint(i), mkint(i * 10));
    }
    BST *opt = bstOptimalBuild(intcmp, lk, lv);
    double avg_opt = bstAverageNodeDepth(opt);
    double avg_deg = bstAverageNodeDepth(deg);
    ASSERT(avg_opt < avg_deg, "optimal build has smaller average depth than degenerate");
    ASSERT(bstHeight(opt) <= 4, "optimal height for 15 elements is at most 4");
    listFree(lk, true);
    listFree(lv, true);
    bstFree(opt, false, false);
    bstFree(deg, true, true);
}

/* ======= bstRangeSearch tests ======= */

static void test_range_search(void) {
    printf("\n[BST] bstRangeSearch\n");
    int ks[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    int vs[] = {10, 20, 30, 40, 50, 60, 70, 80, 90, 100};
    List *lk = intlist(ks, 10);
    List *lv = intlist(vs, 10);
    BST *bst = bstOptimalBuild(intcmp, lk, lv);

    int mn = 3, mx = 7;
    List *r = bstRangeSearch(bst, &mn, &mx);
    ASSERT(r->size == 5, "range [3,7] returns 5 elements");
    listFree(r, false);

    int mn2 = 1, mx2 = 10;
    r = bstRangeSearch(bst, &mn2, &mx2);
    ASSERT(r->size == 10, "range [1,10] returns all 10 elements");
    listFree(r, false);

    int mn3 = 5, mx3 = 3;
    r = bstRangeSearch(bst, &mn3, &mx3);
    ASSERT(r->size == 0, "range [5,3] (inverted) returns 0 elements");
    listFree(r, false);

    int mn4 = 5, mx4 = 5;
    r = bstRangeSearch(bst, &mn4, &mx4);
    ASSERT(r->size == 1, "range [5,5] returns 1 element");
    int *val = r->head->value;
    ASSERT(*val == 50, "range [5,5] returns correct value");
    listFree(r, false);

    int mn5 = 20, mx5 = 30;
    r = bstRangeSearch(bst, &mn5, &mx5);
    ASSERT(r->size == 0, "range [20,30] outside all keys returns 0");
    listFree(r, false);

    int mn6 = 1, mx6 = 1;
    r = bstRangeSearch(bst, &mn6, &mx6);
    ASSERT(r->size == 1, "range [1,1] returns 1 element");
    listFree(r, false);

    listFree(lk, true);
    listFree(lv, true);
    bstFree(bst, false, false);
}

static void test_range_search_duplicates(void) {
    printf("\n[BST] bstRangeSearch with duplicates\n");
    BST *bst = bstNew(intcmp);
    for (int i = 0; i < 3; i++)
        bstInsert(bst, mkint(5), mkint(i));
    bstInsert(bst, mkint(3), mkint(30));
    bstInsert(bst, mkint(7), mkint(70));

    int mn = 5, mx = 5;
    List *r = bstRangeSearch(bst, &mn, &mx);
    ASSERT(r->size == 3, "range [5,5] with 3 duplicates returns 3");
    listFree(r, false);

    int mn2 = 4, mx2 = 6;
    r = bstRangeSearch(bst, &mn2, &mx2);
    ASSERT(r->size == 3, "range [4,6] returns 3 duplicate elements");
    listFree(r, false);
    bstFree(bst, true, true);
}

static void test_range_search_on_inserted_tree(void) {
    printf("\n[BST] bstRangeSearch on bstInsert tree (not optimal)\n");
    BST *bst = bstNew(intcmp);
    /* insert in order that creates unbalanced tree: 10, 5, 15, 3, 7, 12, 20 */
    int keys[] = {10, 5, 15, 3, 7, 12, 20};
    int *vals[7];
    for (int i = 0; i < 7; i++) {
        vals[i] = mkint(keys[i] * 2);
        bstInsert(bst, mkint(keys[i]), vals[i]);
    }

    int lo = 5, hi = 15;
    List *r = bstRangeSearch(bst, &lo, &hi);
    ASSERT(r->size == 5, "range [5,15] on inserted tree returns 5 elements");
    listFree(r, false);

    int lo2 = 1, hi2 = 4;
    r = bstRangeSearch(bst, &lo2, &hi2);
    ASSERT(r->size == 1, "range [1,4] returns only key=3");
    listFree(r, false);

    int lo3 = 16, hi3 = 25;
    r = bstRangeSearch(bst, &lo3, &hi3);
    ASSERT(r->size == 1, "range [16,25] returns only key=20");
    listFree(r, false);

    bstFree(bst, true, false);
    for (int i = 0; i < 7; i++) free(vals[i]);
}

static void test_range_search_negative_keys(void) {
    printf("\n[BST] bstRangeSearch with negative keys\n");
    int ks[] = {-5, -3, -1, 0, 2, 4, 6};
    int vs[] = {-50, -30, -10, 0, 20, 40, 60};
    List *lk = intlist(ks, 7);
    List *lv = intlist(vs, 7);
    BST *bst = bstOptimalBuild(intcmp, lk, lv);

    int lo = -3, hi = 2;
    List *r = bstRangeSearch(bst, &lo, &hi);
    ASSERT(r->size == 4, "range [-3,2] returns 4 elements (-3,-1,0,2)");
    listFree(r, false);

    int lo2 = -10, hi2 = -4;
    r = bstRangeSearch(bst, &lo2, &hi2);
    ASSERT(r->size == 1, "range [-10,-4] returns only key=-5");
    listFree(r, false);

    int lo3 = -5, hi3 = -5;
    r = bstRangeSearch(bst, &lo3, &hi3);
    ASSERT(r->size == 1, "range [-5,-5] returns exactly 1 element");
    listFree(r, false);

    listFree(lk, true);
    listFree(lv, true);
    bstFree(bst, false, false);
}

/* ======= bstAverageNodeDepth tests ======= */

static void test_average_depth(void) {
    printf("\n[BST] bstAverageNodeDepth\n");
    BST *bst = bstNew(intcmp);
    /* root=4, left=2, right=6, left-left=1, left-right=3, right-left=5, right-right=7
       depths: 0+1+1+2+2+2+2 = 10, n=7, avg=10/7 */
    int order[] = {4, 2, 6, 1, 3, 5, 7};
    for (int i = 0; i < 7; i++)
        bstInsert(bst, mkint(order[i]), mkint(i));
    double avg = bstAverageNodeDepth(bst);
    ASSERT(fabs(avg - 10.0/7.0) < 0.001, "average depth is 10/7 for balanced tree");
    bstFree(bst, true, true);
}

/* ======= main ======= */

int main(void) {
    printf("=== BST Tests ===\n");

    test_empty_tree();
    test_single_element();
    test_insert_and_search();
    test_duplicates();
    test_height();
    test_optimal_build();
    test_optimal_build_single();
    test_optimal_build_depth();
    test_range_search();
    test_range_search_duplicates();
    test_range_search_on_inserted_tree();
    test_range_search_negative_keys();
    test_average_depth();

    printf("\n=== Results: %d passed, %d failed ===\n", passed, failed);
    return failed > 0 ? 1 : 0;
}
