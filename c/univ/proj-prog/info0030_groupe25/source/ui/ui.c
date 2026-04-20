/**
 * @file ui.c
 *
 * @brief Implementation of the user interface module
*/

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <gdk-pixbuf/gdk-pixbuf.h>
#include <gtk/gtk.h>

#include "my_gtk_utils.h"
#include "point.h"

#include "ui.h"

#include "game.h"
#include "leaderboard.h"

/* ======= Constants ======= */

static const Point RESOLUTION = POINT_NEW(800, 600);

static const char *ICON = "source/assets/images/orange.png";

static const char *IMAGES[CELL_COUNT] = {
   NULL,
   "source/assets/images/red.png",
   "source/assets/images/orange.png",
   "source/assets/images/yellow.png",
   "source/assets/images/green.png",
   "source/assets/images/cyan.png",
   "source/assets/images/blue.png",
   "source/assets/images/purple.png",
};

static const char *MINECRAFT_IMAGES[CELL_COUNT] = {
   NULL,
   "source/assets/mine_images/red.png",
   "source/assets/mine_images/orange.png",
   "source/assets/mine_images/yellow.png",
   "source/assets/mine_images/green.png",
   "source/assets/mine_images/cyan.png",
   "source/assets/mine_images/blue.png",
   "source/assets/mine_images/purple.png",
};

static const Point IMAGE_SIZE = POINT_NEW(50, 50);

/* ======= Private Function Prototypes ======= */

/**
 * @brief Creates and configures the main application window
 *
 * @param ui Pointer to UI structure
 */
static void create_window(Ui *ui);

/**
 * @brief Creates the main vertical layout container
 *
 * @param ui Pointer to UI structure
 */
static void create_main_vbox(Ui *ui);

/**
 * @brief Creates the menu bar at the top of the window
 *
 * @param ui Pointer to UI structure
 */
static void create_menu_bar(Ui *ui);

/**
 * @brief Creates the Game menu and its items
 *
 * @param ui Pointer to UI structure
 */
static void create_game_menu(Ui *ui);

/**
 * @brief Creates the Difficulty menu and its items
 *
 * @param ui Pointer to UI structure
 */
static void create_difficulty_menu(Ui *ui);

/**
 * @brief Creates the information bar below menu
 *
 * @param ui Pointer to UI structure
 */
static void create_information_bar(Ui *ui);

/**
 * @brief Destroys all next ball preview images
 *
 * @param ui Pointer to UI structure
 */
static void destroy_next_balls(Ui *ui);

/**
 * @brief Destroys the game field table and buttons
 *
 * @param ui Pointer to UI structure
 */
static void destroy_game_field_table(Ui *ui);

/* ======= Public Functions ======= */

Ui *create_ui(void) {
   Ui *ui = malloc(sizeof(Ui));
   if (ui == NULL)
      return NULL;

   ui->window = NULL;
   ui->mainVBox = NULL;
   ui->gameMenuItem = NULL;
   ui->newGameItem = NULL;
   ui->leaderboardItem = NULL;
   ui->exitItem = NULL;
   ui->difficultyItem = NULL;
   ui->easyItem = NULL;
   ui->mediumItem = NULL;
   ui->hardItem = NULL;
   ui->helpItem = NULL;
   ui->nextBallsHBox = NULL;
   ui->scoreLabel = NULL;
   ui->gameFieldTable = NULL;
   ui->game_field_width = 0;
   ui->game_field_height = 0;
   ui->gameFieldButtons = NULL;
   ui->switchImagesItem = NULL;
   ui-> original_images = true;
   ui->images = IMAGES;

   create_window(ui);
   gtk_widget_show_all(ui->window);

   return ui;
}

int show_leaderboard_dialog(Ui *ui, Leaderboard *leaderboard) {
   if (ui == NULL || ui->window == NULL || leaderboard == NULL)
      return -1;

   size_t records_count = get_records_count(leaderboard);
   Record **records = get_records(leaderboard);

   char text[1024] = "Best scores:\n";
   size_t offset = strlen(text);

   for (size_t i = 0; i < records_count; i++) {
      const char *name = record_get_name(records[i]);
      unsigned int score = record_get_score(records[i]);

      offset += snprintf(
         text + offset,
         1024 - offset,
         "%zu. %s: %u\n",
         i + 1, name, score
      );

      if (offset >= 1024 - 1)
         break;
   }

   GtkWidget *dialog = gtk_message_dialog_new(
      GTK_WINDOW(ui->window),
      GTK_DIALOG_DESTROY_WITH_PARENT,
      GTK_MESSAGE_INFO,
      GTK_BUTTONS_CLOSE,
      "%s", text
   );

   gtk_window_set_title(GTK_WINDOW(dialog), "Leaderboard");

   gtk_dialog_run(GTK_DIALOG(dialog));
   gtk_widget_destroy(dialog);

   return 0;
}

