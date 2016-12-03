#ifndef GTK_RECOGNISE_H
#define GTK_RECOGNISE_H

#include <gtk-3.0/gtk/gtk.h>

#include "img_history.h"

void recognise_ui_init(GtkBuilder *builder, t_img_history *img_history);
void recognise_ui_run(t_img_history_e *hist_e);

#endif
