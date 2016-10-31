#include "../bitmap/img.h"
#include "../error.h"
#include "../tdefs.h"
#include <stdio.h>
#include "../base_structs/vector.h"
#include "bounding_box.h"
#include <math.h>

t_bw_img *rotate_img(t_bw_img *input, double angle)
{
  int newHeight, newWidth, oldHeight, oldWidth;
  int halfNewHeight, halfNewWidth, nx, ny;
  int halfOldHeight, halfOldWidth;
  double radians, cosval, sinval;
  t_bw_img *output;

  radians = -(angle)/((180/3.142));
  cosval = cos(radians);
  sinval = sin(radians);

  oldHeight = (int)input->height;
  oldWidth = (int)input->width;

  newWidth = (int)abs((int)(oldWidth*cosval)) +
    (int)abs((int)(oldHeight*sinval));
  newHeight = (int)abs((int)(-oldWidth*sinval)) +
    (int)abs((int)(oldHeight*cosval));

  halfNewHeight = newHeight / 2;
  halfNewWidth = newWidth / 2;
  halfOldWidth = oldWidth /2;
  halfOldHeight = oldHeight /2 ;

  output = malloc(sizeof(t_bw_img) +
		  sizeof(unsigned char)*(uint)newWidth*(uint)newHeight);
  output->width	= (uint)newWidth;
  output->height= (uint)newHeight;

  for(int i=0;i < newHeight;i++)
  {
    for(int j=0;j < newWidth;j++)
    {
      nx = (int)( (j - halfNewWidth)*cosval +
		  (i-halfNewHeight)*sinval);
      ny = (int)( -((j - halfNewWidth)*sinval) +
		  (i - halfNewHeight)*cosval);
      nx = nx + halfOldWidth;
      ny = ny + halfOldHeight;
      if ((nx < oldWidth) && (ny < oldHeight)
	  && (nx > 0) && (ny > 0))
      {
	if(AT(input,(uint)nx,(uint)ny)==0)
	  AT(output, (uint)j, (uint)i) = 0;
	else
	  AT(output, (uint)j, (uint)i) = 255;
      }
      else
      {
	AT(output, (uint)j, (uint)i) = 255;
      }
    }
  }
  return output;

}
