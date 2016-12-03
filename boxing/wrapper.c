#include "../bitmap/img.h"
#include "../gtk/render.h"
#include "../boxing/bounding_box.h"
#include "../boxing/morpho.h"
#include "../base_structs/vector.h"
#include "../boxing/skew.h"
#include "../rescale/rescale.h"

t_sub_bw_img_vect *block_segment(t_bw_img *input_img)
{
  t_bw_img *temp1 = alloc_bw_img_twin(input_img);
  t_bw_img *temp2 = alloc_bw_img_twin(input_img);

  close_hor(input_img, temp1, input_img->width/30);

  close_ver(temp1, temp2, input_img->height/60);

  open_ver(temp2,temp1,input_img->height/360);
  open_hor(temp1,temp2,input_img->width/180);


  t_box_vect *block_list;
  block_list = list_boxes(temp2);
  update_true_size(block_list, input_img);
  block_list = trim_box_list(block_list, input_img);
  //quicksort_list_box(block_list,0,VECT_GET_SIZE(block_list)-1);
  //block_list = get_rows(block_list);

  t_sub_bw_img_vect *rv = VECT_ALLOC(sub_bw_img, 1000);
  for (unsigned int i = 0; i < VECT_GET_SIZE(block_list); i++)
  {
    //box_print(VECT_GET(block_list, i));
    t_sub_bw_img *linked = alloc_sub_bw_img(
      input_img,
      VECT_GET(block_list, i).left,
      VECT_GET(block_list, i).top,
      VECT_GET(block_list, i).right - VECT_GET(block_list, i).left + 1,
      VECT_GET(block_list, i).bottom - VECT_GET(block_list, i).top + 1);
    VECT_PUSH(rv, linked);
  }
  return rv;
}
