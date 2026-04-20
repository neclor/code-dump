/**
 * @file ui.c
 * @brief Implementation of the user interface.
 *
 * @date 29.03.2025
 * @author Aleksandr Pavlov (s2400691)
*/

#include <stdbool.h>
#include <gdk-pixbuf/gdk-pixbuf.h>
#include <gtk/gtk.h>

#include "game.h"
#include "ui.h"

/* ======= Constants ======= */

static const unsigned int WINDOW_WIDTH = 400;
static const unsigned int WINDOW_HEIGHT = 200;

static const unsigned int IMAGE_WIDTH = 100;
static const unsigned int IMAGE_HEIGHT = 100;

static const char *CLOSED_CHEST = "assets/images/closed_chest.png";
static const char *EMPTY_CHEST = "assets/images/empty_chest.png";
static const char *TREASURE_CHEST = "assets/images/treasure_chest.png";

/* ======= Variables ======= */

static GtkWidget *wins_label;
static GtkWidget *losses_label;
static GtkWidget *status_label;
static GtkWidget *chest_buttons[CHESTS_NUMBER];

/* ======= Internal Function Prototypes ======= */

/**
 * @brief Updates the UI when the game begins.
 *
 * @post Update status label, reset chest buttons.
 */
static void game_begun(void);

/**
 * @brief Updates the UI when a chest is opened.
 *
 * @param chest_index Index of the chest that was opened.
 * @param treasured if treasure chest, true, else false.
 *
 * @pre 0 < chest_index < CHESTS_NUMBER.
 * @post Update chest button.
 */
static void chest_opened(int chest_index, bool treasured);

/**
 * @brief Updates the UI when the game ends.
 *
 * @param win if the player wins, true, else false.
 *
 * @post Update status label, disable all chest buttons.
 */
static void game_over(bool win);

/**
 * @brief Update wins label.
 */
static void update_wins_label(void);

/**
 * @brief Update losses label.
 */
static void update_losses_label(void);

/**
 * @brief Update status label with text.
 *
 * @param text The text to display in the status label.
 */
static void status_label_set_text(const char *text);

/**
 * @brief Callback invoked when the restart button is clicked.
 *
 * @param button UNUSED
 * @param data UNUSED
 *
 * @post Restart game.
 */
static void on_restart_button_clicked(
   GtkWidget *button G_GNUC_UNUSED, gpointer data G_GNUC_UNUSED
);

/**
 * @brief Callback invoked when a chest button is clicked.
 *
 * @param button UNUSED
 * @param data UNUSED
 */
static void on_chest_button_clicked(
   GtkWidget *button G_GNUC_UNUSED, gpointer data G_GNUC_UNUSED
);

/**
 * @brief Initializes the main game window.
 *
 * @return Pointer to GTK window
 */
static GtkWidget *init_window(void);

/**
 * @brief Initializes the menu.
 *
 * @param window Pointer to GTK window
 */
static void init_menu(GtkWidget *window);

/**
 * @brief Sets an image on a button.
 *
 * @param button Pointer to GTK button
 * @param image_path The file path to the image
 *
 * @pre button is a valid GTK button, image_path is a valid image file.
 */
static void button_set_image(GtkWidget *button, const char *image_path);

/**
 * @brief Handles the window close event.
 *
 * @param window Pointer to GTK window
 * @param data UNUSED
 *
 * @post Ends the program, destroys window.
 */
static void on_window_close(
   GtkWidget *window, gpointer data G_GNUC_UNUSED
);

/* ======= External Functions ======= */

void init_ui(void) {
   GtkWidget *window = init_window();
   init_menu(window);

   on_game_begun = game_begun;
   on_chest_opened = chest_opened;
   on_game_over = game_over;

   gtk_widget_show_all(window);
}

/* ======= Internal functions ======= */

static void game_begun() {
   status_label_set_text("Select chest");

   for (unsigned int i = 0; i < CHESTS_NUMBER; i++) {
      button_set_image(chest_buttons[i], CLOSED_CHEST);
      gtk_widget_set_sensitive(chest_buttons[i], TRUE);
   }
}

static void chest_opened(int chest_index, bool treasured) {
   status_label_set_text("Select chest");

   const char *image = treasured ? TREASURE_CHEST : EMPTY_CHEST;
   button_set_image(chest_buttons[chest_index], image);

   gtk_widget_set_sensitive(chest_buttons[chest_index], FALSE);
}

