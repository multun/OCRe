#ifndef GTK_RESIZE_H
#define GTK_RESIZE_H

#include <gtk-3.0/gtk/gtk.h>

#include "img_history.h"

typedef struct s_resize_ui
{
  GtkSpinButton	*height_field;
  GtkSpinButton	*width_field;
} t_resize_ui;

void resize_ui_init(GtkBuilder *builder, t_img_history *img_history);
void resize_ui_run(t_img_history *img_history, t_img_history_e *hist_e);

#endif
