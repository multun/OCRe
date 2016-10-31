#ifndef GTK_OPEN_FILE_H
#define GTK_OPEN_FILE_H

#include <gtk-3.0/gtk/gtk.h>

#include "img_history.h"

void open_file_ui_init(GtkBuilder *builder, t_img_history *img_history);
void open_file_ui_run(t_img_history *img_history);

#endif
