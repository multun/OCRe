int isWhiteColumn(t_sub_bw_img *img, int column)
{
    int isWhite = 1;
    
    for(uint i = 0; i< img -> height; i++)
    {
        if(img[i][column].color() != [255,255,255]) ##Not sure about the syntax('if this pixel of img is not white')
        {
            isWhite = 0;
        }
    }
    
    return isWhite;
}

t_sub_bw_img_vect segmentation(t_sub_bw_img *img)
{
    t_sub_bw_img_vect result;
    int elements = 0;
    int aux = 0;
    
    for(uint i = 0; i < img -> width; i++)
    {
        if (isWhiteColumn(img, i) || i == width-1)
        {
            if(i>0)
            {
                if(isWhiteColumn(img, i-1) == 0)
                {
                    result[elements++] = img[aux->i] ##result gets appended img(from column aux to i)
                }
            }
            aux++;
        }     
    }
    return result;
}