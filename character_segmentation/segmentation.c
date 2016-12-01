#include "segmentation.h"
#include "../math_macros.h"

IMAGE_DECLARE(int, int_mat)
ALLOC_IMAGE_DEFINE(int, int_mat)

typedef struct s_off_c
{
  int x, y;
} t_off_c;

static void update_shape(t_shape *shape, uint x, uint y)
{
  shape->size++;
  shape->Xmin = MIN(shape->Xmin, x);
  shape->Xmax = MAX(shape->Xmax, x);
  shape->Ymin = MIN(shape->Ymin, y);
  shape->Ymax = MAX(shape->Ymax, y);
}

static void taint(t_shape *shape, t_int_mat *mat, uint x, uint y, int label)
{
  t_off_c const neighbours[] = {
    {-1, -1},
    {-1, 0},
    {-1, 1},
    {0, -1},
    {0, 1},
    {1, -1},
    {1, 0},
    {1, 1},
  };

  switch(AT(mat, x, y))
  {
  case 0:
  default:
    return;
  case -1:
    break;
  }

  update_shape(shape, x, y);
  AT(mat, x, y) = label;
  for (uint i = 0; i < sizeof(neighbours)/sizeof(neighbours[0]); i++)
  {
    t_off_c npos = neighbours[i];
    npos.x += (int)x;
    npos.y += (int)y;
    if(npos.x > 0 && npos.y > 0 && \
       npos.x < (int)mat->width && npos.y < (int)mat->height)
      taint(shape, mat, (uint)npos.x, (uint)npos.y, label);
  }
}

static void add_shape(t_int_mat *mat,uint x,uint y,t_shape_vect *shapes)
{
  t_shape *ret = malloc(sizeof(t_shape));
  ret->Xmin = ret->Xmax = x;
  ret->Ymin = ret->Ymax = y;
  ret->size = 1;
  VECT_PUSH(shapes, ret);
  taint(VECT_GET_LAST(shapes), mat, x, y, (int)VECT_GET_SIZE(shapes));
}

t_sub_bw_img_vect *char_segmentation(t_sub_bw_img *img)
{
  t_sub_bw_img_vect *result = VECT_ALLOC(sub_bw_img, 32);
  t_shape_vect *shapes = VECT_ALLOC(shape, 16);

  t_int_mat *mat = alloc_int_mat(img->width, img->height);

  for (uint i = 0; i < img->width; i++)
    for (uint j = 0; j < img->height; j++)
      AT(mat, i, j) = (SUB_AT(img, i, j) == 0) ? -1 : 0;

  for(uint x = 0; x < img->width; x++)
    for(uint y = 0; y < img->height; y++)
      if(AT(mat, x, y) == -1)
	add_shape(mat, x, y, shapes);

  for (unsigned int i = 0; i < VECT_GET_SIZE(shapes);i++)
  {
    t_shape *tempShape = VECT_GET(shapes, i);
    t_sub_bw_img *sub = relink_sub_bw_img(
      img,
      tempShape->Xmin,
      tempShape->Ymin,
      tempShape->Xmax - tempShape->Xmin,
      tempShape->Ymax - tempShape->Ymin);
    VECT_PUSH(result, sub);
  }
  return result;
}
