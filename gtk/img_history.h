#ifndef GTK_IMG_HISTORY_H
#define GTK_IMG_HISTORY_H

#include <gtk-3.0/gtk/gtk.h>
#include "../base_structs/vector.h"
#include "../bitmap/img.h"

typedef enum e_img_type
{
  COLOR = 0,
  L_COLOR_VECTOR = 1,
  BW = 2,
  L_BW_VECTOR = 3,
} t_img_type;

typedef struct s_img_history_e
{
  t_img_type	type;
  void		*img;
  GdkPixbuf	*pixbuf;
  GtkImage	*widget;
  void		*user_data;
} t_img_history_e;

DECL_NAMED_VECTOR(t_img_history_e*, img_history_e);

typedef struct s_img_history
{
  t_img_history_e_vect	*vect;
  t_img_history_e	*selected;
  GtkListBox		*listbox;
  int			width;
  void			(*click_callback)(struct s_img_history*,
					  t_img_history_e*,
					  void*);
  void			*click_user_data;
  void			(*add_callback)(struct s_img_history*,
				        t_img_history_e*,
					void*);
  void			*add_user_data;
} t_img_history;

// forced to repeat, declaration order conflict
typedef void (*history_callback)(t_img_history  *,
				 t_img_history_e*,
				 void*);

t_img_history *history_init(GtkBuilder*);
void history_add_img(t_img_history *hist,
		     t_img_type    type,
		     void *img,
		     void *user_data);

void set_history_click_callback(t_img_history *img_history,
				history_callback callback,
				void *user_data);
void set_history_add_callback(t_img_history *img_history,
			      history_callback callback,
			      void *user_data);

#endif
