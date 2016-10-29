#include <stdio.h>
#include "block_to_line.h"
#include "../tdefs.h"

t_sub_bw_img_vect *line_subdivision(t_sub_bw_img *img)
{
  printf("Line_subdivision fnc launched\n");
  t_coordinates_vect* vector_of_coordinates = img_to_coordinates(img);
  t_sub_bw_img_vect* result = coordinates_to_img(img,vector_of_coordinates);
  return result;
}

int* img_to_array(t_sub_bw_img *img)
{
  printf("img_to_array launched\n");
  int stack = 0;
  //int to_add;
  printf("image width = %d\n",img->width);
  printf("image height = %d\n",img->height);
  int *array = malloc((sizeof(int) * img->height));
  printf("size or the array prefunction = %d\n",(uint)sizeof(array));
  for(uint k = 0; k<(img->height); k++)
  {
    for (uint l = 0; l < (img->width); l++)
    {
      if (SUB_AT(img,k,l) != 0)
        stack += 1;
    }
    array[k] = (int)(img->width) - stack;
    printf("Array[%d] = %d\n",k,array[k]);
    stack = 0;
  }
  printf("img_to_array closed\n");
  printf("array size = %lu\n",sizeof(array));
  return array;
}

// Objectif: prend le tableau des sommes de pixels noirs par ligne
//           retourne la hauteur moyenne d'une ligne
int array_to_average(int *array, t_sub_bw_img *img)
{
  printf("array_to_average\n");
  t_bool is_line = false;
  int avg_line;
  int stack_line = 0;
  //int stack_space = 0;
  int nb_line = 0;
  //int nb_space = 0;
  int stack = 0;
  uint i;
  uint array_length = img->height;
  printf("size of array = %d\n",array_length);
  for (i = 0; i < array_length; i++)
  {
    printf("boucle for nbr %d\n",i);
    if (array[i] == 0){
      printf("array = 0 nbr %d\n",i);
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
  if(nb_line == 0){
    nb_line = 1;
  }

  avg_line = stack_line/nb_line;
  //avg_space = stack_space/nb_space;
  printf("array_to_average closed: average = %d\n",avg_line);
  return avg_line;
}

// Objectif:
// Refaire une traversée de l'image en enregistrant les addresses des lignes à extraire, éliminant les hors-seuil
t_coordinates_vect *img_to_coordinates(t_sub_bw_img *img)
{
  printf("img_to_coordinates launched\n");
  t_coordinates_vect *lines_results;
  lines_results = VECT_ALLOC(coordinates, 4);
  t_coordinates thisline = {0,0};
  t_bool is_line = false;
  int* line_array = img_to_array(img);
  int average = array_to_average(line_array, img);
  int low_limit = (int)((double)average * 0.66);
  int high_limit = (int)((double)average * 1.33);
  for (int i = 0; (uint)i < img->height; i++){
    if (line_array[i] == 0){
      if (is_line == true){
        is_line = false;
        if (i - thisline.debut < (int)((double)average*0.25))
          continue;
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
  printf("number of coordinates = %lu\n",VECT_GET_SIZE(lines_results));
  printf("img_to_coordinates closed\n");
  return lines_results;
}

// Objectif: Prendre le vecteur de coordonées, retourner un vecteur d'images correspondantes
t_sub_bw_img_vect *coordinates_to_img(t_sub_bw_img *img,
				      t_coordinates_vect *vect_of_coord)
{
  printf("coordinates_to_img launched: \n");
  t_sub_bw_img_vect *img_results;
  t_coordinates thiscoordinates;
  t_sub_bw_img *thislineimg;
  size_t size = VECT_GET_SIZE(vect_of_coord);
  img_results = VECT_ALLOC(sub_bw_img, size);
  printf("Size of vect_of_coord: %lu\n",size);
  for (uint i = 0; i < size; i++)
  {
    printf("Into the For Loop:\n");
    printf("i = %d\n",i);
    thiscoordinates = VECT_GET(vect_of_coord,i);
    thislineimg = relink_sub_bw_img(
      img,
      0,
      (uint)thiscoordinates.debut,
      img->width,
      (uint)(thiscoordinates.fin - thiscoordinates.debut));
    VECT_PUSH(img_results, thislineimg);
  }
  printf("coordinates_to_img over\n");
  return img_results;
}
