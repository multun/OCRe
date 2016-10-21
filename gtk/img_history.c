#include <gtk-3.0/gtk/gtk.h>
#include "img_history.h"
#include "img_autoscale.h"
#include "pixbuf.h"
#include "../error.h"
#include "helpers.h"
#include "../base_structs/vector.h"
#include "img_history.h"
#include "../tdefs.h"

#define THUMBNAILS_WIDTH 60

static void row_activated(GtkListBox    *box,
			  GtkListBoxRow *row,
			  t_img_history *hist)
{
  UNUSED(box);
  if(hist->callback)
  {
    size_t max_index = VECT_GET_SIZE(hist->vect) - 1;
    size_t vect_index = max_index - (size_t)gtk_list_box_row_get_index(row);
    t_img_history_e *hist_e = VECT_GET(hist->vect, vect_index);
    hist->selected = hist_e;
    hist->callback(hist, hist_e);
  }
}

void set_history_callback(t_img_history *hist,
			  history_callback callback,
			  void *user_data)
{
  // TODO: add mutex
  hist->user_data = user_data;
  hist->callback = callback;
}

t_img_history *history_init(GtkBuilder *builder)
{
  GtkListBox *listbox = GTK_LIST_BOX(_GET_WIDGET(builder, "img_history"));
  t_img_history *hist = malloc(sizeof(t_img_history));
  hist->user_data = NULL;
  hist->callback = (history_callback)NULL;
  hist->width = THUMBNAILS_WIDTH;
  hist->vect = VECT_ALLOC(img_history_e, 16);
  hist->listbox = listbox;
  hist->selected = NULL;
  _GTK_CONNECT(listbox, "row-activated", row_activated, hist);
  return hist;
}


void history_add_img(t_img_history *hist,
		     t_img_type    type,
		     void *img)
{
  GdkPixbuf *pixbuf;
  GtkListBox *listbox = hist->listbox;

  t_img_history_e *hist_elem = malloc(sizeof(t_img_history_e));
  hist_elem->type = type;
  hist_elem->img  = img;

  if (hist->selected == NULL)
    hist->selected = hist_elem;

  switch(type)
  {
  case COLOR_IMG:
    pixbuf = alloc_pixbuf_from_color_img((t_color_img*)img);
    refresh_pixbuf_from_color_img(pixbuf, (t_color_img*)img);
    break;
  case BW_IMG:
    pixbuf = alloc_pixbuf_from_bw_img((t_bw_img*)img);
    refresh_pixbuf_from_bw_img(pixbuf, (t_bw_img*)img);
    break;
  }
  GtkImage *wimg = GTK_IMAGE(
    gtk_image_new_from_pixbuf(
      resize_image(pixbuf, hist->width, hist->width)));
  hist_elem->pixbuf = pixbuf;
  hist_elem->widget = wimg;
  gtk_list_box_prepend(listbox, GTK_WIDGET(wimg));
  gtk_widget_show_all(GTK_WIDGET(listbox));
  VECT_PUSH(hist->vect, hist_elem);
}
