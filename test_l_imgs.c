#include "bitmap/img.h"
#include "gtk/render.h"
#include "boxing/bounding_box.h"
#include "boxing/morpho.h"
#include "base_structs/vector.h"

t_l_color_img_vect *test_block_segmentation(t_color_img *block)
{
  t_l_color_img_vect *rv = VECT_ALLOC(l_color_img, 2);
  for (unsigned int i = 0; i < 2; i++)
  {
    t_l_color_img *linked = alloc_l_color_img(block,
					      i * 5,
					      i * 5,
					      4,
					      4);
    VECT_PUSH(rv, linked);
  }
  return rv;
}


t_l_bw_img_vect *new_test_block_segmentation(t_bw_img *input_img)
{
  t_bw_img *temp;
  temp = alloc_bw_img_twin(input_img);


  uint iter = (temp->height + temp->width)/200;
  temp = close_morph(input_img, iter);

  t_box_vect *block_list;
  block_list = list_boxes(temp);

  
  t_l_bw_img_vect *rv = VECT_ALLOC(l_bw_img, 2);
  for (unsigned int i = 0; i < 2; i++)
  {
    t_l_bw_img *linked = alloc_l_bw_img(input_img,
					VECT_GET(block_list, i).left,
					VECT_GET(block_list, i).top,
					VECT_GET(block_list, i).right - VECT_GET(block_list, i).left,
					VECT_GET(block_list, i).bottom - VECT_GET(block_list, i).top);
    VECT_PUSH(rv, linked);
  }
  return rv;
}
