#include "bitmap/img.h"
#include "math_macros.h"
#include "tdefs.h"

uint img_mean(t_bw_img *img, uint x, uint y, uint window)
{
  uint min_y = (uint)MAX(0, (int)(y - window/2));
  uint max_y = (uint)MIN(img->height - 1, y + window/2);
  uint min_x = (uint)MAX(0, (int)(x - window/2));
  uint max_x = (uint)MIN(img->width - 1, x + window/2);

  luint acc = 0;
  for (uint j = min_y; j <= max_y; j++)
    for (uint i = min_x; i <= max_x; i++)
      acc += (luint) AT(img, i, j);
  return (uint)(acc/(window*window));
}

int img_deviation(t_bw_img *img, uint x, uint y)
{
  uchar cur = AT(img, x, y);
  uchar d_x = AT(img, x, (uint)MAX(0, (int)y - 1));
  uchar d_y = AT(img, (uint)MAX(0, (int)x - 1), y);
  return (int)cur - (int)d_x - (int)d_y;
}

t_double_mat *integral(t_bw_img *img)
{
  t_double_mat *mat = alloc_double_mat(img->width, img->height);
  for(uint x = 0; x < mat->width; x++)
    for(uint y = 0; y < mat->height; y++)
    {
      double left = (x > 0) ? AT(mat, x-1, y) : 0.0;
      double top = (y > 0) ? AT(mat, x, y-1) : 0.0;
      AT(mat, x, y) += left + top + (double)AT(img, x, y);
    }
  return mat;
}
