#include <gtk-3.0/gtk/gtk.h>
#include "helpers.h"
#include "img_history.h"

#include "open_file.h"
#include "preproc.h"
#include "filter_noise.h"
#include "detect_skew.h"
#include "detect_blocks.h"
#include "detect_lines.h"
#include "detect_chars.h"
#include "resize.h"
#include "recognise.h"
#include "../boxing/morpho.h"
#include "../boxing/bounding_box.h"


void on_window_main_destroy(void)
{
  gtk_main_quit();
}

void ui_init(GtkBuilder *builder, t_img_history *img_history)
{
  preprocess_ui_init(builder, img_history);
  recognise_ui_init(builder, img_history);
  resize_ui_init(builder, img_history);
  open_file_ui_init(builder, img_history);
  detect_skew_ui_init(builder, img_history);
  filter_noise_ui_init(builder, img_history);
  detect_blocks_ui_init(builder, img_history);
  detect_lines_ui_init(builder, img_history);
  detect_chars_ui_init(builder, img_history);
}


GtkWidget *gtk_bootstrap(GtkBuilder **builder)
{
  *builder = gtk_builder_new();
  GError *p_err = NULL;
  gtk_builder_add_from_file(*builder, "window_main.glade", &p_err);
  if (p_err != NULL)
  {
    g_error ("%s", p_err->message);
    g_error_free (p_err);
  }
  GtkWidget *window = _GET_WIDGET(*builder, "window_main");
  _GTK_CONNECT(window, "destroy", on_window_main_destroy, NULL);
  return window;
}
