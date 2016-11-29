#include <stdio.h>
#include "block_to_line.h"
#include "../tdefs.h"

t_sub_bw_img_vect *line_subdivision(t_sub_bw_img *img){
  int* blackpixelsarray = img_to_array(img);
  int averageperline = avgblackpxlperline(blackpixelsarray, img);
  t_bool* bool_array = bool_array_generation(averageperline,
                                                        blackpixelsarray, img);
  int lineheightaverage = line_height_avg(bool_array,img);
  bool_array_modification(bool_array,img,lineheightaverage);
  t_coordinates_vect* vect_of_coo = bool_array_to_coordinates(bool_array, img);
  t_sub_bw_img_vect* imgexport = coordinates_to_img(vect_of_coo, img);
  return imgexport;
}

int* img_to_array(t_sub_bw_img *img)
{
  int stack = 0;
  int *array = malloc((sizeof(int) * img->height));
  for(uint k = 0; k<(img->height); k++)
  {
    for (uint l = 0; l < (img->width); l++)
    {
      if (SUB_AT(img,l,k) == 0){
        stack += 1;
      }
    }
    array[k] = stack;
    stack = 0;
  }
  return array;
}


int avgblackpxlperline(int *array, t_sub_bw_img *img)
{
    int stack = 0;
    for(uint i = 0; i < img->height; i++)
      stack += array[i];
    int avg = stack/(int)img->height;
    return avg;
}

t_bool *bool_array_generation(int average, int *array, t_sub_bw_img *img)
{
  t_bool *bool_array = malloc(sizeof(t_bool) * img->height);
  int *averages_array = malloc(sizeof(int) * img->height);
  int *sum_array = malloc(sizeof(int) * img->height);
  for(int i = 0; i < (int)img->height; i++){
    average = 10;
    averages_array[i] = average;
    sum_array[i] = array[i];
    if (sum_array[i] < averages_array[i]){
      bool_array[i] = false;
    }
    else{
      bool_array[i] = true;
    }
    //printf("Array[%d] = %d, bool = %u, this_sum = %d, this_average = %d\n"
                  //,i,array[i],bool_array[i],sum_array[i],averages_array[i]);
  }

// RETOUCHE 1 de bool_array
// Rattache les lignes sattelites aux lignes précédentes/suivantes
  for(int i = 3; i < (int)img->height - 3; i++){
    if(bool_array[i] == false){
      t_bool previous_bool = bool_array[i-3];
      if (previous_bool == true){
        previous_bool = (bool_array[i-4]+bool_array[i-3]
                                          +bool_array[i-2]+bool_array[i-1])%2;
        if((sum_array[i-3] + sum_array[i-2] + sum_array[i-1]
                                    + sum_array[i]) > averages_array[i]){
          bool_array[i] = true;
          continue;
        }
      }
      else{
        previous_bool = (bool_array[i+1]+bool_array[i+2]
                                          +bool_array[i+3]+bool_array[i+4])%2;
        if((sum_array[i] + sum_array[i+1] + sum_array[i+2]
                                  + sum_array[i+3]) > averages_array[i]){
          bool_array[i] = true;
          continue;
        }
      }
    }
  }
  return bool_array;
}

int line_height_avg(t_bool* bool_array, t_sub_bw_img* img){
  t_bool previous_bool = false;
  int totalpixellines = 0;
  int totaltextlines = 0;
  for (int i = 0; i < (int)img->height; i++){
    if (bool_array[i] == true){
      if (previous_bool == false){
        previous_bool = true;
      }
      else{
        totalpixellines+=1;
      }
    }
    else{
      if(previous_bool == true){
        previous_bool = false;
        totaltextlines += 1;
      }
      else{
        continue;
      }
    }
  }
  if (totaltextlines == 0)
    totaltextlines = 1;
  int lineheightaverage = totalpixellines/totaltextlines;
  //printf("lineheightaverage is %d\n",lineheightaverage);
  return lineheightaverage;
}

