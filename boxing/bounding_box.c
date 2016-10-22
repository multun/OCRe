#include "../bitmap/img.h"
#include "../error.h"
#include "../tdefs.h"
#include "morpho.h"
#include <stdio.h>
#include "../base_structs/vector.h"

typedef struct Box
{
  uint left, right, top, bottom, size;
}box;

uint min(uint a, uint b){
  return ((a < b)? a : b);
    }
uint max(uint a, uint b){
  return ((a > b)? a : b);
    }


DECL_NAMED_VECTOR(struct Box, box);

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


void update_box(box input_box, uint x, uint y)
{
  input_box.size += 1;
  input_box.left = min(input_box.left, x);
  input_box.right = max(input_box.right, x);
  input_box.top = min(input_box.top, y);
  input_box.bottom = max(input_box.bottom, y);
}

box create_box(uint x, uint y)
{
  box output_box;
  output_box.size = 1;
  output_box.left = x;
  output_box.right = x;
  output_box.top = y;
  output_box.bottom = y;
  return output_box;
}

int is_in_box(box input_box, uint x, uint y){
  return ((x>input_box.left && x<input_box.right && y>input_box.top && y<input_box.bottom)? 1 : 0);
}

int is_in_box_list(t_box_vect *box_list, uint x, uint y)
{
  int bool = 0;
  for(uint i = 0; i < VECT_GET_SIZE(box_list); ++i)
    bool += is_in_box(VECT_GET(box_list, i), x, y);

  return bool;
}

void connected_box(t_bw_img *input_img, box input_box, uint x, uint y){
  if (AT(input_img, x-1, y-1) == 0 && is_in_box(input_box, x-1, y-1) == 1){
    update_box(input_box, x-1, y-1);
    connected_box(input_img, input_box, x-1, y-1);
  }
  if (AT(input_img, x-1, y) == 0 && is_in_box(input_box, x-1, y) == 1){
    update_box(input_box, x-1, y);
    connected_box(input_img, input_box, x-1, y);
  }
  if (AT(input_img, x-1, y+1) == 0 && is_in_box(input_box, x-1, y+1) == 1){
    update_box(input_box, x-1, y+1);
    connected_box(input_img, input_box, x-1, y+1);
  }
  if (AT(input_img, x, y-1) == 0 && is_in_box(input_box, x, y-1) == 1){
    update_box(input_box, x, y-1);
    connected_box(input_img, input_box, x, y-1);
  }
  if (AT(input_img, x+1, y-1) == 0 && is_in_box(input_box, x+1, y-1) == 1){
    update_box(input_box, x+1, y-1);
    connected_box(input_img, input_box, x+1, y-1);
  }
  if (AT(input_img, x+1, y) == 0 && is_in_box(input_box, x+1, y) == 1){
    update_box(input_box, x+1, y);
    connected_box(input_img, input_box, x+1, y);
  }
  if (AT(input_img, x+1, y+1) == 0 && is_in_box(input_box, x+1, y+1) == 1){
    update_box(input_box, x+1, y+1);
    connected_box(input_img, input_box, x+1, y+1);
  }
  if (AT(input_img, x, y+1) == 0 && is_in_box(input_box, x, y+1) == 1){
    update_box(input_box, x, y+1);
    connected_box(input_img, input_box, x, y+1);
  }
}

t_box_vect *list_boxes(t_bw_img *input_img)
{
  t_box_vect *box_list;
  box_list = VECT_ALLOC(box, 0);

  for(uint y = 1; y < input_img->height - 1; ++y)
    for(uint x = 1; x < input_img->width - 1; ++x)
      if (AT(input_img, x, y) == 0 && is_in_box_list(box_list, x, y) == 0){
	VECT_PUSH(box_list, create_box(x, y));
	connected_box(input_img, VECT_GET_LAST(box_list), x, y);
      }

  return box_list;
}

void draw_boxes(t_bw_img *bw_img, t_box_vect *box_list)
{
  for (unsigned int i = 0; i < VECT_GET_SIZE(box_list);i++)
    draw_box(bw_img, VECT_GET(box_list, i));
}


