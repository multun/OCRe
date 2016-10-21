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
#include "gtk/preproc.h"
#include "test_l_imgs.h"

void thumbnail_clicked(struct s_img_history *hist,
		       t_img_history_e *hist_e,
		       void *user_data)
{
  UNUSED(hist);
  autoscale_set_image(user_data, hist_e->pixbuf);
}

int main(int argc, char *argv[])
{
  if (argc < 2)
    FAIL("use: %s image", argv[argc - 1]);
  t_color_img *img = color_img_from_file(argv[(argc--) - 1]);

  // GTK PART

  GtkBuilder      *builder;
  GtkWidget       *window;

  gtk_init(&argc, &argv);
  window = gtk_bootstrap(&builder);


  t_img_autoscale_data *autosc_data = autoscale_init(builder);

  gtk_widget_show(window);

  t_img_history *img_history	= history_init(builder);
  preprocess_ui_init(builder, img_history);

  set_history_add_callback(img_history, thumbnail_clicked, autosc_data);
  set_history_click_callback(img_history, thumbnail_clicked, autosc_data);

  history_add_img(img_history, COLOR, (void*)img, NULL);
  history_add_img(img_history, L_COLOR_VECTOR, test_block_segmentation(img), NULL);

  gtk_main();

  autoscale_free(autosc_data);
  g_object_unref(builder);
  // GTK_END

  free_color_img(img);
  return 0;
}
