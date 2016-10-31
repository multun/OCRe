#include "../bitmap/img.h"
#include "../error.h"
#include "../tdefs.h"
#include "morpho.h"
#include <stdio.h>
#include "../base_structs/vector.h"
#include "bounding_box.h"

uint min(uint a, uint b){
  return ((a < b)? a : b);
}
uint max(uint a, uint b){
  return ((a > b)? a : b);
}


uint get_height(box input_box){
  return input_box.bottom - input_box.top + 1;
}
uint get_width(box input_box){
  return input_box.right - input_box.left + 1;
}
uint get_fullsize(box input_box){
  return get_height(input_box) * get_width(input_box);
}


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
  return ((x>=input_box.left && x<=input_box.right
	   && y>=input_box.top && y<=input_box.bottom)? 1 : 0);
}

void update_box(box *input_box, uint x, uint y)
{
  input_box->size = input_box->size + 1;
  input_box->left = min(input_box->left, x);
  input_box->right = max(input_box->right, x);
  input_box->top = min(input_box->top, y);
  input_box->bottom = max(input_box->bottom, y);
}

box init_box(uint x, uint y)
{
  box output;
  output.size = 1;
  output.left = x;
  output.right = x;
  output.top = y;
  output.bottom = y;
  return output;
}

box fus_boxes(box box_1,box box_2)
{
  box output;
  output.size = box_1.size + box_2.size;
  output.left = min(box_1.left, box_2.left);
  output.right = max(box_1.right, box_2.right);
  output.top = min(box_1.top, box_2.top);
  output.bottom = max(box_1.bottom, box_2.bottom);
  return output;
}

void box_print(box input_box)
{
  printf("%u %u %u %u %u\n",
	 input_box.left, input_box.right,
	 input_box.top, input_box.bottom,
	 input_box.size);
}



int is_in_box_list(t_box_vect *box_list, uint x, uint y)
{
  int bool = 0;
  for(uint i = 0; i < VECT_GET_SIZE(box_list); i++)
    bool += is_in_box(VECT_GET(box_list, i), x, y);

  return bool;
}

void expand_box(box *input_box, uint iter)
{
  for(uint i = 0; i<iter;i++)
  {
    input_box->left--;
    input_box->right++;
  }
  for(uint i = 0; i<iter;i++)
  {
    input_box->top--;
    input_box->bottom++;
  }
}

void connected_box_iter(t_bw_img *input_img, box *input_box,
			uint init_x, uint init_y,
			char *array, size_t width)
{
  t_tuple_uint_vect *coor_stack;
  coor_stack = VECT_ALLOC(tuple_uint, width);
  VECT_PUSH(coor_stack, ((t_tuple_uint){.x = init_x, .y = init_y}));

  while(VECT_GET_SIZE(coor_stack))
  {
    uint x, y;
    x = VECT_GET_LAST(coor_stack).x;
    y = VECT_GET_LAST(coor_stack).y;
    VECT_POP(coor_stack);

    if((x > 0 && x < input_img->width-1	&& y > 0 && y < input_img->height-1)
       && (array[x + (y) * width] == -1))
    {
      update_box(input_box, x, y);
      array[x + y * width] = 0;
    }


    if (AT(input_img, x+1, y) == 0 && (array[x+1 + (y) * width] == -1))
      VECT_PUSH(coor_stack, ((t_tuple_uint){.x = x+1, .y = y}));
    if (AT(input_img, x-1, y) == 0 && (array[x-1 + (y) * width] == -1))
      VECT_PUSH(coor_stack, ((t_tuple_uint){.x = x-1, .y = y}));
    if (AT(input_img, x, y+1) == 0 && (array[x + (y+1) * width] == -1))
      VECT_PUSH(coor_stack, ((t_tuple_uint){.x = x, .y = y+1}));
    if (AT(input_img, x, y-1) == 0 && (array[x + (y-1) * width] == -1))
      VECT_PUSH(coor_stack, ((t_tuple_uint){.x = x, .y = y-1}));
    if (AT(input_img, x+1, y+1) == 0 && (array[x+1 + (y+1) * width] == -1))
      VECT_PUSH(coor_stack, ((t_tuple_uint){.x = x+1, .y = y+1}));
    if (AT(input_img, x-1, y+1) == 0 && (array[x-1 + (y+1) * width] == -1))
      VECT_PUSH(coor_stack, ((t_tuple_uint){.x = x-1, .y = y+1}));
    if (AT(input_img, x+1, y-1) == 0 && (array[x+1 + (y-1) * width] == -1))
      VECT_PUSH(coor_stack, ((t_tuple_uint){.x = x+1, .y = y-1}));
    if (AT(input_img, x-1, y-1) == 0 && (array[x-1 + (y-1) * width] == -1))
      VECT_PUSH(coor_stack, ((t_tuple_uint){.x = x-1, .y = y-1}));
  }

  VECT_FREE(coor_stack);
}

