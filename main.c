#include <stdio.h>
#include <unistd.h>
#include <libgen.h>
#include <gtk-3.0/gtk/gtk.h>
#include "bitmap/img.h"
#include "error.h"
#include "gtk/img_autoscale.h"
#include "gtk/bootstrap.h"
#include "gtk/img_history.h"

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
  ui_init(builder, img_history);

  set_history_add_callback(img_history, thumbnail_clicked, autosc_data);
  set_history_click_callback(img_history, thumbnail_clicked, autosc_data);

  gtk_main();

  autoscale_free(autosc_data);
  g_object_unref(builder);
  return 0;
}
