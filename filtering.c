#include "bitmap/img.h"
#include "tdefs.h"
#include "math_macros.h"



#include <stdio.h>
// TODO: generic convolution filter
void filter(t_bw_img *img)
{
  const uint gauss_matrix[3][3] = {
    {1, 2, 1},
    {2, 4, 2},
    {1, 2, 1},
  };

  for(uint y = 0; y < img->height; y++)
    for(uint x = 0; x < img->width; x++)
    {
      printf("%d %d\n", x, y);
      uint gauss = 0;
      uint lxb = (x > 0) ? (x-1) : x;
      uint lxe = (x <= img->width - 1) ? (x+1) : x;
      uint lyb = (y > 0) ? (y-1) : y;
      uint lye = (y <= img->height - 1) ? (y+1) : y;

      for(uint ly=lyb; ly < lye; ly++)
	for(uint lx=lxb; lx < lxe; lx++)
	{
	  uint gauss_coef = gauss_matrix[lx - x +1][ly - y + 1];
	  gauss += gauss_coef * AT(img, lx, ly);
	}
      gauss /= 16;
      AT(img, x, y) = (uchar)gauss;
    }
}
