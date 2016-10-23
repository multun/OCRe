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

int is_in_box(box input_box, uint x, uint y){
  return ((x>=input_box.left && x<=input_box.right && y>=input_box.top && y<=input_box.bottom)? 1 : 0);
}

void update_box(box *input_box, uint x, uint y)
{
  if(is_in_box(*input_box, x, y) == 0){
    input_box->size = input_box->size + 1;
    input_box->left = min(input_box->left, x);
    input_box->right = max(input_box->right, x);
    input_box->top = min(input_box->top, y);
    input_box->bottom = max(input_box->bottom, y);
  }
}

void create_box(box *input_box, uint x, uint y)
{
  input_box->size = 1;
  input_box->left = x;
  input_box->right = x;
  input_box->top = y;
  input_box->bottom = y;
}

void box_print(box input_box)
{
  printf("%u %u %u %u %u\n", input_box.left, input_box.right, input_box.top, input_box.bottom, input_box.size);
}



int is_in_box_list(t_box_vect *box_list, uint x, uint y)
{
  int bool = 0;
  for(uint i = 0; i < VECT_GET_SIZE(box_list); i++)
    bool += is_in_box(VECT_GET(box_list, i), x, y);

  return bool;
}

box connected_box(t_bw_img *input_img, box input_box, uint x, uint y){

  box result;
  result = input_box;
  printf("%d\n", (int)AT(input_img, input_img->width-1, input_img->height - 1));
  if(x > 1 && x < input_img->width-2 && y > 1 && y < input_img->height-2){
    update_box(&input_box, x, y);
    //printf("%u %u %d  ", x, y, AT(input_img, x, y));
    //box_print(input_box);
    if (is_in_box(input_box, x+1, y+1) == 0 && AT(input_img, x+1, y+1) == 0)
      result = connected_box(input_img, result, x+1, y+1);
    if (is_in_box(input_box, x+1, y) == 0 && AT(input_img, x+1, y) == 0)
      result = connected_box(input_img, result, x+1, y);    
    if (is_in_box(input_box, x, y+1) == 0 && AT(input_img, x, y+1) == 0)
      result = connected_box(input_img, result, x, y+1);
    if (is_in_box(input_box, x-1, y+1) == 0 && AT(input_img, x-1, y+1) == 0)
      result = connected_box(input_img, result, x-1, y+1);
    if (is_in_box(input_box, x-1, y) == 0 && AT(input_img, x-1, y) == 0)
      result = connected_box(input_img, result, x-1, y);
    if (is_in_box(input_box, x-1, y-1) == 0 && AT(input_img, x-1, y-1) == 0)
      result = connected_box(input_img, result, x-1, y-1);
    if (is_in_box(input_box, x, y-1) == 0 && AT(input_img, x, y-1) == 0)
      result = connected_box(input_img, result, x, y-1);
    unsigned char *paddrlol = &AT(input_img, x+1, y-1);
    printf("%p\n", (void*)paddrlol);
    if (is_in_box(input_box, x+1, y-1) == 0 && *paddrlol == 0)
      result = connected_box(input_img, result, x+1, y-1);    
  }
  return result;
}
t_box_vect *list_boxes(t_bw_img *input_img)
{
  t_box_vect *box_list;
  box_list = VECT_ALLOC(box, 42);
  VECT_PRINT(box_list);
  printf("listboxes %p\n", (void*)input_img);
  
  for(uint y = 1; y < input_img->height - 1; y++)
    for(uint x = 1; x < input_img->width - 1; x++)
      if (AT(input_img, x, y) == 0 && is_in_box_list(box_list, x, y) == 0){
	box temp_box;
	create_box(&temp_box, x, y);
	//box_print(temp_box);
	temp_box = connected_box(input_img, temp_box, x, y);
	//box_print(temp_box);
	VECT_PUSH(box_list, temp_box);
	//VECT_PRINT(box_list);		
      }
  return box_list;
}

void draw_boxes(t_bw_img *bw_img, t_box_vect *box_list)
{
  for (unsigned int i = 0; i < VECT_GET_SIZE(box_list);i++)
    draw_box(bw_img, VECT_GET(box_list, i));
}

