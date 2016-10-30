#ifndef BLOCK_TO_LINES_H
#define BLOCK_TO_LINES_H

#include "../base_structs/vector.h"
#include "../bitmap/bmp.h"
#include "../bitmap/img.h"
#include "../gtk/vectors.h"

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
int* img_to_array(t_sub_bw_img *img);
int avgblackpxlperline(int *array, t_sub_bw_img *img);
t_bool *bool_array_generation(int average, int *array, t_sub_bw_img *img);
t_coordinates_vect *bool_array_to_coordinates(t_bool *bool_array, t_sub_bw_img *img);
t_sub_bw_img_vect *coordinates_to_img(t_coordinates_vect *vectorofcoordinates,
                                      t_sub_bw_img *img);


#endif
