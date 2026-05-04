#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "PointDct.h"
#include "Point.h"
#include "List.h"

/* ======= Simple test framework ======= */

static int passed = 0;
static int failed = 0;

#define ASSERT(cond, msg) do { \
    if (cond) { printf("  [PASS] %s\n", msg); passed++; } \
    else       { printf("  [FAIL] %s  (line %d)\n", msg, __LINE__); failed++; } \
} while(0)

/* ======= Helpers ======= */

static int *mkint(int v) {
    int *p = malloc(sizeof(int));
    *p = v;
    return p;
}

static List *make_point_list(double *xs, double *ys, int n) {
    List *l = listNew();
    for (int i = 0; i < n; i++)
        listInsertLast(l, ptNewFromXY(xs[i], ys[i]));
    return l;
}

static List *make_value_list(int n) {
    List *l = listNew();
    for (int i = 0; i < n; i++)
        listInsertLast(l, mkint(i));
    return l;
}

static void free_point_list(List *l) {
    for (LNode *n = l->head; n; n = n->next)
        ptFree((Point *)n->value);
    listFree(l, false);
}

/* ======= Tests ======= */

static void test_empty(void) {
    printf("\n[PointDct] Empty dictionary\n");
    List *lp = listNew();
    List *lv = listNew();
    PointDct *pd = pdctCreate(lp, lv);

    /* pdctCreate with empty lists is implementation-defined:
       some implementations return NULL, others return an empty structure */
    if (!pd) {
        printf("  [SKIP] pdctCreate with empty lists returns NULL (implementation-defined)\n");
        listFree(lp, false);
        listFree(lv, false);
        return;
    }

    ASSERT(pdctSize(pd) == 0, "size is 0");

    Point *q = ptNewFromXY(0.5, 0.5);
    ASSERT(pdctExactSearch(pd, q) == NULL, "exactSearch on empty returns NULL");

    List *r = pdctBallSearch(pd, q, 1.0);
    ASSERT(r != NULL, "ballSearch on empty returns non-NULL list");
    ASSERT(r->size == 0, "ballSearch on empty returns empty list");
    listFree(r, false);

    ptFree(q);
    pdctFree(pd);
    listFree(lp, false);
    listFree(lv, false);
}

static void test_single_point(void) {
    printf("\n[PointDct] Single point\n");
    double xs[] = {0.5}, ys[] = {0.5};
    List *lp = make_point_list(xs, ys, 1);
    List *lv = make_value_list(1);
    PointDct *pd = pdctCreate(lp, lv);
    ASSERT(pdctSize(pd) == 1, "size is 1");

    Point *q = ptNewFromXY(0.5, 0.5);
    void *found = pdctExactSearch(pd, q);
    ASSERT(found != NULL, "exactSearch finds the point");

    Point *q2 = ptNewFromXY(0.1, 0.1);
    ASSERT(pdctExactSearch(pd, q2) == NULL, "exactSearch not found returns NULL");

    List *r = pdctBallSearch(pd, q, 0.1);
    ASSERT(r->size == 1, "ballSearch with r=0.1 finds the point");
    listFree(r, false);

    r = pdctBallSearch(pd, q2, 0.01);
    ASSERT(r->size == 0, "ballSearch with small r finds nothing");
    listFree(r, false);

    ptFree(q);
    ptFree(q2);
    pdctFree(pd);
    free_point_list(lp);
    listFree(lv, true);
}

