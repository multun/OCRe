#include <gtk/gtk.h>
#include "../bitmap/img.h"
#include "../tdefs.h"

void pixbuf_color_img_destroy(guchar *pixels, gpointer img)
{
  (void)pixels;
  free_color_img((t_color_img*)img);
}

void refresh_pixbuf_from_img(GdkPixbuf *gimg, t_color_img *img)
{
  gpointer row = gdk_pixbuf_get_pixels(gimg);
  int rowstride = gdk_pixbuf_get_rowstride(gimg);

  for(uint y = 0; y < img->height; y++)
  {
    guchar *cur = (guchar*)row;
    for(uint x = 0; x < img->width; x++)
    {
      t_color_pix pix = AT(img, x, y);
      *(cur++) = pix.r;
      *(cur++) = pix.g;
      *(cur++) = pix.b;
    }
    row = (gpointer)((guchar*)row + rowstride);
  }
}

GdkPixbuf *alloc_pixbuf_from_img(t_color_img *img)
{
  return gdk_pixbuf_new(GDK_COLORSPACE_RGB,
			FALSE,
			8,
			(int)img->width,
			(int)img->height);
}
