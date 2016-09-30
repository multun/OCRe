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
IMAGE_DECLARE(double, double_mat)


#define AT(img, x, y) \
  (img->pixels[y * img->width + x])

#define DEFAULT_IMG_TYPES_APPLY(F, SEP)			\
  F(color_img) SEP					\
  F(bw_img) SEP						\
  F(double_mat) SEP

#define ALLOC_IMAGE_DECLARE(TYPE)					\
  t_ ## TYPE * alloc_ ## TYPE (unsigned int width, unsigned int height)

#define FREE_IMAGE_DECLARE(TYPE)		\
  void free_ ## TYPE (t_ ## TYPE *img)
#define ALLOC_IMAGE_TWIN_DECLARE(TYPE)			\
  t_ ## TYPE * alloc_ ## TYPE ## _twin(t_ ## TYPE *img)

DEFAULT_IMG_TYPES_APPLY(ALLOC_IMAGE_DECLARE, ;)
DEFAULT_IMG_TYPES_APPLY(FREE_IMAGE_DECLARE, ;)
DEFAULT_IMG_TYPES_APPLY(ALLOC_IMAGE_TWIN_DECLARE, ;)

t_bw_img	*greyscale(unsigned char (*intensity)(t_color_pix), const t_color_img*);

#endif
