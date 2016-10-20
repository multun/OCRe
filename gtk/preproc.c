#include "preproc.h"
#include "helpers.h"
#include "../tdefs.h"
#include "../error.h"
#include "../binarisation/binarise.h"

t_preprocess_ui preprocess_ui;

static void preprocess_button_clicked(GtkButton *button,
				      t_img_history *hist)
{
  UNUSED(button);
  preprocess_ui_run(hist, hist->selected);
}

void preprocess_ui_init(GtkBuilder *builder, t_img_history *img_history)
{
  preprocess_ui.stack = GTK_STACK(_GET_WIDGET(builder, "bin_stack"));
  preprocess_ui.fixed_treshold = GTK_SPIN_BUTTON(_GET_WIDGET(
						   builder, "fixed_treshold"));
  GtkButton *proc_button = GTK_BUTTON(_GET_WIDGET(
					builder, "preprocess_button"));
  _GTK_CONNECT(proc_button, "clicked", preprocess_button_clicked, img_history);
}


void preprocess_ui_run(t_img_history *img_history, t_img_history_e *hist_e)
{
  if (hist_e->type != COLOR_IMG)
    return;

  t_bin_opts opts;
  const char *name = gtk_stack_get_visible_child_name(preprocess_ui.stack);
  t_bintype type = (t_bintype)(*name - '0');
  switch(type)
  {
  case FIXED:
    opts.fixed = (uchar)gtk_spin_button_get_value_as_int(
      preprocess_ui.fixed_treshold);
    break;
  case RATIO:
    return;
  case SAUVOLA:
    return;
  }
  t_bw_img *bw_img = binarise(type, (t_color_img*)hist_e->img, (void*)&opts);
  history_add_img(img_history, BW_IMG, (void*)bw_img);
}
