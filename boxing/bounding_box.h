#ifndef CLOSING_H_
#define CLOSING_H_

#include "../bitmap/img.h"
#include "../error.h"
#include "../tdefs.h"

typedef struct Box
{
  uint left, right, top, bottom;
}box;


void draw_box(t_bw_img *input_img, box input_box);


#endif
