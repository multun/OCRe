#ifndef IMG_AUTOSCALE_H
#define IMG_AUTOSCALE_H

typedef struct s_img_autoscale_data
{
  GtkImage *img;
  GdkPixbuf *pixbuf;
  GtkAllocation alloc;
} t_img_autoscale_data;

gboolean auto_resize_image(GtkWidget *container,
			   GdkEvent *event,
			   t_img_autoscale_data *img_data);


GdkPixbuf *resize_image(GdkPixbuf *orig,
		        int width, int height);

t_img_autoscale_data *autoscale_init(
  GtkWidget *container,
  GtkImage *gimg,
  GdkPixbuf *pixbuf);

void autoscale_free(t_img_autoscale_data *data);

#endif
