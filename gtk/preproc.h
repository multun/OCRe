#ifndef BIN_PREPROC_H
#define BIN_PREPROC_H

#include <gtk-3.0/gtk/gtk.h>

#include "../binarisation/bin_fixed.h"
#include "../binarisation/bin_ratio.h"
#include "../binarisation/bin_sauvola.h"

#include "img_history.h"

typedef union u_bin_opts
{
  t_bin_fixed_opts	fixed;
  t_bin_sauvola_opts	sauvola;
  t_bin_ratio_opts	ratio;
} t_bin_opts;

typedef struct s_preprocess_ui
{
  GtkStack	*stack;
  GtkSpinButton	*fixed_treshold;
} t_preprocess_ui;

void preprocess_ui_init(GtkBuilder *builder, t_img_history *img_history);
void preprocess_ui_run(t_img_history *img_history, t_img_history_e *hist_e);

#endif