void test_traversal(t_box_vect *box_list)
{
  printf("begin traversal : \n\n");
  for (unsigned int i = 0; i < VECT_GET_SIZE(box_list);i++)
  {
    printf("%u  %u  ratio: %f\n",
	   VECT_GET(box_list,i).size, get_fullsize(VECT_GET(box_list,i)),
	   (float)(int)get_fullsize(VECT_GET(box_list,i))/
	   (float)(int)VECT_GET(box_list,i).size);
  }
  printf("end traversal : \n\n");
}

void update_true_size(t_box_vect *box_list, t_bw_img *input_img)
{
  for(size_t i = 0; i< VECT_GET_SIZE(box_list); i++)
  {
    VECT_GET(box_list, i).size = 0;
    for(uint x = VECT_GET(box_list, i).left;
	x <= VECT_GET(box_list, i).right;x++)
      for(uint y = VECT_GET(box_list, i).top;
	  y <= VECT_GET(box_list, i).bottom; y++)
	if(AT(input_img,x,y) == 0)
	  VECT_GET(box_list, i).size++;
  }
}

t_box_vect *list_boxes(t_bw_img *input_img)
{
  t_box_vect *box_list;
  box_list = VECT_ALLOC(box, 16);

  char array[input_img->width * input_img->height];
  for(size_t i = 0; i<input_img->width * input_img->height; i++)
    array[i] = -1;

  for(uint y = 1; y < input_img->height - 1; y++)
    for(uint x = 1; x < input_img->width - 1; x++)
      if (AT(input_img, x, y) == 0 && (array[x + (y) * input_img->width] == -1)
	  && !is_in_box_list(box_list, x, y)){
	box *temp_box;
	temp_box = malloc(20);
	*temp_box = init_box(x, y);
	connected_box_iter(input_img, temp_box, x, y, array, input_img->width);
	expand_box(temp_box, 1);
	VECT_PUSH(box_list, *temp_box);
      }

  return box_list;
}

char are_boxes_entwined(box box1, box box2)
{
  if((box1.top > box2.top && box1.top < box2.bottom) ||
     (box1.bottom > box2.top && box1.bottom < box2.bottom) ||
     (box1.left > box2.left && box1.left < box2.right) ||
     (box1.right < box2.right && box1.right > box2.left))
    return 1;
  return 0;
}

char are_boxes_linked(box box1, box box2)
{
  if(box1.size < box2.size * 1.01 && box1.size < box2.size*0.99)
    return 1;
  return 0;
}

t_box_vect *trim_box_list(t_box_vect *box_list, t_bw_img *input_img)
{
  t_box_vect *new_box_list;
  new_box_list = VECT_ALLOC(box, 16);
  uint min_width = input_img->width / 100;
  uint min_height = input_img->height / 100;
  uint min_size = min_width*min_height;

  for (unsigned int i = 0; i < VECT_GET_SIZE(box_list);i++)
  {
    if ((get_width(VECT_GET(box_list,i))*3 > get_height(VECT_GET(box_list,i))
	 && VECT_GET(box_list,i).size > min_size
	 && get_width(VECT_GET(box_list,i)) > min_width
	 && get_height(VECT_GET(box_list,i)) > min_height)
	&& get_fullsize(VECT_GET(box_list,i))/VECT_GET(box_list,i).size <
      input_img->width*input_img->height/20000)
    {
      VECT_PUSH(new_box_list, VECT_GET(box_list,i));
    }
  }
  return new_box_list;
}

t_box_vect *trim_line_list(t_box_vect *box_list)
{
  t_box_vect *new_box_list;
  new_box_list = VECT_ALLOC(box, 16);

  for (unsigned int i = 0; i < VECT_GET_SIZE(box_list);i++)
  {
    if (get_width(VECT_GET(box_list,i))*3 > get_height(VECT_GET(box_list,i))
	&& get_fullsize(VECT_GET(box_list,i))/VECT_GET(box_list,i).size < 11
	&& get_fullsize(VECT_GET(box_list,i))/VECT_GET(box_list,i).size > 1)
    {
      VECT_PUSH(new_box_list, VECT_GET(box_list,i));
    }
  }
  return new_box_list;
}

t_box_vect *congregate_box_list(t_box_vect *box_list)
{
  t_box_vect *new_box_list;
  new_box_list = VECT_ALLOC(box, 16);
  for (unsigned int i = 0; i < VECT_GET_SIZE(box_list) - 1;i++)
  {
    if (VECT_GET(box_list, i).right > VECT_GET(box_list, i+1).left
      && VECT_GET(box_list, i).bottom > VECT_GET(box_list, i+1).top
      && VECT_GET(box_list, i).top < VECT_GET(box_list, i+1).top){
      VECT_GET(box_list, i+1) =
	fus_boxes(VECT_GET(box_list,i), VECT_GET(box_list,i+1));
    }
    else
      VECT_PUSH(new_box_list, VECT_GET(box_list,i));
  }
  return new_box_list;
}
