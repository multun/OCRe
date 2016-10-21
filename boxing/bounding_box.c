#include "../bitmap/img.h"
#include "../error.h"
#include "../tdefs.h"
#include "morpho.h"
#include <stdio.h>


typedef struct Box
{
  uint left, right, top, bottom;
}box;


void draw_box(t_bw_img *input_img, box input_box)
{
  for(uint i = input_box.left; i<input_box.right; i++){
    AT(input_img, i, input_box.top) = 0;
    AT(input_img, i, input_box.bottom) = 0;
  }

  for(uint i = input_box.top; i<input_box.bottom; i++){
    AT(input_img, input_box.left, i) = 0;
    AT(input_img, input_box.right, i) = 0;
  }
}

