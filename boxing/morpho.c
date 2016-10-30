#include "../bitmap/img.h"
#include "../error.h"
#include "../tdefs.h"
#include "morpho.h"
#include <stdio.h>

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
  for(uint i = 0; i < iterations; ++i)
    result = erode(result);

  return result;
}
 
t_bw_img *close_hor_morph(t_bw_img *img, uint iterations){
  t_bw_img *result;

  result = dilate(img);

  for(uint i = 1; i < iterations; ++i)
    result = dilate_hor(result);
  for(uint i = 1; i < iterations; ++i)
    result = erode_hor(result);

  return result;
}

t_bw_img *close_ver_morph(t_bw_img *img, uint iterations){
  t_bw_img *result;

  result = dilate(img);

  for(uint i = 1; i < iterations; ++i)
    result = dilate_ver(result);
  for(uint i = 1; i < iterations; ++i)
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
