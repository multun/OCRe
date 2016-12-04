#include "preproc.h"
#include "helpers.h"
#include "../tdefs.h"
#include "../error.h"
#include "detect_lines.h"
#include "../block_to_line/wrapper.h"

static void detect_lines_button_clicked(GtkButton *button,
					 t_img_history *hist)
{
  UNUSED(button);

  if(hist->selected)
    detect_lines_ui_run(hist, hist->selected);
}

#define GET_SPIN(builder, name) GTK_SPIN_BUTTON(_GET_WIDGET(builder, name))

void detect_lines_ui_init(GtkBuilder *builder, t_img_history *img_history)
{
  GtkButton *proc_button = GTK_BUTTON(_GET_WIDGET(
					builder, "detect_lines_button"));
  _GTK_CONNECT(proc_button, "clicked",
	       detect_lines_button_clicked, img_history);
}


void detect_lines_ui_run(t_img_history *img_history, t_img_history_e *hist_e)
{
  if (!hist_e || hist_e->type != SUB_BW_VECT)
    return;

  t_sub_bw_img_vect* blocks = (t_sub_bw_img_vect*)hist_e->img;
  t_sub_bw_img_vect_vect *vimg = line_extraction(blocks);
  history_add_img(img_history, SUB_BW_VECT2, (void*)vimg, (void*)vimg);
}
