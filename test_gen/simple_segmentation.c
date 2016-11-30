#include "simple_segmentation.h"

char is_white_column(t_sub_bw_img *img, uint column)
{
  for(uint i = 0; i< img->height; i++)
    if(SUB_AT(img,column,i) == 0)
      return 0;
  return 1;
}

char is_white_line(t_sub_bw_img *img, uint line)
{
  for(uint i = 0; i< img->width; i++)
    if(SUB_AT(img,i,line) == 0)
      return 0;
  return 1;
}

void char_vseg(t_sub_bw_img *lsub)
{
  while(lsub->height && is_white_line(lsub, 0))
  {
    lsub->yoff++;
    lsub->height--;
  }
  while(lsub->height && is_white_line(lsub, lsub->height - 1))
    lsub->height--;
}


t_sub_bw_img_vect *simple_char_segmentation(t_sub_bw_img *img)
{
  t_sub_bw_img_vect *result = VECT_ALLOC(sub_bw_img, 32);

  for(uint aux = 0, i = 0; i < img->width; i++)
    if (is_white_column(img, i) || i == img->width-1)
    {
      if(i > 0 && is_white_column(img, i-1) == 0)
      {
	t_sub_bw_img *sub = relink_sub_bw_img(
	  img,
	  aux,
	  0,
	  i - aux,
	  img->height);
	char_vseg(sub);
	VECT_PUSH(result, sub);
      }
      aux = i+1;
    }
  return result;
}
