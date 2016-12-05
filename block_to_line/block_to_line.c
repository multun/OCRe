#include <stdio.h>
#include "block_to_line.h"
#include "../tdefs.h"


t_sub_bw_img_vect *line_subdivision(t_sub_bw_img *img)
{
  int *blackpixelsarray = img_to_array(img);
  int averageperline = avgblackpxlperline(blackpixelsarray,
					  img);
  t_bool* bool_array = bool_array_generation(averageperline,
					     blackpixelsarray,
					     img);
  int lineheightaverage = line_height_avg(bool_array,img);
  bool_array_modification(bool_array,img,lineheightaverage);
  widen_lines(bool_array,img);
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
      if (SUB_AT(img,l,k) == 0)
        stack += 1;
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
  for(int i = 0; i < (int)img->height; i++)
  {
    average = 10;
    averages_array[i] = average;
    sum_array[i] = array[i];

    bool_array[i] = !(sum_array[i] < averages_array[i]);
  }

// RETOUCHE 1 de bool_array
// Rattache les lignes sattelites aux lignes précédentes/suivantes
  for(int i = 3; i < (int)img->height - 3; i++)
  {
    if(bool_array[i] == false)
    {
      t_bool previous_bool = bool_array[i-3];
      if (previous_bool == true)
      {
        previous_bool = ( bool_array[i-4]
			 + bool_array[i-3]
			 + bool_array[i-2]
			 + bool_array[i-1]) % 2;
        if((sum_array[i-2]
	    + sum_array[i-1]
	    + sum_array[i]) > averages_array[i])
          bool_array[i] = true;
      }
      else
      {
        previous_bool = (bool_array[i+1]
			 +bool_array[i+2]
			 +bool_array[i+3]
			 +bool_array[i+4]) % 2;
        if((sum_array[i]
	    + sum_array[i+1]
	    + sum_array[i+2]) > averages_array[i])
          bool_array[i] = true;
      }
    }
  }
  return bool_array;
}

int line_height_avg(t_bool* bool_array, t_sub_bw_img* img)
{
  t_bool previous_bool = false;
  int totalpixellines = 0;
  int totaltextlines = 0;
  for (int i = 0; i < (int)img->height; i++){
    if (bool_array[i] == true)
    {
      if (previous_bool == false)
        previous_bool = true;
      else
        totalpixellines += 1;
    }
    else if(previous_bool == true)
    {
      previous_bool = false;
      totaltextlines += 1;
    }
  }
  if (totaltextlines == 0)
    totaltextlines = 1;
  return totalpixellines/totaltextlines;
}

void bool_array_modification(t_bool* bool_array,
			     t_sub_bw_img* img,
			     int lineheightaverage)
{
  t_coordinates_vect *coordinates_vect;
  coordinates_vect = bool_array_to_coordinates(bool_array,img);
  int vect_size = (int)VECT_GET_SIZE(coordinates_vect);
  t_bool has_changed = false;

  for (int i = 1; i < vect_size; i++)
  {
    t_coordinates thiscoordinates = VECT_GET(coordinates_vect,i);
    t_coordinates previouscoordinates = VECT_GET(coordinates_vect,i-1);
    if (thiscoordinates.fin - thiscoordinates.debut > lineheightaverage * 3.5)
    {
      for (int j = thiscoordinates.debut - 2; j <= thiscoordinates.fin; j++)
        bool_array[j] = false;
      has_changed = true;
    }

    if (thiscoordinates.fin - thiscoordinates.debut < lineheightaverage * 0.3)
    {
      if (thiscoordinates.debut - previouscoordinates.fin
	  < lineheightaverage * 0.3)
      {
        previouscoordinates.fin = thiscoordinates.fin;
        for(int j=previouscoordinates.debut;j < previouscoordinates.fin;j++)
          bool_array[j] = true;
        has_changed = true;
      }
      else
      {
        for(int k=thiscoordinates.debut; k<= thiscoordinates.fin; k++)
	{
          bool_array[k] = false;
          has_changed = true;
        }
      }
    }
  }

  t_coordinates linezero = VECT_GET(coordinates_vect,0);
  t_coordinates lineone = VECT_GET(coordinates_vect,1);
  if (linezero.fin - linezero.debut < lineheightaverage * 0.3)
  {
    if (lineone.debut - linezero.fin < lineheightaverage * 0.3)
    {
      lineone.debut = linezero.debut;
      for(int j=lineone.debut;j < lineone.fin;j++)
        bool_array[j] = true;
    }
    else
      for(int k=linezero.debut; k<= linezero.fin; k++)
        bool_array[k] = false;
    has_changed = true;
  }

  if(has_changed == true)
    bool_array_modification(bool_array,img,lineheightaverage);
}

void widen_lines(t_bool* bool_array, t_sub_bw_img* img)
{
  t_coordinates_vect *coordinates_vect;
  coordinates_vect = bool_array_to_coordinates(bool_array,img);
  int vect_size = (int)VECT_GET_SIZE(coordinates_vect);

  t_coordinates linezero = VECT_GET(coordinates_vect,0);
  for (int j = 0; j < linezero.fin; j++)
    bool_array[j] = true;

  for(int i = 1; i < vect_size; i++)
  {
    t_coordinates thisline = VECT_GET(coordinates_vect,i);
    t_coordinates previousline = VECT_GET(coordinates_vect,i-1);

    thisline.debut = previousline.fin +1;
    for (int j = thisline.debut; j < thisline.fin; j++)
      bool_array[j] = true;
  }

  t_coordinates lastline = VECT_GET(coordinates_vect,vect_size - 1);
  lastline.fin = (int)img->height - 1;
  for (int j = lastline.debut; j < lastline.fin; j++)
    bool_array[j] = true;
}

t_coordinates_vect *bool_array_to_coordinates(t_bool *bool_array,
                                              t_sub_bw_img *img)
{
  t_bool previousline = false;
  t_coordinates thislinecoordinates = {0,0};
  t_coordinates_vect *result;
  result = VECT_ALLOC(coordinates,4);
  for(int i = 0; i < (int)img->height; i++)
    if(bool_array[i] == true)
    {
      if (previousline == false)
      {
        previousline = true;
        thislinecoordinates.debut = i;
      }
    }
    else if (previousline == true)
    {
      previousline = false;
      thislinecoordinates.fin = i;
      VECT_PUSH(result,thislinecoordinates);
    }

  return result;
}

t_sub_bw_img_vect *coordinates_to_img(t_coordinates_vect *vectorofcoordinates,
				      t_sub_bw_img *img)
{
  t_sub_bw_img_vect *imgresults;
  size_t nboflines = VECT_GET_SIZE(vectorofcoordinates);
  imgresults = VECT_ALLOC(sub_bw_img,nboflines);
  t_coordinates thiscoordinates;
  t_sub_bw_img *thislineimg;
  for (uint i = 0; i < VECT_GET_SIZE(vectorofcoordinates); i++)
  {
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
