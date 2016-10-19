#include <gtk-3.0/gtk/gtk.h>
#include "img_history.h"
#include "img_autoscale.h"
#include "pixbuf.h"
#include "../error.h"
#include "helpers.h"
#include <stdio.h>
#include "../base_structs/vector.h"

#define THUMBNAILS_WIDTH 60

t_img_history *img_history;

void row_activated(GtkListBox    *box,
		   GtkListBoxRow *row,
		   gpointer       user_data)
{
/*  if (img_history)
  {
    int index = gtk_list_box_row_get_index(row);
    t_img_history_e *hist_e = VECT_GET(img_history->vect, (uint)index);
*/
    UNUSED(box);
    UNUSED(row);
    puts((char*)user_data);
    //}
}


t_img_history *history_init(GtkListBox *listbox)
{
  t_img_history *hist = malloc(sizeof(t_img_history));
  _GTK_CONNECT(listbox, "row-activated", row_activated, NULL);
  hist->width = THUMBNAILS_WIDTH;
  hist->vect = VECT_ALLOC(img_history_e, 16);
  hist->listbox = listbox;
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
  hist_elem->widget = wimg;
  gtk_list_box_prepend(listbox, GTK_WIDGET(wimg));
  gtk_widget_show_all(GTK_WIDGET(listbox));
  VECT_PUSH(hist->vect, hist_elem);
}