int show_help_dialog(Ui *ui) {
   if (ui == NULL || ui->window == NULL)
      return -1;

   GtkWidget *dialog = gtk_message_dialog_new(
      GTK_WINDOW(ui->window),
      GTK_DIALOG_DESTROY_WITH_PARENT | GTK_DIALOG_MODAL,
      GTK_MESSAGE_INFO,
      GTK_BUTTONS_CLOSE,
      "Five or More:\n"
      "Objective: align 5 or more identical pieces on a vertical or horizontal line to score.\n"
      "How to play:\n"
      " - Click to select a piece to move.\n"
      " - Click an empty space to place it.\n"
      " - Pieces must follow a clear path eather in vertical, horizontal or diagonal.\n"
      " - The game end if you cannot move any balls\n"
      "\n"
      "Aleksandr Pavlov (s2400691) and Alexandre Gendebien (s2404939)"
   );

   gtk_dialog_run(GTK_DIALOG(dialog));
   gtk_widget_destroy(dialog);

   return 0;
}

char *show_game_over_dialog(Ui *ui, const GameData *data) {
   if (ui == NULL || ui->window == NULL || data == NULL)
      return NULL;

   char text[128];
   sprintf(
      text,
      "Congratulations!\nYour score: %u\nEnter your name:",
      get_score(data)
   );

   GtkWidget *dialog = gtk_dialog_new_with_buttons(
      "Game Over",
      GTK_WINDOW(ui->window),
      GTK_DIALOG_DESTROY_WITH_PARENT | GTK_DIALOG_MODAL,
      GTK_STOCK_OK, GTK_RESPONSE_ACCEPT,
      GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
      NULL
   );

   GtkWidget *content = gtk_dialog_get_content_area(GTK_DIALOG(dialog));

   GtkWidget *label = gtk_label_new(text);
   gtk_container_add(GTK_CONTAINER(content), label);

   GtkWidget *entry = gtk_entry_new();
   gtk_entry_set_max_length(GTK_ENTRY(entry), MAX_NAME_LENGTH);
   gtk_container_add(GTK_CONTAINER(content), entry);

   gtk_widget_show_all(dialog);

   char *name = NULL;
   if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT) {
      const char *entry_text = gtk_entry_get_text(GTK_ENTRY(entry));
      if (entry_text != NULL && entry_text[0] != '\0') {
         name = g_strdup(entry_text);
      }
   }

   gtk_widget_destroy(dialog);

   return name;
}

int update_next_balls(Ui *ui, const GameData *data) {
   if (ui == NULL || ui->nextBallsHBox == NULL || data == NULL)
      return -1;

   size_t next_balls_count = get_next_balls_count(data);
   const Cell *next_balls = get_next_balls(data);
   if (next_balls == NULL)
      return -1;

   destroy_next_balls(ui);

   for (size_t i = 0; i < next_balls_count; i++) {
      GtkWidget *image = load_image(
         ui->images[next_balls[i]], IMAGE_SIZE.x, IMAGE_SIZE.y
      );
      if (image == NULL)
         image = gtk_image_new();

      gtk_box_pack_start(GTK_BOX(ui->nextBallsHBox), image, TRUE, TRUE, 5);
   }
   gtk_widget_show_all(ui->nextBallsHBox);
   return 0;
}

int update_score_label(Ui *ui, const GameData *data) {
   if (ui == NULL || ui->scoreLabel == NULL || data == NULL)
      return -1;

   char text[128];
   snprintf(text, 128, "Score: %u", get_score(data));
   gtk_label_set_text(GTK_LABEL(ui->scoreLabel), text);

   return 0;
}

