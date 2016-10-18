#ifndef PIXBUF_H
#define PIXBUF_H

#include <gtk/gtk.h>

void pixbuf_color_img_destroy(guchar *pixels, gpointer img);
void refresh_img_from_pixbuf(GdkPixbuf *gimg, t_color_img *img);
GdkPixbuf *alloc_pixbuf_from_img(t_color_img *img);

#endif
