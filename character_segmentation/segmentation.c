#include "segmentation.h"

void stack_init(struct stack *stack)
{
  stack->head = NULL;
}

void stack_push(struct stack *stack,uint x, uint y)
{
  struct list *newhead = malloc(1 * sizeof(struct list));
  newhead->x = x;
  newhead->y = y;
  newhead->next = stack->head;
  stack->head = newhead;
  stack->size += 1;
  free(newhead);
}

struct list *stack_pop(struct stack *stack)
{
  if (stack->head == NULL)
    printf("ERROR: poped nothing\n");
    return stack->head;
  struct list *temp = stack->head;
  stack->head = stack->head->next;
  stack->size -= 1;
  return temp;
}

char is_white_column(t_sub_bw_img *img, uint column)
{
  for(uint i = 0; i< img->height; i++)
    if(SUB_AT(img,column,i) == 0)
      return 0;
  return 1;
}

char is_white_line(t_sub_bw_img *img, uint line)
{
  for(uint i = 0; i< img->width; i++)
    if(SUB_AT(img,i,line) == 0)
      return 0;
  return 1;
}

void char_vseg(t_sub_bw_img *lsub)
{
  while(lsub->height && is_white_line(lsub, 0))
  {
    lsub->yoff++;
    lsub->height--;
  }
  while(lsub->height && is_white_line(lsub, lsub->height - 1))
    lsub->height--;
}


struct coords grid_to_coords(uint x, uint y, uint curlab, uint grid[x][y])
{
  struct coords newCoords;
  newCoords.Xmin = 0;
  newCoords.Ymin = 0;
  newCoords.Xmax = x-1;
  newCoords.Ymax = y-1;
  for (uint i = 0; i < x; i++) {
    for (uint j = 0; j < y; j++) {
      if (newCoords.Xmin == 0 && grid[i][j] == curlab)
        newCoords.Xmin = i;
      if (newCoords.Ymin == 0 && grid[i][j] == curlab)
        newCoords.Ymin = j;
      if (grid[i][j] == curlab)
        if (newCoords.Xmax <= i)
          newCoords.Xmax = i;
        if (newCoords.Ymax <= j) {
          newCoords.Ymax = j;
        }
    }
  }
  return newCoords;
}


t_sub_bw_img_vect *char_segmentation(t_sub_bw_img *img)
{/*
  t_sub_bw_img_vect *result = VECT_ALLOC(sub_bw_img, 32);

  for(uint aux = 0, i = 0; i < img->width; i++)
    if (is_white_column(img, i) || i == img->width-1)
    {
      if(is_white_column(img, i-1) == 0)
      {
	       t_sub_bw_img *sub = relink_sub_bw_img(img,aux,0,i - aux,img->height);
	        char_vseg(sub);
	       VECT_PUSH(result, sub);
      }
      aux = i+1;
    }
  return result;*/

  struct stack *stack = malloc(sizeof(struct stack));
  struct list *pixel;
  t_sub_bw_img_vect *result = VECT_ALLOC(sub_bw_img, 32);
  stack_init(stack);
  uint grid[img->width][img->height];
  for (uint a = 0; a < img->width; a++) {
    for (uint b = 0; b < img->height; b++)
      grid[a][b] = 0;
  }
  uint curlab = 1, m, n;
  grid[0][0] = curlab;
  printf("%d\n", img->width);
  printf("%d\n", img->height);
  for (uint i = 0; i < img->width; i++) {
    for (uint j = 0; j < img->height; j++) {
      //if the pixel is black and was not visited before
      if (SUB_AT(img, i, j) == 0 && grid[i][j] == 0) {
        //give it the current label
        grid[i][j] = curlab;
        //push it in the stack
        stack_push(stack,i,j);
        //pop it off the stack
        pixel = stack_pop(stack);
        m = pixel->x;
        n = pixel->y;
        //Check the neighbours
        for (uint row = m-1; row <= m+1; row++) {
          for (uint col = n-1; col <= n+1; col++) {
            if (!(m == row && n == col) && row >= 1 && col>= 1 && row < img->width && col < img->height)
            {
              //if the neighbour is black and was not already visited
              if (SUB_AT(img, row, col) == 0 && grid[row][col] == 0)
              {
                grid[row][col] = curlab;
                stack_push(stack, row, col);
              }
            }
          }
        }
      }
      curlab++;
    }
  }
  for (uint z = 1; z <= curlab; z++) {
    struct coords newCoords = grid_to_coords((img->width)-1, (img->height)-1, z, grid);
    t_sub_bw_img *sub = relink_sub_bw_img(
  	  img,
  	  newCoords.Xmin,
  	  newCoords.Ymin,
  	  newCoords.Xmax,
  	  newCoords.Ymax);
    VECT_PUSH(result, sub);
  }
  return result;
}
