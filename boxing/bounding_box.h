#ifndef CLOSING_H_
#define CLOSING_H_

#include "../bitmap/img.h"
#include "../error.h"
#include "../tdefs.h"
#include "../base_structs/vector.h"

typedef struct s_tuple_uint
{
  uint x, y;
}t_tuple_uint;

DECL_NAMED_VECTOR(struct s_tuple_uint, tuple_uint);

typedef struct s_box
{
  uint left, right, top, bottom, size;
}box;

DECL_NAMED_VECTOR(struct s_box, box);

box fus_boxes(box box_1,box box_2);

void draw_box(t_bw_img *input_img, box input_box);

void update_box(box *input_box, uint x, uint y);

void update_true_size(t_box_vect *input_box, t_bw_img *input_img);

box init_box(uint x, uint y);

int is_in_box(box input_box, uint x, uint y);

int is_in_box_list(t_box_vect *box_list, uint x, uint y);

void connected_box(t_bw_img *input_img, box *input_box, uint x, uint y, char *array, size_t width);

void connect_neigh(t_bw_img *input_img, box *input_box, uint x, uint y, char *array, size_t width);

t_box_vect *list_boxes(t_bw_img *input_img);

void test_traversal(t_box_vect *box_list);

void box_print(box input_box);

t_box_vect *trim_box_list(t_box_vect *box_list, t_bw_img *input_img);

t_box_vect *trim_line_list(t_box_vect *box_list, t_bw_img *input_img);

t_box_vect *congregate_box_list(t_box_vect *box_list);
#endif
