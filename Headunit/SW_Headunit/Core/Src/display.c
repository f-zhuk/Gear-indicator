/**
 * @file display.c
 *
 */

/*********************
 *      INCLUDES
 *********************/

#include "display.h"

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/

/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *  VARIABLES
 **********************/

 display_descriptor display;

/**********************
 * GLOBAL PROTOTYPES
 **********************/

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

void setCursor(float x, float y)
{
    display.xcursor = x;
    display.ycursor = y;
}

void putChar(uint8_t character)
{
    font_t font = lv_font_montserrat_20;
    uint8_t cmap = 0;
    character -= (font.cmaps[cmap].range_start - font.cmaps[cmap].glyph_id_start);
    uint32_t bmi = font.glyph_dsc[character].bitmap_index; //Bitmap index
    uint8_t * bm = font.glyph_bitmap;  // Bitmap

    int16_t xstart = (int16_t)display.xcursor+font.glyph_dsc[character].ofs_x;
    int16_t ystart = (int16_t)display.ycursor+font.glyph_dsc[character].ofs_y+font.base_line-font.glyph_dsc[character].box_h;

    int16_t box_w = font.glyph_dsc[character].box_w;
    if(xstart+box_w>display.width) // Check right
        box_w = display.width-xstart;
    int16_t box_h = font.glyph_dsc[character].box_h;
    if(ystart+box_h>display.height) // Check bottom
        box_h = display.height-ystart;

    uint16_t i=0;
    if(ystart<0) // Check top
        i = -ystart;
    while (i<box_h)
    {
        uint16_t j=0;
        if(xstart<0) // Check left
            j = -xstart;
        while (j<box_w)
        {
            uint8_t nibble = (bm[bmi+((i*font.glyph_dsc[character].box_w+j)>>1)] >> ((~(i*font.glyph_dsc[character].box_w+j)&0x01)<<2));
            if(nibble)
            {
                uint16_t wbi = ((display.width)*(i+ystart)+xstart+j); // Working buffer index
                display.target_buffer[wbi>>1] &= (0x0F<<((wbi&0x01)<<2));
                display.target_buffer[wbi>>1] |=  (nibble << ((~wbi&0x01)<<2));
            }
            j++;
        }
        i++;
    }
    display.xcursor = xstart+box_w+1;
}


void fillRectangle(uint16_t width, uint16_t height, uint8_t color)
{
    color &= 0x0f;

    if ((int16_t)display.xcursor+width > display.width) // Check right
        width = display.width-display.xcursor;
    if ((int16_t)display.ycursor+height > display.height) // Check bottom
        height = display.height-display.ycursor;

    uint16_t i=0;    
    if ((int16_t)display.ycursor < 0) // Check top
        i = -(int16_t)display.ycursor;
    while (i<height)
    {
        uint16_t j=0;
        if ((int16_t)display.xcursor < 0) // Check left
            j = -(int16_t)display.xcursor;
        while (j<width)
        {
            uint16_t wbi = (((display.width)*(i+(int16_t)display.ycursor))+(int16_t)display.xcursor+j); // Working buffer index
            display.target_buffer[wbi>>1] &= (0x0F<<((wbi&0x01)<<2));
            display.target_buffer[wbi>>1] |=  (color << ((~wbi&0x01)<<2));
            j++;
        }
        i++;
    }
    display.xcursor += width;
    display.ycursor += height;
}

void displayInit(uint8_t *target_buffer, uint16_t width, uint16_t height)
{
    display.target_buffer = target_buffer;
    display.width = width;
    display.height = height;
    setCursor(0,0);
    for(uint32_t i = 0; i<(width*height); i++)
        display.target_buffer[i>>1] = (display.target_buffer[i>>1] & (0x0F<<((i&1)<<2))) | (0x00<<((~i&1)<<2));
}

/**********************
 *   STATIC FUNCTIONS
 **********************/