static void test_exact_search(void) {
    printf("\n[PointDct] Exact search\n");
    double xs[] = {0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9};
    double ys[] = {0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9};
    int n = 9;
    List *lp = make_point_list(xs, ys, n);
    List *lv = make_value_list(n);
    PointDct *pd = pdctCreate(lp, lv);
    ASSERT(pdctSize(pd) == (size_t)n, "size is 9");

    for (int i = 0; i < n; i++) {
        Point *q = ptNewFromXY(xs[i], ys[i]);
        void *found = pdctExactSearch(pd, q);
        ASSERT(found != NULL, "exactSearch finds each point");
        ptFree(q);
    }

    Point *q = ptNewFromXY(0.55, 0.55);
    ASSERT(pdctExactSearch(pd, q) == NULL, "exactSearch for non-existing returns NULL");
    ptFree(q);

    q = ptNewFromXY(0.0, 0.0);
    ASSERT(pdctExactSearch(pd, q) == NULL, "exactSearch at origin returns NULL");
    ptFree(q);

    /* point at exact boundary of stored range */
    q = ptNewFromXY(0.1, 0.9);
    ASSERT(pdctExactSearch(pd, q) == NULL, "exactSearch for (0.1,0.9) not in dict returns NULL");
    ptFree(q);

    pdctFree(pd);
    free_point_list(lp);
    listFree(lv, true);
}

static void test_ball_search_basic(void) {
    printf("\n[PointDct] Ball search basic\n");
    double xs[] = {0.0, 1.0, 0.0, 1.0, 0.5};
    double ys[] = {0.0, 0.0, 1.0, 1.0, 0.5};
    int n = 5;
    List *lp = make_point_list(xs, ys, n);
    List *lv = make_value_list(n);
    PointDct *pd = pdctCreate(lp, lv);

    Point *center = ptNewFromXY(0.5, 0.5);

    List *r = pdctBallSearch(pd, center, 0.1);
    ASSERT(r->size == 1, "ballSearch r=0.1 around center finds only center point");
    listFree(r, false);

    /* r=sqrt(0.5) ≈ 0.707: exactly on boundary of corners */
    r = pdctBallSearch(pd, center, 1.0);
    ASSERT(r->size == 5, "ballSearch r=1.0 finds all 5 points");
    listFree(r, false);

    /* radius that finds corners: dist to corner = sqrt(0.5) ≈ 0.707 */
    r = pdctBallSearch(pd, center, 0.71);
    ASSERT(r->size == 5, "ballSearch r=0.71 finds center + 4 corners");
    listFree(r, false);

    r = pdctBallSearch(pd, center, 0.70);
    ASSERT(r->size == 1, "ballSearch r=0.70 finds only center (corners just outside)");
    listFree(r, false);

    Point *far = ptNewFromXY(2.0, 2.0);
    r = pdctBallSearch(pd, far, 0.1);
    ASSERT(r->size == 0, "ballSearch far from all points finds nothing");
    listFree(r, false);
    ptFree(far);

    ptFree(center);
    pdctFree(pd);
    free_point_list(lp);
    listFree(lv, true);
}

static void test_ball_search_boundary(void) {
    printf("\n[PointDct] Ball search boundary cases\n");
    double r = 0.1;
    double xs[] = {0.5};
    double ys[] = {0.5 + r};
    List *lp = make_point_list(xs, ys, 1);
    List *lv = make_value_list(1);
    PointDct *pd = pdctCreate(lp, lv);

    Point *center = ptNewFromXY(0.5, 0.5);
    List *res = pdctBallSearch(pd, center, r);
    ASSERT(res->size == 1, "point exactly at boundary distance is included");
    listFree(res, false);

    res = pdctBallSearch(pd, center, r - 1e-9);
    ASSERT(res->size == 0, "point just outside boundary is excluded");
    listFree(res, false);

    ptFree(center);
    pdctFree(pd);
    free_point_list(lp);
    listFree(lv, true);
}