void bool_array_modification(t_bool* bool_array, t_sub_bw_img* img,
                                                   int lineheightaverage){
  t_coordinates_vect *coordinates_vect;
  coordinates_vect = bool_array_to_coordinates(bool_array,img);
  int vect_size = (int)VECT_GET_SIZE(coordinates_vect);
  t_bool has_changed = false;

  for (int i = 1; i < vect_size; i++){
    t_coordinates thiscoordinates = VECT_GET(coordinates_vect,i);
    t_coordinates previouscoordinates = VECT_GET(coordinates_vect,i-1);
    // Si la ligne est bien trop grande pour être une ligne:
    if (thiscoordinates.fin - thiscoordinates.debut > lineheightaverage * 3.5){
      // printf("ligne n° %d trop épaisse\nMaxHeight = %lf, thisline is %d\n",i,lineheightaverage*3.5,thiscoordinates.fin - thiscoordinates.debut);
      // On efface la ligne du tableau de booléens
      // printf("Ligne effacée\n");
      for (int j = thiscoordinates.debut - 2; j <= thiscoordinates.fin; j++){
        bool_array[j] = false;
      }
      has_changed = true;
    }

    // Si la ligne est bien trop petite pour être une ligne
    if (thiscoordinates.fin - thiscoordinates.debut < lineheightaverage * 0.3){
      // printf("ligne n° %d trop fine\nMinHeight = %lf, thisline is %d\n",i,lineheightaverage*0.3,thiscoordinates.fin - thiscoordinates.debut);

      // Si l'espace entre cette ligne et celle du dessus est bcp trop petit
      if (thiscoordinates.debut - previouscoordinates.fin
                                                  < lineheightaverage * 0.3){
        // On étend la ligne du dessus, supprime cette ligne
        // printf("ligne étendue\n");
        previouscoordinates.fin = thiscoordinates.fin;
        for(int j=previouscoordinates.debut;j < previouscoordinates.fin;j++){
          bool_array[j] = true;
        }
        has_changed = true;
      }

      // Sinon, on efface la ligne
      else{
        // printf("ligne supprimée\n");
        for(int k=thiscoordinates.debut; k<= thiscoordinates.fin; k++){
          bool_array[k] = false;
          has_changed = true;
        }
      }
    }
    // printf("Line n°%d treated\n",i);
  }

  t_coordinates linezero = VECT_GET(coordinates_vect,0);
  t_coordinates lineone = VECT_GET(coordinates_vect,1);
  if (linezero.fin - linezero.debut < lineheightaverage * 0.3){
    if (lineone.debut - linezero.fin < lineheightaverage * 0.3){
      lineone.debut = linezero.debut;
      for(int j=lineone.debut;j < lineone.fin;j++){
        bool_array[j] = true;
      }
    }
    else{
      for(int k=linezero.debut; k<= linezero.fin; k++){
        bool_array[k] = false;
      }
    }
    has_changed = true;
  }

  //Si des changements ont été faits, on recommence
  if((has_changed == true)) /* && (vect_size > 4)) */ {
    bool_array_modification(bool_array,img,lineheightaverage);
  }
  // printf("Bool Array Modif: Exit\n");// Sinon, c'est fini
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
        // printf("begin = %d, end = %d\n",thislinecoordinates.debut, thislinecoordinates.fin);
        VECT_PUSH(result,thislinecoordinates);
      }
      else
        continue;
    }
  }
  for(int j=0; j < (int)VECT_GET_SIZE(result); j++){
    // printf("Coord: début: %d, fin: %d\n",VECT_GET(result,j).debut,VECT_GET(result,j).fin);
  }
  return result;
}

t_sub_bw_img_vect *coordinates_to_img(t_coordinates_vect                *vectorofcoordinates
                    ,t_sub_bw_img *img)
{
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
  }
  return imgresults;
}
