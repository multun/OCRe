
#include <gtk-3.0/gtk/gtk.h>
#include "../error.h"
#include "../bitmap/img.h"
#include "pixbuf.h"
#include "img_autoscale.h"
#include "../math_macros.h"
#include "helpers.h"

static void get_alloc(GtkWidget *container, GtkAllocation *alloc)
{
  gtk_widget_get_allocation(gtk_bin_get_child(GTK_BIN(container)), alloc);
}

GdkPixbuf *resize_image(GdkPixbuf *orig,
		        int width, int height)
{
  int imgw = gdk_pixbuf_get_width(orig);
  int imgh = gdk_pixbuf_get_height(orig);

  if (width < 2 || height < 2)
  {
    height = imgh;
    width = imgw;
  }

  double hscale = (double)width / imgw;
  double vscale = (double)height / imgh;
  double scale = MIN(1.0, MIN(hscale, vscale));

  return gdk_pixbuf_scale_simple(
    orig,
    (int)((double)imgw * scale),
    (int)((double)imgh * scale),
    GDK_INTERP_BILINEAR);
}


void autoscale_free(t_img_autoscale_data *data)
{
  g_object_unref(data->pixbuf);
  g_object_unref(gtk_image_get_pixbuf(data->img));
  free(data); // UNCHECKED
}


t_img_autoscale_data *autoscale_init(
  GtkWidget *container,
  GtkImage *gtk_img,
  GdkPixbuf *pixbuf)
{
  t_img_autoscale_data *data = malloc(sizeof(t_img_autoscale_data));
  data->img    = gtk_img;
  data->pixbuf = pixbuf;
  data->container = container;

  get_alloc(container, &data->alloc);


  GdkPixbuf *npixbuf = resize_image(pixbuf,
				    data->alloc.width,
				    data->alloc.height);

  gtk_image_set_from_pixbuf(gtk_img, npixbuf);
  gtk_widget_queue_draw(container);

  _GTK_CONNECT(container, "size-allocate", auto_resize_image, data);
  return data;
}


void autoscale_set_image(t_img_autoscale_data *img_data, GdkPixbuf *nbuf)
{
  img_data->pixbuf = nbuf;
  GdkPixbuf *oldbuf = gtk_image_get_pixbuf(img_data->img);
  get_alloc(img_data->container, &img_data->alloc);
  GdkPixbuf *sized_pixbuf = resize_image(nbuf,
					 img_data->alloc.width,
					 img_data->alloc.height);

  gtk_image_set_from_pixbuf(img_data->img, sized_pixbuf);
  g_object_unref(oldbuf);

  gtk_widget_queue_draw(img_data->container);
}


gboolean auto_resize_image(GtkWidget *container,
			   GdkEvent *event,
			   t_img_autoscale_data *img_data)
{
  UNUSED(event);
  GdkPixbuf *orig_buf = img_data->pixbuf;

  GtkAllocation alloc;
  get_alloc(container, &alloc);
  if (alloc.width != img_data->alloc.width
      || alloc.height != img_data->alloc.height)
  {
    GdkPixbuf *oldbuf = gtk_image_get_pixbuf(img_data->img);
    GdkPixbuf *new_buf = resize_image(orig_buf,
				      alloc.width,
				      alloc.height);
    gtk_image_set_from_pixbuf(img_data->img, new_buf);
    g_object_unref(oldbuf);
  }

  img_data->alloc = alloc;
  gtk_widget_queue_draw(container);
  return FALSE;
}
