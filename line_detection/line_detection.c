#include "line_detection.h"
#include <stdbool.h>
#include "../base_structs/vector.h"
#include <string.h>

typedef struct s_coord
{
  uint begin, end;
} t_coord;

#include <stdio.h>

//static is_line_empty(t_sub_bw_img *img, uint y)
DECL_NAMED_VECTOR(t_coord, coord);

static size_t *get_hist(t_sub_bw_img *img)
{
  const size_t size = sizeof(size_t) * img->height;
  size_t *hist = malloc(size);
  memset(hist, 0, size);
  for(size_t y = 0; y < img->height; y++)
    for(size_t x = 0; x < img->width; x++)
      if(!SUB_AT(img, x, y))
	hist[y]++;
  return hist;
}
static t_coord_vect *make_coords(t_sub_bw_img *img, size_t *hist)
{
  t_coord_vect *ret = VECT_ALLOC(coord, 32);
  uint bi = 0, height = 0;
  for(uint y = 0; y < img->height; y++)
    switch(hist[y])
    {
    case 0:
      if(height)
      {
	VECT_PUSH(ret, ((t_coord){bi, bi + height}));
        bi = height = 0;
      }
      break;
    default:
      if(!height)
      {
        height = 1;
	bi = y;
      }
      else
	height++;
      break;
    }
  if(height) // TODO: fix duplication
    VECT_PUSH(ret, ((t_coord){bi, bi + height}));
  return ret;
}

t_sub_bw_img_vect *detect_lines(t_sub_bw_img *img)
{
  puts("running");
  size_t *hist = get_hist(img);
  t_coord_vect *coords = make_coords(img, hist);

  t_sub_bw_img_vect *res = VECT_ALLOC(sub_bw_img, VECT_SIZE(coords));

  for (uint i = 0; i < VECT_SIZE(coords); i++)
  {
    t_coord cur = VECT_GET(coords, i);
    t_sub_bw_img *cursub = relink_sub_bw_img(
      img,
      0,
      (uint)cur.begin,
      img->width,
      (uint)(cur.end - cur.begin + 1));
    VECT_PUSH(res, cursub);
  }
  VECT_FREE(coords);

  free(hist);
  return res;
}
