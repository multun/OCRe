#include "../bitmap/img.h"
#include "../error.h"
#include <stdio.h>

int isWhiteColumn(t_sub_bw_img *img, int column)
{
  int isWhite = 1;
    
  for(uint i = 0; i< img -> height; i++)
    if(AT(img,i,column) == 0)
      isWhite = 0;
    
  return isWhite;
}

t_sub_bw_img_vect segmentation(t_sub_bw_img *img)
{
  t_sub_bw_img_vect result;
  int elements = 0;
  int aux = 0;
    
  for(uint i = 0; i < img -> width; i++)
    if (isWhiteColumn(img, i) || i == width-1)
    {
      if(i>0 && (isWhiteColumn(img, i-1) == 0))
        result[elements++] = imgToSmallerImg(img, aux, i);           
      aux++;
    } 
  return result;
}

t_sub_bw_img imgToSmallerImg(t_sub_bw_img *img, int firstColumn, int lastColumn)
{
  t_sub_bw_img *result;
  result = alloc_sub_bw_img(img)
  int aux;
  for(int i = firstColumn; i <= lastColumn; i++)
  {
    aux = i - firstColumn;
    for(int j = 0; j < img -> height; j++)
      AT(result, j, aux) = AT(img, j, i);            
  }
  return result;
}
