#include<stdio.h>
#include<block_to_lines.h>

typedef enum { false, true } bool;

int main{
  return 0;
}



int img_to_array(t_bw_img img){
  int stack = 0;
  int array[img->height];
  for(k = 0; k<(img->height); k++){
    for (l = 0; l < (img->length); l++){
      stack += AT(img,k,l);
    }
    array[k] = (img->length) - stack;
    stack = 0;

  }
  return array;
}

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
      else{
        stack++;
      }
    }
    else{
      if is_line == false{
        is_line = true
        stack_space += stack
        stack = 0;
        nbspace++;
      }
      else{
        stack++;
      }
    }
  }
  avg_line = stack_line/nbline;
  avg_space = stack_space/nbspace;
  //Question à poser: comment faire sortir plusieurs valeurs d'une fonction?
  //Apparemment on a pas le droit aux arrays
  return avg_line;
}

// Objectif:
