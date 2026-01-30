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

void setCursor(int16_t x, int16_t y)
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

    uint8_t box_w = font.glyph_dsc[character].box_w;
    uint8_t box_h = font.glyph_dsc[character].box_h;

    int16_t xstart = display.xcursor+font.glyph_dsc[character].ofs_x;
    int16_t ystart = display.ycursor+font.base_line-font.line_height+font.glyph_dsc[character].ofs_y;

    for (uint16_t i=0; i<box_h; i++)
    {
        for (uint16_t j=0; j<box_w; j++)
        {
            uint8_t nibble = (bm[bmi+((i*box_w+j)>>1)] >> ((~(i*box_w+j)&0x01)<<2));
            if(nibble)
            {
                uint16_t wbi = ((display.width)*(i+ystart)+xstart+j); // Working buffer index
                display.target_buffer[wbi>>1] &= (0x0F<<((wbi&0x01)<<2));
                display.target_buffer[wbi>>1] |=  (nibble << ((~wbi&0x01)<<2));
            }
        }
    }
    display.xcursor = xstart+box_w+1;
}

void displayInit(uint8_t *target_buffer, uint16_t width, uint16_t height)
{
    display.target_buffer = target_buffer;
    display.width = width;
    display.height = height;
    setCursor(0,0);
    for(uint32_t i = 0; i<(width*height); i++)
        display.target_buffer[i>>1] = (display.target_buffer[i>>1] & (0x0F<<((i&1)<<2))) | (0x02<<((~i&1)<<2));
}

/**********************
 *   STATIC FUNCTIONS
 **********************/
