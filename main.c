#include <stdio.h>
#include <gtk-3.0/gtk/gtk.h>
#include "bitmap/img.h"
#include "bitmap/bmp.h"
#include "binarisation/binarise.h"
#include "memtools.h"
#include "error.h"
#include "filtering.h"
#include "binarisation/bin_sauvola.h"
#include "gtk/img_autoscale.h"
#include "gtk/bootstrap.h"
#include "gtk/helpers.h"
#include "gtk/pixbuf.h"
#include "gtk/img_history.h"

void thumbnail_clicked(struct s_img_history *hist, t_img_history_e *hist_e)
{
  autoscale_set_image(hist->user_data, hist_e->pixbuf);
}


int main(int argc, char *argv[])
{
  if (argc < 2)
    FAIL("use: %s image", argv[argc - 1]);
  t_color_img *img = color_img_from_file(argv[(argc--) - 1]);

  t_bin_sauvola_opts bin_opts = {.window=5, .k=0.05f};
  t_bw_img *bw_img = binarise(SAUVOLA, img, &bin_opts);
  t_sub_bw_img *sub = alloc_sub_bw_img(bw_img, 12, 1, 42, 42);

  // GTK PART

  GtkBuilder      *builder;
  GtkWidget       *window, *container;
  t_img_autoscale_data *autosc_data;

  gtk_init(&argc, &argv);
  window = gtk_bootstrap(&builder);



  GtkImage *gtk_img = (GtkImage*)_GET_WIDGET(builder, "image");
  container	= _GET_WIDGET(builder, "image_scrolled_window");

  GdkPixbuf *pixbuf = alloc_pixbuf_from_bw_img(bw_img);
  refresh_pixbuf_from_bw_img(pixbuf, bw_img);
  autosc_data = autoscale_init(container, gtk_img, pixbuf);


  gtk_widget_show(window);

  GtkWidget *img_h = _GET_WIDGET(builder, "img_history");
  t_img_history *img_history	= history_init(GTK_LIST_BOX(img_h));
  history_add_img(img_history, COLOR_IMG, (void*)img);
  history_add_img(img_history, BW_IMG, (void*)bw_img);
  set_history_callback(img_history,
		       thumbnail_clicked,
		       autosc_data);

  gtk_main();

  autoscale_free(autosc_data);
  g_object_unref(builder);
  // GTK_END

  free_bw_img(bw_img);
  free_sub_bw_img(sub);
  free_color_img(img);
  return 0;
}
