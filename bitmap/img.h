#ifndef  IMG_H
# define IMG_H


#define IMAGE_DECLARE(PIX_TYPE, NAME)		\
  typedef struct s_ ##NAME			\
  {						\
    unsigned int	width, height;		\
    PIX_TYPE		pixels[];		\
  } t_ ##NAME;					\
  typedef struct s_sub_ ##NAME			\
  {						\
    t_ ##NAME		*father;		\
    unsigned int	width, height;		\
    unsigned int	xoff, yoff;		\
  } t_sub_ ##NAME;


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

#define DEFAULT_IMG_TYPES_APPLY(F, SEP)			\
  F(t_color_pix, color_img) SEP				\
  F(t_bw_pix, bw_img) SEP				\
  F(double, double_mat) SEP

DEFAULT_IMG_TYPES_APPLY(IMAGE_DECLARE,)

#define AT(img, x, y) \
  (img->pixels[y * img->width + x])

#define SUB_AT(img, x, y)				\
  (img->father->pixels[(y + img->yoff) * img->father->width + x + img->xoff])


// IMAGE DECLARATION //////////////////////
#define ALLOC_IMAGE_DECLARE(PIX_TYPE, TYPE)				\
  t_ ## TYPE * alloc_ ## TYPE (unsigned int width, unsigned int height)
#define ALLOC_IMAGE_DEFINE(PIX_TYPE, TYPE)				\
  ALLOC_IMAGE_DECLARE(PIX_TYPE, TYPE)					\
  {									\
    t_ ## TYPE *ret	= malloc(					\
      sizeof(t_ ## TYPE ) + sizeof(PIX_TYPE) * width * height);		\
    ret->width	= width;						\
    ret->height	= height;						\
    return ret;								\
  }

// SUB IMAGE DECLARATION //////////////////////
#define ALLOC_SUB_IMAGE_DECLARE(PIX_TYPE, TYPE)				\
  t_sub_ ## TYPE * alloc_sub_ ## TYPE (					\
    t_ ## TYPE * father,						\
    unsigned int xoff,							\
    unsigned int yoff,							\
    unsigned int width,							\
    unsigned int height)
#define ALLOC_SUB_IMAGE_DEFINE(PIX_TYPE, TYPE)				\
  ALLOC_SUB_IMAGE_DECLARE(PIX_TYPE, TYPE)				\
  {									\
    t_sub_ ## TYPE *ret	= malloc(sizeof(t_sub_ ## TYPE ));		\
    ret->father = father;						\
    ret->xoff	= xoff;							\
    ret->yoff	= yoff;							\
    ret->width	= width;						\
    ret->height	= height;						\
    return ret;								\
  }

// IMAGE FREEING //////////////////////
#define FREE_IMAGE_DECLARE(PIX_TYPE, TYPE)	\
  void free_ ## TYPE (t_ ## TYPE *img)
#define FREE_IMAGE_DEFINE(PIX_TYPE, TYPE)				\
  FREE_IMAGE_DECLARE(PIX_TYPE, TYPE)					\
  {									\
    free(img);								\
  }

// SUB IMAGE FREEING //////////////////////
#define FREE_SUB_IMAGE_DECLARE(PIX_TYPE, TYPE)	\
  void free_sub_ ## TYPE (t_sub_ ## TYPE *img)
#define FREE_SUB_IMAGE_DEFINE(PIX_TYPE, TYPE)				\
  FREE_SUB_IMAGE_DECLARE(PIX_TYPE, TYPE)				\
  {									\
    free(img);								\
  }

// IMAGE MIRRORING //////////////////////
#define ALLOC_IMAGE_TWIN_DECLARE(PIX_TYPE, TYPE)	\
  t_ ## TYPE * alloc_ ## TYPE ## _twin(t_ ## TYPE *img)
#define ALLOC_IMAGE_TWIN_DEFINE(PIX_TYPE, TYPE)				\
  ALLOC_IMAGE_TWIN_DECLARE(PIX_TYPE, TYPE)				\
  {									\
    return alloc_ ## TYPE (img->width, img->height);			\
  }

#define DEFINE_IMG_TOOLS(PIX_TYPE, TYPE)			\
  ALLOC_IMAGE_DEFINE(PIX_TYPE, TYPE)				\
  ALLOC_SUB_IMAGE_DEFINE(PIX_TYPE, TYPE)			\
  FREE_IMAGE_DEFINE(PIX_TYPE, TYPE)				\
  FREE_SUB_IMAGE_DEFINE(PIX_TYPE, TYPE)				\
  ALLOC_IMAGE_TWIN_DEFINE(PIX_TYPE, TYPE)			\

DEFAULT_IMG_TYPES_APPLY(ALLOC_IMAGE_DECLARE, ;)
DEFAULT_IMG_TYPES_APPLY(ALLOC_SUB_IMAGE_DECLARE, ;)
DEFAULT_IMG_TYPES_APPLY(FREE_IMAGE_DECLARE, ;)
DEFAULT_IMG_TYPES_APPLY(FREE_SUB_IMAGE_DECLARE, ;)
DEFAULT_IMG_TYPES_APPLY(ALLOC_IMAGE_TWIN_DECLARE, ;)

t_bw_img	*greyscale(unsigned char (*intensity)(t_color_pix), const t_color_img*);

#endif
