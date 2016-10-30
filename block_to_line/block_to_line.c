#include <stdio.h>
#include "block_to_line.h"
#include "../tdefs.h"

t_sub_bw_img_vect *line_subdivision(t_sub_bw_img *img){
  int* blackpixelsarray = img_to_array(img);
  int averageperline = avgblackpxlperline(blackpixelsarray, img);
  t_bool* bool_array = bool_array_generation(averageperline,
                                                        blackpixelsarray, img);
  t_coordinates_vect* vect_of_coor = bool_array_to_coordinates(bool_array, img);
  t_sub_bw_img_vect* imgexport = coordinates_to_img(vect_of_coor, img);
  return imgexport;
}

int* img_to_array(t_sub_bw_img *img)
{
  printf("img_to_array launched\n");
  int stack = 0;
  printf("image width = %d\n",img->width);
  printf("image height = %d\n",img->height);
  int *array = malloc((sizeof(int) * img->height));
  printf("before loops\n");
  for(uint k = 0; k<(img->height); k++)
  {
    for (uint l = 0; l < (img->width); l++)
    {
      //printf("k = %d, l = %d",k,l);
      if (SUB_AT(img,l,k) != 0){
        //printf(", Pixel = %d",SUB_AT(img,k,l));
        stack += 1;
        //printf(", stack = %d\n",stack);
      }
      //else
        //printf("\n");
    }
    array[k] = (int)(img->width) - stack;
    //printf("Array[%d] = %d\n",k,array[k]);
    printf("%0*d\n",array[k],0);
    stack = 0;
  }
  printf("img_to_array closed\n");
  return array;
}


int avgblackpxlperline(int *array, t_sub_bw_img *img)
{
    int stack = 0;
    for(uint i = 0; i < img->height; i++)
      stack += array[i];
    int avg = stack/(int)img->height;
    printf("Average = %d\n",avg);
    return avg;
}

t_bool *bool_array_generation(int average, int *array, t_sub_bw_img *img)
{
  printf("bool_array_generation launched\n");
  t_bool *bool_array = malloc(sizeof(t_bool) * img->height);
  int *averages_array = malloc(sizeof(int) * img->height);
  int *sum_array = malloc(sizeof(int) * img->height);
  for(int i = 0; i < (int)img->height; i++){
    /*
    if (i == 0){
      sum_array[i] = array[i] + array[i+1] + array[i+2];
      averages_array[i] = average*3;
    }
    else if (i==1){
      sum_array[i] = array[i-1] + array[i] + array[i+1] + array[i+2];
      averages_array[i] = average*4;
    }
    else if (i==(int)img->height-2){
      sum_array[i] = array[i-2] + array[i-1] + array[i] + array[i+1];
      averages_array[i] = average*4;
    }
    else if (i==(int)img->height-1){
      sum_array[i] = array[i-2] + array[i-1] + array[i];
      averages_array[i] = average*3;
    }
    else{
      sum_array[i] = array[i-2] + array[i-1] + array[i]
                              + array[i+1] + array[i+2];
      averages_array[i] = average*5;
    }
    */
    average = 1;
    averages_array[i] = average;
    sum_array[i] = array[i];
    if (sum_array[i] < averages_array[i]){
      bool_array[i] = false;
    }
    else{
      bool_array[i] = true;
    }
    printf("Array[%d] = %d, bool = %u, this_sum = %d, this_average = %d\n"
                      ,i,array[i],bool_array[i],sum_array[i],averages_array[i]);
  }
//RETOUCHE 1 de bool_array

  t_bool this_bool = false;
  for(int i = 3; i < (int)img->height - 3; i++){
    if(bool_array[i] == false){
      if (this_bool == true){
        if((sum_array[i-3] + sum_array[i-2] + sum_array[i-1]
                                    + sum_array[i]) > averages_array[i]*2){
          bool_array[i] = true;
          continue;
        }
      }
      else{
        if((sum_array[i] + sum_array[i+1] + sum_array[i+2]
                                  + sum_array[i+3]) > averages_array[i]*2){
          bool_array[i] = true;
          continue;
        }
      }
    }
    else{
      continue;
    }

  }

//RETOUCHE 2 de bool_array

  printf("bool_array_generation closed\n");
  return bool_array;
}

t_coordinates_vect *bool_array_to_coordinates(t_bool *bool_array,
                                                              t_sub_bw_img *img)
{
  t_bool previousline = false;
  t_coordinates thislinecoordinates = {0,0};
  t_coordinates_vect *result;
  result = VECT_ALLOC(coordinates,4);
  for(int i = 0; i < (int)img->height; i++){
    if(bool_array[i] == true){
      if (previousline == false){
        previousline = true;
        thislinecoordinates.debut = i;
      }
      else
        continue;
    }
    else{
      if (previousline == true){
        previousline = false;
        thislinecoordinates.fin = i;
        VECT_PUSH(result,thislinecoordinates);
      }
      else
        continue;
    }
  }
  return result;
}

t_sub_bw_img_vect *coordinates_to_img(t_coordinates_vect *vectorofcoordinates,
                                      t_sub_bw_img *img)
{
  printf("coordinates_to_img launched\n");
  t_sub_bw_img_vect *imgresults;
  size_t nboflines = VECT_GET_SIZE(vectorofcoordinates);
  imgresults = VECT_ALLOC(sub_bw_img,nboflines);
  t_coordinates thiscoordinates;
  t_sub_bw_img *thislineimg;
  for (uint i = 0; i < VECT_GET_SIZE(vectorofcoordinates); i++){
    thiscoordinates = VECT_GET(vectorofcoordinates,i);
    thislineimg = relink_sub_bw_img(
      img,
      0,
      (uint)thiscoordinates.debut,
      img->width,
      (uint)(thiscoordinates.fin - thiscoordinates.debut));
    VECT_PUSH(imgresults,thislineimg);
    printf("Debut: %u - Fin: %u\n",thiscoordinates.debut, thiscoordinates.fin);
  }
  printf("coordinates_to_img closed\n");
  return imgresults;
}
