/**
 * @file main.c
 * @brief Application entry point
 *
 * @date 05.05.2025
 * @version 1.0.0
 * @authors Aleksandr Pavlov (s2400691), Alexandre Gendebien (s2404939)
*/

#include <getopt.h>
#include <gtk/gtk.h>
#include <strings.h>

#include "controller.h"
#include "game.h"
#include "leaderboard.h"
#include "ui.h"

/* ======= Constants ======= */

#define VERSION "1.0.0"
#define AUTHORS "Pavlov Aleksandr (s2400691), Alexandre Gendebien (s2404939)"

enum {
   GETOPT_VERSION_CHAR = (CHAR_MIN - 3),
};

static struct option const longopts[] = {
   {"difficulty", required_argument, NULL, 'n'},
   {"file", required_argument, NULL, 'f'},
   {"help", no_argument, NULL, 'h'},
   {"version", no_argument, NULL, GETOPT_VERSION_CHAR},
   {NULL, no_argument, NULL, 0},
};

const char *program_name;

/* ======= Private Function Prototypes ======= */

/**
 * @brief Displays help about using the command and exits the program.
 *
 * This function prints the usage instructions for the program, including
 * details about the available options and filters.
 *
 * @param status Exit status to terminate the program with.
 */
static void usage(int status);

/* ======= Functions ======= */

int main(int argc, char **argv) {
   program_name = argv[0];

   const char *input_difficulty = NULL;
   const char *score_filename = NULL;

   int optc;
   while ((optc = getopt_long(argc, argv, "n:f:h", longopts, NULL)) != -1) {
      switch (optc) {
         case 'n':
            input_difficulty = optarg;
            break;
         case 'f':
            score_filename = optarg;
            break;
         case 'h':
            usage(EXIT_SUCCESS);
            break;
         case GETOPT_VERSION_CHAR:
            fprintf(
               stdout,
               "%s %s\n\nWritten by %s.\n",
               program_name,
               VERSION,
               AUTHORS
            );
            exit(EXIT_SUCCESS);
            break;
         default:
            usage(EXIT_FAILURE);
            break;
      }
   }

   Difficulty difficulty;
   if (input_difficulty == NULL) {
      difficulty = DIFFICULTY_EASY;
   } else if (!strcasecmp(input_difficulty, "0")) {
      difficulty = DIFFICULTY_EASY;
   } else if (!strcasecmp(input_difficulty, "1")) {
      difficulty = DIFFICULTY_MEDIUM;
   } else if (!strcasecmp(input_difficulty, "2")) {
      difficulty = DIFFICULTY_HARD;
   } else {
      fprintf(
         stderr,
         "%s: invalid difficulty '%s'\n",
         program_name,
         input_difficulty
      );
      usage(EXIT_FAILURE);
   }

   gtk_init(&argc, &argv);

   GameData *game_data = new_game_data();
   Leaderboard *leaderboard = create_leaderboard(score_filename);
   Ui *ui = create_ui();
   Controller *controller = create_controller(
      game_data,
      ui,
      leaderboard,
      difficulty
   );

   gtk_main();

   free(controller);
   free_game_data(game_data);
   free_leaderboard(leaderboard);
   free_ui(ui);

   return EXIT_SUCCESS;
}

static void usage(int status) {
   if (status != EXIT_SUCCESS) {
      fprintf(stderr, "Try '%s --help or -h' for more information.\n",
         program_name);
   } else {
      printf("Usage: %s -n DIFFICULTY -f FILE\n",
         program_name);
      fputs("\
Five or More\n\
\n\
Mandatory arguments to long options are mandatory for short options too.\n\
  -n, --difficulty=DIFFICULTY  specify difficulty (0, 1, 2)\n\
  -f, --file=FILE              specify score file\n\
\n\
  -h, --help                   display this help and exit\n\
      --version                output version information and exit\n\
", stdout);
   }
   exit(status);
}
