/**
 * @file main.c
 * @brief Treasure Hunt
 *
 * @date 29.03.2025
 * @version 1.0.0
 * @author Aleksandr Pavlov (s2400691)
*/

#include <gtk/gtk.h>

#include "game.h"
#include "ui.h"

/* ======= Functions ======= */

/**
 * @brief Entry point for the game.
 *
 * @param argc The number of command-line arguments.
 * @param argv The array of command-line arguments.
 */
int main(int argc, char **argv) {
   gtk_init(&argc, &argv);

   init_ui();
   start_game();

   gtk_main();
   return EXIT_SUCCESS;
}
