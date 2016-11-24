#include "../bitmap/img.h"
#include "../error.h"
#include "../tdefs.h"
#include <stdio.h>
#include "../base_structs/vector.h"
#include "bounding_box.h"
#include <math.h>

uint get_box_height(box input_box){
  return input_box.bottom - input_box.top + 1;
}
uint get_box_width(box input_box){
  return input_box.right - input_box.left + 1;
}
uint get_box_fullsize(box input_box){
  return get_box_height(input_box) * get_box_width(input_box);
}

t_bw_img *rotate_img(t_bw_img *input, double angle)
{
  int newHeight, newWidth, oldHeight, oldWidth;
  int halfNewHeight, halfNewWidth, nx, ny;
  int halfOldHeight, halfOldWidth;
  double radians, cosval, sinval;
  t_bw_img *output;

  radians = -(angle)/((180/3.142));
  cosval = cos(radians);
  sinval = sin(radians);

  oldHeight = (int)input->height;
  oldWidth = (int)input->width;

  newWidth = (int)abs((int)(oldWidth*cosval)) +
    (int)abs((int)(oldHeight*sinval));
  newHeight = (int)abs((int)(-oldWidth*sinval)) +
    (int)abs((int)(oldHeight*cosval));

  halfNewHeight = newHeight / 2;
  halfNewWidth = newWidth / 2;
  halfOldWidth = oldWidth /2;
  halfOldHeight = oldHeight /2 ;

  output = malloc(sizeof(t_bw_img) +
		  sizeof(unsigned char)*(uint)newWidth*(uint)newHeight);
  output->width	= (uint)newWidth;
  output->height= (uint)newHeight;

  for(int i=0;i < newHeight;i++)
  {
    for(int j=0;j < newWidth;j++)
    {
      nx = (int)( (j - halfNewWidth)*cosval +
		  (i-halfNewHeight)*sinval);
      ny = (int)( -((j - halfNewWidth)*sinval) +
		  (i - halfNewHeight)*cosval);
      nx = nx + halfOldWidth;
      ny = ny + halfOldHeight;
      if ((nx < oldWidth) && (ny < oldHeight)
	  && (nx > 0) && (ny > 0))
      {
	if(AT(input,(uint)nx,(uint)ny)==0)
	  AT(output, (uint)j, (uint)i) = 0;
	else
	  AT(output, (uint)j, (uint)i) = 255;
      }
      else
      {
	AT(output, (uint)j, (uint)i) = 255;
      }
    }
  }
  return output;

}

void filter_box_list(t_box_vect *box_list, t_bw_img *input_img)
{
  t_box_vect *new_box_list;
  new_box_list = VECT_ALLOC(box, 16);
  uint min_height = input_img->height / 400;

  for (unsigned int i = 0; i < VECT_GET_SIZE(box_list);i++)
  {
    if ((get_box_width(VECT_GET(box_list,i))*2 >
	 get_box_height(VECT_GET(box_list,i))
	 && get_box_width(VECT_GET(box_list,i)) <
	 get_box_height(VECT_GET(box_list,i))*2
	 && get_box_height(VECT_GET(box_list,i)) > min_height))
      VECT_PUSH(new_box_list, VECT_GET(box_list,i));
  }
  box_list = new_box_list;
}

uint ver_overlap(box box1, box box2)
{
  uint overlap = 0;
  if(box2.top > box1.top && box2.top < box1.bottom)
  {
    if(box2.bottom < box1.bottom)
      overlap = box2.bottom - box2.top;
    else
      overlap = box1.bottom - box2.top;
  }
  return overlap;
}

int get_biggest_overlap(t_box_vect *row_list, box input_box)
{
  uint index = 0;
  uint i = 1;
  uint max_overlap = ver_overlap(VECT_GET(row_list,0), input_box);
  while(VECT_GET(row_list,i).top < input_box.bottom
	&& i < VECT_GET_SIZE(row_list))
  {
    if(ver_overlap(VECT_GET(row_list,i), input_box)> max_overlap)
    {
      index = i;
      max_overlap = ver_overlap(VECT_GET(row_list,i), input_box);
    }
    i++;
  }
  if(max_overlap == 0)
    return -1;
  return (int)index;
}


t_box_vect *get_rows(t_box_vect *box_list)
{
  t_box_vect *row_list;
  row_list = VECT_ALLOC(box, 16);

  for (unsigned int i = 0; i < VECT_GET_SIZE(box_list);i++)
  {
    int index = get_biggest_overlap(row_list, VECT_GET(box_list,i));
    if(index == -1)
      VECT_PUSH(row_list, VECT_GET(box_list,i));
    else
    {
      VECT_GET(row_list, index) = fus_boxes(VECT_GET(row_list, index),
					     VECT_GET(box_list,i));
    }
  }
  return row_list;
}

double get_angle(t_box_vect *row_list)
{
  return atan(get_box_height(VECT_GET(row_list,0)) /
		get_box_width(VECT_GET(row_list,0)));
}

void box_swap(t_box_vect *box_list, size_t i, size_t j)
{
  uint tleft, tright, tbottom, ttop, tsize;
  tleft = VECT_GET(box_list, i).left;
  tright = VECT_GET(box_list, i).right;
  tbottom = VECT_GET(box_list, i).bottom;
  ttop = VECT_GET(box_list, i).top;
  tsize = VECT_GET(box_list, i).size;

  VECT_GET(box_list, i).size = VECT_GET(box_list, j).size;
  VECT_GET(box_list, i).left = VECT_GET(box_list, j).left;
  VECT_GET(box_list, i).right = VECT_GET(box_list, j).right;
  VECT_GET(box_list, i).top = VECT_GET(box_list, j).top;
  VECT_GET(box_list, i).bottom = VECT_GET(box_list, j).bottom;

  VECT_GET(box_list, j).size = tsize;
  VECT_GET(box_list, j).left = tleft;
  VECT_GET(box_list, j).right = tright;
  VECT_GET(box_list, j).top = ttop;
  VECT_GET(box_list, j).bottom = tbottom;
}


size_t quick_partition(t_box_vect *A,size_t lo,size_t hi)
{
  size_t mid = lo+(hi-lo)/2;
  uint pivot = VECT_GET(A, mid).left;
  size_t i = lo;
  size_t j = hi+1;
  while(i<j)
  {
    i++;
    while (VECT_GET(A,i-1).left < pivot)
      i = i + 1;

    j = j - 1;
    while (VECT_GET(A,j).left > pivot)
      j = j - 1;

    if(i<=j)
      box_swap(A,i-1,j);
  }
  return j;
}

void quicksort_list_box(t_box_vect *box_list, size_t left, size_t right)
{
  printf("1");
  if (left < right)
  {
    size_t p = quick_partition(box_list, left, right);
    printf("\nfirst\n");
    if(left<p)
    {
      for(size_t i = left; i<=p; i++)
	printf("%u--",VECT_GET(box_list,i).left);
      quicksort_list_box(box_list, left, p);
    }
    printf("\n");
    if(p+1<right)
    {
      for(size_t i = p+1; i<=right; i++)
	printf("%u--",VECT_GET(box_list,i).left);
      quicksort_list_box(box_list, p + 1, right);
    }
  }
}