static void test_ball_search_count(void) {
    printf("\n[PointDct] Ball search count matches brute force\n");
    int n = 81, idx = 0;
    double xs[81], ys[81];
    for (int i = 1; i <= 9; i++)
        for (int j = 1; j <= 9; j++) {
            xs[idx] = i * 0.1;
            ys[idx] = j * 0.1;
            idx++;
        }

    List *lp = make_point_list(xs, ys, n);
    List *lv = make_value_list(n);
    PointDct *pd = pdctCreate(lp, lv);

    Point *center = ptNewFromXY(0.5, 0.5);
    int expected = 0;
    for (int i = 0; i < n; i++) {
        double dx = xs[i] - 0.5, dy = ys[i] - 0.5;
        if (dx*dx + dy*dy <= 0.15*0.15) expected++;
    }

    List *r = pdctBallSearch(pd, center, 0.15);
    ASSERT((int)r->size == expected, "ballSearch count matches brute force");
    listFree(r, false);

    /* r=0 at non-grid point: finds nothing */
    Point *offcenter = ptNewFromXY(0.55, 0.55);
    r = pdctBallSearch(pd, offcenter, 0.0);
    ASSERT(r->size == 0, "ballSearch r=0 at non-grid point finds nothing");
    listFree(r, false);
    ptFree(offcenter);

    /* r=0 at grid point: finds exactly 1 */
    Point *incenter = ptNewFromXY(0.5, 0.5);
    r = pdctBallSearch(pd, incenter, 0.0);
    ASSERT(r->size == 1, "ballSearch r=0 at grid point finds exactly 1");
    listFree(r, false);
    ptFree(incenter);

    ptFree(center);
    pdctFree(pd);
    free_point_list(lp);
    listFree(lv, true);
}

static void test_height_and_depth(void) {
    printf("\n[PointDct] Height and average depth\n");
    int n = 31; /* 2^5 - 1, perfect binary tree */
    double xs[31], ys[31];
    for (int i = 0; i < n; i++) {
        xs[i] = (double)(i + 1) / (n + 1);
        ys[i] = (double)(i + 1) / (n + 1);
    }
    List *lp = make_point_list(xs, ys, n);
    List *lv = make_value_list(n);
    PointDct *pd = pdctCreate(lp, lv);

    size_t h = pdctHeight(pd);
    ASSERT(h <= 5, "height of 31 elements is at most 5 (optimal)");
    size_t avg = pdctAverageNodeDepth(pd);
    ASSERT(avg <= 4, "average depth of 31 elements is at most 4 (optimal)");

    pdctFree(pd);
    free_point_list(lp);
    listFree(lv, true);
}

static void test_duplicates(void) {
    printf("\n[PointDct] Duplicate points\n");
    double xs[] = {0.5, 0.5, 0.5};
    double ys[] = {0.5, 0.5, 0.5};
    List *lp = make_point_list(xs, ys, 3);
    List *lv = make_value_list(3);
    PointDct *pd = pdctCreate(lp, lv);

    ASSERT(pdctSize(pd) == 3, "size is 3 with duplicate points");

    Point *q = ptNewFromXY(0.5, 0.5);
    void *found = pdctExactSearch(pd, q);
    ASSERT(found != NULL, "exactSearch finds one of the duplicates");

    List *r = pdctBallSearch(pd, q, 0.01);
    ASSERT(r->size == 3, "ballSearch finds all 3 duplicates");
    listFree(r, false);

    ptFree(q);
    pdctFree(pd);
    free_point_list(lp);
    listFree(lv, true);
}

