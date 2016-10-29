#include <gtk-3.0/gtk/gtk.h>
#include "pixbuf.h"
#include "../tdefs.h"
#include "img_history.h"
#include "render.h"

// snippet from the gdkpixbuf documentation
static void put_red(GdkPixbuf *pixbuf, size_t ux, size_t uy)
{
  int width, height, rowstride, n_channels;
  guchar *pixels, *p;
  int x = (int)ux;
  int y = (int)uy;

  n_channels = gdk_pixbuf_get_n_channels (pixbuf);
  width = gdk_pixbuf_get_width (pixbuf);
  height = gdk_pixbuf_get_height (pixbuf);

  g_assert (x >= 0 && x < width);
  g_assert (y >= 0 && y < height);

  rowstride = gdk_pixbuf_get_rowstride (pixbuf);
  pixels = gdk_pixbuf_get_pixels (pixbuf);

  p = pixels + y * rowstride + x * n_channels;
  p[0] = 255;
  p[1] = 0;
  p[2] = 0;
  p[3] = 0;
}

#define RENDER_SUB_BODY							\
  {									\
    for(size_t y = 0; y < img->height; y++)				\
    {									\
      put_red(pixbuf, img->xoff		 , img->yoff + y);		\
      put_red(pixbuf, img->xoff + img->width - 1, img->yoff + y);	\
    }									\
    for(size_t x = 0; x < img->width; x++)				\
    {									\
      put_red(pixbuf, img->xoff + x, img->yoff);			\
      put_red(pixbuf, img->xoff + x, img->yoff + img->height - 1);	\
    }									\
  }


#define DEFINE_RENDER_L_IMG(TYPE) \
  static void render_l_ ## TYPE ## _img(GdkPixbuf *pixbuf,		\
					t_l_ ## TYPE ## _img *img)	\
  RENDER_SUB_BODY

DEFINE_RENDER_L_IMG(color)
DEFINE_RENDER_L_IMG(bw)

#define DEFINE_RENDER_SUB_IMG(TYPE)					\
  static void render_sub_ ## TYPE ## _img(GdkPixbuf *pixbuf,		\
					  t_sub_ ## TYPE ## _img *img)	\
  RENDER_SUB_BODY

DEFINE_RENDER_SUB_IMG(color)
DEFINE_RENDER_SUB_IMG(bw)

GdkPixbuf *pixbuf_render(t_img_type type, void *img)
{
  GdkPixbuf *pixbuf;
  union
  {
    t_l_color_img_vect		*color;
    t_l_bw_img_vect		*bw;
    t_sub_color_img_vect	*scolor;
    t_sub_bw_img_vect		*sbw;
    t_sub_bw_img_vect_vect	*sbwv;
  } vimg;

  switch(type)
  {
  case COLOR:
    pixbuf = alloc_pixbuf_from_color_img((t_color_img*)img);
    refresh_pixbuf_from_color_img(pixbuf, (t_color_img*)img);
    break;
  case L_COLOR_VECT:
    vimg.color = (t_l_color_img_vect*)img;
    pixbuf = pixbuf_render(COLOR, VECT_GET(vimg.color, 0)->father);
    for(size_t i = 0; i < VECT_GET_SIZE(vimg.color); i++)
      render_l_color_img(pixbuf, VECT_GET(vimg.color, i));
    break;
  case SUB_COLOR_VECT:
    vimg.scolor = (t_sub_color_img_vect*)img;
    pixbuf = pixbuf_render(COLOR, VECT_GET(vimg.scolor, 0)->father);
    for(size_t i = 0; i < VECT_GET_SIZE(vimg.scolor); i++)
      render_sub_color_img(pixbuf, VECT_GET(vimg.scolor, i));
    break;
  case BW:
    pixbuf = alloc_pixbuf_from_bw_img((t_bw_img*)img);
    refresh_pixbuf_from_bw_img(pixbuf, (t_bw_img*)img);
    break;
  case L_BW_VECT:
    vimg.bw = (t_l_bw_img_vect*)img;
    pixbuf = pixbuf_render(BW, VECT_GET(vimg.bw, 0)->father);
    for(size_t i = 0; i < VECT_GET_SIZE(vimg.bw); i++)
      render_l_bw_img(pixbuf, VECT_GET(vimg.bw, i));
    break;
  case SUB_BW_VECT:
    vimg.sbw = (t_sub_bw_img_vect*)img;
    pixbuf = pixbuf_render(BW, VECT_GET(vimg.sbw, 0)->father);
    for(size_t i = 0; i < VECT_GET_SIZE(vimg.sbw); i++)
      render_sub_bw_img(pixbuf, VECT_GET(vimg.sbw, i));
    break;
  case SUB_BW_VECT_VECT:
    vimg.sbwv = (t_sub_bw_img_vect_vect*)img;
    pixbuf = pixbuf_render(BW, VECT_GET(VECT_GET(vimg.sbwv, 0),0)->father);
    for(size_t i = 0; i < VECT_GET_SIZE(vimg.sbwv); i++)
    {
      t_sub_bw_img_vect *cv = VECT_GET(vimg.sbwv, i);
      for(size_t j = 0; j < VECT_GET_SIZE(cv); j++)
	render_sub_bw_img(pixbuf, VECT_GET(cv, j));
    }
    break;
  }
  return pixbuf;
}
