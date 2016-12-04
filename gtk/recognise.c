#include "preproc.h"
#include "helpers.h"
#include "../tdefs.h"
#include "../error.h"
#include "recognise.h"
#include "../boxing/wrapper.h"
#include "../recognise/recognise.h"
#include "../recognise/init.h"


static GtkTextBuffer *result_buffer;
static t_net_pool *net_pool;

static void recognise_button_clicked(GtkButton *button,
				     t_img_history *hist)
{
  UNUSED(button);
  if(hist->selected)
    recognise_ui_run(hist->selected);
}

void recognise_ui_init(GtkBuilder *builder, t_img_history *img_h)
{
  net_pool = init_recon();

  GtkButton *proc_button = GET_BUTTON(builder, "recognise_button");
  _GTK_CONNECT(proc_button, "clicked", recognise_button_clicked, img_h);
  result_buffer = _GTK_GET(builder, TEXT_BUFFER, "result_buffer");
}

void recognise_ui_run(t_img_history_e *hist_e)
{
  if (hist_e->type != BW)
    return;
  t_string *res = recognise_bw_img(net_pool, (t_bw_img*)hist_e->img);
  gtk_text_buffer_set_text (result_buffer, string_get_c(res), -1);
}
