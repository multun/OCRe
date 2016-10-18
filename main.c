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

int main(int argc, char *argv[])
{
  if (argc < 2)
    FAIL("use: %s image", argv[argc - 1]);
  t_color_img *img = load_bmp(argv[(argc--) - 1]);

  t_bin_sauvola_opts bin_opts = {.window=4, .k=0.3f};
  t_bw_img *bw_img = binarise(SAUVOLA, img, &bin_opts);
  t_sub_bw_img *sub = alloc_sub_bw_img(bw_img, 12, 1, 42, 42);

  // GTK PART

  GtkBuilder      *builder;
  GtkWidget       *window, *container;
  t_img_autoscale_data *autosc_data;

  gtk_init(&argc, &argv);
  window = gtk_bootstrap(&builder);

  container	= _GET_WIDGET(builder, "image_scrolled_window");


  GtkImage *gtk_img = (GtkImage*)_GET_WIDGET(builder, "image");
  autosc_data = autoscale_init(container, gtk_img, img);


  gtk_widget_show(window);
  gtk_main();

  autoscale_free(autosc_data);
  g_object_unref(builder);
  // GTK_END

  free_bw_img(bw_img);
  free_sub_bw_img(sub);
  free_color_img(img);
  return 0;
}
