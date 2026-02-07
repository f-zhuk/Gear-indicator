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
    float xcursor;
    float ycursor;
} display_descriptor;

/**********************
 * GLOBAL PROTOTYPES
 **********************/

void setCursor(float x, float y);
void putChar(uint8_t character);
void fillRectangle(uint16_t width, uint16_t height, uint8_t color);
void displayInit(uint8_t * target_buffer, uint16_t width, uint16_t height);

#endif /*DISPLAY_H*/
