#ifndef GTK_RENDER_H
#define GTK_RENDER_H

#include <gtk-3.0/gtk/gtk.h>
#include "../base_structs/vector.h"
#include "img_history.h"

DECL_NAMED_VECTOR(t_l_color_img*, l_color_img);
DECL_NAMED_VECTOR(t_l_bw_img*, l_bw_img);

DECL_NAMED_VECTOR(t_sub_color_img*, sub_color_img);
DECL_NAMED_VECTOR(t_sub_bw_img*, sub_bw_img);

GdkPixbuf *pixbuf_render(t_img_type type, void *img);

#endif
