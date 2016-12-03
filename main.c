#include <stdio.h>
#include <unistd.h>
#include <libgen.h>
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
#include "gtk/open_file.h"
#include "gtk/preproc.h"
#include "gtk/filter_noise.h"
#include "gtk/detect_skew.h"
#include "gtk/detect_blocks.h"
#include "gtk/detect_lines.h"
#include "gtk/detect_chars.h"
#include "gtk/resize.h"
#include "gtk/recognise.h"

void thumbnail_clicked(struct s_img_history *hist,
		       t_img_history_e *hist_e,
		       void *user_data)
{
  UNUSED(hist);
  autoscale_set_image(user_data, hist_e->pixbuf);
}

int main(int argc, char *argv[])
{
  GtkBuilder      *builder;
  GtkWidget       *window;

  WPERROR(chdir(dirname(argv[0])), "chdir");

  gtk_init(&argc, &argv);
  window = gtk_bootstrap(&builder);

  t_img_autoscale_data *autosc_data = autoscale_init(builder);

  gtk_widget_show(window);

  t_img_history *img_history	= history_init(builder);
  preprocess_ui_init(builder, img_history);
  recognise_ui_init(builder, img_history);
  resize_ui_init(builder, img_history);
  open_file_ui_init(builder, img_history);
  detect_skew_ui_init(builder, img_history);
  filter_noise_ui_init(builder, img_history);
  detect_blocks_ui_init(builder, img_history);
  detect_lines_ui_init(builder, img_history);
  detect_chars_ui_init(builder, img_history);

  set_history_add_callback(img_history, thumbnail_clicked, autosc_data);
  set_history_click_callback(img_history, thumbnail_clicked, autosc_data);

  gtk_main();

  autoscale_free(autosc_data);
  g_object_unref(builder);
  return 0;
}
