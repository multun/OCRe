#include "../boxing/bounding_box.h"
#include "../boxing/morpho.h"
#include "../base_structs/vector.h"
#include "../gtk/vectors.h"
#include "wrapper.h"

t_sub_bw_img_vect_vect *line_extraction(t_sub_bw_img_vect *blocks)
{
  printf("Wrapper block_to_line launched\n ");
  t_sub_bw_img_vect_vect *rv = VECT_ALLOC(sub_bw_img_vect,
					  VECT_GET_SIZE(blocks));
  for(size_t i = 0; i < VECT_GET_SIZE(blocks); i++)
  {
    printf("Wrapper: blocks n° %lu\n",i);
    VECT_PUSH(rv, line_subdivision(VECT_GET(blocks, i)));
    printf("block n°%lu pushed\n",i);
  }
  printf("Wrapper does return\n");
  return rv;
}
