#ifndef SDL_DISPLAY_H
#define SDL_DISPLAY_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "../bitmap/img.h"

SDL_Surface *load_color_img(t_color_img *img);
SDL_Surface *load_bw_img(t_bw_img *img);

#endif
