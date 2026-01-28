#include "ST7735.h"

#include <limits.h>

#include "main.h"

#define SPI_DEFAULT_FREQ 32000000 ///< Default SPI data clock frequency

uint8_t _colstart = 0;   ///< Some displays need this changed to offset
uint8_t _rowstart = 0;       ///< Some displays need this changed to offset
uint8_t _xstart = 0;
uint8_t _ystart = 0;
uint8_t _height = 0;
uint8_t _width = 0;
uint8_t spiMode = 0; ///< Certain display needs MODE3 instead

// Regular grayscale
const uint32_t pallete[16] = 
{
    0x000000,
    0x111111,
    0x222222,
    0x333333,
    0x444444,
    0x555555,
    0x666666,
    0x777777,
    0x888888,
    0x999999,
    0xAAAAAA,
    0xBBBBBB,
    0xCCCCCC,
    0xDDDDDD,
    0xEEEEEE,
    0xFFFFFF,
};

SPI_HandleTypeDef *ST7735_SPI;

/**************************************************************************/
/*!
    @brief  Companion code to the initiliazation tables. Reads and issues
            a series of LCD commands stored in PROGMEM byte array.
    @param  addr  Flash memory array with commands and data to send
*/
/*************************************************************************
void displayInit(const uint8_t *addr) {

  uint8_t numCommands, cmd, numArgs;
  uint16_t ms;

  numCommands = *(addr++); // Number of commands to follow
  while (numCommands--) {              // For each command...
    cmd = *(addr++);       // Read command
    numArgs = *(addr++);   // Number of args to follow
    ms = numArgs & ST_CMD_DELAY;       // If hibit set, delay follows args
    numArgs &= ~ST_CMD_DELAY;          // Mask out delay bit
    sendCommand(cmd, addr, numArgs);
    addr += numArgs;

    if (ms) {
      ms = pgm_read_byte(addr++); // Read post-command delay time (ms)
      if (ms == 255)
        ms = 500; // If 255, delay for 500 ms
      delay(ms);
    }
  }
}
*/

void sendCommandData(uint8_t cmd, const uint8_t *addr, uint16_t numArgs)
{
  HAL_GPIO_WritePin(ST7735_CS_PIN, GPIO_PIN_SET);
  HAL_GPIO_WritePin(ST7735_DC_PIN, GPIO_PIN_RESET);
  HAL_Delay(1);
  HAL_GPIO_WritePin(ST7735_CS_PIN, GPIO_PIN_RESET);
  HAL_SPI_Transmit(ST7735_SPI, &cmd, 1, 100);
  if(numArgs)
  {
    HAL_GPIO_WritePin(ST7735_DC_PIN, GPIO_PIN_SET);
    HAL_SPI_Transmit_IT(ST7735_SPI, addr, numArgs); //Sending in Interrupt mode
  }
}
void sendCommand(uint8_t cmd)
{
  HAL_GPIO_WritePin(ST7735_CS_PIN, GPIO_PIN_SET);
  HAL_GPIO_WritePin(ST7735_DC_PIN, GPIO_PIN_RESET);
  HAL_Delay(1);
  HAL_GPIO_WritePin(ST7735_CS_PIN, GPIO_PIN_RESET);
  HAL_SPI_Transmit(ST7735_SPI, &cmd, 1, 100);
  HAL_GPIO_WritePin(ST7735_CS_PIN, GPIO_PIN_SET);
  HAL_GPIO_WritePin(ST7735_DC_PIN, GPIO_PIN_SET);
}

void displayInit(const uint8_t *addr) {

  uint8_t numCommands, cmd, numArgs;
  uint16_t ms;

  numCommands = *(addr++); // Number of commands to follow
  while (numCommands--) {              // For each command...
    cmd = *(addr++);       // Read command
    numArgs = *(addr++);   // Number of args to follow
    ms = numArgs & ST_CMD_DELAY;       // If hibit set, delay follows args
    numArgs &= ~ST_CMD_DELAY;          // Mask out delay bit
    sendCommandData(cmd, addr, numArgs);
    addr += numArgs;

    if (ms) {
      ms = *(addr++); // Read post-command delay time (ms)
      if (ms == 255)
        ms = 500; // If 255, delay for 500 ms
      HAL_Delay(ms);
    }
  }
}


