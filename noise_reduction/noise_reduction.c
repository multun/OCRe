#include"noise_reduction.h"
#include "../tdefs.h"

size_t ibw(size_t a, size_t b, size_t c)
{
  if (b < a)
    return a;
  if (b > c)
    return c;
  return b;
}

t_bw_img* reduce_noise(t_bw_img *img)
{
  t_bw_img *img2 = alloc_bw_img_twin(img);
  uint hgt = img->height;
  uint wdt = img->width;
  for(uint i = 0; i < hgt; i++){
    for(uint j = 0; j < wdt; j++){
      int sum =
         AT(img,ibw(0,j-1,wdt),ibw(0,i-1,hgt))
        +AT(img,ibw(0,j-1,wdt),ibw(0,i,hgt))
        +AT(img,ibw(0,j-1,wdt),ibw(0,i+1,hgt))
        +AT(img,ibw(0,j,wdt),ibw(0,i-1,hgt))
        +AT(img,ibw(0,j,wdt),ibw(0,i+1,hgt))
        +AT(img,ibw(0,j+1,wdt),ibw(0,i-1,hgt))
        +AT(img,ibw(0,j+1,wdt),ibw(0,i,hgt))
        +AT(img,ibw(0,j+1,wdt),ibw(0,i+1,hgt));
      uint average = (uint)sum/8;
      AT(img2,j,i) = (t_bw_pix)average;
    }
  }
  return img2;
}
