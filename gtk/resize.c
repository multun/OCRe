#include "resize.h"
#include "helpers.h"
#include "../tdefs.h"
#include "../error.h"
#include "../rescale/rescale.h"

static t_resize_ui resize_ui;

static void resize_button_clicked(GtkButton *button,
				      t_img_history *hist)
{
  UNUSED(button);
  resize_ui_run(hist, hist->selected);
}

void resize_ui_init(GtkBuilder *builder, t_img_history *img_history)
{
  resize_ui.height_field = GET_SPIN(builder, "height_resize_field");
  resize_ui.width_field  = GET_SPIN(builder, "width_resize_field");

  GtkButton *resize_button = GET_BUTTON(builder, "resize_button");
  _GTK_CONNECT(resize_button, "clicked", resize_button_clicked, img_history);
}

void resize_ui_run(t_img_history *img_history, t_img_history_e *hist_e)
{
  if (hist_e->type != BW)
    return;

  uint width  = (uint)gtk_spin_button_get_value_as_int(resize_ui.width_field);
  uint height = (uint)gtk_spin_button_get_value_as_int(resize_ui.height_field);

  t_bw_img *bw_img = resize((t_bw_img*)hist_e->img, width, height);
  history_add_img(img_history, BW, (void*)bw_img, NULL);
}
