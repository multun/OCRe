#include <gtk-3.0/gtk/gtk.h>
#include "helpers.h"
/*
  if (gtk_dialog_run(GTK_DIALOG(wdg)) == GTK_RESPONSE_OK) {
    printf("%s", gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(wdg)));

*/
GtkWidget *create_filechooser_dialog(GtkBuilder *builder)
{
  GtkWidget *wdg = gtk_file_chooser_dialog_new("Open file", NULL,
					       GTK_FILE_CHOOSER_ACTION_OPEN,
					       "Cancel", GTK_RESPONSE_CANCEL,
					       "Open", GTK_RESPONSE_OK,
					       NULL);
  GtkWidget *filter = _GET_WIDGET(builder, "bitmap_filter");
  gtk_file_chooser_add_filter((GtkFileChooser*)wdg,
			      (GtkFileFilter*)filter);
  return wdg;
}
