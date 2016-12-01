#include "../bitmap/img.h"
#include "../error.h"
#include "../tdefs.h"
#include "morpho.h"
#include <stdio.h>

void new_dilate(t_bw_img *input,
		t_bw_img *output,
		uint mask_width,
		uint mask_height,
		unsigned char is_erode,
		float mask[mask_height][mask_width])
{
  if(is_erode)
    is_erode = 255;
  uint nx, ny;
  uint fx, fy, rx, ry;
  for(size_t y = 0; y < output->height; y++)
  {
    fy = 0;
    ry = 0;
    ny = (uint)y;
    if((int)ny < (int)mask_height/2)
      fy = (mask_height/2 - ny);
    else if(ny + mask_height/2 >= input->height)
      ry = ny + mask_height/2 - input->height + 1;
    for(size_t x = 0; x < output->width; x++)
    {
      fx = 0;
      rx = 0;
      nx = (uint)x;
      if(nx + mask_width >= input->width + 1)
	rx = nx + mask_width/2 - input->width + 1;
      else if(nx < mask_width/2)
	fx = mask_width/2 - nx;

      AT(output,x,y) = AT(input,x,y);

      for(int j = -(int)(mask_height/2-fy); j <= (int)(mask_height/2-ry); j++)
	for(int i = -(int)(mask_width/2-fx); i <= (int)(mask_width/2-rx); i++)
	{
	  //printf("%f\n", 255-(float)AT(input,(uint)((int)nx + i),(uint)((int)ny + j))
	  //	 *mask[j+(int)(mask_height/2+fy)][i+(int)(mask_width/2+fx)]);
	  if(255-(float)AT(input,(uint)((int)nx + i),(uint)((int)ny + j))
	     *mask[j+(int)(mask_height/2+fy)][i+(int)(mask_width/2+fx)] > 250)
	    goto set;
	}
      continue;
    set:
      AT(output,x,y)= is_erode;
    }
  }
}

void new_hor_dilate(t_bw_img *input,
		    t_bw_img * output,
		    uint size)
{
  float mask[1][size];
  for(uint i = 0; i< size;i++)
    mask[0][i] = 1;

  new_dilate(input,output,size,1,0,mask);
}

void new_ver_dilate(t_bw_img *input,
		    t_bw_img *output,
		    uint size)
{
  float mask[size][1];
  for(uint i = 0; i< size;i++)
    mask[i][0] = 1;

  new_dilate(input,output,1,size,0,mask);
}

void new_hor_erode(t_bw_img *input,
		    t_bw_img * output,
		    uint size)
{
  float mask[1][size];
  for(uint i = 0; i< size;i++)
    mask[0][i] = 1;

  new_dilate(input,output,size,1,1,mask);
}

void new_ver_erode(t_bw_img *input,
		    t_bw_img *output,
		    uint size)
{
  float mask[size][1];
  for(uint i = 0; i< size;i++)
    mask[i][0] = 1;

  new_dilate(input,output,1,size,1,mask);
}

void close_ver(t_bw_img *input, t_bw_img *output, uint size)
{
  float mask[size][1];
  for(uint i = 0; i< size;i++)
    mask[i][0] = 1;

  new_dilate(input,output,1,size,0,mask);
  new_dilate(input,output,1,size,1,mask);
}

void close_hor(t_bw_img *input, t_bw_img *output, uint size)
{
  float mask[1][size];
  for(uint i = 0; i< size;i++)
    mask[0][i] = 1;

  new_dilate(input,output,size,1,0,mask);
  new_dilate(input,output,size,1,1,mask);
}

void open_hor(t_bw_img *input, t_bw_img *output, uint size)
{
  float mask[1][size];
  for(uint i = 0; i< size;i++)
    mask[0][i] = 1;

  new_dilate(input,output,size,1,1,mask);
  new_dilate(input,output,size,1,0,mask);
}


void open_ver(t_bw_img *input, t_bw_img *output, uint size)
{
  float mask[size][1];
  for(uint i = 0; i< size;i++)
    mask[i][0] = 1;

  new_dilate(input,output,1,size,1,mask);
  new_dilate(input,output,1,size,0,mask);
}

t_bw_img *dilate(t_bw_img *input){
  t_bw_img *result;
  result = alloc_bw_img_twin(input);

  for(uint y = 0; y < result->height; ++y)
    for(uint x = 0; x < result->width; ++x)
      AT(result,x,y) = 1;

  for(uint y = 1; y < result->height - 1; ++y)
    for(uint x = 1; x < result->width - 1; ++x)
    {
      for(int i = -1; i<2;++i)
	for(int j = -1; j<2;++j)
	  if (AT(input,(uint)((int)x+i),(uint)((int)y+j))==0)
	    goto set;
      continue;
    set:
      AT(result,x,y) = 0;
    }
  return result;
}

t_bw_img *dilate_hor(t_bw_img *input){
  t_bw_img *result;
  result = alloc_bw_img_twin(input);

  for(uint y = 0; y < result->height; ++y)
    for(uint x = 0; x < result->width; ++x)
      AT(result,x,y) = 1;

  for(uint y = 1; y < result->height - 1; ++y)
    for(uint x = 1; x < result->width - 1; ++x)
    {
      for(int i = -1; i<2;++i){
	int j = 0;
	if (AT(input,(uint)((int)x+i),(uint)((int)y+j))==0)
	  goto set;
	continue;
      set:
	AT(result,x,y) = 0;
      }
    }

  return result;
}

