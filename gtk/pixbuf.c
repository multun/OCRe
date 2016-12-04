#include <gtk/gtk.h>
#include "../bitmap/img.h"
#include "../tdefs.h"
#include "../error.h"


void pixbuf_color_img_destroy(guchar *pixels, gpointer img)
{
  (void)pixels;
  free_color_img((t_color_img*)img);
}


t_color_img *color_img_from_pixbuf(GdkPixbuf *pixbuf)
{
  gpointer row = gdk_pixbuf_get_pixels(pixbuf);
  int rowstride = gdk_pixbuf_get_rowstride(pixbuf);

  uint width  = (uint)gdk_pixbuf_get_width(pixbuf);
  uint height = (uint)gdk_pixbuf_get_height(pixbuf);

  t_color_img *img = alloc_color_img(width, height);

  for(uint y = 0; y < height; y++)
  {
    guchar *cur = (guchar*)row;
    for(uint x = 0; x < width; x++)
    {
      t_color_pix pix;
      pix.r = *(cur++);
      pix.g = *(cur++);
      pix.b = *(cur++);
      AT(img, x, y) = pix;
    }
    row = (gpointer)((guchar*)row + rowstride);
  }
  return img;
}

t_color_img *color_img_from_file(const char *path)
{
  GError *error = NULL;
  GdkPixbuf *pixbuf = gdk_pixbuf_new_from_file(path, &error);
  if (!pixbuf)
  {
    WARN("error: %s", error->message);
    return NULL;
  }
  t_color_img *img = color_img_from_pixbuf(pixbuf);
  g_object_unref(pixbuf);
  return img;
}



void refresh_pixbuf_from_color_img(GdkPixbuf *gimg, t_color_img *img)
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

void refresh_pixbuf_from_bw_img(GdkPixbuf *gimg, t_bw_img *img)
{
  gpointer row = gdk_pixbuf_get_pixels(gimg);
  int rowstride = gdk_pixbuf_get_rowstride(gimg);

  for(uint y = 0; y < img->height; y++)
  {
    guchar *cur = (guchar*)row;
    for(uint x = 0; x < img->width; x++)
    {
      t_bw_pix pix = AT(img, x, y);
      *(cur++) = pix;
      *(cur++) = pix;
      *(cur++) = pix;
    }
    row = (gpointer)((guchar*)row + rowstride);
  }
}


GdkPixbuf *alloc_pixbuf_from_color_img(t_color_img *img)
{
  return gdk_pixbuf_new(GDK_COLORSPACE_RGB,
			FALSE,
			8,
			(int)img->width,
			(int)img->height);
}

GdkPixbuf *alloc_pixbuf_from_bw_img(t_bw_img *img)
{
  return gdk_pixbuf_new(GDK_COLORSPACE_RGB,
			FALSE,
			8,
			(int)img->width,
			(int)img->height);
}
