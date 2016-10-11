#include <gtk/gtk.h>
#include "../bitmap/img.h"

void pixbuf_color_img_destroy(guchar *pixels, gpointer img)
{
  (void)pixels;
  free_color_img((t_color_img*)img);
}

GdkPixbuf *pixbuf_from_img(t_color_img *img)
{
  return gdk_pixbuf_new_from_data ((guchar*)img->pixels,
				   GDK_COLORSPACE_RGB,
				   FALSE,
				   8,
				   (int)img->width,
				   (int)img->height,
				   (int)img->width,
				   pixbuf_color_img_destroy,
				   (gpointer)img);
}
