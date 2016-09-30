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
  (img->pixels[y * img->width + x])


t_bw_img        *alloc_bw_img(unsigned int width, unsigned int height);
t_color_img     *alloc_img(unsigned int width, unsigned int height);
t_color_img	*img_alloc_twin(t_color_img*);
t_bw_img	*bw_img_alloc_twin(t_bw_img*);
void		free_img(t_color_img* image);
void		free_bw_img(t_bw_img* image);

t_bw_img	*greyscale(unsigned char (*intensity)(t_color_pix), const t_color_img*);

#endif
