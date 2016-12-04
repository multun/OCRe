#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include "../bitmap/bmp.h"
#include "../error.h"
#include "../tdefs.h"
#include "../binarisation/bin_treshold.h"
#include "../base_structs/vector.h"
#include "../bitmap/img.h"
#include "simple_segmentation.h"
#include "../rescale/rescale.h"
#include "../neural_network/neurons.h"

/*
 * As we do care about performance here, no memory
 * is freed.
 */


IMAGE_DECLARE(nfloat, nfloat_mat)
ALLOC_IMAGE_DEFINE(nfloat, nfloat_mat)

int main(int argc, char *argv[])
{
  if(argc < 4)
    FAIL("usage: %s in.bmp size out", *argv);

  const uint res_size = (uint)strtoul(argv[2], NULL, 10);
  t_color_img	*input	= load_bmp(argv[1]);

  t_bw_img *bw_img = bin_treshold(127, input);

  t_sub_bw_img sub_bw_img = {
    bw_img,
    bw_img->width,
    bw_img->height,
    0,
    0
  };

  for(uint x = 0; x < bw_img->width; x++)
    if (!(AT(bw_img, x, 0) || AT(bw_img, x, bw_img->height - 1)))
      goto faillol;

  for(uint y = 0; y < bw_img->height; y++)
    if (!(AT(bw_img, 0, y) || AT(bw_img, bw_img->width - 1, y)))
      goto faillol;

  t_sub_bw_img_vect *chars = simple_char_segmentation(&sub_bw_img);
  if(!VECT_GET_SIZE(chars))
  {
    puts("No character detected");
    return 0;
  }

  t_sub_bw_img *res = VECT_GET(chars, 0);

  t_bw_img *bwret = alloc_bw_img(res->width, res->height);

  for(uint i = 0; i < res->height; i++)
    for(uint j = 0; j < res->width; j++)
      AT(bwret, j, i) = SUB_AT(res, j, i);

  t_bw_img *sized_res = resize(bwret, res_size, res_size);

  t_nfloat_mat * fres = alloc_nfloat_mat(res_size, res_size);

  for(uint i = 0; i < res_size * res_size; i++)
    fres->pixels[i] = ((double)(255-sized_res->pixels[i]))/255;

  int fd = open(argv[3], O_WRONLY | O_CREAT, 0644);
  write(fd, fres, IMG_SIZE(t_nfloat_mat, fres->width, fres->height));
  close(fd);
  return 0;

faillol:
  puts("overflowing character");
  return 0;
}
