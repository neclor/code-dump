/**
 * @file my_gtk_utils.h
 *
 * @brief Utility functions for GTK
*/

#ifndef MY_GTK_UTILS_H
#define MY_GTK_UTILS_H

#include <gtk/gtk.h>

/* ======= Public Function Prototypes ======= */

/**
 * @brief Loads an image as a GTK widget
 *
 * @param filename The path to the image file
 * @param width The desired image width
 * @param height The desired image height
 *
 * @return A pointer to a GtkWidget or NULL on failure
 */
GtkWidget *load_image(
   const char *filename,
   unsigned int width,
   unsigned int height
);

#endif // MY_GTK_UTILS_H
