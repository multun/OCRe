#include <stdio.h>
#include "bitmap/img.h"
#include "bitmap/bmp.h"
#include "binarisation/binarise.h"
#include "memtools.h"
#include "error.h"
#include "boxing/morpho.h"


int main(int argc, char *argv[])
{
  if (argc < 2)
    FAIL("use: %s image", argv[0]);
  t_color_img *img = load_bmp(argv[1]);

  t_bw_img *bw_img = binarise(SAUVOLA, img);

  bw_img = dilate(bw_img);

  for(unsigned int y = 0; y < img->height; y++)
  {
    for(unsigned int x = 0; x < img->width; x++)
      {
	printf("%d %d\n", x, y);
	printf((AT(bw_img,x,y) ? "#" : "."));
      }
    printf("\n");
  }

  free_bw_img(bw_img);
  free_color_img(img);
  return 0;
}
