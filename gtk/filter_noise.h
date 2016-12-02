#ifndef GTK_FILTER_H
#define GTK_FILTER_H

#include <gtk-3.0/gtk/gtk.h>

#include "img_history.h"

void filter_noise_ui_init(GtkBuilder *builder, t_img_history *img_history);
void filter_noise_ui_run(t_img_history *img_history, t_img_history_e *hist_e);

#endif
