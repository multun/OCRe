#ifndef PIXBUF_H
#define PIXBUF_H

#include <gtk/gtk.h>

void pixbuf_color_img_destroy(guchar *pixels, gpointer img);
void refresh_pixbuf_from_color_img(GdkPixbuf *gimg, t_color_img *img);
void refresh_pixbuf_from_bw_img(GdkPixbuf *gimg, t_bw_img *img);
GdkPixbuf *alloc_pixbuf_from_color_img(t_color_img *img);
GdkPixbuf *alloc_pixbuf_from_bw_img(t_bw_img *img);

t_color_img *color_img_from_pixbuf(GdkPixbuf *pixbuf);
t_color_img *color_img_from_file(const char *path);

#endif
