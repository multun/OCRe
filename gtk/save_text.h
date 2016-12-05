#ifndef SAVE_TEXT_H
#define SAVE_TEXT_H

#include <gtk-3.0/gtk/gtk.h>

#include "img_history.h"

void save_text_ui_init(GtkBuilder *builder, t_img_history *img_history);
void save_text_ui_run(t_img_history *img_history, t_img_history_e *hist_e);

#endif
