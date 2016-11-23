#ifndef MORPHO_H_
#define MORPHO_H_

#include "../bitmap/img.h"
#include "../error.h"
#include "../tdefs.h"

t_bw_img *dilate(t_bw_img *input);

t_bw_img *erode(t_bw_img *input);

uint pix_diff(t_bw_img *input1, t_bw_img *input2);

void new_dilate(t_bw_img *input,
		t_bw_img *output,
		uint mask_width,
		uint mask_height,
	        unsigned char is_erode,
		float mask[mask_height][mask_width]);

void new_hor_dilate(t_bw_img *input,
		    t_bw_img * output,
		    uint size);

void new_ver_dilate(t_bw_img *input,
		    t_bw_img *output,
		    uint size);

void new_hor_erode(t_bw_img *input,
		   t_bw_img * output,
		   uint size);

void new_ver_erode(t_bw_img *input,
		   t_bw_img *output,
		   uint size);

void close_ver(t_bw_img *input, t_bw_img *output, uint size);

void close_hor(t_bw_img *input, t_bw_img *output, uint size);

void open_hor(t_bw_img *input, t_bw_img *output, uint size);

void open_ver(t_bw_img *input, t_bw_img *output, uint size);

t_bw_img *close_morph(t_bw_img *img, uint iterations);

t_bw_img *close_hor_morph(t_bw_img *img, uint iterations);

t_bw_img *close_ver_morph(t_bw_img *img, uint iterations);

t_bw_img *open_morph(t_bw_img *img, uint iterations);

void xor_morph(t_bw_img *output, t_bw_img *input);

void border_morph(t_bw_img *img);

#endif
