#include "noise_reduction.h"

t_bw_img* reduce_noise(t_bw_img img){
  for(int i = 1; i < img->height - 1; i++){
    for(int j = 1; j < img->width - 1; j++){
      int sum =
         SUB_AT(img,j-1,i-1)
        +SUB_AT(img,j-1,i)
        +SUB_AT(img,j-1,i+1)
        +SUB_AT(img,j,i-1)
        +SUB_AT(img,j,i+1)
        +SUB_AT(img,j+1,i-1)
        +SUB_AT(img,j+1,i)
        +SUB_AT(img,j+1,i+1);
      if (sum = 0)
        sum = 1;
      int average = sum/8;
      SUB_AT(img,j,i) = average;
    }
  }
}
