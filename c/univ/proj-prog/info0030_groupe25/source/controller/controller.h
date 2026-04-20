/**
 * @file controller.h
 *
 * @brief Game controller
*/

#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "game.h"
#include "leaderboard.h"
#include "ui.h"

/* ======= Structures ======= */

/**
 * @brief Controller structure handling game and UI interaction
 */
typedef struct Controller Controller;

/* ======= Public Function Prototypes ======= */

/**
 * @brief Creates a new controller instance
 *
 * @param game_data Pointer to the game data structure
 * @param ui Pointer to the UI structure
 *
 * @return Pointer to the created controller, NULL if creation failed
*/
Controller *create_controller(
   GameData *game_data,
   Ui *ui,
   Leaderboard *leaderboard,
   Difficulty difficulty
);

#endif // CONTROLLER_H
