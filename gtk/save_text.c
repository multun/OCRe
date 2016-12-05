#include "preproc.h"
#include "helpers.h"
#include "../tdefs.h"
#include "../error.h"
#include "open_file.h"
#include "filechooser.h"
#include "pixbuf.h"
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>

static GtkWindow *filesaver_parent;
static GtkTextBuffer *textbox;
static GtkTextView *textview;

void save_text_ui_run(void)
{
  GtkWidget *dialog;
  GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_SAVE;
  gint res;

  dialog = gtk_file_chooser_dialog_new ("Save File",
				        filesaver_parent,
					action,
					"Cancel",
					GTK_RESPONSE_CANCEL,
					"Save",
					GTK_RESPONSE_ACCEPT,
					NULL);
  res = gtk_dialog_run (GTK_DIALOG (dialog));
  if (res == GTK_RESPONSE_ACCEPT)
  {
    GtkFileChooser *chooser = GTK_FILE_CHOOSER (dialog);
    char *filename = gtk_file_chooser_get_filename (chooser);

    GtkTextIter start, end;
    GError *err = NULL;

    gtk_widget_set_sensitive(GTK_WIDGET(textview), FALSE);
    gtk_text_buffer_get_start_iter(textbox, &start);
    gtk_text_buffer_get_end_iter(textbox, &end);
    gchar *text = gtk_text_buffer_get_text(textbox, &start, &end, FALSE);
    gtk_text_buffer_set_modified(textbox, FALSE);
    gtk_widget_set_sensitive(GTK_WIDGET(textview), TRUE);

    gboolean result;
    if (filename != NULL)
      result = g_file_set_contents (filename, text, -1, &err);
    else
      result = g_file_set_contents (filename, text, -1, &err);

    if (result == FALSE)
    {
      fprintf(stderr, "error while saving file: %s\n", err->message);
      g_error_free (err);
    }
    g_free (text);


    g_free (filename);
  }
  gtk_widget_destroy (dialog);
}

static void save_text_button_clicked(GtkButton *button,
					t_img_history *hist)
{
  UNUSED(button);
  UNUSED(hist);
  save_text_ui_run();
}



void save_text_ui_init(GtkBuilder *builder, t_img_history *img_history)
{
  GtkButton *proc_button = _GTK_GET(builder, BUTTON, "save_text_button");
  filesaver_parent	 = _GTK_GET(builder, WINDOW, "window_main");
  textbox		 = _GTK_GET(builder, TEXT_BUFFER, "result_buffer");
  textview		 = _GTK_GET(builder, TEXT_VIEW, "result_buffer_view");
  _GTK_CONNECT(proc_button, "clicked", save_text_button_clicked, img_history);
}
