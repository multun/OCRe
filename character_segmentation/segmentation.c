#include "segmentation.h"

char isWhiteColumn(t_sub_bw_img *img, uint column)
{
  for(uint i = 0; i< img -> height; i++)
    if(SUB_AT(img,column,i) == 0)
      return 0;
  return 1;
}

t_sub_bw_img_vect *char_segmentation(t_sub_bw_img *img)
{
  t_sub_bw_img_vect *result = VECT_ALLOC(sub_bw_img, 32);

  for(uint aux = 0, i = 0; i < img->width; i++)
    if (isWhiteColumn(img, i) || i == img->width-1)
    {
      if(isWhiteColumn(img, i-1) == 0)
        VECT_PUSH(result,relink_sub_bw_img(img,aux,0,i-aux,img->height));
      aux = i+1;
    }

  return result;
}
