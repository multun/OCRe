#include "segmentation.h"
#include "../math_macros.h"

t_sub_bw_img_vect *char_segmentation(t_sub_bw_img *img)
{
  t_sub_bw_img_vect *result = VECT_ALLOC(sub_bw_img, 32);
  t_shape_vect *shapes = VECT_ALLOC(shape, 16);

  int mat[img->width][img->height];
  for (uint i = 0; i < img->width; i++) {
    for (uint j = 0; j < img->height; j++) {
      if (SUB_AT(img, i, j) = 0)
        mat[i][j] = -1;
      else
        mat[i][j] = 0;
    }
  }

  for(uint x = 0; x < img->width; x++)
	  for(uint y = 0; y < img->height; y++)
		  if(mat[x][y] == -1)
			  add_shape(img, x, y, shapes, mat);

  for (unsigned int i = 0; i < VECT_GET_SIZE(shapes);i++){
    t_shape *tempShape = VECT_GET(shapes, i);
    t_sub_bw_img *sub = relink_sub_bw_img(
  	  img,
  	  tempShape->Xmin,
  	  tempShape->Ymin,
  	  tempShape->Xmax,
  	  tempShape->Ymax);
    VECT_PUSH(result, sub);
  }
  return result;
}

//labels all connected pixels of the pixel given in parameters with a label
void taint(t_sub_bw_img *img, uint x, uint y, int label, int mat[][])
{
  int neighbours[2][] = {
    {-1, -1},
	  {-1, 0},
	  {-1, 1},
	  {0, -1},
	  {0, 1},
	  {1, -1},
	  {1, 0},
	  {1, 1},
  };
  if(mat[x][y] == -1)
	  mat[x][y] = label;
  for (uint i = 0; i < sizeof(neighbours)/sizeof(neighbours[0]); i++) {
    int offsets[2] = neighbours[i];
    int npos[2] = {(int)x + offsets[0], (int)y + offsets[1]};
    if(npos[0]>0&&npos[1]>0&&npos[0]<img->width&&npos[1]<img->height)
		  taint(img, (uint)npos[0], (uint)npos[1], label, mat);
  }
}

void update_shape(t_shape *shapes, uint x, uint y)
{
  shapes->Xmin = MIN(shapes->Xmin, x);
  shapes->Xmax = MAX(shapes->Xmax, x);
  shapes->Ymin = MIN(shapes->Ymin, y);
  shapes->Ymax = MAX(shapes->Ymax, y);
}

void add_shape(t_sub_bw_img *img,uint x,uint y,t_shape_vect *shapes,int mat[][])
{
	t_shape *ret = malloc(sizeof(t_shape));
	VECT_PUSH(shapes, ret);
	taint(img, x, y, VECT_GET_SIZE(shapes)-1, mat);
}