int create_game_field_table(Ui *ui, const GameData *data) {
   if (ui == NULL || data == NULL)
      return -1;

   unsigned int field_width = get_field_width(data);
   unsigned int field_height = get_field_height(data);
   const Cell *field = get_field(data);
   if (field == NULL)
      return -1;
   size_t field_size = field_width * field_height;

   destroy_game_field_table(ui);

   ui->game_field_width = field_width;
   ui->game_field_height = field_height;
   ui->gameFieldTable = gtk_table_new(field_width, field_height, TRUE);
   gtk_box_pack_start(GTK_BOX(ui->mainVBox), ui->gameFieldTable, TRUE, TRUE, 0);

   ui->gameFieldButtons = calloc(field_size, sizeof(GtkWidget *));
   if (ui->gameFieldButtons == NULL) {
      destroy_game_field_table(ui);
      return -2;
   }

   for (unsigned int x = 0; x < field_width; x++) {
      for (unsigned int y = 0; y < field_height; y++) {
         Point *point = point_new_pointer(x, y);
         if (point == NULL) {
            destroy_game_field_table(ui);
            return -2;
         }

         GtkWidget *button = gtk_button_new();
         gtk_object_set_data_full(
            GTK_OBJECT(button), "position", point, (GDestroyNotify)point_free
         );
         gtk_table_attach_defaults(
            GTK_TABLE(ui->gameFieldTable), button, x, x + 1, y, y + 1
         );

         ui->gameFieldButtons[XYW_TO_IDX(x, y, field_width)] = button;
      }
   }
   update_game_field_table(ui, data);
   gtk_widget_show_all(ui->gameFieldTable);
   return 0;
}

int update_game_field_table(Ui *ui, const GameData *data) {
   if (
      ui == NULL ||
      ui->gameFieldTable == NULL ||
      ui->gameFieldButtons == NULL ||
      data == NULL
   )
      return -1;

   const Cell *field = get_field(data);
   if (
      field == NULL ||
      ui->game_field_width != get_field_width(data) ||
      ui->game_field_height != get_field_height(data)
   )
      return -1;

   size_t field_size = ui->game_field_width * ui->game_field_height;

   for (size_t i = 0; i < field_size; i++) {
      gtk_button_set_image(
         GTK_BUTTON(ui->gameFieldButtons[i]),
         load_image(ui->images[field[i]], IMAGE_SIZE.x, IMAGE_SIZE.y)
      );
   }
   return 0;
}

int animate_ball_movement(Ui *ui, const Point *path, size_t path_size) {
   if (
      ui == NULL ||
      ui->gameFieldTable == NULL ||
      ui->gameFieldButtons == NULL ||
      path == NULL ||
      path_size == 0
   )
      return -1;

   for (size_t i = 0; i < path_size - 1; i++) {
      GtkWidget *button = ui->gameFieldButtons[
         POINT_TO_IDX(path[i], ui->game_field_width)
      ];
      GtkWidget *nextButton = ui->gameFieldButtons[
         POINT_TO_IDX(path[i + 1], ui->game_field_width)
      ];

      GtkWidget *image = gtk_button_get_image(GTK_BUTTON(button));
      if (image != NULL) {
         GdkPixbuf *pixbuf = gtk_image_get_pixbuf(GTK_IMAGE(image));
         GdkPixbuf *new_pixbuf = gdk_pixbuf_copy(pixbuf);
         GtkWidget *new_image = gtk_image_new_from_pixbuf(new_pixbuf);
         g_object_unref(new_pixbuf);

         gtk_button_set_image(GTK_BUTTON(nextButton), new_image);
         gtk_button_set_image(GTK_BUTTON(button), NULL);
      }

      while (gtk_events_pending())
         gtk_main_iteration_do(FALSE);

      g_usleep(30 * 1000);
   }

   return 0;
}

void switch_images(Ui *ui) {
   if (ui->original_images) {
      ui->original_images = false;
      ui->images = MINECRAFT_IMAGES;
   } else {
      ui->original_images = true;
      ui->images = IMAGES;
   }
}

void free_ui(Ui *ui) {
   free(ui->gameFieldButtons);
   free(ui);
}

/* ======= Private Functions ======= */

static void create_window(Ui *ui) {
   ui->window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
   gtk_window_set_default_size(GTK_WINDOW(ui->window), RESOLUTION.x, RESOLUTION.y);
   gtk_window_set_title(GTK_WINDOW(ui->window), "Five or More");
   gtk_window_set_icon_from_file(GTK_WINDOW(ui->window), ICON, NULL);

   create_main_vbox(ui);
}

static void create_main_vbox(Ui *ui) {
   ui->mainVBox = gtk_vbox_new(FALSE, 0);
   gtk_container_add(GTK_CONTAINER(ui->window), ui->mainVBox);

   create_menu_bar(ui);
   create_information_bar(ui);
}

