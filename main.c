#include <stdio.h>
#include <gtk-3.0/gtk/gtk.h>
#include "bitmap/img.h"
#include "bitmap/bmp.h"
#include "binarisation/binarise.h"
#include "memtools.h"
#include "error.h"
#include "filtering.h"
#include "gtk/pixbuf.h"


// called when window is closed
void on_window_main_destroy(void)
{
  puts("main_quit");
  gtk_main_quit();
}


int main(int argc, char *argv[])
{
  if (argc < 2)
    FAIL("use: %s image", argv[(argc--) - 1]);
  t_color_img *img = load_bmp(argv[1]);

  t_bw_img *bw_img = binarise(SAUVOLA, img);
  t_sub_bw_img *sub = alloc_sub_bw_img(bw_img, 12, 1, 42, 42);

  // GTK PART

  GtkBuilder      *builder;
  GtkWidget       *window;

  gtk_init(&argc, &argv);

  builder = gtk_builder_new();
  gtk_builder_add_from_file (builder, "window_main.glade", NULL);

  window = GTK_WIDGET(gtk_builder_get_object(builder, "window_main"));
  //gtk_builder_connect_signals(builder, NULL);

  g_signal_connect (G_OBJECT(window), "destroy", (GCallback)on_window_main_destroy, NULL);


  GtkImage *gtk_img = (GtkImage*)GTK_WIDGET(gtk_builder_get_object(builder, "image"));
  GdkPixbuf *pixbuf = alloc_pixbuf_from_img(img);
  refresh_pixbuf_from_img(pixbuf, img);
  gtk_image_set_from_pixbuf(gtk_img, pixbuf);

  gtk_widget_show(window);
  gtk_main();

  g_object_unref(builder);
  g_object_unref(pixbuf);

  // GTK_END

  free_bw_img(bw_img);
  free_sub_bw_img(sub);
  free_color_img(img);
  return 0;
}
