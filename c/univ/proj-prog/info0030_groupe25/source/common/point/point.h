/**
 * @file point.h
 * @brief Header file for the Point structure and related operations.
 *
 * This file provides the declaration of the Point structure and functions to
 * create, manipulate, and compare 2D points.
 * It includes operations for point arithmetic such as
 * addition, subtraction, multiplication, and equality checking.
 *
 * @version 1.3.0
 * @author Aleksandr Pavlov (s2400691)
*/

#ifndef POINT_H
#define POINT_H

/* ======= Macros ======= */

/**
 * @brief Creates a new Point structure
 *
 * @param x The X-coordinate for the new point
 * @param y The Y-coordinate for the new point
 *
 * @return A new Point structure
*/
#define POINT_NEW(x, y) {(x), (y)}

/**
 * @brief Calculates a 1D array index for a 2D grid
 *
 * @param x The X-coordinate (column index) within the grid
 * @param y The Y-coordinate (row index) within the grid
 * @param width The total number of columns in the grid
 *
 * @return The calculated index in the 1D array
 */
#define XYW_TO_IDX(x, y, width) ((y) * (width) + (x))

/**
 * @brief Converts a Point into a 1D array index
 *
 * @param point The Point structure containing X and Y coordinates
 * @param width The total number of columns in the grid
 *
 * @return The calculated index in the 1D array
 */
#define POINT_TO_IDX(point, width) XYW_TO_IDX((point).x, (point).y, width)

/* ======= Constants ======= */

#define POINT_ZERO POINT_NEW(0, 0)
#define POINT_ONE POINT_NEW(1, 1)
#define POINT_UP POINT_NEW(0, -1)
#define POINT_DOWN POINT_NEW(0, 1)
#define POINT_LEFT POINT_NEW(-1, 0)
#define POINT_RIGHT POINT_NEW(1, 0)

/* ======= Structures ======= */

/**
 * @struct Point
 *
 * @brief Represents a point in 2D space
 */
typedef struct Point {
   int x;
   int y;
} Point;

/* ======= Public Function Prototypes ======= */

/**
 * @brief Creates a new Point with the given coordinates
 *
 * @param x The X-coordinate of the point
 * @param y The Y-coordinate of the point
 *
 * @return A new Point structure
 */
Point point_new(int x, int y);

/**
 * @brief Allocates and creates a new Point
 *
 * @param x The X-coordinate of the point
 * @param y The Y-coordinate of the point
 *
 * @return Pointer to the newly allocated Point, or NULL if allocation fails
*/
Point *point_new_pointer(int x, int y);

/**
 * @brief Adds two points together
 *
 * @param p1 The first point
 * @param p2 The second point
 *
 * @return A new Point representing the sum of the two points
 */
Point point_add(Point p1, Point p2);

/**
 * @brief Subtracts the second point from the first point
 *
 * @param p1 The first point
 * @param p2 The second point
 *
 * @return A new Point representing the difference of the two points
 */
Point point_sub(Point p1, Point p2);

/**
 * @brief Multiplies a point's coordinates by a scalar value
 *
 * @param p The point to be scaled
 * @param n The scalar value
 *
 * @return A new Point with scaled coordinates
 */
Point point_mul(Point p, int n);

/**
 * @brief Checks if two points are equal
 *
 * @param p1 The first point
 * @param p2 The second point
 *
 * @return
 *     true points are equal
 *     false points are not equal
 */
bool point_equal(Point p1, Point p2);

/**
 * @brief Frees a dynamically allocated Point
 *
 * @param p A pointer to the Point to be freed
 */
void point_free(Point *p);

#endif // POINT_H
