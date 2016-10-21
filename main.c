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
#include "boxing/morpho.h"
#include "boxing/bounding_box.h"
#include "gtk/img_history.h"
#include "gtk/preproc.h"

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

  uint iter = (bw_img->height + bw_img->width)/200;

  bw_img = close_morph(bw_img, iter);
  bw_img = open_morph(bw_img, iter/4);

  box new_box;
  new_box.bottom = 30;
  new_box.top  = 10;
  new_box.left = 3;
  new_box.right = 50;

  draw_box(bw_img, new_box);
  
  t_sub_bw_img *sub = alloc_sub_bw_img(bw_img, 12, 1, 42, 42);

  // GTK PART

  GtkBuilder      *builder;
  GtkWidget       *window;

  gtk_init(&argc, &argv);
  window = gtk_bootstrap(&builder);


  t_img_autoscale_data *autosc_data = autoscale_init(builder);

  gtk_widget_show(window);

  t_img_history *img_history	= history_init(builder);
  preprocess_ui_init(builder, img_history);
  history_add_img(img_history, COLOR_IMG, (void*)img);
  set_history_callback(img_history, thumbnail_clicked, autosc_data);

  gtk_main();

  autoscale_free(autosc_data);
  g_object_unref(builder);
  // GTK_END

  free_color_img(img);
  return 0;
}
