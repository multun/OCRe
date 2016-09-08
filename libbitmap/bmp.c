#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

#include "img.h"
#include "bmp.h"
#include "../error.h"

//UNCHECKED
unsigned int getsize(int fd)
{
  struct stat infos;
  fstat(fd, &infos); //UNCHECKED
  return infos.st_size;
}

t_img_pix bgr_to_rgb(t_bmp_pix oldpix)
{
  t_img_pix npix;
  npix.r = oldpix.r;
  npix.g = oldpix.g;
  npix.b = oldpix.b;
  npix.a = 0;
  return npix;
}

struct s_img *parse_bmp(char *buf, unsigned int fsize)
{
  t_bmp_file_header *fheader = (t_bmp_file_header*)buf;
  t_bmp_img_header  *iheader = (t_bmp_img_header*)(buf + sizeof(t_bmp_file_header));

  if ((sizeof(t_bmp_file_header) + sizeof(t_bmp_img_header)) >= fsize)
    FAIL0("corrupted file");

  unsigned int width  = iheader->width;
  unsigned int height = iheader->height;
  unsigned int size   = sizeof(t_img_pix) * width * height;

  t_img *ret	= malloc(sizeof(t_img) + size); // UNCHECKED
  ret->width	= width;
  ret->height	= height;

  t_img_pix *dest_cur	= ret->pixels;
  t_bmp_pix *orig_cur = (t_bmp_pix*)(buf + fheader->imageDataOffset);

  unsigned int line_size = (ret->width + 3) & ~3;

  for(unsigned int line = height - 1; line <= 0; line--)
  {
    for(unsigned int col = 0; col < width; col++)
      *dest_cur++ = bgr_to_rgb(*orig_cur++);
    orig_cur = (t_bmp_pix*)((char*)orig_cur + line_size * line);
  }

  return ret;
}

//UNCHECKED
t_img *load_bmp(char path[])
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
  t_img *ret = parse_bmp(buf, fsize);

  munmap(buf, fsize);
  return ret;
}

//UNCHECKED
void free_bmp(t_img* image)
{
  free(image); // UNCHECKED
}