static void test_negative_coords(void) {
    printf("\n[PointDct] Negative coordinates\n");
    /* Porto-like: negative longitudes, positive latitudes (after projection) */
    double xs[] = {-2.0, -1.5, -1.0, -0.5, 0.0};
    double ys[] = {-1.0, -0.5,  0.0,  0.5, 1.0};
    int n = 5;
    List *lp = make_point_list(xs, ys, n);
    List *lv = make_value_list(n);
    PointDct *pd = pdctCreate(lp, lv);

    ASSERT(pdctSize(pd) == (size_t)n, "size is 5 with negative coords");

    /* exact search for each stored point */
    for (int i = 0; i < n; i++) {
        Point *q = ptNewFromXY(xs[i], ys[i]);
        ASSERT(pdctExactSearch(pd, q) != NULL, "exactSearch finds point with negative coords");
        ptFree(q);
    }

    /* point not in dict */
    Point *q = ptNewFromXY(-1.0, -1.0);
    ASSERT(pdctExactSearch(pd, q) == NULL, "exactSearch for missing point returns NULL");
    ptFree(q);

    /* ball search around a negative-coordinate point */
    Point *center = ptNewFromXY(-1.0, 0.0);
    List *r = pdctBallSearch(pd, center, 0.6);
    /* (-1.5,-0.5) dist = sqrt(0.25+0.25)=0.707 > 0.6, not included
       (-1.0, 0.0) dist = 0, included
       (-0.5, 0.5) dist = sqrt(0.25+0.25)=0.707 > 0.6, not included */
    ASSERT(r->size == 1, "ballSearch r=0.6 around (-1,0) finds exactly 1 point");
    listFree(r, false);

    r = pdctBallSearch(pd, center, 0.8);
    /* (-1.5,-0.5): sqrt(0.5) ≈ 0.707 < 0.8, included
       (-1.0, 0.0): 0, included
       (-0.5, 0.5): sqrt(0.5) ≈ 0.707 < 0.8, included */
    ASSERT(r->size == 3, "ballSearch r=0.8 around (-1,0) finds 3 points");
    listFree(r, false);

    ptFree(center);
    pdctFree(pd);
    free_point_list(lp);
    listFree(lv, true);
}

static void test_ball_outside_bbox(void) {
    printf("\n[PointDct] Ball search with center outside stored points bbox\n");
    double xs[] = {0.0, 1.0, 2.0};
    double ys[] = {0.0, 1.0, 2.0};
    int n = 3;
    List *lp = make_point_list(xs, ys, n);
    List *lv = make_value_list(n);
    PointDct *pd = pdctCreate(lp, lv);

    /* center far outside bbox to the right, small r: nothing found */
    Point *far = ptNewFromXY(10.0, 10.0);
    List *r = pdctBallSearch(pd, far, 0.5);
    ASSERT(r->size == 0, "center far outside bbox, small r: finds nothing");
    listFree(r, false);

    /* r large enough to reach (2,2): dist = sqrt(64+64) ≈ 11.31, r=12 */
    r = pdctBallSearch(pd, far, 12.0);
    ASSERT(r->size == 1, "center outside bbox, r=12 reaches nearest point (2,2)");
    listFree(r, false);

    /* r large enough to reach (1,1): dist ≈ 12.73, r=13 */
    r = pdctBallSearch(pd, far, 13.0);
    ASSERT(r->size == 2, "center outside bbox, r=13 reaches (2,2) and (1,1)");
    listFree(r, false);

    ptFree(far);

    /* center outside bbox to the left (negative side) */
    Point *left = ptNewFromXY(-5.0, 0.0);
    r = pdctBallSearch(pd, left, 4.0);
    /* dist to (0,0) = 5 > 4: nothing */
    ASSERT(r->size == 0, "center outside bbox on left, small r: finds nothing");
    listFree(r, false);

    r = pdctBallSearch(pd, left, 6.0);
    /* dist to (0,0) = 5 < 6: found */
    ASSERT(r->size == 1, "center outside bbox on left, r=6 reaches (0,0)");
    listFree(r, false);

    ptFree(left);
    pdctFree(pd);
    free_point_list(lp);
    listFree(lv, true);
}

/* ======= main ======= */

int main(void) {
    printf("=== PointDct Tests ===\n");

    test_empty();
    test_single_point();
    test_exact_search();
    test_ball_search_basic();
    test_ball_search_boundary();
    test_ball_search_count();
    test_height_and_depth();
    test_duplicates();
    test_negative_coords();
    test_ball_outside_bbox();

    printf("\n=== Results: %d passed, %d failed ===\n", passed, failed);
    return failed > 0 ? 1 : 0;
}
