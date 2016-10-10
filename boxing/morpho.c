#include "../bitmap/img.h"
#include "../error.h"
#include "../tdefs.h"
#include "morpho.h"
#include <stdio.h>

t_bw_img *dilate(t_bw_img *input){
  t_bw_img *result;
  result = alloc_bw_img_twin(input);

  for(uint y = 0; y < result->width; ++y)
    for(uint x = 0; x < result->height; ++x)
      {
	AT(result,0,y) = 255;
	AT(result,x,0) = 255;
	AT(result,x,result->width - 1) = 255;
	AT(result,result->height - 1,y) = 255;
      }
	
  

  
  for(uint y = 1; y < result->width - 1; ++y)
    for(uint x = 1; x < result->height - 1; ++x)
      {
	AT(result,x,y) = 255;
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

t_bw_img *erode(t_bw_img *input){
  t_bw_img *result;
  result = alloc_bw_img_twin(input);

  for(uint y = 1; y < result -> width-1; ++y)
    for(uint x = 1; x < result -> height-1;++x)
      for(int i = -1; i<2;++i)
	for(int j = -1; j<2;++j)
	  if(AT(input,(uint)((int)x+i),(uint)((int)y+j))!=0)
	    AT(result,x,y) = 255;
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

void close_morph(t_bw_img *img){
  
  t_bw_img *temp;
  temp = alloc_bw_img_twin(img);

  temp = dilate(img);
  img = erode(temp);
}
     
void open_morph(t_bw_img *img){
  
  t_bw_img *temp;
  temp = alloc_bw_img_twin(img);

  temp = erode(img);
  img = dilate(temp);
}

void xor_morph(t_bw_img *output, t_bw_img *input){

  for(uint y = 1; y < output -> width-1; ++y)
    for(uint x = 1; x < output -> height-1;++x)
      if(AT(output,x,y) != AT(input,x,y))
	  AT(output,x,y) = 0;
      else
	  AT(output,x,y) = 255;
}

void border_morph(t_bw_img *img){
  xor_morph(img, erode(img));
}
