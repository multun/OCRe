#ifndef  IMG_H
# define IMG_H

typedef struct
__attribute__((__packed__))
s_img_pix
{
  unsigned char r;
  unsigned char g;
  unsigned char b;
  unsigned char a;
} t_img_pix;

typedef struct  s_img
{
  unsigned int	width;
  unsigned int	height;
  struct s_img_pix   pixels[];
} t_img;


#endif
