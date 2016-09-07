#ifndef  BMP_H
# define BMP_H

#include <stdint.h>

typedef struct __attribute__((__packed__))
s_bmp_file_header
{
  unsigned char	sig_BM[2];
  uint32_t	bfSize;
  uint32_t	unused_zero;
  uint32_t	imageDataOffset;
} t_bmp_file_header;

typedef struct __attribute__((__packed__))
s_bmp_img_header
{
  uint32_t	biSize;
  uint32_t      width;
  uint32_t      height;
  uint16_t	planes;
  uint16_t	bitPix;
  uint32_t	biCompression;
  uint32_t	biSizeImage;
  uint32_t      biXPelsPerMeter;
  uint32_t      biYPelsPerMeter;
  uint32_t	biClrUsed;
  uint32_t	biClrImportant;
} t_bmp_img_header;

typedef struct __attribute__((__packed__))
s_bmp_pix
{
  unsigned char b;
  unsigned char g;
  unsigned char r;
} t_bmp_pix;

unsigned int	getsize(int fd);
t_img_pix	bgr_to_rgb(t_bmp_pix oldpix);
struct s_img	*parse_bmp(char *buf, unsigned int fsize);
t_img		*load_bmp(char path[]);
void		free_bmp(t_img* image);


#endif
