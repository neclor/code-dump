/**
 * @file my_gtk_utils.с
 *
 * @brief Utility functions for GTK
*/

#include <gtk/gtk.h>

#include "my_gtk_utils.h"

/* ======= Public Functions ======= */

GtkWidget *load_image(
   const char *filename,
   unsigned int width,
   unsigned int height
) {
   if (filename == NULL || width == 0 || height == 0)
      return NULL;

   GdkPixbuf *pixbuf = gdk_pixbuf_new_from_file(filename, NULL);
   if (pixbuf == NULL)
      return NULL;

   GdkPixbuf *scaled_pixbuf = gdk_pixbuf_scale_simple(
      pixbuf, width, height, GDK_INTERP_NEAREST
   );
   g_object_unref(pixbuf);
   if (scaled_pixbuf == NULL)
      return NULL;

   GtkWidget *image = gtk_image_new_from_pixbuf(scaled_pixbuf);
   g_object_unref(scaled_pixbuf);

   return image;
}

