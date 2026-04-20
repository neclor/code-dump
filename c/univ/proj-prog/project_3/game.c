/**
 * @file game.c
 * @brief Implementation of game logic.
 *
 * @date 29.03.2025
 * @author Aleksandr Pavlov (s2400691)
*/

#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

#include "game.h"

/* ======= Pointers ======= */

void (*on_game_begun)() = NULL;
void (*on_chest_opened)(int chest_index, bool treasured) = NULL;
void (*on_game_over)(bool win) = NULL;

/* ======= Constants ======= */

typedef enum States_t {
   FIRST_CHOICE,
   SECOND_CHOICE,
   END,
} States;

/* ======= Variables ======= */

static States state = FIRST_CHOICE;

static unsigned int wins = 0;
static unsigned int losses = 0;

static unsigned int treasure_chest_index = 0;
static unsigned int opened_chest_index;

/* ======= Internal Function Prototypes ======= */

/**
 * @brief Handles the first chest selection by the player.
 *
 * @param chest_index Index of the chest selected by the player.
 *
 * @pre state = FIRST_CHOICE
 * @post state = SECOND_CHOICE
 */
static void first_select_chest(unsigned int chest_index);

/**
 * @brief Opens an empty chest.
 *
 * @param selected_chest_index Index of the chest selected by the player.
 *
 * @pre 0 < selected_chest_index < CHESTS_NUMBER
 * @post opened_chest_index = index of an empty chest
 */
static void open_empty_chest(unsigned int selected_chest_index);

/**
 * @brief Handles the second chest selection by the player.
 *
 * @param chest_index Index of the chest selected by the player.
 *
 * @pre state = SECOND_CHOICE, 0 < chest_index < CHESTS_NUMBER
 * @post state = END
 */
static void second_select_chest(unsigned int chest_index);

/* ======= External Functions ======= */

void start_game(void) {
   srand(time(NULL));
   state = FIRST_CHOICE;
   treasure_chest_index = rand() % CHESTS_NUMBER;

   if (on_game_begun!= NULL) on_game_begun();
}

void select_chest(unsigned int i) {
   if (CHESTS_NUMBER <= i) return;

   switch (state) {
      case FIRST_CHOICE:
         first_select_chest(i);
         break;

      case SECOND_CHOICE:
         second_select_chest(i);
         break;

      default:
         break;
   }
}

unsigned int get_wins(void) {
   return wins;
}

unsigned int get_losses(void) {
   return losses;
}

/* ======= Internal Functions ======= */

static void first_select_chest(unsigned int chest_index) {
    open_empty_chest(chest_index);
}

static void open_empty_chest(unsigned int selected_chest_index) {
    do {
        opened_chest_index = rand() % CHESTS_NUMBER;
    } while (
        opened_chest_index == selected_chest_index ||
        opened_chest_index == treasure_chest_index
    );

    state = SECOND_CHOICE;

    if (on_chest_opened != NULL) on_chest_opened(opened_chest_index, false);
}

static void second_select_chest(unsigned int chest_index) {
    if (chest_index == opened_chest_index) return;

    bool win = (chest_index == treasure_chest_index) ? true : false;
    if (win) {
        wins++;
    } else {
        losses++;
    }

    state = END;

    if (on_chest_opened != NULL) on_chest_opened(chest_index, win);
    if (on_game_over != NULL) on_game_over(win);
}
