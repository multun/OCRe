#ifndef GTK_HELPERS_H
#define GTK_HELPERS_H

#define _GTK_CONNECT(o, sig, callback, uo)				\
  g_signal_connect (G_OBJECT(o), sig, G_CALLBACK(callback), (gpointer)(uo))

#define _GET_WIDGET(builder, name)			\
  GTK_WIDGET(gtk_builder_get_object(builder, name))

#define _GTK_GET(builder, type, name)			\
  GTK_## type (gtk_builder_get_object(builder, name))

#define GET_SPIN(builder, name) GTK_SPIN_BUTTON(_GET_WIDGET(builder, name))
#define GET_BUTTON(builder, name) GTK_BUTTON(_GET_WIDGET(builder, name))

#endif