static void game_over(bool win) {
   update_wins_label();
   update_losses_label();

   status_label_set_text(win ? "Victory" : "Defeat");

   for (unsigned int i = 0; i < CHESTS_NUMBER; i++) {
      gtk_widget_set_sensitive(chest_buttons[i], FALSE);
   }
}

static void update_wins_label(void) {
   char text[100];
   sprintf(text, "Wins: %u", get_wins());
   gtk_label_set_text(GTK_LABEL(wins_label), text);
}

static void update_losses_label(void) {
   char text[100];
   sprintf(text, "Losses: %u", get_losses());
   gtk_label_set_text(GTK_LABEL(losses_label), text);
}

static void status_label_set_text(const char *text) {
   gtk_label_set_text(GTK_LABEL(status_label), text);
}

static void on_restart_button_clicked(
   GtkWidget *button G_GNUC_UNUSED, gpointer data G_GNUC_UNUSED
) {
   start_game();
}

static void on_chest_button_clicked(
   GtkWidget *button G_GNUC_UNUSED, gpointer data G_GNUC_UNUSED
) {
   unsigned int chest_index = GPOINTER_TO_UINT(data);
   select_chest(chest_index);
}

static GtkWidget *init_window(void) {
   GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
   gtk_window_set_title(
      GTK_WINDOW(window), "Treasure Hunt or Monty Hall Problem"
   );
   gtk_window_set_default_size(GTK_WINDOW(window), WINDOW_WIDTH, WINDOW_HEIGHT);
   g_signal_connect(window, "destroy", G_CALLBACK(on_window_close), NULL);
   return window;
}

static void init_menu(GtkWidget *window) {
   GtkWidget *vbox = gtk_vbox_new(FALSE, 0);
   gtk_container_add(GTK_CONTAINER(window), vbox);

   GtkWidget *hbox_1 = gtk_hbox_new(FALSE, 0);
   gtk_box_pack_start(GTK_BOX(vbox), hbox_1, FALSE, FALSE, 0);

   wins_label = gtk_label_new("");
   gtk_box_pack_start(GTK_BOX(hbox_1), wins_label, TRUE, FALSE, 0);
   update_wins_label();

   losses_label = gtk_label_new("");
   gtk_box_pack_start(GTK_BOX(hbox_1), losses_label, TRUE, FALSE, 0);
   update_losses_label();

   status_label = gtk_label_new("");
   gtk_box_pack_start(GTK_BOX(vbox), status_label, FALSE, FALSE, 0);

   GtkWidget *restart_button = gtk_button_new_with_label("Restart");
   gtk_box_pack_start(GTK_BOX(vbox), restart_button, FALSE, FALSE, 0);
   g_signal_connect(restart_button, "clicked",
      G_CALLBACK(on_restart_button_clicked), NULL
   );

   GtkWidget *hbox_2 = gtk_hbox_new(FALSE, 0);
   gtk_box_pack_end(GTK_BOX(vbox), hbox_2, TRUE, FALSE, 0);

   for (unsigned int i = 0; i < CHESTS_NUMBER; i++) {
      chest_buttons[i] = gtk_button_new();
      gtk_box_pack_start(GTK_BOX(hbox_2), chest_buttons[i], TRUE, FALSE, 0);
      g_signal_connect(chest_buttons[i], "clicked",
         G_CALLBACK(on_chest_button_clicked), GUINT_TO_POINTER(i)
      );
   }
}

static void button_set_image(GtkWidget *button, const char *image_path) {
   GdkPixbuf *pixbuf = gdk_pixbuf_new_from_file(image_path, NULL);
   if (pixbuf == NULL) {
      fprintf(stderr, "Failed to load image: '%s'\n", image_path);
   }

   GdkPixbuf *scaled_pixbuf = gdk_pixbuf_scale_simple(
      pixbuf, IMAGE_WIDTH, IMAGE_HEIGHT, GDK_INTERP_NEAREST
   );
   if (scaled_pixbuf == NULL) {
      fprintf(stderr, "Failed to create image: '%s'\n", image_path);
   }

   GtkWidget *image = gtk_image_new_from_pixbuf(scaled_pixbuf);
   if (image == NULL) {
      fprintf(stderr, "Failed to create image: '%s'\n", image_path);
   }

   g_object_unref(pixbuf);
   g_object_unref(scaled_pixbuf);

   gtk_button_set_image(GTK_BUTTON(button), image);
}

static void on_window_close(
   GtkWidget *window, gpointer data G_GNUC_UNUSED
) {
   gtk_widget_destroy(window);
   gtk_main_quit();
}
