#ifndef SEGMENT_H_
#define SEGMENT_H_

#include "../base_structs/vector.h"
#include "../bitmap/bmp.h"
#include "../bitmap/img.h"
#include "../tdefs.h"
#include "../error.h"
#include "../gtk/vectors.h"

struct list
{
  struct list *next;
  uint x;
  uint y;
};

struct stack
{
  struct list *head;
  size_t size;
};

struct coords
{
  uint Xmin;
  uint Ymin;
  uint Xmax;
  uint Ymax;
};

void stack_init(struct stack *stack);
void stack_push(struct stack *stack,uint x,uint y);
struct list *stack_pop(struct stack *stack);

char is_white_column(t_sub_bw_img *img, uint column);
char is_white_line(t_sub_bw_img *img, uint line);
void char_vseg(t_sub_bw_img *lsub);

struct coords grid_to_coords(uint x, uint y, uint curlab, uint grid[x][y]);

t_sub_bw_img_vect *char_segmentation(t_sub_bw_img *img);

#endif
