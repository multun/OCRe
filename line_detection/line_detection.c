#include "line_detection.h"
#include <stdbool.h>
#include "../base_structs/vector.h"
#include <string.h>

#include "../math_macros.h"

typedef struct s_coord
{
  uint begin, end;
} t_coord;

static inline bool col_empty(t_sub_bw_img *sub, uint x)
{
  for(uint y = 0; y < sub->height; y++)
    if(!SUB_AT(sub, x, y))
      return false;
  return true;
}

static void crop_cols(t_sub_bw_img *sub)
{
  for(; sub->width > 0 && col_empty(sub, 0);)
  {
    sub->xoff++;
    sub->width--;
  }

  for(; sub->width > 0 && col_empty(sub, sub->width - 1);)
    sub->width--;
}

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
  if(height) // TODO: fix duplication, recode this function
    VECT_PUSH(ret, ((t_coord){bi, bi + height}));
  return ret;
}

#define HEIGHT(coord) ((size_t)((coord).end - (coord).begin))

static size_t average_height(t_coord_vect *coords)
{
  size_t total = 0;
  for(size_t i = 0; i < VECT_SIZE(coords); i++)
  {
    t_coord cur = VECT_GET(coords, i);
    total += HEIGHT(cur);
  }
  return total / VECT_SIZE(coords);
}


static inline void merge_couple(t_coord_vect *coords, size_t i, size_t j)
{
  t_coord ic = VECT_GET(coords, i);
  t_coord jc = VECT_GET(coords, j);

  size_t biggest   = (HEIGHT(ic) > HEIGHT(jc)) ? i : j;
  size_t smallest  = (biggest == i) ? j : i;
  t_coord *target = &VECT_GET(coords, biggest);


  target->begin = MIN(ic.begin, jc.begin);
  target->end   = MAX(ic.end, jc.end);

  VECT_SIZE(coords)--;
  for(size_t k = smallest; k < VECT_SIZE(coords); k++)
    VECT_GET(coords, k) = VECT_GET(coords, k + 1);
}

static inline size_t coord_dist(t_coord_vect *coords, size_t first, size_t last)
{
  return (size_t)(VECT_GET(coords, last).begin - VECT_GET(coords, first).end);
}


static void merge_coords(t_coord_vect *coords)
{
  size_t treshold = average_height(coords) / 2;

  for(size_t i = 0; i < VECT_SIZE(coords); i++)
  {
    if(HEIGHT(VECT_GET(coords, i)) > treshold)
      continue;
    size_t merge_target;
    if(i == 0)
      merge_target = i + 1;
    else if (i == VECT_SIZE(coords) - 1)
      merge_target = i - 1;
    else
    {
      if (coord_dist(coords, i - 1, i) > coord_dist(coords, i, i + 1))
	merge_target = i + 1;
      else
	merge_target = i - 1;
    }
    merge_couple(coords, i, merge_target);
  }
}


t_sub_bw_img_vect *detect_lines(t_sub_bw_img *img)
{
  size_t *hist = get_hist(img);
  t_coord_vect *coords = make_coords(img, hist);
  merge_coords(coords);
  t_sub_bw_img_vect *res = VECT_ALLOC(sub_bw_img, VECT_SIZE(coords));

  for (uint i = 0; i < VECT_SIZE(coords); i++)
  {
    t_coord cur = VECT_GET(coords, i);
    t_sub_bw_img *cursub = relink_sub_bw_img(
      img,
      0,
      (uint)cur.begin,
      img->width,
      (uint)(cur.end - cur.begin));
    crop_cols(cursub);
    VECT_PUSH(res, cursub);
  }
  VECT_FREE(coords);

  free(hist);
  return res;
}
