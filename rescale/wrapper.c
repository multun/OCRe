#include <stdio.h>
#include <stdlib.h>
#include "../bitmap/img.h"
#include "../error.h"
#include "../tdefs.h"
#include "rescale.h"


void wrapper_resize(t_bw_img *input_img)
{
  input_img = resize(input_img, 1000, 1000);
}