t_bw_img *dilate_ver(t_bw_img *input){
  t_bw_img *result;
  result = alloc_bw_img_twin(input);

  for(uint y = 0; y < result->height; ++y)
    for(uint x = 0; x < result->width; ++x)
      AT(result,x,y) = AT(input,x,y);

  for(uint y = 1; y < result->height - 1; ++y)
    for(uint x = 1; x < result->width - 1; ++x)
    {
      for(int i = -1; i<2;++i){
	int j = 0;
	if (AT(input,(uint)((int)x+j),(uint)((int)y+i))==0)
	  goto set;
	continue;
      set:
	AT(result,x,y) = 0;
      }
    }

  return result;
}

t_bw_img *erode(t_bw_img *input){
  t_bw_img *result;
  result = alloc_bw_img_twin(input);

  for(uint y = 0; y < result->height; ++y)
    for(uint x = 0; x < result->width; ++x)
      AT(result,x,y) = AT(input,x,y);

  for(uint y = 1; y < result->height - 1; ++y)
    for(uint x = 1; x < result->width - 1;++x)
    {
      for(int i = -1; i<2;++i)
	for(int j = -1; j<2;++j)
	  if (AT(input,(uint)((int)x+i),(uint)((int)y+j))!=0)
	    goto set;
      continue;
    set:
      AT(result,x,y) = 255;
    }
  return result;
}

t_bw_img *erode_hor(t_bw_img *input){
  t_bw_img *result;
  result = alloc_bw_img_twin(input);

  for(uint y = 0; y < result->height; ++y)
    for(uint x = 0; x < result->width; ++x)
      AT(result,x,y) = AT(input,x,y);

  for(uint y = 1; y < result->height - 1; ++y)
    for(uint x = 1; x < result->width - 1;++x)
    {
      for(int i = -1; i<2;++i){
	int j = 0;
	if (AT(input,(uint)((int)x+i),(uint)((int)y+j))!=0)
	  goto set;
	continue;
      set:
	AT(result,x,y) = 255;
      }
    }
  return result;
}

t_bw_img *erode_ver(t_bw_img *input){
  t_bw_img *result;
  result = alloc_bw_img_twin(input);

  for(uint y = 0; y < result->height; ++y)
    for(uint x = 0; x < result->width; ++x)
      AT(result,x,y) = AT(input,x,y);

  for(uint y = 1; y < result->height - 1; ++y)
    for(uint x = 1; x < result->width - 1;++x)
    {
      for(int i = -1; i<2;++i){
	int j = 0;
	if (AT(input,(uint)((int)x+j),(uint)((int)y+i))!=0)
	  goto set;
	continue;
      set:
	AT(result,x,y) = 255;
      }
    }
  return result;
}

uint pix_diff(t_bw_img *input1, t_bw_img *input2){

  uint count = 0;

  for(uint y = 1; y < input1 -> width-1; ++y)
    for(uint x = 1; x < input1 -> height-1;++y)
      if(AT(input1,x,y) != AT(input2,x,y))
	count++;

  return count;
}

t_bw_img *close_morph(t_bw_img *img, uint iterations){
  t_bw_img *result;
  result = dilate(img);

  for(uint i = 1; i < iterations; i++)
    result = dilate(result);
  for(uint i = 1; i < iterations; ++i)
    result = erode(result);

  return result;
}

t_bw_img *close_hor_morph(t_bw_img *img, uint iterations){
  t_bw_img *result;

  result = dilate(img);

  for(uint i = 1; i < iterations; ++i)
    result = dilate_hor(result);
  for(uint i = 2; i < iterations; ++i)
    result = erode_hor(result);

  return result;
}

t_bw_img *close_ver_morph(t_bw_img *img, uint iterations){
  t_bw_img *result;

  result = dilate(img);

  for(uint i = 1; i < iterations; ++i)
    result = dilate_ver(result);
  for(uint i = 2; i < iterations; ++i)
    result = erode_ver(result);

  return result;
}

t_bw_img *open_morph(t_bw_img *img, uint iterations){

  t_bw_img *result;

  result = erode(img);

  for(uint i = 1; i < iterations; ++i)
    result = erode(result);
  for(uint i = 0; i < iterations; ++i)
    result = dilate(result);

  return result;
}

void xor_morph(t_bw_img *output, t_bw_img *input){

  for(uint y = 1; y < output->height - 1; ++y)
    for(uint x = 1; x < output->width - 1; ++x)
      if(AT(output,x,y) == 0)
	if(AT(input,x,y) != 0)
	  AT(output,x,y) = 255;
	else
	  AT(output,x,y) = 0;
      else
	if(AT(output,x,y) == 0)
	  AT(output,x,y) = 255;
	else
	  AT(output,x,y) = 0;
}

void border_morph(t_bw_img *img){
  xor_morph(dilate(img), img);
}
