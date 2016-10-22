#ifndef BLOCK_TO_LINES_H
#define BLOCK_TO_LINES_H

typedef enum e_bool
{
  false;
  true;
} t_bool;

typedef struct e_coordinates
{
  int debut;
  int fin;
} t_coordinates

int img_to_array(t_bw_img img);
int array_to_average(int array[]);
t_uint_vect img_to_coordinates(t_bw_img img);
t_uint_vect coordinates_to_img(t_bw_img img, t_uint_vect vect_of_coord);

#endif
