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
  gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(wdg),
			      GTK_FILE_FILTER(
				gtk_builder_get_object(
				  builder,
				  "bitmap_filter")));
  return wdg;
}
