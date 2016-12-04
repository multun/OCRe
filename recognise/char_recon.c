#include "char_recon.h"
#include "../neural_network/forward.h"
#include "../rescale/rescale.h"

IMAGE_DECLARE(nfloat, nfloat_mat)
static ALLOC_IMAGE_DEFINE(nfloat, nfloat_mat)
static FREE_IMAGE_DEFINE(nfloat, nfloat_mat)





//REMOVEME
#include <stdio.h>

t_string *char_recon(t_w_network_vect *context, t_l_bw_img *img)
{
  t_bw_img *sized_res = resize((void*)&img->width,
			       NET_MAT_SIZE,
			       NET_MAT_SIZE);
  t_nfloat_mat * fmat= alloc_nfloat_mat(NET_MAT_SIZE, NET_MAT_SIZE);
  free_bw_img(sized_res);

  for(uint i = 0; i < NET_MAT_SIZE * NET_MAT_SIZE; i++)
    fmat->pixels[i] = ((nfloat)sized_res->pixels[i])/255;

  t_w_network *best = NULL;
  nfloat best_score;
  for(size_t i = 0; i < VECT_SIZE(context); i++)
  {
    nfloat output;
    t_w_network *cur = VECT_GET(context, i);
    NET_IN(cur->net)  = &fmat->pixels[0];
    NET_OUT(cur->net) = &output;
    forward(cur->net);
    printf("%s: %f\n", cur->target, output);
    if(!best || output > best_score)
    {
      best_score = output;
      best = cur;
    }
  }
  free_nfloat_mat(fmat);

  return alloc_string(best->target);
}
