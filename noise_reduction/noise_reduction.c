#include<stdio.h>
#include"noise_reduction.h"
#include "../tdefs.h"

int ibw(int a, int b, int c){
  if (b < a){
    return a;
  }
  if (b > c){
    return c;
  }
  return b;
}


t_bw_img* reduce_noise(t_bw_img *img){
  t_bw_img *img2 = img;
  int hgt = img->height;
  int wdt = img->width;
  for(int i = 0; i < hgt; i++){
    for(int j = 0; j < wdt; j++){
      int sum =
         AT(img,ibw(0,j-1,wdt),ibw(0,i-1,hgt))
        +AT(img,ibw(0,j-1,wdt),ibw(0,i,hgt))
        +AT(img,ibw(0,j-1,wdt),ibw(0,i+1,hgt))
        +AT(img,ibw(0,j,wdt),ibw(0,i-1,hgt))
        +AT(img,ibw(0,j,wdt),ibw(0,i+1,hgt))
        +AT(img,ibw(0,j+1,wdt),ibw(0,i-1,hgt))
        +AT(img,ibw(0,j+1,wdt),ibw(0,i,hgt))
        +AT(img,ibw(0,j+1,wdt),ibw(0,i+1,hgt));
      if (sum == 0)
        sum = 1;
      int average = sum/8;
      AT(img2,j,i) = average;
    }
  }
  return img2;
}
