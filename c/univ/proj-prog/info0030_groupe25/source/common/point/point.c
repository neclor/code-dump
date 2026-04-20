/**
 * @file point.c
 * @brief Implements basic 2D point operations.
 *
 * Provides functions to create points (both stack and heap allocated),
 * perform arithmetic (addition, subtraction, multiplication),
 * compare points, and free dynamically allocated points.
*/

#include <stdbool.h>
#include <stdlib.h>

#include "point.h"

/* ======= Public Functions ======= */

Point point_new(int x, int y) {
   Point p = POINT_NEW(x, y);
   return p;
}

Point *point_new_pointer(int x, int y) {
   Point *point = malloc(sizeof(Point));
   if (point == NULL)
      return NULL;
   point->x = x;
   point->y = y;
   return point;
}

Point point_add(Point p1, Point p2) {
   return point_new(p1.x + p2.x, p1.y + p2.y);
}

Point point_sub(Point p1, Point p2) {
   return point_new(p1.x - p2.x, p1.y - p2.y);
}

Point point_mul(Point p, int n) {
   return point_new(p.x * n, p.y * n);
}

bool point_equal(Point p1, Point p2) {
   return p1.x == p2.x && p1.y == p2.y;
}

void point_free(Point *p) {
   free(p);
}
