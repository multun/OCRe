#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "../bitmap/img.h"

typedef union u_color_pix_mask
{
  t_color_pix	pix;
  Uint32	mask;
} t_color_pix_mask;

SDL_Surface *load_color_img(t_color_img *img)
{
  const t_color_pix_mask r = {.pix = {255, 0, 0, 0}};
  const t_color_pix_mask g = {.pix = {0, 255, 0, 0}};
  const t_color_pix_mask b = {.pix = {0, 0, 255, 0}};
  const t_color_pix_mask a = {.pix = {0, 0, 0, 255}};

  return SDL_CreateRGBSurfaceFrom(img->pixels,
				  (int)img->width,
				  (int)img->height,
				  sizeof(t_color_pix),
				  (int)img->width,
				  r.mask,
				  g.mask,
				  b.mask,
				  a.mask);
}

SDL_Surface *load_bw_img(t_bw_img *img)
{
  SDL_Surface *ret = SDL_CreateRGBSurfaceFrom(img->pixels,
					      (int)img->width,
					      (int)img->height,
					      sizeof(t_bw_pix),
					      (int)img->width,
					      0xFF0000,
					      0xFF0000,
					      0xFF0000,
					      0xFF0000);
  SDL_Color colors[256];
  for(unsigned int i = 0; i < 256; i++)
    colors[i].r = colors[i].g = colors[i].b = (unsigned char)i;
  SDL_SetPaletteColors(ret->format->palette, colors, 0, 256);
  return ret;
}
