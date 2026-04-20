/**
 * @file game.h
 * @brief Header file containing function prototypes for the game logic.
 *
 * @date 29.03.2025
 * @author Aleksandr Pavlov (s2400691)
*/

#ifndef _GAME_H
#define _GAME_H

#include <stdbool.h>

/* ======= Constants ======= */

/**
 * @brief Number of chests in the game.
 */
#define CHESTS_NUMBER 3

/* ======= Pointers ======= */

/**
 * @brief Callback invoked when the game begins.
 */
extern void (*on_game_begun)();

/**
 * @brief Callback invoked when a chest is opened.
 *
 * @param chest_index Index of the chest that was opened.
 * @param treasured if treasure chest, true, else false.
 */
extern void (*on_chest_opened)(int chest_index, bool treasured);

/**
 * @brief Callback invoked when the game ends.
 *
 * @param win if the player wins, true, else false.
 */
extern void (*on_game_over)(bool win);

/* ======= Function Prototypes ======= */

/**
 * @brief Initializes and starts a new game.
 */
void start_game(void);

/**
 * @brief Handles the player's chest selection.
 *
 * @param chest_index Index of the chest selected by the player.
 *
 * @pre 0 < chest_index < CHESTS_NUMBER
 */
void select_chest(unsigned int chest_index);

/**
 * @brief Retrieves the total number of wins.
 *
 * @return The total number of games won by the player.
 */
unsigned int get_wins(void);

/**
 * @brief Retrieves the total number of losses.
 *
 * @return The total number of games lost by the player.
 */
unsigned int get_losses(void);

#endif // _GAME_H
