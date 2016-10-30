#include "segmentation.h"


char isWhiteColumn(t_sub_bw_img *img, uint column)
{
  char isWhite = 1;
    
  for(uint i = 0; i< img -> height; i++)
    if(SUB_AT(img,column,i) == 0)
      isWhite = 0;
    
  return isWhite;
}

/*uint avgCharWidth(t_sub_bw_img_vect *vimg)
{
  uint avg, sum, size = VECT_GET_SIZE(vimg);
  for(uint i = 0; i < size; i++)
    sum += (VECT_GET(vimg, i))->width;
  avg = sum/size;
  return avg;
}*/

t_sub_bw_img_vect *segmentation(t_sub_bw_img *img)
{
  t_sub_bw_img_vect *result;
  //uint *avgCompWidth;
  result = VECT_ALLOC(sub_bw_img, 32);
  //avgCompWidth = VECT_ALLOC(uint, 32);
  uint aux = 0;
    
  for(uint i = 0; i < img->width; i++)
    if (isWhiteColumn(img, i) || i == img->width-1)
    {
      if(i>0 && (isWhiteColumn(img, i-1) == 0))      
        VECT_PUSH(result,relink_sub_bw_img(img,aux,0,i-aux,img->height));                  
      aux++;
    } 

  return result;
}
