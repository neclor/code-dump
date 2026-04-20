/**
 * @file rng.h
 *
 * @brief Random number generation header file
*/

#ifndef RNG_H
#define RNG_H

/* ======= Public Function Prototypes ======= */

/**
 * @brief Initializes the random number generator
 *
 * This function should be called once at the beginning of the program.
 */
void randomize(void);

/**
 * @brief Generates a random integer
 *
 * @return A random integer
 */
int randi(void);

/**
 * @brief Generates a random integer within a specified range
 *
 * @param from The lower bound of the range (inclusive)
 * @param to The upper bound of the range (inclusive)
 *
 * @return A random integer between `from` and `to`
 */
int randi_range(int from, int to);

#endif // RNG_H
