#include "recognise.h"
#include "../boxing/wrapper.h"
#include "../block_to_line/block_to_line.h"
#include "../character_segmentation/segmentation.h"
#include "../line_detection/line_detection.h"
#include "../error.h"
// shouldn't take a network as argument, just a placeholder

static t_string *recognise_block(t_w_network_vect *net, t_block *block);
static t_string *recognise_line(t_w_network_vect *net, t_line *img);

t_string *recognise_bw_img(t_w_network_vect *net, t_bw_img *img)
{
  t_string *res = alloc_sized_string("", 2000);
  t_sub_bw_img_vect *blocks = block_segment(img);
  for(size_t i = 0; i < VECT_SIZE(blocks); i++)
  {
    t_block *cur = VECT_GET(blocks, i);
    t_string *block_content = recognise_block(net, cur);
    string_append(res, block_content);
    string_c_append(res, "\n\n");
    string_free(block_content);
    free_sub_bw_img(cur);
  }
  VECT_FREE(blocks);
  return res;
}

static t_string *recognise_block(t_w_network_vect *net, t_block *block)
{
  t_string *res = alloc_sized_string("", 500);
  //t_sub_bw_img_vect *lines = line_subdivision(block);
  t_sub_bw_img_vect *lines = detect_lines(block);
  for(size_t i = 0; i < VECT_SIZE(lines); i++)
  {
    t_line *cur = VECT_GET(lines, i);
    t_string *line_content = recognise_line(net, cur);
    string_append(res, line_content);
    string_c_append(res, "\n");
    string_free(line_content);
    free_sub_bw_img(cur);
  }
  VECT_FREE(lines);
  return res;
}

static t_string *recognise_line(t_w_network_vect *net, t_line *line)
{
  t_string *res = alloc_sized_string("", 100);
  t_l_bw_img_vect *chars = char_segmentation_l(line);
  for(size_t i = 0; i < VECT_SIZE(chars); i++)
  {
    t_char *cur = VECT_GET(chars, i);
    if(cur)
    {
      for(uint y = 0; y < cur->height; y++)
      {
	for(uint x = 0; x < cur->width; x++)
	  printf("%3d", L_AT(cur, x, y));
	puts("");
      }
      puts("");

      t_string *char_content = char_recon(net, cur);
      string_append(res, char_content);
      string_free(char_content);
    }
    else
    {
      puts("=============================");
      string_c_append(res, " ");

    }
    free_l_bw_img(cur);
  }
  VECT_FREE(chars);
  return res;
}
