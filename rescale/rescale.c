#include <stdio.h>
#include <stdlib.h>
#include "../bitmap/img.h"
#include "../error.h"
#include "../tdefs.h"
#include "rescale.h"
#include "../boxing/morpho.h"
#include "../math_macros.h"

const double mask[][5] ={{0.019720, 0.026388, 0.036396, 0.026388, 0.019720},
			 {0.026388, 0.040227, 0.103966, 0.040227, 0.026388},
			 {0.036396, 0.103966, 1.000000, 0.103966, 0.036396},
			 {0.026388, 0.040227, 0.103966, 0.040227, 0.026388},
			 {0.019720, 0.026388, 0.036396, 0.026388, 0.019720}};

int compute_value(t_bw_img *input, uint x, uint y)
{
  double value = 0;
  double mask_t = 0.0;
  for(int j = -2; j <= 2; j++)
    for(int i = -2; i <= 2; i++)
    {
      int ay = ((int)y + j);
      int ax = ((int)x + i);
      if(ay < 0 || ax < 0 ||
	 (uint)ax >= input->width || (uint)ay >= input->height)
	continue;

      double mask_v = mask[j+2][i+2];
      value += (double)AT(input, (uint)ax, (uint)ay)
	* mask_v;
      mask_t += mask_v;
    }

  return (int)(value/mask_t);
}

void interpolate(t_bw_img *input,
		 t_bw_img *output,
		 double factor)
{
  factor = 1/factor;
  uint nx, ny;
  for(size_t y = 0; y < output->height; y++)
  {
    ny = (uint)((double)y*factor);
    for(size_t x = 0; x < output->width; x++)
    {
      nx = (uint)((double)x*factor);

      AT(output,x,y) = (unsigned char)(compute_value(input, nx, ny));
      //AT(output,x,y) = (unsigned char)MIN(255,(uint)(val/2.01234));
    }
  }
}

t_bw_img *resize_tbw(t_bw_img *input, double factor)
{
  /*double mask[][3] ={{1/3, 2/3, 1/3},
		    {2/3,   1, 2/3},
		    {1/3, 2/3, 1/3}};
  */
/*
  //f(x)=-2x^3+3x^2
  double mask[][5] ={{0.1405, 0.1880, 0.2593, 0.1880, 0.1405},
		    {0.1880, 0.2866, 0.7407, 0.2866, 0.1880},
		    {0.2593, 0.7407, 1.0000, 0.7407, 0.2593},
		    {0.1880, 0.2866, 0.7407, 0.2866, 0.1880},
		    {0.1405, 0.1880, 0.2593, 0.1880, 0.1405}};
*/



  t_bw_img *output = alloc_bw_img((uint)((double)input->width*factor),
				  (uint)((double)input->height*factor));

  for(size_t y = 0; y<output->height; y++)
    for(size_t x = 0; x<output->width; x++)
      AT(output, x, y) = 255;

  interpolate(input, output, factor);

  return output;

}

t_bw_img *resize(t_bw_img *input, uint width, uint height)
{


  double yratio, xratio, factor;
  xratio = (double)width/(double)input->width;
  yratio = (double)height/(double)input->height;
  factor = (xratio>yratio) ? yratio : xratio;
  t_bw_img *output = resize_tbw(input, factor);
  output = fill_padding(output, width, height);

  return output;

}


t_bw_img *fill_padding(t_bw_img *input, uint width, uint height)
{
  t_bw_img *output = alloc_bw_img(width, height);
  uint ystart, xstart, xend, yend;
  ystart = (height - input->height)/2 + (height - input->height)%2;
  xstart = (width - input->width)/2 + (width - input->width)%2;
  xend = width - (width - input->width)/2;
  yend = height - (height - input->height)/2;

  for(uint y = 0; y<output->height; y++)
    for(uint x = 0; x<xstart; x++)
      AT(output, x, y) = 255;
  for(uint y = 0; y<output->height; y++)
    for(uint x = xend; x<output->width; x++)
      AT(output, x, y) = 255;
  for(uint y = 0; y<ystart+1; y++)
    for(uint x = 0; x<output->width; x++)
      AT(output, x, y) = 255;
  for(uint y = yend; y<output->height; y++)
    for(uint x = 0; x<output->width; x++)
      AT(output, x, y) = 255;
  for(uint y = ystart; y<yend; y++)
    for(uint x = xstart; x<xend; x++)
      AT(output, x, y) = AT(input, x-xstart, y-ystart);

  free_bw_img(input);

  return output;
}