/**************************************************************************/
/*!
    @brief  Initialization code common to all ST77XX displays
    @param  cmdList  Flash memory array with commands and data to send
*/
/**************************************************************************/
void commonInit(const uint8_t *cmdList) {
  //begin();

  if (cmdList) {
    displayInit(cmdList);
  }
}

/**************************************************************************/
/*!
  @brief  SPI displays set an address window rectangle for blitting pixels
  @param  x  Top left corner x coordinate
  @param  y  Top left corner y coordinate
  @param  w  Width of window
  @param  h  Height of window
*/
/**************************************************************************/
void setAddrWindow(uint8_t x, uint8_t y, uint8_t w, uint8_t h) {
  uint8_t BufferX [4] = {0x00, x, 0x00, (x+w-1)};
  sendCommandData(ST77XX_CASET, BufferX, 4); // Column addr set
  uint8_t BufferY [4] = {0x00, y, 0x00, (y+h-1)};
  sendCommandData(ST77XX_RASET, BufferY, 4);// Row addr set
  //SPI_WRITE32(ya);

  //writeCommand(ST77XX_RAMWR); // write to RAM
}

/**************************************************************************/
/*!
    @brief  Set origin of (0,0) of display with offsets
    @param  col  The offset from 0 for the column address
    @param  row  The offset from 0 for the row address
*/
/**************************************************************************/
void setColRowStart(int8_t col, int8_t row) {
  _colstart = col;
  _rowstart = row;
}

/**************************************************************************/
/*!
 @brief  Change whether display is on or off
 @param  enable True if you want the display ON, false OFF
 */
/**************************************************************************/
void enableDisplay(bool enable) {
  sendCommand(enable ? ST77XX_DISPON : ST77XX_DISPOFF);
}

/**************************************************************************/
/*!
 @brief  Change whether TE pin output is on or off
 @param  enable True if you want the TE pin ON, false OFF
 */
/**************************************************************************/
void enableTearing(bool enable) {
  sendCommand(enable ? ST77XX_TEON : ST77XX_TEOFF);
}

/**************************************************************************/
/*!
 @brief  Change whether sleep mode is on or off
 @param  enable True if you want sleep mode ON, false OFF
 */
/**************************************************************************/
void enableSleep(bool enable) {
  sendCommand(enable ? ST77XX_SLPIN : ST77XX_SLPOUT);
}

////////// stuff not actively being used, but kept for posterity
/*

 uint8_t ST77xx::spiread(void) {
 uint8_t r = 0;
 if (_sid > 0) {
 r = shiftIn(_sid, _sclk, MSBFIRST);
 } else {
 //SID_DDR &= ~_BV(SID);
 //int8_t i;
 //for (i=7; i>=0; i--) {
 //  SCLK_PORT &= ~_BV(SCLK);
 //  r <<= 1;
 //  r |= (SID_PIN >> SID) & 0x1;
 //  SCLK_PORT |= _BV(SCLK);
 //}
 //SID_DDR |= _BV(SID);

 }
 return r;
 }

 void ST77xx::dummyclock(void) {

 if (_sid > 0) {
 digitalWrite(_sclk, LOW);
 digitalWrite(_sclk, HIGH);
 } else {
 // SCLK_PORT &= ~_BV(SCLK);
 //SCLK_PORT |= _BV(SCLK);
 }
 }
 uint8_t ST77xx::readdata(void) {
 *portOutputRegister(rsport) |= rspin;

 *portOutputRegister(csport) &= ~ cspin;

 uint8_t r = spiread();

 *portOutputRegister(csport) |= cspin;

 return r;

 }

 uint8_t ST77xx::readcommand8(uint8_t c) {
 digitalWrite(_rs, LOW);

 *portOutputRegister(csport) &= ~ cspin;

 spiwrite(c);

 digitalWrite(_rs, HIGH);
 pinMode(_sid, INPUT); // input!
 digitalWrite(_sid, LOW); // low
 spiread();
 uint8_t r = spiread();


 *portOutputRegister(csport) |= cspin;


 pinMode(_sid, OUTPUT); // back to output
 return r;
 }


 uint16_t ST77xx::readcommand16(uint8_t c) {
 digitalWrite(_rs, LOW);
 if (_cs)
 digitalWrite(_cs, LOW);

 spiwrite(c);
 pinMode(_sid, INPUT); // input!
 uint16_t r = spiread();
 r <<= 8;
 r |= spiread();
 if (_cs)
 digitalWrite(_cs, HIGH);

 pinMode(_sid, OUTPUT); // back to output
 return r;
 }

 uint32_t ST77xx::readcommand32(uint8_t c) {
 digitalWrite(_rs, LOW);
 if (_cs)
 digitalWrite(_cs, LOW);
 spiwrite(c);
 pinMode(_sid, INPUT); // input!

 dummyclock();
 dummyclock();

 uint32_t r = spiread();
 r <<= 8;
 r |= spiread();
 r <<= 8;
 r |= spiread();
 r <<= 8;
 r |= spiread();
 if (_cs)
 digitalWrite(_cs, HIGH);

 pinMode(_sid, OUTPUT); // back to output
 return r;
 }

 */

