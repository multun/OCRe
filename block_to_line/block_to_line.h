#ifndef BLOCK_TO_LINES_H
#define BLOCK_TO_LINES_H

#include "../base_structs/vector.h"
#include "../bitmap/bmp.h"
#include "../bitmap/img.h"
#include "../gtk/vectors.h"
//#include "../gtk/render.h"

typedef enum e_bool
{
  false,
  true
} t_bool;

typedef struct e_coordinates
{
  int debut;
  int fin;
} t_coordinates;

DECL_NAMED_VECTOR(t_coordinates, coordinates);
DECL_NAMED_VECTOR(unsigned int, uint);

t_sub_bw_img_vect *line_subdivision(t_sub_bw_img *img);
int *img_to_array(t_sub_bw_img *img);
int array_to_average(int *array);
t_coordinates_vect *img_to_coordinates(t_sub_bw_img *img);
t_sub_bw_img_vect *coordinates_to_img(t_sub_bw_img *img,
				      t_coordinates_vect *vect_of_coord);

#endif
