#include "preproc.h"
#include "helpers.h"
#include "../tdefs.h"
#include "../error.h"
#include "../binarisation/binarise.h"
#include "detect_blocks.h"
#include "../boxing/wrapper.h"

static void detect_blocks_button_clicked(GtkButton *button,
					 t_img_history *hist)
{
  UNUSED(button);
  detect_blocks_ui_run(hist, hist->selected);
}

#define GET_SPIN(builder, name) GTK_SPIN_BUTTON(_GET_WIDGET(builder, name))

void detect_blocks_ui_init(GtkBuilder *builder, t_img_history *img_history)
{
  GtkButton *proc_button = GTK_BUTTON(_GET_WIDGET(
					builder, "detect_blocks_button"));
  _GTK_CONNECT(proc_button, "clicked",
	       detect_blocks_button_clicked, img_history);
}


void detect_blocks_ui_run(t_img_history *img_history, t_img_history_e *hist_e)
{
  if (hist_e->type != BW)
    return;

  t_sub_bw_img_vect *vimg = block_segment((t_bw_img*)hist_e->img);
  history_add_img(img_history, SUB_BW_VECT, (void*)vimg, (void*)vimg);
}
