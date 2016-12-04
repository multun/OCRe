#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include "neurons.h"
#include "display_cst.h"
#include "../error.h"
#include "../tdefs.h"
#include "../math_macros.h"

ALLOC_IMAGE_DEFINE(nfloat, nfloat_mat)

t_nfloat_mat *get_mat(char *path)
{
  struct stat buf;
  int fd;
  t_nfloat_mat *ret;
  WPERROR(fd = open(path, O_RDONLY), "open");
  WPERROR(fstat(fd, &buf), "fstat");
  WNPERROR(ret = mmap(
	     NULL,
	     (size_t)buf.st_size,
	     PROT_READ,
	     MAP_SHARED,
	     fd,
	     0),
	   "mmap");
  WPERROR(close(fd), "close");
  return ret;
}

int main(int argc, char *argv[])
{
  if(argc < 2)
    FAIL("usage: %s [image]", argv[0]);

  t_nfloat_mat *mat = get_mat(argv[1]);
  for(uint y = 0; y < mat->height; y++)
  {
    for(uint x = 0; x < mat->width; x++)
      printf("%c", '0' + MIN(9, (uint)(AT(mat, x, y) * CF(10.0))));
    puts("");
  }
}
