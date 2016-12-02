#include "wrapper.h"

t_bw_img* noise_red(t_bw_img *img){
  t_bw_img* img2 = reduce_noise(img);
  return img2;
}
