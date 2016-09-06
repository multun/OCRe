#ifndef  BMP_H
# define BMP_H

#include <stdint.h>

typedef struct __attribute__((__packed__))
s_bmp_file_header
{
  unsigned char	sig_BM[2];
  unsigned int  bfSize;
  uint32_t	unused_zero;
  unsigned int  imageDataOffset;
} t_bmp_file_header;

typedef struct __attribute__((__packed__))
s_bmp_img_header
{
  unsigned int  biSize;
  int           width;
  int           height;
  uint16_t	planes;
  uint16_t	bitPix;
  unsigned int  biCompression;
  unsigned int  biSizeImage;
  int           biXPelsPerMeter;
  int           biYPelsPerMeter;
  unsigned int  biClrUsed;
  unsigned int  biClrImportant;
} t_bmp_img_header;

typedef struct __attribute__((__packed__))
s_bmp_pix
{
  unsigned char b;
  unsigned char g;
  unsigned char r;
} t_bmp_pix;

#endif
