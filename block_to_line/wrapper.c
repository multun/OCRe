#include "../boxing/bounding_box.h"
#include "../boxing/morpho.h"
#include "../base_structs/vector.h"
#include "../gtk/vectors.h"
#include "wrapper.h"

t_sub_bw_img_vect_vect *line_extraction(t_sub_bw_img_vect *blocks)
{
  t_sub_bw_img_vect_vect *rv = VECT_ALLOC(sub_bw_img_vect,
					  VECT_GET_SIZE(blocks));
  for(size_t i = 0; i < VECT_GET_SIZE(blocks); i++)
  {
    //printf("§§§§§§§§§§§§§§§§   pushing block nbr %lu   §§§§§§§§§§§§§§§§§\n",i);
    //printf("block size: height is %d, width is %d\n",VECT_GET(blocks,i)->height,VECT_GET(blocks,i)->width);
    VECT_PUSH(rv, line_subdivision(VECT_GET(blocks, i)));
  }
  return rv;
}
