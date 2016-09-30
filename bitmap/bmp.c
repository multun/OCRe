#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

#include "img.h"
#include "bmp.h"
#include "../error.h"
#include "../memtools.h"

//UNCHECKED
unsigned int getsize(int fd)
{
  struct stat infos;
  fstat(fd, &infos); //UNCHECKED
  return infos.st_size;
}

t_color_pix bgr_to_rgb(t_bmp_pix oldpix)
{
  t_color_pix npix;
  npix.r = oldpix.r;
  npix.g = oldpix.g;
  npix.b = oldpix.b;
  npix.a = 0;
  return npix;
}

t_color_img *parse_bmp(char *buf, unsigned int fsize)
{
  t_bmp_file_header *fheader = (t_bmp_file_header*)buf;
  t_bmp_img_header  *iheader = (t_bmp_img_header*)(buf + sizeof(t_bmp_file_header));

  if ((sizeof(t_bmp_file_header) + sizeof(t_bmp_img_header)) >= fsize)
    FAIL0("corrupted file");

  unsigned int width  = iheader->width;
  unsigned int height = iheader->height;
  unsigned int size   = sizeof(t_color_pix) * width * height;

  t_color_img *ret	= malloc(sizeof(t_color_img) + size); // UNCHECKED
  ret->width	= width;
  ret->height	= height;

  unsigned int dest_offset = sizeof(t_color_pix) * width * (height - 1);
  t_color_pix *dest_cur = (t_color_pix*)((char*)ret->pixels + dest_offset);
  t_bmp_pix *orig_cur = (t_bmp_pix*)(buf + fheader->imageDataOffset);

  unsigned int offset = 4 - ((ret->width * 3) & 3);

  for(unsigned int line = 0; line < height; line++)
  {
    for(unsigned int col = 0; col < width; col++)
      *dest_cur++ = bgr_to_rgb(*orig_cur++);
    orig_cur = (t_bmp_pix*)((char*)orig_cur + offset);
    dest_cur = (t_color_pix*)((char*)dest_cur - 2 * width * sizeof(t_color_pix));
  }

  return ret;
}

//UNCHECKED
t_color_img *load_bmp(char path[])
{
  int		fd	= open(path, 0, O_RDONLY);
  unsigned int	fsize	= getsize(fd);
  void		*buf	= mmap(
    NULL,
    fsize,
    PROT_READ,
    MAP_PRIVATE,
    fd,
    0);
  t_color_img *ret = parse_bmp(buf, fsize);

  munmap(buf, fsize);
  return ret;
}

t_color_img* alloc_img(unsigned int width, unsigned int height)
{
  t_color_img *ret	= malloc(sizeof(t_color_img) + sizeof(t_color_pix) * width * height);
  ret->width	= width;
  ret->height	= height;
  return ret;
}

t_bw_img* alloc_bw_img(unsigned int width, unsigned int height)
{
  t_bw_img *ret	= malloc(sizeof(t_bw_img) + sizeof(t_bw_pix) * width * height);
  ret->width	= width;
  ret->height	= height;
  return ret;
}

t_bw_img *greyscale(unsigned char (*intensity)(t_color_pix), const t_color_img *orig_img)
{
  unsigned int width = orig_img->width;
  unsigned int height = orig_img->height;
  t_bw_img* img = alloc_bw_img(width, height);

  for(unsigned int x = 0; x < width; x++)
    for(unsigned int y = 0; y < height; y++)
    {
      unsigned int offset = y * width + x;
      img->pixels[offset] = intensity(orig_img->pixels[offset]);
    }
  return img;
}

//UNCHECKED
void free_img(t_color_img* image)
{
  free(image); // UNCHECKED
}

void free_bw_img(t_bw_img* image)
{
  free(image); // UNCHECKED
}
