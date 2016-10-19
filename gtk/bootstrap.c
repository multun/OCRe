#include <gtk-3.0/gtk/gtk.h>
#include "helpers.h"

void on_window_main_destroy(void)
{
  gtk_main_quit();
}

GtkWidget *gtk_bootstrap(GtkBuilder **builder)
{
 *builder = gtk_builder_new();
 gtk_builder_add_from_file(*builder, "window_main.glade", NULL);
 GtkWidget *window = _GET_WIDGET(*builder, "window_main");
 printf("builder: %p\nwindow: %p\n", (void*)*builder, (void*)window);
 _GTK_CONNECT(window, "destroy", on_window_main_destroy, NULL);
 return window;
}
