#ifndef GTK_BOOTSTRAP_H
#define GTK_BOOTSTRAP_H

#include <gtk-3.0/gtk/gtk.h>

void on_window_main_destroy(void);
GtkWidget *gtk_bootstrap(GtkBuilder **builder);

#endif