static void create_menu_bar(Ui *ui) {
   GtkWidget *menuBar = gtk_menu_bar_new();
   gtk_box_pack_start(GTK_BOX(ui->mainVBox), menuBar, FALSE, TRUE, 0);

   ui->gameMenuItem = gtk_menu_item_new_with_label("Game");
   gtk_menu_bar_append(GTK_MENU_BAR(menuBar), ui->gameMenuItem);
   create_game_menu(ui);

   ui->difficultyItem = gtk_menu_item_new_with_label("Difficulty");
   gtk_menu_bar_append(GTK_MENU_BAR(menuBar), ui->difficultyItem);
   create_difficulty_menu(ui);

   ui->helpItem = gtk_menu_item_new_with_label("Help");
   gtk_menu_bar_append(GTK_MENU_BAR(menuBar), ui->helpItem);

   ui->switchImagesItem = gtk_menu_item_new_with_label("Switch images");
   gtk_menu_bar_append(GTK_MENU_BAR(menuBar), ui->switchImagesItem);
}

static void create_game_menu(Ui *ui) {
   GtkWidget *gameMenu = gtk_menu_new();
   gtk_menu_item_set_submenu(GTK_MENU_ITEM(ui->gameMenuItem), gameMenu);

   ui->newGameItem = gtk_menu_item_new_with_label("New game");
   gtk_menu_append(GTK_MENU(gameMenu), ui->newGameItem);

   ui->leaderboardItem = gtk_menu_item_new_with_label("Leaderboard");
   gtk_menu_append(GTK_MENU(gameMenu), ui->leaderboardItem);

   gtk_menu_append(GTK_MENU(gameMenu), gtk_separator_menu_item_new());

   ui->exitItem = gtk_menu_item_new_with_label("Exit");
   gtk_menu_append(GTK_MENU(gameMenu), ui->exitItem);
}

static void create_difficulty_menu(Ui *ui) {
   GtkWidget *difficultyMenu = gtk_menu_new();
   gtk_menu_item_set_submenu(GTK_MENU_ITEM(ui->difficultyItem), difficultyMenu);

   ui->easyItem = gtk_menu_item_new_with_label("Easy");
   gtk_object_set_data(
      GTK_OBJECT(ui->easyItem),
      "difficulty",
      GINT_TO_POINTER(DIFFICULTY_EASY)
   );
   gtk_menu_append(GTK_MENU(difficultyMenu), ui->easyItem);

   ui->mediumItem = gtk_menu_item_new_with_label("Medium");
   gtk_object_set_data(
      GTK_OBJECT(ui->mediumItem),
      "difficulty",
      GINT_TO_POINTER(DIFFICULTY_MEDIUM)
   );
   gtk_menu_append(GTK_MENU(difficultyMenu), ui->mediumItem);

   ui->hardItem = gtk_menu_item_new_with_label("Hard");
   gtk_object_set_data(
      GTK_OBJECT(ui->hardItem),
      "difficulty",
      GINT_TO_POINTER(DIFFICULTY_HARD)
   );
   gtk_menu_append(GTK_MENU(difficultyMenu), ui->hardItem);
}

static void create_information_bar(Ui *ui) {
   GtkWidget *hBox = gtk_hbox_new(FALSE, 0);
   gtk_box_pack_start(GTK_BOX(ui->mainVBox), hBox, FALSE, TRUE, 0);

   GtkWidget *nextBallsLabel = gtk_label_new("Next:");
   gtk_box_pack_start(GTK_BOX(hBox), nextBallsLabel, FALSE, TRUE, 10);

   ui->nextBallsHBox = gtk_hbox_new(TRUE, 0);
   gtk_box_pack_start(GTK_BOX(hBox), ui->nextBallsHBox, TRUE, TRUE, 10);

   ui->scoreLabel = gtk_label_new("Score: ");
   gtk_box_pack_end(GTK_BOX(hBox), ui->scoreLabel, FALSE, TRUE, 10);
}

static void destroy_next_balls(Ui *ui) {
   GList *nextBalls = gtk_container_get_children(
      GTK_CONTAINER(ui->nextBallsHBox)
   );
   for (GList *iter = nextBalls; iter != NULL; iter = iter->next) {
      gtk_widget_destroy(GTK_WIDGET(iter->data));
   }
   g_list_free(nextBalls);
}

static void destroy_game_field_table(Ui *ui) {
   if (ui->gameFieldTable != NULL)
      gtk_widget_destroy(ui->gameFieldTable);
   free(ui->gameFieldButtons);
   ui->gameFieldTable = NULL;
   ui->game_field_width = 0;
   ui->game_field_height = 0;
   ui->gameFieldButtons = NULL;
}
