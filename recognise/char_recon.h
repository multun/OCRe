#ifndef CHAR_RECON_H
#define CHAR_RECON_H

#include "../string/string.h"
#include "../bitmap/img.h"

#include "net_pool.h"

#define NET_MAT_SIZE 16

t_string *char_recon(t_net_pool *context, t_l_bw_img *img);

#endif
