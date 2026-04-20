/**
 * @file game.h
 *
 * @brief Game logic module
*/

#ifndef GAME_H
#define GAME_H

#include <stdbool.h>

#include "point.h"

/* ======= Constants ======= */

/**
 * @brief Game difficulty levels
 */
typedef enum Difficulty {
   DIFFICULTY_EASY,
   DIFFICULTY_MEDIUM,
   DIFFICULTY_HARD,
} Difficulty;

/**
 * @brief Cell states in the game field
 */
typedef enum Cell {
   CELL_EMPTY,
   CELL_RED,
   CELL_ORANGE,
   CELL_YELLOW,
   CELL_GREEN,
   CELL_CYAN,
   CELL_BLUE,
   CELL_PURPLE,
   CELL_COUNT,
} Cell;

/* ======= Structures ======= */

/**
 * @brief Game data structure
 *
 * Contains all game state information including:
 * - Current difficulty level
 * - Field dimensions and content
 * - Next balls to be placed
 * - Current score
 * - Game over state
 */
typedef struct GameData GameData;

/* ======= Public Function Prototypes ======= */

/**
 * @brief Creates a new game data instance
 *
 * @return Pointer to new GameData structure, NULL if allocation failed
 */
GameData *new_game_data(void);

/**
 * @brief Initializes a new game with specified difficulty
 *
 * @param data Pointer to game data structure
 * @param difficulty Desired game difficulty level
 *
 * @return 0 on success, negative value on error
 */
int init_new_game(GameData *data, Difficulty difficulty);

/**
 * @brief Gets the count of next balls to be placed
 *
 * @param data Pointer to game data structure
 *
 * @return Number of next balls
 */
size_t get_next_balls_count(const GameData *data);

/**
 * @brief Gets the array of next balls to be placed
 *
 * @param data Pointer to game data structure
 *
 * @return Array of next ball colors, NULL if data is invalid
 */
const Cell *get_next_balls(const GameData *data);

/**
 * @brief Gets the game field width
 *
 * @param data Pointer to game data structure
 *
 * @return Field width in cells
 */
unsigned int get_field_width(const GameData *data);

/**
 * @brief Gets the game field height
 *
 * @param data Pointer to game data structure
 *
 * @return Field height in cells
 */
unsigned int get_field_height(const GameData *data);

/**
 * @brief Gets the current game field state
 *
 * @param data Pointer to game data structure
 *
 * @return Array of field cells, NULL if data is invalid
 */
const Cell *get_field(const GameData *data);

/**
 * @brief Gets the current game score
 *
 * @param data Pointer to game data structure
 *
 * @return Current score
 */
unsigned int get_score(const GameData *data);

/**
 * @brief Checks if the game is over
 *
 * @param data Pointer to game data structure
 *
 * @return true if game is over, false otherwise
 */
bool game_over(const GameData *data);

/**
 * @brief Frees all memory associated with game data
 *
 * @param data Pointer to game data structure
 */
void free_game_data(GameData *data);

/**
 * @brief Attempts to move a ball from start to end position
 *
 * Calculates path and moves ball if possible. Updates game state
 * including score and next balls if move is valid.
 *
 * @param data Pointer to game data structure
 * @param start Starting position
 * @param end Target position
 * @return Array of path points , NULL if move invalid
 */
Point *move_ball(GameData *data, Point start, Point end);

/**
 * @brief Gets the length of a movement path
 *
 * @param data Pointer to game data structure
 * @param path Array of path points
 * @param end End point of the path
 *
 * @return Number of points in path including end point
 */
size_t path_size(const GameData *data, const Point *path, Point end);

#endif // GAME_H
