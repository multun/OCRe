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

void connect_neigh(t_bw_img *input_img, box *input_box, uint x, uint y,char *array, size_t width){
  if (AT(input_img, x+1, y+1) == 0 && (array[x+1 + (y+1) * width] == -1))
    connected_box(input_img, input_box, x+1, y+1, array, width);
  if (AT(input_img, x+1, y) == 0 && (array[x+1 + (y) * width] == -1))
    connected_box(input_img, input_box, x+1, y, array, width);    
  if (AT(input_img, x, y+1) == 0 && (array[x + (y+1) * width] == -1))
    connected_box(input_img, input_box, x, y+1, array, width);
  if (AT(input_img, x-1, y+1) == 0 && (array[x-1 + (y+1) * width] == -1))
    connected_box(input_img, input_box, x-1, y+1, array, width);
  if (AT(input_img, x-1, y) == 0 && (array[x-1 + (y) * width] == -1))
    connected_box(input_img, input_box, x-1, y, array, width);
  if (AT(input_img, x, y-1) == 0 && (array[x + (y-1) * width] == -1))
    connected_box(input_img, input_box, x, y-1, array, width);
  if (AT(input_img, x-1, y-1) == 0 && (array[x-1 + (y-1) * width] == -1))
    connected_box(input_img, input_box, x-1, y-1, array, width);
  if (AT(input_img, x+1, y-1) == 0 && (array[x+1 + (y-1) * width] == -1))
    connected_box(input_img, input_box, x+1, y-1, array, width);
  
}

void connected_box(t_bw_img *input_img, box *input_box, uint x, uint y, char *array, size_t width){
  if((x > 1 && x < input_img->width-2
      && y > 1 && y < input_img->height-2)){
    update_box(input_box, x, y);
    array[x + y * width] = 0;
    //printf("%u %u %d %i ", x, y, AT(input_img, x, y), is_in_box(input_box,x,y));
    //box_print(input_box);
    connect_neigh(input_img, input_box, x, y, array, width);   
  }
}


t_box_vect *list_boxes(t_bw_img *input_img)
{
  t_box_vect *box_list;
  box_list = VECT_ALLOC(box, 1000);
  printf("listboxes %p\n", (void*)input_img);

  char array[input_img->width * input_img->height];
  for(size_t i = 0; i<input_img->width * input_img->height; i++)
    array[i] = -1;
  
  for(uint y = 1; y < input_img->height/2 - 1; y++)
    for(uint x = 1; x < input_img->width/2 - 1; x++)
      if (AT(input_img, x, y) == 0 && (array[x + (y) * input_img->width] == -1)){
	box *temp_box;
	temp_box = malloc(20);
	*temp_box = init_box(x, y);
	//box_print(*temp_box);
	connect_neigh(input_img, temp_box, x, y, array, input_img->width);
	//box_print(*temp_box);
	VECT_PUSH(box_list, *temp_box);
	//VECT_PRINT(box_list);		
      }
  return box_list;
}

void draw_boxes(t_bw_img *bw_img, t_box_vect *box_list)
{
  for (unsigned int i = 0; i < VECT_GET_SIZE(box_list);i++)
    draw_box(bw_img, VECT_GET(box_list, i));
}

