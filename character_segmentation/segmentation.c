#include "segmentation.h"


char isWhiteColumn(t_sub_bw_img *img, uint column)
{
  char isWhite = 1;
    
  for(uint i = 0; i< img -> height; i++)
    if(SUB_AT(img,i,column) == 0)
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
        VECT_PUSH(result, relink_sub_bw_img(img, aux, 0, i - aux, img->height));                  
      aux++;
    } 

  return result;
}

/*t_tuple_sub_img_label *segmentedToProperties(t_sub_bw_img_vect *vimg)
{
  t_tuple_sub_img_label *result;
  result = VECT_ALLOC(t_tuple_sub_img_label, 32);
  for(uint i = 0; i < VECT_GET_SIZE(vimg); i++)
  {
    if(VECT_GET(vimg, i) > avgCharWidth)
      VECT_PUSH(result, ((t_tuple_sub_img_label){.img = VECT_GET(vimg, i), .label = 1}));
    else
      VECT_PUSH(result, ((t_tuple_sub_img_label){.img = VECT_GET(vimg, i), .label = 0}));
  }
  return result;
}*/

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
