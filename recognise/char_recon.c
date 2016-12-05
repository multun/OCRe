#define _XOPEN_SOURCE 600
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>

#include "char_recon.h"
#include "../neural_network/forward.h"
#include "../rescale/rescale.h"
#include "../image_processing/normalise_contrast.h"
#include "../math_macros.h"
#include "../neural_network/cycle.h"
#include "../neural_network/neurons.h"
#include "../neural_network/forward.h"
#include "../neural_network/backward.h"

IMAGE_DECLARE(nfloat, nfloat_mat)
static ALLOC_IMAGE_DEFINE(nfloat, nfloat_mat)
static FREE_IMAGE_DEFINE(nfloat, nfloat_mat)
#include <stdio.h>
t_string *char_recon(t_w_network_vect *context, t_l_bw_img *img)
{
  t_bw_img *sized_res = resize((void*)&img->width,
			       NET_MAT_SIZE,
			       NET_MAT_SIZE);

  //adjust_contrast(sized_res);

  for(uint i = 0; i < NET_MAT_SIZE * NET_MAT_SIZE; i++)
    if (sized_res->pixels[i] < 50)
      sized_res->pixels[i] = 0;

  t_nfloat_mat *fmat = alloc_nfloat_mat(NET_MAT_SIZE, NET_MAT_SIZE);

  for(uint i = 0; i < NET_MAT_SIZE * NET_MAT_SIZE; i++)
    fmat->pixels[i] = ((nfloat)(255 - sized_res->pixels[i]))/255;

  free_bw_img(sized_res);

  t_w_network *best = NULL;
  nfloat best_score;
  for(size_t i = 0; i < VECT_SIZE(context); i++)
  {
    t_w_network *cur = VECT_GET(context, i);

    NET_IN(cur->net) = &fmat->pixels[0];
    forward(cur->net);

    nfloat expected = CF(0.0);
    nfloat output = compute_error(cur->net, &expected);

    if(!best || output > best_score)
    {
      best_score = output;
      best = cur;
    }
  }

  free_nfloat_mat(fmat);
  return alloc_string(best->target);
}


/*

  char fname[] = "/tmp/stuff/ocr_XXXXXX";
  int fd = mkstemp(fname);
  puts(fname);
  size = IMG_SIZE(t_nfloat_mat, fmat->width, fmat->height);
  printf("mat has size %lu\n%u\t%u\n", size, fmat->width, fmat->height);
  write(fd, fmat, size);
  close(fd);

*/
