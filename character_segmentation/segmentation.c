#include "segmentation.h"


char isWhiteColumn(t_sub_bw_img *img, uint column)
{
  char isWhite = 1;
    
  for(uint i = 0; i< img -> height; i++)
    if(SUB_AT(img,i,column) == 0)
      isWhite = 0;
    
  return isWhite;
}

t_sub_bw_img_vect *segmentation(t_sub_bw_img *img)
{
  t_sub_bw_img_vect *result;
  result = VECT_ALLOC(sub_bw_img, 32);
  uint aux = 0;
    
  for(uint i = 0; i < img->width; i++)
    if (isWhiteColumn(img, i) || i == img->width-1)
    {
      if(i>0 && (isWhiteColumn(img, i-1) == 0))      
        VECT_PUSH(result, relink_sub_bw_img(img, aux, 0, i - aux, img->height));                  
      aux++;
    } 

  return result;
}

/*t_sub_bw_img *imgToSmallerImg(t_sub_bw_img *img, int firstColumn, int lastColumn)
{
  t_sub_bw_img *result;
  result = alloc_sub_bw_img(img);
  int aux;
  for(int i = firstColumn; i <= lastColumn; i++)
  {
    aux = i - firstColumn;
    for(int j = 0; j < img -> height; j++)
      SUB_AT(result, j, aux) = SUB_AT(img, j, i);            
  }
  return result;
}*/