// SCREEN INITIALIZATION ***************************************************

// Rather than a bazillion writecommand() and writedata() calls, screen
// initialization commands and arguments are organized in these tables
// stored in PROGMEM.  The table may look bulky, but that's mostly the
// formatting -- storage-wise this is hundreds of bytes more compact
// than the equivalent code.  Companion function follows.

// clang-format off
static const uint8_t
  Bcmd[] = {                        // Init commands for 7735B screens
    18,                             // 18 commands in list:
    ST77XX_SWRESET,   ST_CMD_DELAY, //  1: Software reset, no args, w/delay
      50,                           //     50 ms delay
    ST77XX_SLPOUT,    ST_CMD_DELAY, //  2: Out of sleep mode, no args, w/delay
      255,                          //     255 = max (500 ms) delay
    ST77XX_COLMOD,  1+ST_CMD_DELAY, //  3: Set color mode, 1 arg + delay:
      0x05,                         //     16-bit color
      10,                           //     10 ms delay
    ST7735_FRMCTR1, 3+ST_CMD_DELAY, //  4: Frame rate control, 3 args + delay:
      0x00,                         //     fastest refresh
      0x06,                         //     6 lines front porch
      0x03,                         //     3 lines back porch
      10,                           //     10 ms delay
    ST77XX_MADCTL,  1,              //  5: Mem access ctl (directions), 1 arg:
      0x08,                         //     Row/col addr, bottom-top refresh
    ST7735_DISSET5, 2,              //  6: Display settings #5, 2 args:
      0x15,                         //     1 clk cycle nonoverlap, 2 cycle gate
                                    //     rise, 3 cycle osc equalize
      0x02,                         //     Fix on VTL
    ST7735_INVCTR,  1,              //  7: Display inversion control, 1 arg:
      0x0,                          //     Line inversion
    ST7735_PWCTR1,  2+ST_CMD_DELAY, //  8: Power control, 2 args + delay:
      0x02,                         //     GVDD = 4.7V
      0x70,                         //     1.0uA
      10,                           //     10 ms delay
    ST7735_PWCTR2,  1,              //  9: Power control, 1 arg, no delay:
      0x05,                         //     VGH = 14.7V, VGL = -7.35V
    ST7735_PWCTR3,  2,              // 10: Power control, 2 args, no delay:
      0x01,                         //     Opamp current small
      0x02,                         //     Boost frequency
    ST7735_VMCTR1,  2+ST_CMD_DELAY, // 11: Power control, 2 args + delay:
      0x3C,                         //     VCOMH = 4V
      0x38,                         //     VCOML = -1.1V
      10,                           //     10 ms delay
    ST7735_PWCTR6,  2,              // 12: Power control, 2 args, no delay:
      0x11, 0x15,
    ST7735_GMCTRP1,16,              // 13: Gamma Adjustments (pos. polarity), 16 args + delay:
      0x09, 0x16, 0x09, 0x20,       //     (Not entirely necessary, but provides
      0x21, 0x1B, 0x13, 0x19,       //      accurate colors)
      0x17, 0x15, 0x1E, 0x2B,
      0x04, 0x05, 0x02, 0x0E,
    ST7735_GMCTRN1,16+ST_CMD_DELAY, // 14: Gamma Adjustments (neg. polarity), 16 args + delay:
      0x0B, 0x14, 0x08, 0x1E,       //     (Not entirely necessary, but provides
      0x22, 0x1D, 0x18, 0x1E,       //      accurate colors)
      0x1B, 0x1A, 0x24, 0x2B,
      0x06, 0x06, 0x02, 0x0F,
      10,                           //     10 ms delay
    ST77XX_CASET,   4,              // 15: Column addr set, 4 args, no delay:
      0x00, 0x02,                   //     XSTART = 2
      0x00, 0x81,                   //     XEND = 129
    ST77XX_RASET,   4,              // 16: Row addr set, 4 args, no delay:
      0x00, 0x02,                   //     XSTART = 1
      0x00, 0x81,                   //     XEND = 160
    ST77XX_NORON,     ST_CMD_DELAY, // 17: Normal display on, no args, w/delay
      10,                           //     10 ms delay
    ST77XX_DISPON,    ST_CMD_DELAY, // 18: Main screen turn on, no args, delay
      255 },                        //     255 = max (500 ms) delay

  Rcmd1[] = {                       // 7735R init, part 1 (red or green tab)
    15,                             // 15 commands in list:
    ST77XX_SWRESET,   ST_CMD_DELAY, //  1: Software reset, 0 args, w/delay
      150,                          //     150 ms delay
    ST77XX_SLPOUT,    ST_CMD_DELAY, //  2: Out of sleep mode, 0 args, w/delay
      255,                          //     500 ms delay
    ST7735_FRMCTR1, 3,              //  3: Framerate ctrl - normal mode, 3 arg:
      0x01, 0x2C, 0x2D,             //     Rate = fosc/(1x2+40) * (LINE+2C+2D)
    ST7735_FRMCTR2, 3,              //  4: Framerate ctrl - idle mode, 3 args:
      0x01, 0x2C, 0x2D,             //     Rate = fosc/(1x2+40) * (LINE+2C+2D)
    ST7735_FRMCTR3, 6,              //  5: Framerate - partial mode, 6 args:
      0x01, 0x2C, 0x2D,             //     Dot inversion mode
      0x01, 0x2C, 0x2D,             //     Line inversion mode
    ST7735_INVCTR,  1,              //  6: Display inversion ctrl, 1 arg:
      0x07,                         //     No inversion
    ST7735_PWCTR1,  3,              //  7: Power control, 3 args, no delay:
      0xA2,
      0x02,                         //     -4.6V
      0x84,                         //     AUTO mode
    ST7735_PWCTR2,  1,              //  8: Power control, 1 arg, no delay:
      0xC5,                         //     VGH25=2.4C VGSEL=-10 VGH=3 * AVDD
    ST7735_PWCTR3,  2,              //  9: Power control, 2 args, no delay:
      0x0A,                         //     Opamp current small
      0x00,                         //     Boost frequency
    ST7735_PWCTR4,  2,              // 10: Power control, 2 args, no delay:
      0x8A,                         //     BCLK/2,
      0x2A,                         //     opamp current small & medium low
    ST7735_PWCTR5,  2,              // 11: Power control, 2 args, no delay:
      0x8A, 0xEE,
    ST7735_VMCTR1,  1,              // 12: Power control, 1 arg, no delay:
      0x0E,
    ST77XX_INVOFF,  0,              // 13: Don't invert display, no args
    ST77XX_MADCTL,  1,              // 14: Mem access ctl (directions), 1 arg:
      0xC8,                         //     row/col addr, bottom-top refresh
    ST77XX_COLMOD,  1,              // 15: set color mode, 1 arg, no delay:
      0x03 },                       //     16-bit color

  Rcmd2green160x80[] = {            // 7735R init, part 2 (mini 160x80)
    2,                              //  2 commands in list:
    ST77XX_CASET,   4,              //  1: Column addr set, 4 args, no delay:
      0x00, 0x00,                   //     XSTART = 0
      0x00, 0x4F,                   //     XEND = 79
    ST77XX_RASET,   4,              //  2: Row addr set, 4 args, no delay:
      0x00, 0x00,                   //     XSTART = 0
      0x00, 0x9F },                 //     XEND = 159

  Rcmd3[] = {                       // 7735R init, part 3 (red or green tab)
    4,                              //  4 commands in list:
    ST7735_GMCTRP1, 16      ,       //  1: Gamma Adjustments (pos. polarity), 16 args + delay:
      0x02, 0x1c, 0x07, 0x12,       //     (Not entirely necessary, but provides
      0x37, 0x32, 0x29, 0x2d,       //      accurate colors)
      0x29, 0x25, 0x2B, 0x39,
      0x00, 0x01, 0x03, 0x10,
    ST7735_GMCTRN1, 16      ,       //  2: Gamma Adjustments (neg. polarity), 16 args + delay:
      0x03, 0x1d, 0x07, 0x06,       //     (Not entirely necessary, but provides
      0x2E, 0x2C, 0x29, 0x2D,       //      accurate colors)
      0x2E, 0x2E, 0x37, 0x3F,
      0x00, 0x00, 0x02, 0x10,
    ST77XX_NORON,     ST_CMD_DELAY, //  3: Normal display on, no args, w/delay
      10,                           //     10 ms delay
    ST77XX_DISPON,    ST_CMD_DELAY, //  4: Main screen turn on, no args w/delay
      100 };                        //     100 ms delay

