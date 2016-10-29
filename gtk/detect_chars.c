#include "preproc.h"
#include "helpers.h"
#include "../tdefs.h"
#include "../error.h"
#include "../binarisation/binarise.h"
#include "detect_blocks.h"
#include "detect_chars.h"
#include "../character_segmentation/wrapper.h"

static void detect_chars_button_clicked(GtkButton *button,
					t_img_history *hist)
{
  UNUSED(button);
  detect_chars_ui_run(hist, hist->selected);
}

void detect_chars_ui_init(GtkBuilder *builder, t_img_history *img_history)
{
  GtkButton *proc_button = GTK_BUTTON(_GET_WIDGET(
					builder, "detect_chars_button"));
  _GTK_CONNECT(proc_button, "clicked",
	       detect_chars_button_clicked, img_history);
}


void detect_chars_ui_run(t_img_history *img_history, t_img_history_e *hist_e)
{
  UNUSED(img_history);
  if (hist_e->type != SUB_BW_VECT2)
    return;

  t_sub_bw_img_vect_vect *vimg = line_segment((t_sub_bw_img_vect*)hist_e->img);
  history_add_img(img_history, SUB_BW_VECT3, (void*)vimg, (void*)vimg);
}
