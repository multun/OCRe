#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "img.h"
#include "bmp.h"
#include "../tdefs.h"
#include "../error.h"

//UNCHECKED
static luint getsize(int fd)
{
  struct stat infos;
  fstat(fd, &infos); //UNCHECKED
  return (luint)infos.st_size;
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

t_color_img *parse_bmp(char *buf, luint fsize)
{
  t_bmp_file_header *fheader = (t_bmp_file_header*)buf;
  t_bmp_img_header  *iheader = (t_bmp_img_header*)(buf + sizeof(t_bmp_file_header));

  const char sig[] = "BM";
  if (fsize < 3 || memcmp(fheader->sig_BM, sig, 2) != 0)
    FAIL0("not a BMP file");

  if ((sizeof(t_bmp_file_header) + sizeof(t_bmp_img_header)) >= fsize)
    FAIL0("corrupted file: smaller than minimum size");

  uint width  = iheader->width;
  uint height = iheader->height;
  luint size  = sizeof(t_color_pix) * width * height;

  if (width * height * sizeof(t_bmp_pix) > (fsize - sizeof(t_bmp_file_header) - sizeof(t_bmp_img_header)))
    FAIL0("corrupted file: invalid size");

  t_color_img *ret	= malloc(sizeof(t_color_img) + size); // UNCHECKED
  ret->width	= width;
  ret->height	= height;

  luint dest_offset = sizeof(t_color_pix) * width * (height - 1);
  t_color_pix *dest_cur = (t_color_pix*)((char*)ret->pixels + dest_offset);
  t_bmp_pix *orig_cur = (t_bmp_pix*)(buf + fheader->imageDataOffset);

  uint offset = (4 - width % 4) % 4;

  for(uint line = 0; line < height; line++)
  {
    for(uint col = 0; col < width; col++)
      *dest_cur++ = bgr_to_rgb(*orig_cur++);
    orig_cur = (t_bmp_pix*)((char*)orig_cur + offset);
    dest_cur = (t_color_pix*)((char*)dest_cur - 2 * width * sizeof(t_color_pix));
  }

  return ret;
}

//UNCHECKED
t_color_img *load_bmp(char path[])
{
  int   fd	= open(path, 0, O_RDONLY);
  luint	fsize	= getsize(fd);
  void	*buf	= mmap(
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
