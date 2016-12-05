#include <gtk-3.0/gtk/gtk.h>
#include "img_history.h"
#include "img_autoscale.h"
#include "pixbuf.h"
#include "../error.h"
#include "helpers.h"
#include "../base_structs/vector.h"
#include "../tdefs.h"
#include "render.h"

#define THUMBNAILS_WIDTH 60

static void row_activated(GtkListBox    *box,
			  GtkListBoxRow *row,
			  t_img_history *hist)
{
  UNUSED(box);
  if(hist->click_callback)
  {
    size_t max_index = VECT_GET_SIZE(hist->vect) - 1;
    size_t vect_index = max_index - (size_t)gtk_list_box_row_get_index(row);
    t_img_history_e *hist_e = VECT_GET(hist->vect, vect_index);
    hist->selected = hist_e;
    hist->click_callback(hist, hist_e, hist->click_user_data);
  }
}

void set_history_click_callback(t_img_history *hist,
			  history_callback callback,
			  void *user_data)
{
  // TODO: add mutex
  hist->click_user_data = user_data;
  hist->click_callback = callback;
}

void set_history_add_callback(t_img_history *hist,
			      history_callback callback,
			      void *user_data)
{
  // TODO: add mutex
  hist->add_user_data = user_data;
  hist->add_callback = callback;
}

t_img_history *history_init(GtkBuilder *builder)
{
  GtkListBox *listbox = GTK_LIST_BOX(_GET_WIDGET(builder, "img_history"));
  t_img_history *hist = malloc(sizeof(t_img_history));
  hist->click_user_data = hist->add_user_data = NULL;
  hist->click_callback  = hist->add_callback  = (history_callback)NULL;
  hist->width = THUMBNAILS_WIDTH;
  hist->vect = VECT_ALLOC(img_history_e, 16);
  hist->listbox = listbox;
  hist->selected = NULL;
  _GTK_CONNECT(listbox, "row-activated", row_activated, hist);
  return hist;
}


void history_add_img(t_img_history *hist,
		     t_img_type    type,
		     void *img,
		     void *user_data)
{
  GdkPixbuf *pixbuf;
  GtkListBox *listbox = hist->listbox;

  t_img_history_e *hist_elem = malloc(sizeof(t_img_history_e));
  hist_elem->type = type;
  hist_elem->img  = img;
  hist_elem->user_data = user_data;

  hist->selected = hist_elem;

  pixbuf = pixbuf_render(type, img);
  GtkImage *wimg = GTK_IMAGE(
    gtk_image_new_from_pixbuf(
      resize_image(pixbuf, hist->width, hist->width)));
  hist_elem->pixbuf = pixbuf;
  hist_elem->widget = wimg;
  gtk_list_box_prepend(listbox, GTK_WIDGET(wimg));
  gtk_widget_show_all(GTK_WIDGET(listbox));
  VECT_PUSH(hist->vect, hist_elem);

  if (hist->add_callback)
    hist->add_callback(hist, hist_elem, hist->add_user_data);
}
