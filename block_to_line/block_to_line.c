#include <stdio.h>
#include "block_to_line.h"
#include "../tdefs.h"

t_sub_bw_img_vect *line_subdivision(t_sub_bw_img *img){
  t_coordinates_vect* vector_of_coordinates = img_to_coordinates(img);
  t_sub_bw_img_vect* result = coordinates_to_img(img,vector_of_coordinates);
  return result;
}

int* img_to_array(t_sub_bw_img *img)
{
  int stack = 0;
  int *array = malloc(sizeof(int) * img->height);
  for(uint k = 0; k<(img->height); k++)
  {
    for (uint l = 0; l < (img->width); l++)
      stack += SUB_AT(img,k,l);

    array[k] = (int)(img->width) - stack;
    stack = 0;
  }
  return array;
}

// Objectif: prend le tableau des sommes de pixels noirs par ligne
//           retourne la hauteur moyenne d'une ligne
int array_to_average(int *array)
{
  t_bool is_line = true;
  int avg_line;
  int stack_line = 0;
  //int stack_space = 0;
  int nb_line = 0;
  //int nb_space = 0;
  int stack = 0;
  for (uint i = 0; i < sizeof(array); i++)
  {
    if (array[i] == 0){
      if (is_line == true){
        is_line = false;
        stack_line += stack;
        stack = 0;
        nb_line++;
      }
      else
        stack++;
    }
    else{
      if (is_line == false)
      {
        is_line = true;
        //stack_space += stack;
        stack = 0;
        //nb_space++;
      }
      else
        stack++;
    }
  }
  avg_line = stack_line/nb_line;
  //avg_space = stack_space/nb_space;
  return avg_line;
}

// Objectif:
// Refaire une traversée de l'image en enregistrant les addresses des lignes à extraire, éliminant les hors-seuil
t_coordinates_vect *img_to_coordinates(t_sub_bw_img *img)
{
  t_coordinates_vect *lines_results;
  lines_results = VECT_ALLOC(coordinates, 4);
  t_coordinates thisline = {0,0};
  t_bool is_line = true;
  int* line_array = img_to_array(img);
  int average = array_to_average(line_array);
  int low_limit = (int)((double)average * 0.66);
  int high_limit = (int)((double)average * 1.33);
  for (int i = 0; (uint)i < img->height; i++){
    if (line_array[i] == 0){
      if (is_line == true){
        if (i - thisline.debut < (int)((double)average*0.25))
          continue;
        is_line = false;
        thisline.fin = i;
        if ((thisline.fin - thisline.debut > low_limit) && (thisline.fin - thisline.debut < high_limit))
          VECT_PUSH(lines_results, thisline);
        thisline = (t_coordinates){0,0};
      }
    }
    else
    {
      if (is_line == false){
        is_line = true;
        thisline.debut = i;
      }
    }
  }
  return lines_results;
}

// Objectif: Prendre le vecteur de coordonées, retourner un vecteur d'images correspondantes
t_sub_bw_img_vect *coordinates_to_img(t_sub_bw_img *img,
				      t_coordinates_vect *vect_of_coord)
{
  t_sub_bw_img_vect *img_results;
  size_t size = VECT_GET_SIZE(vect_of_coord);
  img_results = VECT_ALLOC(sub_bw_img, size);
  for (uint i = 0; i < size; i++)
  {
    t_coordinates thiscoordinates = VECT_GET(vect_of_coord,i);
    t_sub_bw_img *thislineimg = relink_sub_bw_img(
      img,
      0,
      (uint)thiscoordinates.debut,
      img->width,
      (uint)(thiscoordinates.fin - thiscoordinates.debut));
    VECT_PUSH(img_results, thislineimg);
  }
  return img_results;
}
