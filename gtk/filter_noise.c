#include "preproc.h"
#include "helpers.h"
#include "../tdefs.h"
#include "../error.h"
#include "detect_blocks.h"
#include "filter_noise.h"
#include "../noise_reduction/wrapper.h"


static void filter_noise_button_clicked(GtkButton *button,
					 t_img_history *hist)
{
  UNUSED(button);

  if(hist->selected)
    filter_noise_ui_run(hist, hist->selected);
}

void filter_noise_ui_init(GtkBuilder *builder, t_img_history *img_hist)
{
  GtkButton *proc_button = GET_BUTTON(builder, "filter_noise_button");
  _GTK_CONNECT(proc_button, "clicked", filter_noise_button_clicked, img_hist);
}


void filter_noise_ui_run(t_img_history *img_history, t_img_history_e *hist_e)
{
  if (!hist_e || hist_e->type != BW)
    return;

  t_bw_img *vimg = noise_red((t_bw_img*)hist_e->img);
  history_add_img(img_history, BW, (void*)vimg, (void*)vimg);
}
