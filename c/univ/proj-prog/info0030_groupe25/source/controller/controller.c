/**
 * @file controller.c
 *
 * @brief Implementation of the game controller module.
*/

#include <stdbool.h>
#include <stdlib.h>

#include <gtk/gtk.h>

#include "point.h"

#include "controller.h"

#include "game.h"
#include "leaderboard.h"
#include "ui.h"

/* ======= Structures ======= */

struct Controller {
   GameData *game_data;
   Ui *ui;
   Leaderboard *leaderboard;

   Difficulty chosed_difficulty;
   GtkWidget *selectedGameFieldButton;
};

/* ======= Private Function Prototypes ======= */

/**
 * @brief Connects all UI elements with their corresponding callbacks
 *
 * @param c Pointer to the Controller instance
 */
static void connect_ui(Controller *c);

/**
 * @brief Callback handler for window destroy event
 *
 * @param widget Pointer to the widget that triggered the event
 * @param data Unused data pointer (G_GNUC_UNUSED)
 */
static void on_window_destroy(GtkWidget *widget, gpointer data G_GNUC_UNUSED);

/**
 * @brief Callback handler for starting a new game
 *
 * @param widget Unused widget pointer (G_GNUC_UNUSED)
 * @param data Pointer to the Controller instance
 */
static void on_new_game(GtkWidget *widget G_GNUC_UNUSED, gpointer data);

/**
 * @brief Connects click handlers to all game field buttons
 *
 * @param c Pointer to the Controller instance
 */
static void connect_game_field_buttons(Controller *c);

/**
 * @brief Callback handler for game field button clicks
 *
 * @param widget Pointer to the button widget that was clicked
 * @param data Pointer to the Controller instance
 */
static void on_game_field_button(GtkWidget *widget, gpointer data);

/**
 * @brief Callback handler for scores menu item
 *
 * @param widget Unused widget pointer (G_GNUC_UNUSED)
 * @param data Pointer to the Controller instance
 */
static void on_scores(GtkWidget *widget G_GNUC_UNUSED, gpointer data);

/**
 * @brief Callback handler for difficulty menu items
 *
 * @param widget Pointer to the menu item widget that was activated
 * @param data Pointer to the Controller instance
 */
static void on_difficulty(GtkWidget *widget, gpointer data);

/**
 * @brief Callback handler for help menu item
 *
 * @param widget Unused widget pointer (G_GNUC_UNUSED)
 * @param data Pointer to the Controller instance
 */
static void on_help(GtkWidget *widget G_GNUC_UNUSED, gpointer data);

/**
 * @brief Callback handler for switching game images
 *
 * @param widget Unused widget pointer (G_GNUC_UNUSED)
 * @param data Pointer to the Controller instance
 */
static void on_switch(GtkWidget *widget G_GNUC_UNUSED, gpointer data);

/* ======= Public Functions ======= */

Controller *create_controller(
   GameData *game_data,
   Ui *ui,
   Leaderboard *leaderboard,
   Difficulty difficulty
) {
   if (game_data == NULL || ui == NULL || leaderboard == NULL)
      return NULL;

   Controller *controller = malloc(sizeof(Controller));
   if (controller == NULL)
      return NULL;

   controller->game_data = game_data;
   controller->ui = ui;
   controller->leaderboard = leaderboard;

   controller->chosed_difficulty = difficulty;
   controller->selectedGameFieldButton = NULL;

   connect_ui(controller);

   on_new_game(NULL, controller);

   return controller;
}

/* ======= Private Functions ======= */