// clang-format on

/**************************************************************************/
/*!
    @brief  Initialization code common to all ST7735B displays
*/
/**************************************************************************/
void initB(void) {
  commonInit(Bcmd);
  setRotation(0);
}

/**************************************************************************/
/*!
    @brief  Initialization code common to all ST7735R displays
    @param  options  Tab color from adafruit purchase
*/
/**************************************************************************/
void initR(SPI_HandleTypeDef *hspi) {
  ST7735_SPI = hspi;

  HAL_GPIO_WritePin(ST7735_RES_PIN, GPIO_PIN_SET);
  HAL_GPIO_WritePin(ST7735_CS_PIN, GPIO_PIN_SET);
  HAL_GPIO_WritePin(ST7735_DC_PIN, GPIO_PIN_SET);
  commonInit(Rcmd1);

  _height = ST7735_TFTWIDTH;
  _width = ST7735_TFTHEIGHT;
  displayInit(Rcmd2green160x80);
  _colstart = 24;
  _rowstart = 0;

  displayInit(Rcmd3);

  // Black tab, change MADCTL color filter
  uint8_t data = 0xC0;
  sendCommandData(ST77XX_MADCTL, &data, 1);

  setRotation(0);
}

// OTHER FUNCTIONS *********************************************************

/**************************************************************************/
/*!
    @brief  Set origin of (0,0) and orientation of TFT display
    @param  m  The index for rotation, from 0-3 inclusive
*/
/**************************************************************************/
void setRotation(uint8_t m) {
  uint8_t madctl = ST77XX_MADCTL_RGB;

  uint8_t rotation = m & 3; // can't be higher than 3

  switch (rotation) {
  case 0:
    
    madctl |= ST77XX_MADCTL_MX | ST77XX_MADCTL_MY;
    
    _height = ST7735_TFTHEIGHT;
    _width = ST7735_TFTWIDTH;
    _xstart = _colstart;
    _ystart = _rowstart;
    break;
  case 1:

    madctl |= ST77XX_MADCTL_MY | ST77XX_MADCTL_MV;
    
    _width = ST7735_TFTHEIGHT;
    _height = ST7735_TFTWIDTH;
    _ystart = _colstart;
    _xstart = _rowstart;
    break;
  case 2:
    _height = ST7735_TFTHEIGHT;
    _width = ST7735_TFTWIDTH;
    _xstart = _colstart;
    _ystart = _rowstart;
    break;
  case 3:
  
    madctl |= ST77XX_MADCTL_MX | ST77XX_MADCTL_MV;

    _width = ST7735_TFTHEIGHT;
    _height = ST7735_TFTWIDTH;
    _ystart = _colstart;
    _xstart = _rowstart;
    break;
  }

  sendCommandData(ST77XX_MADCTL, &madctl, 1);
}
