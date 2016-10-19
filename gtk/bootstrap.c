#include <gtk-3.0/gtk/gtk.h>
#include "helpers.h"

void on_window_main_destroy(void)
{
  gtk_main_quit();
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
