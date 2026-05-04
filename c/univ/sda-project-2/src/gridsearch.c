#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "PointDct.h"
#include "List.h"
#include "Point.h"

static const size_t LINE_SIZE = 1024;

static List *parseCsvPoints(const char *filename, List **ltrips_out) {
    FILE *f = fopen(filename, "r");
    if (!f) { fprintf(stderr, "Cannot open %s\n", filename); exit(1); }
    List *lpoints = listNew();
    List *ltrips  = listNew();
    char line[LINE_SIZE];
    size_t nb = 0;
    char delim = ';';
    while (fgets(line, sizeof(line), f)) {
        size_t c = 0;
        while (line[c] != delim) c++; c++;
        while (line[c] != delim) c++; c++;
        while (line[c] != delim) c++; c++;
        double lon = strtod(line + c, NULL);
        while (line[c] != delim) c++; c++;
        double lat = strtod(line + c, NULL);
        listInsertLast(lpoints, ptNewFromLL(lon, lat));
        /* store lon/lat as value (packed in heap double[2]) */
        double *ll = malloc(2 * sizeof(double));
        ll[0] = lon; ll[1] = lat;
        listInsertLast(ltrips, ll);
        nb++;
    }
    fclose(f);
    printf("Loaded %zu trips\n", nb);
    *ltrips_out = ltrips;
    return lpoints;
}

int main(void) {
    List *ltrips;
    List *lpoints = parseCsvPoints("taxitripsporto.csv", &ltrips);

    /* find x/y bounds */
    double xmin, xmax, ymin, ymax;
    LNode *n = lpoints->head;
    xmin = xmax = ptGetx((Point*)n->value);
    ymin = ymax = ptGety((Point*)n->value);
    for (n = n->next; n; n = n->next) {
        double x = ptGetx((Point*)n->value);
        double y = ptGety((Point*)n->value);
        if (x < xmin) xmin = x;
        if (x > xmax) xmax = x;
        if (y < ymin) ymin = y;
        if (y > ymax) ymax = y;
    }
    printf("x: [%f, %f]  y: [%f, %f]\n", xmin, xmax, ymin, ymax);

    printf("Building BST2D...");
    fflush(stdout);
    PointDct *pd = pdctCreate(lpoints, ltrips);
    printf("Done\n");

    double step = 0.1; /* 100m in km */
    double r    = 0.1;

    size_t best_count = 0;
    double best_x = 0, best_y = 0;

    long nx = (long)((xmax - xmin) / step) + 1;
    long ny = (long)((ymax - ymin) / step) + 1;
    printf("Grid: %ld x %ld = %ld points\n", nx, ny, nx * ny);
    fflush(stdout);

    long total = nx * ny, done = 0;
    for (long ix = 0; ix <= nx; ix++) {
        double x = xmin + ix * step;
        for (long iy = 0; iy <= ny; iy++) {
            double y = ymin + iy * step;
            Point *p = ptNewFromXY(x, y);
            List *res = pdctBallSearch(pd, p, r);
            if (res->size > best_count) {
                best_count = res->size;
                best_x = x;
                best_y = y;
            }
            listFree(res, false);
            ptFree(p);
            done++;
        }
        if (ix % 10 == 0) {
            printf("\r  %.1f%%  best so far: %zu", 100.0 * done / total, best_count);
            fflush(stdout);
        }
    }
    printf("\n");

    /* convert best point back to lon/lat */
    Point *best = ptNewFromXY(best_x, best_y);
    printf("\nResult:\n");
    printf("  Max taxis in 100m radius: %zu\n", best_count);
    printf("  x=%.6f km, y=%.6f km\n", best_x, best_y);
    printf("  longitude=%.6f, latitude=%.6f\n",
           ptGetLongitude(best), ptGetLatitude(best));
    ptFree(best);

    pdctFree(pd);
    for (LNode *p = lpoints->head; p; p = p->next) ptFree(p->value);
    listFree(lpoints, false);
    for (LNode *p = ltrips->head; p; p = p->next) free(p->value);
    listFree(ltrips, false);
    return 0;
}
