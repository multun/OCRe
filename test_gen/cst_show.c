#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <stdbool.h>
#include "../error.h"
#include "load_img.h"
#include <sys/mman.h>

int main(int argc, char ** argv)
{
  bool quit = false;
  SDL_Event event;

  if (argc < 2)
    FAIL0("missing argument");

  SDL_Init(SDL_INIT_VIDEO);

  SDL_Window * window = SDL_CreateWindow("SDL2 Displaying Image",
					 SDL_WINDOWPOS_UNDEFINED,
					 SDL_WINDOWPOS_UNDEFINED,
					 640,
					 480,
					 0);

  SDL_Renderer * renderer = SDL_CreateRenderer(window, -1, 0);
  int fd = open(argv[1], 0);
  t_bw_img *bw_img = mmap(NULL, 684, PROT_READ, MAP_PRIVATE, fd, 0);

  SDL_Surface * image = load_bw_img(bw_img);
  //  SDL_Surface * image = SDL_LoadBMP(argv[1]);
  SDL_Texture * texture = SDL_CreateTextureFromSurface(renderer, image);

  while (!quit)
  {
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);

    SDL_WaitEvent(&event);

    switch (event.type)
    {
    case SDL_QUIT:
      quit = true;
      break;
    }

    //SDL_Rect dstrect = { 5, 5, 320, 240 };
    //SDL_RenderCopy(renderer, texture, NULL, &dstrect);
  }

  SDL_DestroyTexture(texture);
  SDL_FreeSurface(image);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);

  SDL_Quit();
  munmap(bw_img, 684);

  return 0;
}
