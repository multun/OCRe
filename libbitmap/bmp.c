#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

#include "img.h"
#include "bmp.h"

void FAIL(char*s)
{
  (void)s;
}

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
  t_bmp_img_header  *iheader;

  if (sizeof(t_bmp_file_header) >= fsize || fheader->bfSize != fsize)
    goto corrupted;

  iheader = (t_bmp_img_header*)(buf + fheader->imageDataOffset);

  if (fheader->imageDataOffset + iheader->biSizeImage < fsize)
    goto corrupted;

  t_img *ret	= malloc(sizeof(t_img)); // UNCHECKED
  ret->width	= iheader->width;
  ret->height	= iheader->height;

  return ret;
corrupted:
  FAIL("corrupted file");
  return NULL;
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
