#include <stdio.h>
#include "block_to_lines.h"
#include "../base_structs/vector.h"

DECL_NAMED_VECTOR(unsigned int, uint);

int main()
{
  return 0;
}



int img_to_array(t_bw_img img){
  int stack = 0;
  //int array[img->height];
  int *array = malloc(sizeof(int) * img->height);
  for(k = 0; k<(img->height); k++){
    for (l = 0; l < (img->length); l++){
      stack += AT(img,k,l);
    }
    array[k] = (img->length) - stack;
    stack = 0;
  }
  return array;
}

// Objectif: prend le tableau des sommes de pixels noirs par ligne
//           retourne la hauteur moyenne d'une ligne
int array_to_average(int array[]){
  bool is_line = true;
  int avg_line;
  int avg_space;
  int stack_line = 0;
  int stack_space = 0;
  int nb_line = 0;
  int nb_space = 0;
  int i;
  for (i = 0; i < sizeof(array[]); i++){
    if (array[i] == 0){
      if (is_line == true){
        is_line = false;
        stack_line += stack;
        stack = 0;
        nbline++;
      }
      else
        stack++;
    }
    else{
      if (is_line == false){
        is_line = true
        stack_space += stack
        stack = 0;
        nbspace++;
      }
      else
        stack++;
    }
  }
  avg_line = stack_line/nbline;
  avg_space = stack_space/nbspace;
  return avg_line;
}

// Objectif:
// Refaire une traversée de l'image en enregistrant les addresses des lignes à extraire, éliminant les hors-seuil
t_uint_vect img_to_coordinates(t_bw_img img){
  t_uint_vect *lines_results;
  lines_results = VECT_ALLOC(uint,0);
  thisline = (t_coordinates){0,0};
  bool is_line = true;
  int line_array[] = img_to_array(img);
  int average = array_to_average(line_array);
  int low_limit = average*0,66;
  int high_limit = average*1,33;
  for (i = 0; i < img->height; i++){
    if (line_array[i] == 0){
      if (is_line == true){
        if (i - thisline.debut < average*0,25)
          continue
        is_line = false;
        thisline.fin = i;
        if ((thisline.fin - thisline.début > low_limit) && (thisline.fin - thisline.debut < high_limit))
          VECT_PUSH(lines_results,thisline);
        thisline = (t_coordinates){0,0};
      }
    }
    else{
      if (is_line == false){
        is_line = true;
        thisline.debut = i;
      }
    }
  }
  return lines_results;
}

// Objectif: Prendre le vecteur de coordonées, retourner un vecteur d'images correspondantes
t_uint_vect coordinates_to_img(t_bw_img img,t_uint_vect vect_of_coord){
  t_uint_vect *img_results;
  img_results = VECT_ALLOC(uint,0);
  for (int i = 0; i < VECT_GET_SIZE(vect_of_coord); i++){
    t_coordinates thiscoordinates = VECT_GET(vect_of_coord,i);
    t_bw_img thislineimg;
    for(int j = thiscoordinates.debut; j < thiscoordinates.fin; j++){
      for(int k = 0; k < img->length; k++)
        AT(thislineimg,(j - thiscoordinates.debut),k) = AT(img,j,k);
    }
    VECT_GET(img_results,i) = thislineimg;
  }
  return img_results;
}
