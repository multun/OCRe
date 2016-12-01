#ifndef SKEW_H_
#define SKEW_H_

#include "../bitmap/img.h"
#include "../error.h"
#include "../tdefs.h"
#include <stdio.h>
#include "../base_structs/vector.h"
#include "bounding_box.h"
#include <math.h>

t_bw_img *rotate_img(t_bw_img *input, double angle);

t_box_vect *filter_box_list(t_box_vect *box_list, t_bw_img *input_img);

t_box_vect *get_rows(t_box_vect *box_list);

double get_angle(t_box_vect *row_list);

void box_swap(t_box_vect *box_list, size_t i, size_t j);

size_t quick_partition(t_box_vect *A,size_t lo,size_t hi);

void quicksort_list_box(t_box_vect *box_list, size_t left, size_t right);

double get_rotation(t_bw_img *input, double precision);

double get_ray(t_bw_img *input, uint y, double angle);

double get_variance(t_bw_img *input, double angle);

#endif
