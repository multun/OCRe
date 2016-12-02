#include <stdio.h>
#include <stdlib.h>
#include "../bitmap/img.h"
#include "../error.h"
#include "../tdefs.h"
#include "rescale.h"
#include "../boxing/morpho.h"
#include "../math_macros.h"

void interpolate(t_bw_img *input,
		 t_bw_img *output,
		 uint mask_len,
		 double mask[][mask_len],
		 double factor)
{
  factor = 1/factor;
  uint nx, ny;
  uint fx, fy, rx, ry;
  double val;
  for(size_t y = 0; y < output->height; y++)
  {
    fy = 0;
    ry = 0;
    ny = (uint)((double)y*factor);
    if((int)ny < (int)mask_len/2)
      fy = (mask_len/2 - ny);
    else if(ny + mask_len/2 >= input->height)
      ry = ny + mask_len/2 - input->height +1;
    for(size_t x = 0; x < output->width; x++)
    {
      val = 0;
      fx = 0;
      rx = 0;
      nx = (uint)((double)x*factor);
      if(nx + mask_len >= input->width)
	rx = nx + mask_len/2 - input->width + 1;
      else if(nx < mask_len/2)
	fx = (mask_len/2) - nx;

      double mask_t = 0.0;
      for(int j = -(int)(mask_len/2-fy); j <= (int)(mask_len/2-ry); j++)
	for(int i = -(int)(mask_len/2-fx); i <= (int)(mask_len/2-rx); i++)
	{

	  double mask_v = mask[j+(int)(mask_len/2+fy)][i+(int)(mask_len/2+fx)];
	  val += (((double)AT(input,(uint)((int)nx + i),(uint)((int)ny + j)))?255:0)
	    * mask_v;
	  mask_t += mask_v;
	}

      AT(output,x,y) = (unsigned char)MIN(255,(uint)(val/mask_t));
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
  double mask[][5] ={{0.019720, 0.026388, 0.036396, 0.026388, 0.019720},
		     {0.026388, 0.040227, 0.103966, 0.040227, 0.026388},
		     {0.036396, 0.103966, 1.000000, 0.103966, 0.036396},
		     {0.026388, 0.040227, 0.103966, 0.040227, 0.026388},
		     {0.019720, 0.026388, 0.036396, 0.026388, 0.019720}};


  t_bw_img *output = alloc_bw_img((uint)((double)input->width*factor),
				  (uint)((double)input->height*factor));

  for(size_t y = 0; y<output->height; y++)
    for(size_t x = 0; x<output->width; x++)
      AT(output, x, y) = 255;

  interpolate(input, output, 5, mask, factor);

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
