/**
 * @file ui.h
 *
 * @brief User interface module interface
*/

#ifndef UI_H
#define UI_H

#include <gtk/gtk.h>

#include "game.h"
#include "leaderboard.h"

/* ======= Structures ======= */

/**
 * @brief Main UI structure containing all interface elements
 *
 * Contains all GTK widgets and game interface elements including:
 * - Main window and layout containers
 * - Menu items and buttons
 * - Game field representation
 * - Score and status displays
 */
typedef struct Ui {
   /* === Window === */

   GtkWidget *window;
   GtkWidget *mainVBox;

   /* === Menu Bar === */

   GtkWidget *gameMenuItem;
   GtkWidget *newGameItem;
   GtkWidget *leaderboardItem;
   GtkWidget *exitItem;

   GtkWidget *difficultyItem;
   GtkWidget *easyItem;
   GtkWidget *mediumItem;
   GtkWidget *hardItem;

   GtkWidget *helpItem;

   /* === Information Bar === */

   GtkWidget *nextBallsHBox;
   GtkWidget *scoreLabel;

   /* === Game Field === */

   unsigned int game_field_width;
   unsigned int game_field_height;
   GtkWidget *gameFieldTable;
   GtkWidget **gameFieldButtons;

   GtkWidget *switchImagesItem;
   bool original_images;
   const char **images;
} Ui;

/* ======= Public Function Prototypes ======= */

/**
 * @brief Creates and initializes the UI structure
 *
 * @return Pointer to new UI structure, NULL if creation failed
 */
Ui *create_ui(void);

/**
 * @brief Shows the high scores screen
 *
 * @param ui Pointer to UI structure
 *
 * @return 0 on success, negative value on error
 */
int show_leaderboard_dialog(Ui *ui, Leaderboard *leaderboard);

/**
 * @brief Shows the help screen with game instructions
 *
 * @param ui Pointer to UI structure
 *
 * @return 0 on success, negative value on error
 */
int show_help_dialog(Ui *ui);

/**
 * @brief Shows game over screen with final score
 *
 * @param ui Pointer to UI structure
 * @param data Pointer to game data structure
 *
 * @return Pointer to player name string, NULL if dialog was closed
 */
char *show_game_over_dialog(Ui *ui, const GameData *data);

/**
 * @brief Updates the display of next balls
 *
 * @param ui Pointer to UI structure
 * @param data Pointer to game data structure
 *
 * @return 0 on success, negative value on error
 */
int update_next_balls(Ui *ui, const GameData *data);

/**
 * @brief Updates the score display
 *
 * @param ui Pointer to UI structure
 * @param data Pointer to game data structure
 *
 * @return 0 on success, negative value on error
 */
int update_score_label(Ui *ui, const GameData *data);

/**
 * @brief Creates new game field table
 *
 * @param ui Pointer to UI structure
 * @param data Pointer to game data structure
 *
 * @return 0 on success, negative value on error
 */
int create_game_field_table(Ui *ui, const GameData *data);

/**
 * @brief Updates the game field display
 *
 * @param ui Pointer to UI structure
 * @param data Pointer to game data structure
 *
 * @return 0 on success, negative value on error
 */
int update_game_field_table(Ui *ui, const GameData *data);

/**
 * @brief Animates ball movement along a path
 *
 * @param ui Pointer to UI structure
 * @param path Array of points defining movement path
 * @param path_size Number of points in path
 *
 * @return 0 on success, negative value on error
 */
int animate_ball_movement(Ui *ui, const Point *path, size_t path_size);

/**
 * @brief Switches between different sets of game ball images
 *
 * @param ui Pointer to UI structure
 */
void switch_images(Ui *ui);

/**
 * @brief Frees all memory associated with UI structure
 *
 * @param ui Pointer to UI structure
 */
void free_ui(Ui *ui);

#endif // UI_H
