#ifndef RESCALE_H_
#define RESCALE_H_

/*

*/
void interpolate(t_bw_img *input,
		 t_bw_img *output,
		 uint mask_len,
		 float mask[][mask_len],
		 double factor);


/*
Return an scaled image according to a ratio.
The aspect ratio is kept.
 */
t_bw_img *resize_tbw(t_bw_img *input, double factor);

/*
Given an input image, return a scaled image with padding

resize free the input image

Function to be called.
 */
t_bw_img *resize(t_bw_img *input, uint width, uint height);

/*
Fill the input image in order to get an image of width and height given
Input size should not be bigger than in width or height

fill_padding free the input image
 */
t_bw_img *fill_padding(t_bw_img *input, uint width, uint height);

#endif
