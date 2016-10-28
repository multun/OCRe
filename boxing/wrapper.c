#include "../bitmap/img.h"
#include "../gtk/render.h"
#include "../boxing/bounding_box.h"
#include "../boxing/morpho.h"
#include "../base_structs/vector.h"

t_l_bw_img_vect *block_segment(t_bw_img *input_img)
{
  t_bw_img *temp;

  temp = close_hor_morph(input_img, input_img->width/65);

  //  Comment next line for : Only Lines instead of blocks
  temp = close_ver_morph(temp, input_img->height/150);
  //temp = open_morph(temp, input_img->width/180);

  
  t_box_vect *block_list;
  block_list = list_boxes(temp);

  t_l_bw_img_vect *rv = VECT_ALLOC(l_bw_img, 1000);
  for (unsigned int i = 0; i < VECT_GET_SIZE(block_list); i++)
  {
    //box_print(VECT_GET(block_list, i));
    t_l_bw_img *linked = alloc_l_bw_img(input_img,
					VECT_GET(block_list, i).left,
					VECT_GET(block_list, i).top,
					VECT_GET(block_list, i).right - VECT_GET(block_list, i).left + 1,
					VECT_GET(block_list, i).bottom - VECT_GET(block_list, i).top + 1);
    VECT_PUSH(rv, linked);
    }
  printf("%p\n", (void*)VECT_GET(rv, 0)->father);
  return rv;
}
