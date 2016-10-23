#ifndef CLOSING_H_
#define CLOSING_H_

#include "../bitmap/img.h"
#include "../error.h"
#include "../tdefs.h"
#include "../base_structs/vector.h"

typedef struct Box
{
  uint left, right, top, bottom, size;
}box;

DECL_NAMED_VECTOR(struct Box, box);

void draw_box(t_bw_img *input_img, box input_box);

void update_box(box *input_box, uint x, uint y);

box init_box(uint x, uint y);

int is_in_box(box input_box, uint x, uint y);

int is_in_box_list(t_box_vect *box_list, uint x, uint y);

void connected_box(t_bw_img *input_img, box *input_box, uint x, uint y, char *array, size_t width);

void connect_neigh(t_bw_img *input_img, box *input_box, uint x, uint y, char *array, size_t width);

t_box_vect *list_boxes(t_bw_img *input_img);

void draw_boxes(t_bw_img *bw_img, t_box_vect *box_list);

void box_print(box input_box);

#endif
