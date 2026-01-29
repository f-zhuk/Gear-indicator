#ifndef _ADAFRUIT_ST7735H_
#define _ADAFRUIT_ST7735H_

#include <stdint.h>
#include "stm32f1xx_hal.h"

#define ST7735_RES_PIN GPIOA, GPIO_PIN_3
#define ST7735_CS_PIN GPIOA, GPIO_PIN_4
#define ST7735_DC_PIN GPIOA, GPIO_PIN_6

// Some register settings
#define ST7735_MADCTL_BGR 0x08
#define ST7735_MADCTL_MH 0x04

#define ST7735_FRMCTR1 0xB1
#define ST7735_FRMCTR2 0xB2
#define ST7735_FRMCTR3 0xB3
#define ST7735_INVCTR 0xB4
#define ST7735_DISSET5 0xB6

#define ST7735_PWCTR1 0xC0
#define ST7735_PWCTR2 0xC1
#define ST7735_PWCTR3 0xC2
#define ST7735_PWCTR4 0xC3
#define ST7735_PWCTR5 0xC4
#define ST7735_VMCTR1 0xC5

#define ST7735_PWCTR6 0xFC

#define ST7735_GMCTRP1 0xE0
#define ST7735_GMCTRN1 0xE1

#define ST7735_XOFFSET 26
#define ST7735_YOFFSET 1
#define ST7735_WIDTH 80
#define ST7735_HEIGHT 160
#define ST7735_BUFFER (ST7735_WIDTH/2)*(ST7735_HEIGHT/2)/2

#define ST_CMD_DELAY 0x80 // special signifier for command lists

#define ST77XX_NOP 0x00
#define ST77XX_SWRESET 0x01
#define ST77XX_RDDID 0x04
#define ST77XX_RDDST 0x09

#define ST77XX_SLPIN 0x10
#define ST77XX_SLPOUT 0x11
#define ST77XX_PTLON 0x12
#define ST77XX_NORON 0x13

#define ST77XX_INVOFF 0x20
#define ST77XX_INVON 0x21
#define ST77XX_DISPOFF 0x28
#define ST77XX_DISPON 0x29
#define ST77XX_CASET 0x2A
#define ST77XX_RASET 0x2B
#define ST77XX_RAMWR 0x2C
#define ST77XX_RAMRD 0x2E

#define ST77XX_PTLAR 0x30
#define ST77XX_TEOFF 0x34
#define ST77XX_TEON 0x35
#define ST77XX_MADCTL 0x36
#define ST77XX_IDMOFF 0x38
#define ST77XX_IDMON 0x39
#define ST77XX_COLMOD 0x3A

#define ST77XX_MADCTL_MY 0x80
#define ST77XX_MADCTL_MX 0x40
#define ST77XX_MADCTL_MV 0x20
#define ST77XX_MADCTL_ML 0x10
#define ST77XX_MADCTL_RGB 0x00

#define ST77XX_RDID1 0xDA
#define ST77XX_RDID2 0xDB
#define ST77XX_RDID3 0xDC
#define ST77XX_RDID4 0xDD


extern SPI_HandleTypeDef *ST7735_SPI;
extern uint8_t st7735_buffer[ST7735_BUFFER];
extern uint32_t st7735_pallete[16];

void sendCommandData(uint8_t cmd, const uint8_t *addr, uint16_t numArgs);
void sendCommand(uint8_t cmd);
void redraw(void);

void setAddrWindow(uint8_t x, uint8_t y, uint8_t w, uint8_t h);
void setRotation(uint8_t r);
void enableDisplay(bool enable);
void enableTearing(bool enable);
void enableSleep(bool enable);
void enableInvert(bool enable);

//void begin(uint32_t freq = 0);
void commonInit(const uint8_t *cmdList);
void displayInit(const uint8_t *addr);
void setColRowStart(int8_t col, int8_t row);
//static const uint8_t Bcmd[];

void initB(void);                             // for ST7735B displays
void initR(SPI_HandleTypeDef *hspi);          // for ST7735R

void setRotation(uint8_t m);

#endif // _ADAFRUIT_ST7735H_
