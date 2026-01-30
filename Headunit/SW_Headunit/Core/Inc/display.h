/**
 * @file display.h
 *
 */

#ifndef DISPLAY_H
#define DISPLAY_H

/*********************
 *      INCLUDES
 *********************/

#include <stdint.h>
#include <stddef.h>
#include "lv_font_montserrat_20.h"

/*********************
 *      DEFINES
 *********************/


/**********************
 *      TYPEDEFS
 **********************/

typedef struct {
    uint8_t * target_buffer;    // Pointer to a working buffer
    uint16_t width;              // Working buffer width
    uint16_t height;             // Working buffer height
    int16_t xcursor;
    int16_t ycursor;
} display_descriptor;

/**********************
 * GLOBAL PROTOTYPES
 **********************/

void setCursor(int16_t x, int16_t y);
void putChar(uint8_t character);
void displayInit(uint8_t * target_buffer, uint16_t width, uint16_t height);

#endif /*DISPLAY_H*/
