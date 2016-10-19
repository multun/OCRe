#ifndef GTK_IMG_HISTORY_H
#define GTK_IMG_HISTORY_H

#include <gtk-3.0/gtk/gtk.h>
#include "../base_structs/vector.h"
#include "../bitmap/img.h"

typedef enum e_img_type
{
  COLOR_IMG,
  BW_IMG
} t_img_type;

typedef struct s_img_history_e
{
  t_img_type	type;
  void		*img;
  GdkPixbuf	*pixbuf;
  GtkImage	*widget;
} t_img_history_e;

DECL_NAMED_VECTOR(t_img_history_e*, img_history_e);

typedef struct s_img_history
{
  t_img_history_e_vect	*vect;
  GtkListBox		*listbox;
  int			width;
} t_img_history;

t_img_history *history_init(GtkListBox *listbox);
void history_add_img(t_img_history *hist,
		     t_img_type    type,
		     void *img);

extern t_img_history *img_history;

#endif
