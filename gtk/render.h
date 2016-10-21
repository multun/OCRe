#ifndef GTK_RENDER_H
#define GTK_RENDER_H

#include <gtk-3.0/gtk/gtk.h>
#include "img_history.h"


GdkPixbuf *pixbuf_render(t_img_type type, void *img);

#endif
