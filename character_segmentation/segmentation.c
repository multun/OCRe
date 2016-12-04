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
    if(npos.x >= 0 && npos.y >= 0 && \
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

static t_shape *merge_shape(t_shape *shape1, t_shape *shape2)
{
  shape1->Xmin = MIN(shape1->Xmin, shape2->Xmin);
  shape1->Ymin = MIN(shape1->Ymin, shape2->Ymin);
  shape1->Xmax = MAX(shape1->Xmax, shape2->Xmax);
  shape1->Ymax = MAX(shape1->Ymax, shape2->Ymax);
  free(shape2);
  return shape1;
}

static char are_mergeable(t_shape *shape1, t_shape *shape2)
{
  if((shape1->Xmin<=shape2->Xmin && shape1->Xmax>=shape2->Xmin && \
(float)(shape1->Xmax-shape2->Xmin)>=0.5*(float)(shape2->Xmax-shape2->Xmin)) || \
     (shape2->Xmin<=shape1->Xmin && shape2->Xmax>=shape1->Xmin && \
(float)(shape2->Xmax-shape1->Xmin) >= 0.5*(float)(shape1->Xmax-shape1->Xmin))||\
    ((shape1->Xmin<=shape2->Xmin && shape1->Xmax>=shape2->Xmin)))
    return 1;
  return 0;
}

static void check_merge(t_shape_vect *shapes)
{
  for (unsigned int i = 0; i < (VECT_GET_SIZE(shapes))-1;i++)
  {
    t_shape *shape1 = VECT_GET(shapes, i);
    t_shape *shape2 = VECT_GET(shapes, i+1);
    if(are_mergeable(shape1, shape2))
    {
      VECT_GET(shapes, i) = merge_shape(shape1, shape2);
      VECT_GET(shapes, i+1) = VECT_GET(shapes, i);
    }
  }
}

static t_shape_vect *check_spaces(t_shape_vect *shapes, t_sub_bw_img *img)
{
  t_shape_vect *finalShapes = VECT_ALLOC(shape, 128);
  /*uint allChars = 0;
  for (uint i = 0; i < VECT_GET_SIZE(shapes); i++)
    allChars += VECT_GET(shapes, i)->Ymax - VECT_GET(shapes, i)->Ymin;
  float avgSpace = ((float)allChars/(float)VECT_GET_SIZE(shapes));*/
  for (uint j = 0; j < (VECT_GET_SIZE(shapes))-1; j++) {
    VECT_PUSH(finalShapes, VECT_GET(shapes, j));
    if ((float)(VECT_GET(shapes, j+1)->Xmin) -
        (float)(VECT_GET(shapes, j)->Xmax) > (float)(img->height)/3.5)
      VECT_PUSH(finalShapes, NULL);
  }
  VECT_PUSH(finalShapes, VECT_GET(shapes, VECT_GET_SIZE(shapes)-1));
  return finalShapes;
}

static t_int_mat *build_shape_vect(t_sub_bw_img *img, t_shape_vect **shapes)
{
  t_int_mat *mat = alloc_int_mat(img->width, img->height);
  *shapes = VECT_ALLOC(shape, 128);
  for (uint i = 0; i < img->width; i++)
    for (uint j = 0; j < img->height; j++)
      AT(mat, i, j) = (SUB_AT(img, i, j) == 0) ? -1 : 0;

  for(uint x = 0; x < img->width; x++)
    for(uint y = 0; y < img->height; y++)
      if(AT(mat, x, y) == -1)
	add_shape(mat, x, y, *shapes);


  check_merge(*shapes);
  t_shape_vect *nshapes = check_spaces(*shapes, img);
  VECT_FREE(*shapes);
  *shapes = nshapes;
  return mat;
}

t_l_bw_img_vect *char_segmentation_l(t_sub_bw_img *img)
{
  t_l_bw_img_vect *result = VECT_ALLOC(l_bw_img, 64);

  t_shape_vect *shapes;
  t_int_mat *mat = build_shape_vect(img, &shapes);

  for (unsigned int i = 0; i < VECT_GET_SIZE(shapes);i++)
  {
    t_shape *tshp = VECT_GET(shapes, i);
    if(!tshp)
    {
      VECT_PUSH(result, NULL);
      continue;
    }

    t_l_bw_img *sub = alloc_l_bw_img(img->father,
				     img->xoff + tshp->Xmin,
				     img->yoff + tshp->Ymin,
				     tshp->Xmax - tshp->Xmin + 1,
				     tshp->Ymax - tshp->Ymin + 1);

    for(uint x = 0; x < sub->width; x++)
      for(uint y = 0; y < sub->height; y++)
      {
    /*    int cp = AT(mat, tshp->Xmin + x, tshp->Ymin + y);
        if(VECT_GET(shapes, cp - 1) != tshp)
          L_AT(sub, x, y) = 255;
	  else*/
	  L_AT(sub, x, y) = SUB_AT(img, tshp->Xmin + x, tshp->Ymin + y);
      }
    VECT_PUSH(result, sub);
  }
  free(mat);
  return result;
}

t_sub_bw_img_vect *char_segmentation(t_sub_bw_img *img)
{
  t_sub_bw_img_vect *result = VECT_ALLOC(sub_bw_img, 64);

  t_shape_vect *shapes;
  t_int_mat *mat = build_shape_vect(img, &shapes);

  for (unsigned int i = 0; i < VECT_GET_SIZE(shapes);i++)
  {
    t_shape *tshp = VECT_GET(shapes, i);
    if(!tshp)
      continue;
    t_sub_bw_img *sub = relink_sub_bw_img(
      img,
      tshp->Xmin,
      tshp->Ymin,
      tshp->Xmax - tshp->Xmin + 1,
      tshp->Ymax - tshp->Ymin + 1);

    VECT_PUSH(result, sub);
  }
  free(mat);
  return result;
}
