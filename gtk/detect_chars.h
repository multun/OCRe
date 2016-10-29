#ifndef GTK_CHARS_H
#define GTK_CHARS_H

#include <gtk-3.0/gtk/gtk.h>

#include "img_history.h"

void detect_chars_ui_init(GtkBuilder *builder, t_img_history *img_history);
void detect_chars_ui_run(t_img_history *img_history, t_img_history_e *hist_e);

#endif
