gcc -o bmp_to_custom {bmp_to_custom,simple_segmentation}.c ../{bitmap/{bmp,img},binarisation/{bin_{sauvola,fixed,ratio,treshold},binarise,histogram},stats,rescale/rescale,base_structs/vector}.c -g