static void connect_ui(Controller *c) {
   Ui *ui = c->ui;

   /* Game Menu */
   g_signal_connect(
      ui->window,
      "destroy",
      G_CALLBACK(on_window_destroy),
      NULL
   );
   g_signal_connect(ui->newGameItem, "activate", G_CALLBACK(on_new_game), c);
   g_signal_connect(ui->leaderboardItem, "activate", G_CALLBACK(on_scores), c);
   g_signal_connect(
      ui->exitItem,
      "activate",
      G_CALLBACK(on_window_destroy),
      NULL
   );

   /* Difficulty Menu */
   g_signal_connect(ui->easyItem, "activate", G_CALLBACK(on_difficulty), c);
   g_signal_connect(ui->mediumItem, "activate", G_CALLBACK(on_difficulty), c);
   g_signal_connect(ui->hardItem, "activate", G_CALLBACK(on_difficulty), c);

   /* Help */
   g_signal_connect(ui->helpItem, "activate", G_CALLBACK(on_help), c);

   g_signal_connect(ui->switchImagesItem, "activate", G_CALLBACK(on_switch), c);
}

static void on_window_destroy(GtkWidget *widget, gpointer data G_GNUC_UNUSED) {
   gtk_widget_destroy(widget);
   gtk_main_quit();
}

static void on_new_game(GtkWidget *widget G_GNUC_UNUSED, gpointer data) {
   Controller *c = (Controller *)data;

   c->selectedGameFieldButton = NULL;

   if (
      init_new_game(c->game_data, c->chosed_difficulty) != 0 ||
      update_next_balls(c->ui, c->game_data) != 0 ||
      update_score_label(c->ui, c->game_data) != 0 ||
      create_game_field_table(c->ui, c->game_data) != 0
   )
      return;

   connect_game_field_buttons(c);
}

static void connect_game_field_buttons(Controller *c) {
   Ui *ui = c->ui;

   size_t field_size = ui->game_field_width * ui->game_field_height;
   for (size_t i = 0; i < field_size; i++) {
      g_signal_connect(
         ui->gameFieldButtons[i],
         "clicked",
         G_CALLBACK(on_game_field_button),
         c
      );
   }
}

static void on_game_field_button(GtkWidget *widget, gpointer data) {
   Controller *c = (Controller *)data;

   unsigned int field_width = get_field_width(c->game_data);
   const Cell *field = get_field(c->game_data);
   if (field == NULL)
      return;

   Point *position = g_object_get_data(G_OBJECT(widget), "position");
   Cell cell = field[POINT_TO_IDX(*position, field_width)];

   if (cell != CELL_EMPTY) {
      c->selectedGameFieldButton = widget;
      return;
   }

   if (c->selectedGameFieldButton == NULL)
      return;

   Point *start = g_object_get_data(G_OBJECT(c->selectedGameFieldButton), "position");
   Point *end = position;

   c->selectedGameFieldButton = NULL;

   if (start == NULL || end == NULL)
      return;

   Point *path = move_ball(c->game_data, *start, *end);
   if (path == NULL)
      return;

   animate_ball_movement(c->ui, path, path_size(c->game_data, path, *end));
   update_game_field_table(c->ui, c->game_data);
   update_next_balls(c->ui, c->game_data);
   update_score_label(c->ui, c->game_data);

   if(!game_over(c->game_data))
      return;

   char *name = show_game_over_dialog(c->ui, c->game_data);
   if (name != NULL) {
      Record *record = create_record(name, get_score(c->game_data));
      add_record(c->leaderboard, record);
      free(name);
      save_leaderboard(c->leaderboard);
   }

   on_new_game(NULL, c);
}

static void on_scores(GtkWidget *widget G_GNUC_UNUSED, gpointer data) {
   Controller *c = (Controller *)data;

   show_leaderboard_dialog(c->ui, c->leaderboard);
}

static void on_difficulty(GtkWidget *widget, gpointer data) {
   Controller *c = (Controller *)data;

   Difficulty difficulty = (Difficulty)g_object_get_data(
      G_OBJECT(widget),
      "difficulty"
   );

   c->chosed_difficulty = difficulty;
}

static void on_help(GtkWidget *widget G_GNUC_UNUSED, gpointer data) {
   Controller *c = (Controller *)data;
   show_help_dialog(c->ui);
}

static void on_switch(GtkWidget *widget G_GNUC_UNUSED, gpointer data) {
   Controller *c = (Controller *)data;
   switch_images(c->ui);
   update_game_field_table(c->ui, c->game_data);
   update_next_balls(c->ui, c->game_data);
}
