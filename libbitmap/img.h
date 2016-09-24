#ifndef  IMG_H
# define IMG_H


#define IMAGE_DECLARE(PIX_TYPE, NAME)		\
  typedef struct s_ ##NAME			\
  {						\
    unsigned int	width;			\
    unsigned int	height;			\
    PIX_TYPE		pixels[];		\
  } t_ ##NAME;

typedef struct
__attribute__((__packed__))
s_color_pix
{
  unsigned char r;
  unsigned char g;
  unsigned char b;
  unsigned char a;
} t_color_pix;

typedef unsigned char t_bw_pix;


IMAGE_DECLARE(t_color_pix, color_img)
IMAGE_DECLARE(t_bw_pix, bw_img)


#define AT(img, x, y) \
  (img->pixels + y * width + x)
#define IMG_ALLOC_TWIN(img) \
  malloc(sizeof(*img) + sizeof(*img->pixels) * img->height * img->width)

#endif
