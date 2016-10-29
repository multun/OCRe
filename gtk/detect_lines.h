#ifndef GTK_LINES_H
#define GTK_LINES_H

#include <gtk-3.0/gtk/gtk.h>

#include "img_history.h"

void detect_lines_ui_init(GtkBuilder *builder, t_img_history *img_history);
void detect_lines_ui_run(t_img_history *img_history, t_img_history_e *hist_e);

#endif
