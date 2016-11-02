#include "preproc.h"
#include "helpers.h"
#include "../tdefs.h"
#include "../error.h"
#include "open_file.h"
#include "filechooser.h"
#include "pixbuf.h"

GtkWindow *filechooser_parent;
GtkFileFilter *filechooser_filter;

static void open_file_button_clicked(GtkButton *button,
					t_img_history *hist)
{
  UNUSED(button);
  open_file_ui_run(hist);
}



void open_file_ui_init(GtkBuilder *builder, t_img_history *img_history)
{
  GtkButton *proc_button = GTK_BUTTON(_GET_WIDGET(builder,
						  "open_file_button"));
  filechooser_parent = GTK_WINDOW(_GET_WIDGET(builder, "window_main"));
  filechooser_filter = _GTK_GET(builder, FILE_FILTER, "bitmap_filter");
  _GTK_CONNECT(proc_button, "clicked", open_file_button_clicked, img_history);
}


void open_file_ui_run(t_img_history *img_history)
{
  GtkWidget *dialog;
  GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_OPEN;
  gint res;

  dialog = gtk_file_chooser_dialog_new ("Open File",
				        filechooser_parent,
					action,
					"Cancel",
					GTK_RESPONSE_CANCEL,
					"Open",
					GTK_RESPONSE_ACCEPT,
					NULL);
  GtkFileChooser *fchooser = GTK_FILE_CHOOSER(dialog);
  gtk_file_chooser_set_filter(fchooser, filechooser_filter);
  res = gtk_dialog_run (GTK_DIALOG (dialog));
  if (res == GTK_RESPONSE_ACCEPT)
  {
    GtkFileChooser *chooser = GTK_FILE_CHOOSER (dialog);
    char *filename = gtk_file_chooser_get_filename (chooser);
    history_add_img(img_history, COLOR,
		    (void*)color_img_from_file(filename), NULL);
    g_free (filename);
  }
  gtk_widget_destroy (dialog);
}
