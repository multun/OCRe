#include "bitmap/img.h"
#include "math_macros.h"
#include "tdefs.h"

luint img_mean(t_bw_img *inpImg, uint x, uint y, uint window)
{
  uint min_y = (uint)MAX(0, (lint)(y - window/2));
  uint max_y = MIN(inpImg->height - 1, y + window/2);
  uint min_x = (uint)MAX(0, (lint)(x - window/2));
  uint max_x = MIN(inpImg->width - 1, x + window/2);

  luint acc = 0;
  for (uint j = min_y; j <= max_y; j++)
    for (uint i = min_x; i <= max_x; i++)
      acc += (luint) AT(inpImg, j,i);
  return (uint)(acc/(window*window));
}

int img_deviation(t_bw_img *inpImg, uint x, uint y)
{
  uchar cur = AT(inpImg, y, x);
  uchar d_x = AT(inpImg, y, (uint)MAX(0, (lint)x - 1));
  uchar d_y = AT(inpImg, (uint)MAX(0, (lint)y - 1), x);

  return (int)cur - (int)d_x - (int)d_y;
}
