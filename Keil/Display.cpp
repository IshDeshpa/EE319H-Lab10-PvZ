#include "ST7735SD.h"
#include "Display.h"
#include <stdint.h>
#include <stdlib.h>

static uint8_t ColStart, RowStart; // some displays need this changed
static uint8_t Rotation;           // 0 to 3
static enum initRFlags TabColor;
static int16_t _width = DISPLAY_TFTWIDTH;   // this could probably be a constant, except it is used in Adafruit_GFX and depends on image rotation
static int16_t _height = DISPLAY_TFTHEIGHT;

// 16 rows (0 to 15) and 21 characters (0 to 20)
// Requires (11 + size*size*6*8) bytes of transmission for each character
uint32_t StX=0; // position along the horizonal axis 0 to 20
uint32_t StY=0; // position along the vertical axis 0 to 15
uint16_t StTextColor = DISPLAY_YELLOW;

static cmdList Bcmd = {
	18,                       // 18 commands in list:
    {{DISPLAY_SWRESET,   0, {},  //  1: Software reset, no args, w/delay
      50},                     //     50 ms delay
    {DISPLAY_SLPOUT ,   0, {},  //  2: Out of sleep mode, no args, w/delay
      255},                    //     255 = 500 ms delay
    {DISPLAY_COLMOD , 1,   //  3: Set color mode, 1 arg + delay:
      {0x05},                   //     16-bit color
      10},                     //     10 ms delay
    {DISPLAY_FRMCTR1, 3,  //  4: Frame rate control, 3 args + delay:
      {0x00,                   //     fastest refresh
      0x06,                   //     6 lines front porch
      0x03},                   //     3 lines back porch
      10},                     //     10 ms delay
    {DISPLAY_MADCTL , 1      ,  //  5: Memory access ctrl (directions), 1 arg:
      0x08,											//     Row addr/col addr, bottom to top refresh
			0},												//   No delay
    {DISPLAY_DISSET5, 2      ,  //  6: Display settings #5, 2 args, no delay:
      {0x15,                   //     1 clk cycle nonoverlap, 2 cycle gate
                              //     rise, 3 cycle osc equalize
      0x02},
			0},												//     Fix on VTL
    {DISPLAY_INVCTR , 1      ,  //  7: Display inversion control, 1 arg:
      {0x0},
				0},											//     Line inversion
    {DISPLAY_PWCTR1 , 2,  //  8: Power control, 2 args + delay:
      {0x02,                   //     GVDD = 4.7V
      0x70},                   //     1.0uA
      10},                     //     10 ms delay
    {DISPLAY_PWCTR2 , 1      ,  //  9: Power control, 1 arg, no delay:
      {0x05},
			0},												//     VGH = 14.7V, VGL = -7.35V
    {DISPLAY_PWCTR3 , 2      ,  // 10: Power control, 2 args, no delay:
      {0x01,                   //     Opamp current small
      0x02},
			0},												//     Boost frequency
    {DISPLAY_VMCTR1 , 2,  // 11: Power control, 2 args + delay:
      {0x3C,                   //     VCOMH = 4V
      0x38},                   //     VCOML = -1.1V
      10},                     //     10 ms delay
    {DISPLAY_PWCTR6 , 2      ,  // 12: Power control, 2 args, no delay:
      {0x11, 0x15}, 0},
    {DISPLAY_GMCTRP1,16      ,  // 13: Magical unicorn dust, 16 args, no delay:
      {0x09, 0x16, 0x09, 0x20, //     (seriously though, not sure what
      0x21, 0x1B, 0x13, 0x19, //      these config values represent)
      0x17, 0x15, 0x1E, 0x2B,
      0x04, 0x05, 0x02, 0x0E}, 0},
    {DISPLAY_GMCTRN1,16,  // 14: Sparkles and rainbows, 16 args + delay:
      {0x0B, 0x14, 0x08, 0x1E, //     (ditto)
      0x22, 0x1D, 0x18, 0x1E,
      0x1B, 0x1A, 0x24, 0x2B,
      0x06, 0x06, 0x02, 0x0F},
      10},                     //     10 ms delay
    {DISPLAY_CASET  , 4      ,  // 15: Column addr set, 4 args, no delay:
      {0x00, 0x02,             //     XSTART = 2
      0x00, 0x81}, 0},			//     XEND = 129
    {DISPLAY_RASET  , 4      ,  // 16: Row addr set, 4 args, no delay:
      {0x00, 0x02,             //     XSTART = 1
      0x00, 0x81}, 0},             //     XEND = 160
    {DISPLAY_NORON  ,   0, {},  // 17: Normal display on, no args, w/delay
      10},                     //     10 ms delay
    {DISPLAY_DISPON ,   0, {}, // 18: Main screen turn on, no args, w/delay
      255}}, 										//     255 = 500 ms delay
};

static cmdList Rcmd1 = {                 // Init for 7735R, part 1 (red or green tab)
    15,                       // 15 commands in list:
    {{DISPLAY_SWRESET,   0, {},  //  1: Software reset, 0 args, w/delay
      150},                    //     150 ms delay
    {DISPLAY_SLPOUT ,   0, {},  //  2: Out of sleep mode, 0 args, w/delay
      255},                    //     500 ms delay
    {DISPLAY_FRMCTR1, 3      ,  //  3: Frame rate ctrl - normal mode, 3 args:
      {0x01, 0x2C, 0x2D}, 0},       //     Rate = fosc/(1x2+40) * (LINE+2C+2D)
    {DISPLAY_FRMCTR2, 3      ,  //  4: Frame rate control - idle mode, 3 args:
      {0x01, 0x2C, 0x2D}, 0},      //     Rate = fosc/(1x2+40) * (LINE+2C+2D)
    {DISPLAY_FRMCTR3, 6      ,  //  5: Frame rate ctrl - partial mode, 6 args:
      {0x01, 0x2C, 0x2D,       //     Dot inversion mode
      0x01, 0x2C, 0x2D}, 0},       //     Line inversion mode
    {DISPLAY_INVCTR , 1      ,  //  6: Display inversion ctrl, 1 arg, no delay:
      {0x07}, 0},                   //     No inversion
    {DISPLAY_PWCTR1 , 3      ,  //  7: Power control, 3 args, no delay:
      {0xA2,
      0x02,                   //     -4.6V
      0x84}, 0},                   //     AUTO mode
    {DISPLAY_PWCTR2 , 1      ,  //  8: Power control, 1 arg, no delay:
      {0xC5}, 0},                   //     VGH25 = 2.4C VGSEL = -10 VGH = 3 * AVDD
    {DISPLAY_PWCTR3 , 2      ,  //  9: Power control, 2 args, no delay:
      {0x0A,                   //     Opamp current small
      0x00}, 0},                   //     Boost frequency
    {DISPLAY_PWCTR4 , 2      ,  // 10: Power control, 2 args, no delay:
      {0x8A,                   //     BCLK/2, Opamp current small & Medium low
      0x2A}, 0},
    {DISPLAY_PWCTR5 , 2      ,  // 11: Power control, 2 args, no delay:
      {0x8A, 0xEE}, 0},
    {DISPLAY_VMCTR1 , 1      ,  // 12: Power control, 1 arg, no delay:
      {0x0E}, 0},
    {DISPLAY_INVOFF , 0, {}, 0},  // 13: Don't invert display, no args, no delay
    {DISPLAY_MADCTL , 1      ,  // 14: Memory access control (directions), 1 arg:
      {0xC8}, 0},                   //     row addr/col addr, bottom to top refresh
    {DISPLAY_COLMOD , 1      ,  // 15: set color mode, 1 arg, no delay:
      {0x05}, 0}}};                 //     16-bit color
	
static const cmdList Rcmd2green = {            // Init for 7735R, part 2 (green tab only)
    2,                        //  2 commands in list:
    {{DISPLAY_CASET  , 4      ,  //  1: Column addr set, 4 args, no delay:
      {0x00, 0x02,             //     XSTART = 0
      0x00, 0x7F+0x02}, 0},        //     XEND = 127
    {DISPLAY_RASET  , 4      ,  //  2: Row addr set, 4 args, no delay:
      {0x00, 0x01,             //     XSTART = 0
      0x00, 0x9F+0x01}, 0}}};      //     XEND = 159

static const cmdList Rcmd2red = {              // Init for 7735R, part 2 (red tab only)
    2,                        //  2 commands in list:
    {{DISPLAY_CASET  , 4      ,  //  1: Column addr set, 4 args, no delay:
      {0x00, 0x00,             //     XSTART = 0
      0x00, 0x7F}, 0},             //     XEND = 127
    {DISPLAY_RASET  , 4      ,  //  2: Row addr set, 4 args, no delay:
      {0x00, 0x00,             //     XSTART = 0
      0x00, 0x9F}, 0 }}};           //     XEND = 159

static const cmdList Rcmd3 = {                 // Init for 7735R, part 3 (red or green tab)
    4,                        //  4 commands in list:
    {{DISPLAY_GMCTRP1, 16      , //  1: Magical unicorn dust, 16 args, no delay:
      {0x02, 0x1c, 0x07, 0x12,
      0x37, 0x32, 0x29, 0x2d,
      0x29, 0x25, 0x2B, 0x39,
      0x00, 0x01, 0x03, 0x10}, 0},
    {DISPLAY_GMCTRN1, 16      , //  2: Sparkles and rainbows, 16 args, no delay:
      {0x03, 0x1d, 0x07, 0x06,
      0x2E, 0x2C, 0x29, 0x2D,
      0x2E, 0x2E, 0x37, 0x3F,
      0x00, 0x00, 0x02, 0x10}, 0},
    {DISPLAY_NORON  ,    0, {}, //  3: Normal display on, no args, w/delay
      10},                     //     10 ms delay
    {DISPLAY_DISPON ,    0, {}, //  4: Main screen turn on, no args w/delay
      100}}};                  //     100 ms delay

			
// Initialization code common to both 'B' and 'R' type displays
void static commonInit(const cmdList* c) {
  ColStart  = RowStart = 0; // May be overridden in init func
  
	SSI0_Init(2);
	SwitchToDisplay();
  if(c) commandList(c);
}

//------------Display_InitB------------
// Initialization for ST7735B screens.
// Input: none
// Output: none
void Display_InitB(void) {
  commonInit(&Bcmd);
  Display_SetCursor(0,0);
  StTextColor = DISPLAY_YELLOW;
  Display_FillScreen(0);                 // set screen to black
}


//------------ST7735_InitR------------
// Initialization for ST7735R screens (green or red tabs).
// Input: option one of the enumerated options depending on tabs
// Output: none
void ST7735_InitR(enum initRFlags option) {
	commonInit(&Rcmd1);
	SwitchToDisplay();
  if(option == INITR_GREENTAB) {
    commandList(&Rcmd2green);
    ColStart = 2;
    RowStart = 1;
  } else {
    // colstart, rowstart left at default '0' values
    commandList(&Rcmd2red);
  }
  commandList(&Rcmd3);

  // if black, change MADCTL color filter
  if (option == INITR_BLACKTAB) {
    xchg_spi(DISPLAY_MADCTL, DC_COMMAND);
    xchg_spi(0xC0, DC_DATA);
  }
  TabColor = option;
  Display_SetCursor(0,0);
  StTextColor = DISPLAY_YELLOW;
  Display_FillScreen(0);                 // set screen to black
}

// Set the region of the screen RAM to be modified
// Pixel colors are sent left to right, top to bottom
// (same as Font table is encoded; different from regular bitmap)
// Requires 11 bytes of transmission
void static setAddrWindow(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1) {
	uint8_t x0n = convX(x0);
	uint8_t x1n = convX(x1);
	uint8_t y0n = convY(y0);
	uint8_t y1n = convY(y1);
	
	uint8_t temp1 = x0n + RowStart;
	uint8_t temp2 = x1n + RowStart;
	
	x0n = y0n + ColStart;
	x1n = y1n + ColStart;
	y0n = temp1;
	y1n = temp2;
	
	if(x0n >= x1n && y0n >= y1n){
		temp1 = x0n;
		temp2 = y0n;
		x0n = x1n;
		y0n = y1n;
		x1n = temp1;
		y1n = temp2;
	}
	
	SwitchToDisplay();
  xchg_spi(DISPLAY_CASET, DC_COMMAND);
  xchg_spi(0x00, DC_DATA);
  xchg_spi(x0n, DC_DATA);
  xchg_spi(0x00, DC_DATA);
  xchg_spi(x1n, DC_DATA);
	
	xchg_spi(DISPLAY_RASET, DC_COMMAND);
  xchg_spi(0x00, DC_DATA);
  xchg_spi(y0n, DC_DATA);
  xchg_spi(0x00, DC_DATA);
  xchg_spi(y1n, DC_DATA);
	
	xchg_spi(DISPLAY_RAMWR, DC_COMMAND);
}

// Send two bytes of data, most significant byte first
// Requires 2 bytes of transmission
void static pushColor(uint16_t color) {
	SwitchToDisplay();
  xchg_spi((uint8_t)(color >> 8), DC_DATA);
  xchg_spi((uint8_t)(color), DC_DATA);
}

//------------Display_DrawPixel------------
// Color the pixel at the given coordinates with the given color.
// Requires 13 bytes of transmission
// Input: x     horizontal position of the pixel, columns from the left edge
//               must be less than 128
//               0 is on the left, 126 is near the right
//        y     vertical position of the pixel, rows from the top edge
//               must be less than 160
//               159 is near the wires, 0 is the side opposite the wires
//        color 16-bit color, which can be produced by ST7735_Color565()
// Output: none
void Display_DrawPixel(int16_t x, int16_t y, uint16_t color) {

  if((x < 0) || (x >= _width) || (y < 0) || (y >= _height)) return;
	//if((x < 0) || (x >= DISPLAY_TFTWIDTH) || (y < 0) || (y >= DISPLAY_TFTHEIGHT)) return;
	
  setAddrWindow(x,y,x,y);  // ST7735_DrawPixel bug fixed 9/2/2014

  pushColor(color);
}

//------------Display_DrawFastVLine------------
// Draw a vertical line at the given coordinates with the given height and color.
// A vertical line is parallel to the longer side of the rectangular display
// Requires (11 + 2*h) bytes of transmission (assuming image fully on screen)
// Input: x     horizontal position of the start of the line, columns from the left edge
//        y     vertical position of the start of the line, rows from the top edge
//        h     vertical height of the line
//        color 16-bit color, which can be produced by Display_Color565()
// Output: none
void Display_DrawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color) {
  uint8_t hi = color >> 8, lo = color;

  // Rudimentary clipping
  if((x >= _width) || (y >= _height)) return;
  if((y+h-1) >= _height) h = _height-y;
  setAddrWindow(x, y, x, y+h-1);

	SwitchToDisplay();
  while (h--) {
    xchg_spi(hi, DC_DATA);
    xchg_spi(lo, DC_DATA);
  }
}

//------------Display_DrawFastHLine------------
// Draw a horizontal line at the given coordinates with the given width and color.
// A horizontal line is parallel to the shorter side of the rectangular display
// Requires (11 + 2*w) bytes of transmission (assuming image fully on screen)
// Input: x     horizontal position of the start of the line, columns from the left edge
//        y     vertical position of the start of the line, rows from the top edge
//        w     horizontal width of the line
//        color 16-bit color, which can be produced by Display_Color565()
// Output: none
void ST7735_DrawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color) {
  uint8_t hi = color >> 8, lo = color;

  // Rudimentary clipping
  if((x >= _width) || (y >= _height)) return;
  if((x+w-1) >= _width)  w = _width-x;
  setAddrWindow(x, y, x+w-1, y);
	
	SwitchToDisplay();
  while (w--) {
    xchg_spi(hi, DC_DATA);
    xchg_spi(lo, DC_DATA);
  }
}

//------------Display_FillScreen------------
// Fill the screen with the given color.
// Requires 40,971 bytes of transmission
// Input: color 16-bit color, which can be produced by ST7735_Color565()
// Output: none
void Display_FillScreen(uint16_t color) {
  Display_FillRect(0, 0, _width, _height, color);  // original
//  screen is actually 129 by 161 pixels, x 0 to 128, y goes from 0 to 160
}

//------------Display_FillRect------------
// Draw a filled rectangle at the given coordinates with the given width, height, and color.
// Requires (11 + 2*w*h) bytes of transmission (assuming image fully on screen)
// Input: x     horizontal position of the top left corner of the rectangle, columns from the left edge
//        y     vertical position of the top left corner of the rectangle, rows from the top edge
//        w     horizontal width of the rectangle
//        h     vertical height of the rectangle
//        color 16-bit color, which can be produced by Display_Color565()
// Output: none
void Display_FillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) {
  uint8_t hi = color >> 8, lo = color;

  // rudimentary clipping (drawChar w/big text requires this)
  if((x >= _width) || (y >= _height)) return;
  if((x + w - 1) >= _width)  w = _width  - x;
  if((y + h - 1) >= _height) h = _height - y;

  //setAddrWindow(x, y, x+w-1, y+h-1);
	setAddrWindow(x, y, x+w-1, y+h-1);
	
	SwitchToDisplay();
  for(y=h; y>0; y--) {
    for(x=w; x>0; x--) {
      xchg_spi(hi, DC_DATA);
			xchg_spi(lo, DC_DATA);
    }
  }
}

//------------Display_DrawSmallCircle------------
// Draw a small circle (diameter of 6 pixels)
// rectangle at the given coordinates with the given color.
// Requires (11*6+24*2)=114 bytes of transmission (assuming image on screen)
// Input: x     horizontal position of the top left corner of the circle, columns from the left edge
//        y     vertical position of the top left corner of the circle, rows from the top edge
//        color 16-bit color, which can be produced by Display_Color565()
// Output: none
int16_t const smallCircle[6][3]={
    {2,3,    2},
  {1  ,  4,  4},
 {0   ,   5, 6},
 {0   ,   5, 6},
  {1  ,  4,  4},
    {2,3,    2}};
void Display_DrawSmallCircle(int16_t x, int16_t y, uint16_t color) {
  uint32_t i,w;
  uint8_t hi = color >> 8, lo = color;
  // rudimentary clipping 
  if((x>_width-5)||(y>_height-5)) return; // doesn't fit
  SwitchToDisplay();
	for(i=0; i<6; i++){
    setAddrWindow(x+smallCircle[i][0], y+i, x+smallCircle[i][1], y+i);
    w = smallCircle[i][2];
    while (w--) {
      xchg_spi(hi, DC_DATA);
			xchg_spi(lo, DC_DATA);
    }
  }
}

//------------Display_DrawCircle------------
// Draw a small circle (diameter of 10 pixels)
// rectangle at the given coordinates with the given color.
// Requires (11*10+68*2)=178 bytes of transmission (assuming image on screen)
// Input: x     horizontal position of the top left corner of the circle, columns from the left edge
//        y     vertical position of the top left corner of the circle, rows from the top edge
//        color 16-bit color, which can be produced by Display_Color565()
// Output: none
int16_t const circle[10][3]={
      { 4,5,         2},
    {2   ,   7,      6},
  {1     ,      8,   8},
  {1     ,      8,   8},
 {0      ,       9, 10},
 {0      ,       9, 10},
  {1     ,      8,   8},
  {1     ,      8,   8},
    {2   ,    7,     6},
     {  4,5,         2}};
void Display_DrawCircle(int16_t x, int16_t y, uint16_t color) {
  uint32_t i,w;
  uint8_t hi = color >> 8, lo = color;
  // rudimentary clipping 
  if((x>_width-9)||(y>_height-9)) return; // doesn't fit
  SwitchToDisplay();
	for(i=0; i<10; i++){
    setAddrWindow(x+circle[i][0], y+i, x+circle[i][1], y+i);
    w = circle[i][2];
    while (w--) {
      xchg_spi(hi, DC_DATA);
			xchg_spi(lo, DC_DATA);
    }
  }
}

//------------Display_Color565------------
// Pass 8-bit (each) R,G,B and get back 16-bit packed color.
// Input: r red value
//        g green value
//        b blue value
// Output: 16-bit color
uint16_t Display_Color565(uint8_t r, uint8_t g, uint8_t b) {
  return ((b & 0xF8) << 8) | ((g & 0xFC) << 3) | (r >> 3);
}

//------------Display_SwapColor------------
// Swaps the red and blue values of the given 16-bit packed color;
// green is unchanged.
// Input: x 16-bit color in format B, G, R
// Output: 16-bit color in format R, G, B
uint16_t Display_SwapColor(uint16_t x) {
  return (x << 11) | (x & 0x07E0) | (x >> 11);
}

//------------Display_DrawBitmap------------
// Displays a 16-bit color BMP image.  A bitmap file that is created
// by a PC image processing program has a header and may be padded
// with dummy columns so the data have four byte alignment.  This
// function assumes that all of that has been stripped out, and the
// array image[] has one 16-bit halfword for each pixel to be
// displayed on the screen (encoded in reverse order, which is
// standard for bitmap files).  An array can be created in this
// format from a 24-bit-per-pixel .bmp file using the associated
// converter program.
// (x,y) is the screen location of the lower left corner of BMP image
// Requires (11 + 2*w*h) bytes of transmission (assuming image fully on screen)
// Input: x     horizontal position of the bottom left corner of the image, columns from the left edge
//        y     vertical position of the bottom left corner of the image, rows from the top edge
//        image pointer to a 16-bit color BMP image
//        w     number of pixels wide
//        h     number of pixels tall
// Output: none
// Must be less than or equal to 128 pixels wide by 160 pixels high
void Display_DrawBitmap(int16_t x, int16_t y, const uint16_t *image, int16_t w, int16_t h){
	int16_t originalWidth = w;
	int i = w-1;	// We start at the top right corner
	if(x >= _width || y >= _height || (x+w-1) <= 0 || (y+h-1)<= 0){
		return;	// Out of bounds
	}
	
	if((w > _width) || (h > _height)){
		return;	// image bigger than screen
	}
	
	// Image goes off-screen
	if((x + w - 1) >= _width){
    w = _width - x;
		i = w - 1;
	}
	
	if((y + h - 1) >= _height){                  // image exceeds top of screen
		i = (y + h -_height) * originalWidth + w - 1;
		h = _height - y;
  }
	
	if(x < 0){                            // image exceeds left of screen
    w = w + x;
    x = 0;
  }
	if(y < 0){                     // image exceeds bottom of screen
    h = h + y;
    y = 0;
  }
	
	setAddrWindow(x+w-1, y+h-1, x, y);	// x y is the bottom left corner, so we need top left and bottom right
	
	SwitchToDisplay();
	for(x=w; x>0; x--){
		for(y=h; y>0; y--){
			xchg_spi((uint8_t)(image[i] >> 8), DC_DATA);
                                        // send the bottom 8 bits
      xchg_spi((uint8_t)image[i], DC_DATA);
      i += originalWidth;                        // go to the next pixel
		}
		i--;
		i-=h*originalWidth;
	}
	
	
	//setAddrWindow(convX(x), 
	
	/*int16_t skipC = 0;                      // non-zero if columns need to be skipped due to clipping
  int16_t originalWidth = w;              // save this value; even if not all columns fit on the screen, the image is still this width in ROM
  int i = w*(h - 1);

  if((x >= _width) || ((y - h + 1) >= _height) || ((x + w) <= 0) || (y < 0)){
    return;                             // image is totally off the screen, do nothing
  }
  if((w > _width) || (h > _height)){    // image is too wide for the screen, do nothing
    //***This isn't necessarily a fatal error, but it makes the
    //following logic much more complicated, since you can have
    //an image that exceeds multiple boundaries and needs to be
    //clipped on more than one side.
    return;
  }
  if((x + w - 1) >= _width){            // image exceeds right of screen
    skipC = (x + w) - _width;           // skip cut off columns
    w = _width - x;
  }
  if((y - h + 1) < 0){                  // image exceeds top of screen
    i = i - (h - y - 1)*originalWidth;  // skip the last cut off rows
    h = y + 1;
  }
  if(x < 0){                            // image exceeds left of screen
    w = w + x;
    skipC = -1*x;                       // skip cut off columns
    i = i - x;                          // skip the first cut off columns
    x = 0;
  }
  if(y >= _height){                     // image exceeds bottom of screen
    h = h - (y - _height + 1);
    y = _height - 1;
  }

  setAddrWindow(convX(x), convY(y-h+1), convX(x+w-1), convY(y));
	
	SwitchToDisplay();
  for(y=0; y<h; y=y+1){
    for(x=0; x<w; x=x+1){
                                        // send the top 8 bits
      xchg_spi((uint8_t)(image[i] >> 8), DC_DATA);
                                        // send the bottom 8 bits
      xchg_spi((uint8_t)image[i], DC_DATA);
      i = i + 1;                        // go to the next pixel
    }
    i = i + skipC;
    i = i - 2*originalWidth;
  }
	
	setAddrWindow(x, y-h+1, x+w-1, y);*/
	
	
	
}

void Display_RenderSprite(int16_t x, int16_t y, const uint16_t *image, int16_t w, int16_t h, uint16_t greenScreen, const uint16_t* background){
	int16_t originalWidth = w;
	int i = w-1;	// We start at the top right corner
	if(x >= _width || y >= _height || (x+w-1) <= 0 || (y+h-1)<= 0){
		return;	// Out of bounds
	}
	
	if((w > _width) || (h > _height)){
		return;	// image bigger than screen
	}
	
	// Image goes off-screen
	if((x + w - 1) >= _width){
    w = _width - x;
		i = w - 1;
	}
	
	if((y + h - 1) >= _height){                  // image exceeds top of screen
		i = (y + h -_height) * originalWidth + w - 1;
		h = _height - y;
  }
	
	if(x < 0){                            // image exceeds left of screen
    w = w + x;
    x = 0;
  }
	if(y < 0){                     // image exceeds bottom of screen
    h = h + y;
    y = 0;
  }
	
	int16_t originalX = x;
	int16_t originalY = y;
	
	setAddrWindow(x+w-1, y+h-1, x, y);	// x y is the bottom left corner, so we need top left and bottom right
	
	SwitchToDisplay();
	for(x=w; x>0; x--){
		for(y=h; y>0; y--){
			if(image[i] != greenScreen){
				xchg_spi((uint8_t)(Display_SwapColor(image[i]) >> 8), DC_DATA);
																					// send the bottom 8 bits
				xchg_spi((uint8_t)Display_SwapColor(image[i]), DC_DATA);
			}
			else{
				uint16_t screenInd = originalX+(_width)*convY(originalY+h-1) + (x-1) + (h-y-1)*_width;
				uint16_t fillColor = background[screenInd];
				//background[screenInd] = image[i];
				xchg_spi((uint8_t)(fillColor >> 8), DC_DATA);
																					// send the bottom 8 bits
				xchg_spi((uint8_t)fillColor, DC_DATA);
			}
			i += originalWidth;                        // go to the next pixel
		}
		i--;
		i-=h*originalWidth;
	}
}
void Display_UnrenderSprite(int16_t x, int16_t y, int16_t w, int16_t h, const uint16_t* bg){
		int16_t originalWidth = w;
	int i = w-1;	// We start at the top right corner
	if(x >= _width || y >= _height || (x+w-1) <= 0 || (y+h-1)<= 0){
		return;	// Out of bounds
	}
	
	if((w > _width) || (h > _height)){
		return;	// image bigger than screen
	}
	
	// Image goes off-screen
	if((x + w - 1) >= _width){
    w = _width - x;
		i = w - 1;
	}
	
	if((y + h - 1) >= _height){                  // image exceeds top of screen
		i = (y + h -_height) * originalWidth + w - 1;
		h = _height - y;
  }
	
	if(x < 0){                            // image exceeds left of screen
    w = w + x;
    x = 0;
  }
	if(y < 0){                     // image exceeds bottom of screen
    h = h + y;
    y = 0;
  }
	
	int16_t originalX = x;
	int16_t originalY = y;
	
	setAddrWindow(x+w-1, y+h-1, x, y);	// x y is the bottom left corner, so we need top left and bottom right
	for(x=w; x>0; x--){
		for(y=h; y>0; y--){
			uint16_t screenInd = originalX+(_width)*convY(originalY+h-1) + (x-1) + (h-y-1)*_width;
			uint16_t fillColor = bg[screenInd];
			//background[screenInd] = image[i];
			xchg_spi((uint8_t)(fillColor >> 8), DC_DATA);
																				// send the bottom 8 bits
			xchg_spi((uint8_t)fillColor, DC_DATA);
			i += originalWidth;                        // go to the next pixel
		}
		i--;
		i-=h*originalWidth;
	}
	

}
void Display_RenderCursor(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color){
	int16_t originalWidth = w;
	int i = w-1;	// We start at the top right corner
	if(x >= _width || y >= _height || (x+w-1) <= 0 || (y+h-1)<= 0){
		return;	// Out of bounds
	}
	
	if((w > _width) || (h > _height)){
		return;	// image bigger than screen
	}
	
	// Image goes off-screen
	if((x + w - 1) >= _width){
    w = _width - x;
		i = w - 1;
	}
	
	if((y + h - 1) >= _height){                  // image exceeds top of screen
		i = (y + h -_height) * originalWidth + w - 1;
		h = _height - y;
  }
	
	if(x < 0){                            // image exceeds left of screen
    w = w + x;
    x = 0;
  }
	if(y < 0){                     // image exceeds bottom of screen
    h = h + y;
    y = 0;
  }
	
	int16_t originalX = x;
	int16_t originalY = y;
	
	//setAddrWindow(x+w-1, y+h-1, x, y);	// x y is the bottom left corner, so we need top left and bottom right
	
	SwitchToDisplay();
	
	for(x=w; x>0; x--){
		for(y=h; y>0; y--){
			if(!(x-1 < w-borderWidth && x-1 >= borderWidth && y-1 < h-borderWidth && y-1 >= borderWidth)){
				Display_DrawPixel(originalX+x-1, originalY+y-1, Display_SwapColor(color));
			}
			i += originalWidth;                        // go to the next pixel
		}
		
		i--;
		i-=h*originalWidth;
	}

}
void Display_UnrenderCursor(int16_t x, int16_t y, int16_t w, int16_t h, const uint16_t* bg){
	int16_t originalWidth = w;
	int i = w-1;	// We start at the top right corner
	if(x >= _width || y >= _height || (x+w-1) <= 0 || (y+h-1)<= 0){
		return;	// Out of bounds
	}
	
	if((w > _width) || (h > _height)){
		return;	// image bigger than screen
	}
	
	// Image goes off-screen
	if((x + w - 1) >= _width){
    w = _width - x;
		i = w - 1;
	}
	
	if((y + h - 1) >= _height){                  // image exceeds top of screen
		i = (y + h -_height) * originalWidth + w - 1;
		h = _height - y;
  }
	
	if(x < 0){                            // image exceeds left of screen
    w = w + x;
    x = 0;
  }
	if(y < 0){                     // image exceeds bottom of screen
    h = h + y;
    y = 0;
  }
	
	int16_t originalX = x;
	int16_t originalY = y;
	
	//setAddrWindow(x+w-1, y+h-1, x, y);	// x y is the bottom left corner, so we need top left and bottom right
	
	SwitchToDisplay();
	
	for(x=w; x>0; x--){
		for(y=h; y>0; y--){
			uint16_t screenInd = originalX+(_width)*convY(originalY+h-1) + (x-1) + (h-y-1)*_width;
			if(!(x-1 < w-borderWidth && x-1 >= borderWidth && y-1 < h-borderWidth && y-1 >= borderWidth)){
				Display_DrawPixel(originalX+x-1, originalY+y-1, bg[screenInd]);
			}
			i += originalWidth;                        // go to the next pixel
		}
		
		i--;
		i-=h*originalWidth;
	}
}


//------------Display_DrawCharS------------
// Simple character draw function.  This is the same function from
// Adafruit_GFX.c but adapted for this processor.  However, each call
// to Display_DrawPixel() calls setAddrWindow(), which needs to send
// many extra data and commands.  If the background color is the same
// as the text color, no background will be printed, and text can be
// drawn right over existing images without covering them with a box.
// Requires (11 + 2*size*size)*6*8 (image fully on screen; textcolor != bgColor)
// Input: x         horizontal position of the top left corner of the character, columns from the left edge
//        y         vertical position of the top left corner of the character, rows from the top edge
//        c         character to be printed
//        textColor 16-bit color of the character
//        bgColor   16-bit color of the background
//        size      number of pixels per character pixel (e.g. size==2 prints each pixel of font as 2x2 square)
// Output: none
void Display_DrawCharS(int16_t x, int16_t y, char c, int16_t textColor, int16_t bgColor, uint8_t size){
  uint8_t line; // vertical column of pixels of character in font
  int32_t i, j;
  if((x >= _width)            || // Clip right
     (y >= _height)           || // Clip bottom
     ((x + 5 * size - 1) < 0) || // Clip left
     ((y + 8 * size - 1) < 0))   // Clip top
    return;

  for (i=0; i<6; i++ ) {
    if (i == 5)
      line = 0x0;
    else
      line = Font[(c*5)+i];
    for (j = 0; j<8; j++) {
      if ((line & 0x80) >> 7) {
        if (size == 1) // default size
          Display_DrawPixel(x+i, y+j, textColor);
        else {  // big size
          Display_FillRect(x+(i*size), y+(j*size), size, size, textColor);
        }
      } else if (bgColor != textColor) {
        if (size == 1){ // default size
          //Display_DrawPixel(x+i, y+j, bgColor);
				}
        else {  // big size
          //Display_FillRect(x+i*size, y+j*size, size, size, bgColor);
        }
      }
      line <<= 1;
    }
  }
}


//------------Display_DrawChar------------
// Advanced character draw function.  This is similar to the function
// from Adafruit_GFX.c but adapted for this processor.  However, this
// function only uses one call to setAddrWindow(), which allows it to
// run at least twice as fast.
// Requires (11 + size*size*6*8) bytes of transmission (assuming image fully on screen)
// Input: x         horizontal position of the top left corner of the character, columns from the left edge
//        y         vertical position of the top left corner of the character, rows from the top edge
//        c         character to be printed
//        textColor 16-bit color of the character
//        bgColor   16-bit color of the background
//        size      number of pixels per character pixel (e.g. size==2 prints each pixel of font as 2x2 square)
// Output: none
void Display_DrawChar(int16_t x, int16_t y, char c, int16_t textColor, int16_t bgColor, uint8_t size){
  uint8_t line; // horizontal row of pixels of character
  int32_t col, row, i, j;// loop indices
  if(((x + 5*size - 1) >= _width)  || // Clip right
     ((y + 8*size - 1) >= _height) || // Clip bottom
     ((x + 5*size - 1) < 0)        || // Clip left
     ((y + 8*size - 1) < 0)){         // Clip top
    return;
  }

  setAddrWindow(x, y, x+6*size-1, y+8*size-1);

  line = 0x01;        // print the top row first
  // print the rows, starting at the top
  for(row=0; row<8; row=row+1){
    for(i=0; i<size; i=i+1){
      // print the columns, starting on the left
      for(col=0; col<5; col=col+1){
        if(Font[(c*5)+col]&line){
          // bit is set in Font, print pixel(s) in text color
          for(j=0; j<size; j=j+1){
            pushColor(textColor);
          }
        } else{
          // bit is cleared in Font, print pixel(s) in background color
          for(j=0; j<size; j=j+1){
            pushColor(bgColor);
          }
        }
      }
      // print blank column(s) to the right of character
      for(j=0; j<size; j=j+1){
        pushColor(bgColor);
      }
    }
    line = line<<1;   // move up to the next row
  }
}
//------------Display_DrawString------------
// String draw function.
// 16 rows (0 to 15) and 21 characters (0 to 20)
// Requires (11 + size*size*6*8) bytes of transmission for each character
// Input: x         columns from the left edge (0 to 20)
//        y         rows from the top edge (0 to 15)
//        pt        pointer to a null terminated string to be printed
//        textColor 16-bit color of the characters
// bgColor is Black and size is 1
// Output: number of characters printed
uint32_t Display_DrawString(uint16_t x, uint16_t y, char *pt, int16_t textColor){
  uint32_t count = 0;
  if(y>15) return 0;
  while(*pt){
    Display_DrawCharS(x*6, y*10, *pt, textColor, DISPLAY_BLACK, 1);
    pt++;
    x = x+1;
    if(x>20) return count;  // number of characters printed
    count++;
  }
  return count;  // number of characters printed
}

//-----------------------fillmessage-----------------------
// Output a 32-bit number in unsigned decimal format
// Input: 32-bit number to be transferred
// Output: none
// Variable format 1-10 digits with no space before or after
char Message[12];
uint32_t Messageindex;

void fillmessage(uint32_t n){
// This function uses recursion to convert decimal number
//   of unspecified length as an ASCII string
  if(n >= 10){
    fillmessage(n/10);
    n = n%10;
  }
  Message[Messageindex] = (n+'0'); /* n is between 0 and 9 */
  if(Messageindex<11)Messageindex++;
}
//********Display_SetCursor*****************
// Move the cursor to the desired X- and Y-position.  The
// next character will be printed here.  X=0 is the leftmost
// column.  Y=0 is the top row.
// inputs: newX  new X-position of the cursor (0<=newX<=20)
//         newY  new Y-position of the cursor (0<=newY<=15)
// outputs: none
void Display_SetCursor(uint32_t newX, uint32_t newY){
  if((newX > 20) || (newY > 15)){       // bad input
    return;                             // do nothing
  }
  StX = newX;
  StY = newY;
}
//-----------------------Display_OutUDec-----------------------
// Output a 32-bit number in unsigned decimal format
// Position determined by Display_SetCursor command
// Color set by Display_SetTextColor
// Input: 32-bit number to be transferred
// Output: none
// Variable format 1-10 digits with no space before or after
void Display_OutUDec(uint32_t n, uint16_t color){
  Messageindex = 0;
  fillmessage(n);
  Message[Messageindex] = 0; // terminate
  Display_DrawString(StX,StY,Message,color);
  StX = StX+Messageindex;
  if(StX>20){
    StX = 20;
    Display_DrawCharS(StX*6,StY*10,'*',DISPLAY_RED,DISPLAY_BLACK, 1);
  }
}





#define MADCTL_MY  0x80
#define MADCTL_MX  0x40
#define MADCTL_MV  0x20
#define MADCTL_ML  0x10
#define MADCTL_RGB 0x00
#define MADCTL_BGR 0x08
#define MADCTL_MH  0x04

//------------Display_SetRotation------------
// Change the image rotation.
// Requires 2 bytes of transmission
// Input: m new rotation value (0 to 3)
// Output: none
void Display_SetRotation(uint8_t m) {
	SwitchToDisplay();
  xchg_spi(DISPLAY_MADCTL, DC_COMMAND);
  Rotation = m % 4; // can't be higher than 3
  switch (Rotation) {
   case 0:
     if (TabColor == INITR_BLACKTAB) {
       xchg_spi(MADCTL_MX | MADCTL_MY | MADCTL_RGB, DC_DATA);
     } else {
       xchg_spi(MADCTL_MX | MADCTL_MY | MADCTL_BGR, DC_DATA);
     }
     _width  = DISPLAY_TFTWIDTH;
     _height = DISPLAY_TFTHEIGHT;
     break;
   case 1:
     if (TabColor == INITR_BLACKTAB) {
       xchg_spi(MADCTL_MY | MADCTL_MV | MADCTL_RGB, DC_DATA);
     } else {
       xchg_spi(MADCTL_MY | MADCTL_MV | MADCTL_BGR, DC_DATA);
     }
     _width  = DISPLAY_TFTHEIGHT;
     _height = DISPLAY_TFTWIDTH;
     break;
  case 2:
     if (TabColor == INITR_BLACKTAB) {
       xchg_spi(MADCTL_RGB, DC_DATA);
     } else {
       xchg_spi(MADCTL_BGR, DC_DATA);
     }
     _width  = DISPLAY_TFTWIDTH;
     _height = DISPLAY_TFTHEIGHT;
    break;
   case 3:
     if (TabColor == INITR_BLACKTAB) {
       xchg_spi(MADCTL_MX | MADCTL_MV | MADCTL_RGB, DC_DATA);
     } else {
       xchg_spi(MADCTL_MX | MADCTL_MV | MADCTL_BGR, DC_DATA);
     }
     _width  = DISPLAY_TFTHEIGHT;
     _height = DISPLAY_TFTWIDTH;
     break;
  }
}


//------------Display_InvertDisplay------------
// Send the command to invert all of the colors.
// Requires 1 byte of transmission
// Input: i 0 to disable inversion; non-zero to enable inversion
// Output: none
void Display_InvertDisplay(int i) {
  SwitchToDisplay();
	if(i){
    xchg_spi(DISPLAY_INVON, DC_COMMAND);
  } else{
    xchg_spi(DISPLAY_INVOFF, DC_COMMAND);
  }
}
// graphics routines
// y coordinates 0 to 31 used for labels and messages
// y coordinates 32 to 159  128 pixels high
// x coordinates 0 to 127   128 pixels wide

int32_t Ymax,Ymin,X;        // X goes from 0 to 127
int32_t Yrange; //YrangeDiv2;

// *************** Display_PlotClear ********************
// Clear the graphics buffer, set X coordinate to 0
// This routine clears the display
// Inputs: ymin and ymax are range of the plot
// Outputs: none
void Display_PlotClear(int32_t ymin, int32_t ymax){
  Display_FillRect(0, 32, 128, 128, Display_Color565(228,228,228)); // light grey
  if(ymax>ymin){
    Ymax = ymax;
    Ymin = ymin;
    Yrange = ymax-ymin;
  } else{
    Ymax = ymin;
    Ymin = ymax;
    Yrange = ymax-ymin;
  }
  //YrangeDiv2 = Yrange/2;
  X = 0;
}

// *************** Display_PlotPoint ********************
// Used in the voltage versus time plot, plot one point at y
// It does output to display
// Inputs: y is the y coordinate of the point plotted
// Outputs: none
void Display_PlotPoint(int32_t y){int32_t j;
  if(y<Ymin) y=Ymin;
  if(y>Ymax) y=Ymax;
  // X goes from 0 to 127
  // j goes from 159 to 32
  // y=Ymax maps to j=32
  // y=Ymin maps to j=159
  j = 32+(127*(Ymax-y))/Yrange;
  if(j<32) j = 32;
  if(j>159) j = 159;
  Display_DrawPixel(X,   j,   DISPLAY_BLUE);
  Display_DrawPixel(X+1, j,   DISPLAY_BLUE);
  Display_DrawPixel(X,   j+1, DISPLAY_BLUE);
  Display_DrawPixel(X+1, j+1, DISPLAY_BLUE);
}
// *************** Display_PlotLine ********************
// Used in the voltage versus time plot, plot line to new point
// It does output to display
// Inputs: y is the y coordinate of the point plotted
// Outputs: none
int32_t lastj=0;
void Display_PlotLine(int32_t y){int32_t i,j;
  if(y<Ymin) y=Ymin;
  if(y>Ymax) y=Ymax;
  // X goes from 0 to 127
  // j goes from 159 to 32
  // y=Ymax maps to j=32
  // y=Ymin maps to j=159
  j = 32+(127*(Ymax-y))/Yrange;
  if(j < 32) j = 32;
  if(j > 159) j = 159;
  if(lastj < 32) lastj = j;
  if(lastj > 159) lastj = j;
  if(lastj < j){
    for(i = lastj+1; i<=j ; i++){
      Display_DrawPixel(X,   i,   DISPLAY_BLUE) ;
      Display_DrawPixel(X+1, i,   DISPLAY_BLUE) ;
    }
  }else if(lastj > j){
    for(i = j; i<lastj ; i++){
      Display_DrawPixel(X,   i,   DISPLAY_BLUE) ;
      Display_DrawPixel(X+1, i,   DISPLAY_BLUE) ;
    }
  }else{
    Display_DrawPixel(X,   j,   DISPLAY_BLUE) ;
    Display_DrawPixel(X+1, j,   DISPLAY_BLUE) ;
  }
  lastj = j;
}

// *************** Display_PlotPoints ********************
// Used in the voltage versus time plot, plot two points at y1, y2
// It does output to display
// Inputs: y1 is the y coordinate of the first point plotted
//         y2 is the y coordinate of the second point plotted
// Outputs: none
void Display_PlotPoints(int32_t y1,int32_t y2){int32_t j;
  if(y1<Ymin) y1=Ymin;
  if(y1>Ymax) y1=Ymax;
  // X goes from 0 to 127
  // j goes from 159 to 32
  // y=Ymax maps to j=32
  // y=Ymin maps to j=159
  j = 32+(127*(Ymax-y1))/Yrange;
  if(j<32) j = 32;
  if(j>159) j = 159;
  Display_DrawPixel(X, j, DISPLAY_BLUE);
  if(y2<Ymin) y2=Ymin;
  if(y2>Ymax) y2=Ymax;
  j = 32+(127*(Ymax-y2))/Yrange;
  if(j<32) j = 32;
  if(j>159) j = 159;
  Display_DrawPixel(X, j, DISPLAY_BLACK);
}
// *************** Display_PlotBar ********************
// Used in the voltage versus time bar, plot one bar at y
// It does not output to display until RIT128x96x4ShowPlot called
// Inputs: y is the y coordinate of the bar plotted
// Outputs: none
void Display_PlotBar(int32_t y){
int32_t j;
  if(y<Ymin) y=Ymin;
  if(y>Ymax) y=Ymax;
  // X goes from 0 to 127
  // j goes from 159 to 32
  // y=Ymax maps to j=32
  // y=Ymin maps to j=159
  j = 32+(127*(Ymax-y))/Yrange;
  Display_DrawFastVLine(X, j, 159-j, DISPLAY_BLACK);

}

// full scaled defined as 3V
// Input is 0 to 511, 0 => 159 and 511 => 32
uint8_t const dBfs[512]={
159, 159, 145, 137, 131, 126, 123, 119, 117, 114, 112, 110, 108, 107, 105, 104, 103, 101,
  100, 99, 98, 97, 96, 95, 94, 93, 93, 92, 91, 90, 90, 89, 88, 88, 87, 87, 86, 85, 85, 84,
  84, 83, 83, 82, 82, 81, 81, 81, 80, 80, 79, 79, 79, 78, 78, 77, 77, 77, 76, 76, 76, 75,
  75, 75, 74, 74, 74, 73, 73, 73, 72, 72, 72, 72, 71, 71, 71, 71, 70, 70, 70, 70, 69, 69,
  69, 69, 68, 68, 68, 68, 67, 67, 67, 67, 66, 66, 66, 66, 66, 65, 65, 65, 65, 65, 64, 64,
  64, 64, 64, 63, 63, 63, 63, 63, 63, 62, 62, 62, 62, 62, 62, 61, 61, 61, 61, 61, 61, 60,
  60, 60, 60, 60, 60, 59, 59, 59, 59, 59, 59, 59, 58, 58, 58, 58, 58, 58, 58, 57, 57, 57,
  57, 57, 57, 57, 56, 56, 56, 56, 56, 56, 56, 56, 55, 55, 55, 55, 55, 55, 55, 55, 54, 54,
  54, 54, 54, 54, 54, 54, 53, 53, 53, 53, 53, 53, 53, 53, 53, 52, 52, 52, 52, 52, 52, 52,
  52, 52, 52, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 50, 50, 50, 50, 50, 50, 50, 50, 50,
  50, 49, 49, 49, 49, 49, 49, 49, 49, 49, 49, 49, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48,
  48, 47, 47, 47, 47, 47, 47, 47, 47, 47, 47, 47, 47, 46, 46, 46, 46, 46, 46, 46, 46, 46,
  46, 46, 46, 46, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 44, 44, 44, 44, 44,
  44, 44, 44, 44, 44, 44, 44, 44, 44, 43, 43, 43, 43, 43, 43, 43, 43, 43, 43, 43, 43, 43,
  43, 43, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 41, 41, 41, 41, 41,
  41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40,
  40, 40, 40, 40, 40, 40, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39,
  39, 39, 38, 38, 38, 38, 38, 38, 38, 38, 38, 38, 38, 38, 38, 38, 38, 38, 38, 38, 38, 37,
  37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 36, 36, 36, 36,
  36, 36, 36, 36, 36, 36, 36, 36, 36, 36, 36, 36, 36, 36, 36, 36, 36, 35, 35, 35, 35, 35,
  35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 34, 34, 34, 34, 34, 34,
  34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 33, 33, 33, 33, 33,
  33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 32, 32, 32,
  32, 32, 32, 32, 32, 32, 32, 32, 32, 32
};

// *************** Display_PlotdBfs ********************
// Used in the amplitude versus frequency plot, plot bar point at y
// 0 to 0.625V scaled on a log plot from min to max
// It does output to display
// Inputs: y is the y ADC value of the bar plotted
// Outputs: none
void Display_PlotdBfs(int32_t y){
int32_t j;
  y = y/2; // 0 to 2047
  if(y<0) y=0;
  if(y>511) y=511;
  // X goes from 0 to 127
  // j goes from 159 to 32
  // y=511 maps to j=32
  // y=0 maps to j=159
  j = dBfs[y];
  Display_DrawFastVLine(X, j, 159-j, DISPLAY_BLACK);

}

// *************** Display_PlotNext ********************
// Used in all the plots to step the X coordinate one pixel
// X steps from 0 to 127, then back to 0 again
// It does not output to display
// Inputs: none
// Outputs: none
void Display_PlotNext(void){
  if(X==127){
    X = 0;
  } else{
    X++;
  }
}

// *************** Display_PlotNextErase ********************
// Used in all the plots to step the X coordinate one pixel
// X steps from 0 to 127, then back to 0 again
// It clears the vertical space into which the next pixel will be drawn
// Inputs: none
// Outputs: none
void Display_PlotNextErase(void){
  if(X==127){
    X = 0;
  } else{
    X++;
  }
  Display_DrawFastVLine(X,32,128,Display_Color565(228,228,228));
}

// Used in all the plots to write buffer to LCD
// Example 1 Voltage versus time
//    Display_PlotClear(0,4095);  // range from 0 to 4095
//    Display_PlotPoint(data); Display_PlotNext(); // called 128 times

// Example 2a Voltage versus time (N data points/pixel, time scale)
//    Display_PlotClear(0,4095);  // range from 0 to 4095
//    {   for(j=0;j<N;j++){
//          Display_PlotPoint(data[i++]); // called N times
//        }
//        Display_PlotNext();
//    }   // called 128 times

// Example 2b Voltage versus time (N data points/pixel, time scale)
//    Display_PlotClear(0,4095);  // range from 0 to 4095
//    {   for(j=0;j<N;j++){
//          Display_PlotLine(data[i++]); // called N times
//        }
//        Display_PlotNext();
//    }   // called 128 times

// Example 3 Voltage versus frequency (512 points)
//    perform FFT to get 512 magnitudes, mag[i] (0 to 4095)
//    Display_PlotClear(0,1023);  // clip large magnitudes
//    {
//        Display_PlotBar(mag[i++]); // called 4 times
//        Display_PlotBar(mag[i++]);
//        Display_PlotBar(mag[i++]);
//        Display_PlotBar(mag[i++]);
//        Display_PlotNext();
//    }   // called 128 times

// Example 4 Voltage versus frequency (512 points), dB scale
//    perform FFT to get 512 magnitudes, mag[i] (0 to 4095)
//    Display_PlotClear(0,511);  // parameters ignored
//    {
//        Display_PlotdBfs(mag[i++]); // called 4 times
//        Display_PlotdBfs(mag[i++]);
//        Display_PlotdBfs(mag[i++]);
//        Display_PlotdBfs(mag[i++]);
//        Display_PlotNext();
//    }   // called 128 times

// *************** Display_OutChar ********************
// Output one character to the LCD
// Position determined by Display_SetCursor command
// Color set by Display_SetTextColor
// Inputs: 8-bit ASCII character
// Outputs: none
void Display_OutChar(char ch){
  if((ch == 10) || (ch == 13) || (ch == 27)){
    StY++; StX=0;
    if(StY>15){
      StY = 0;
    }
    Display_DrawString(0,StY,"                     ",StTextColor);
    return;
  }
  Display_DrawCharS(StX*6,StY*10,ch,DISPLAY_YELLOW,DISPLAY_BLACK, 1);
  StX++;
  if(StX>20){
    StX = 20;
    Display_DrawCharS(StX*6,StY*10,'*',DISPLAY_RED,DISPLAY_BLACK, 1);
  }
  return;
}
//********Display_OutString*****************
// Print a string of characters to the Display LCD.
// Position determined by Display_SetCursor command
// Color set by Display_SetTextColor
// The string will not automatically wrap.
// inputs: ptr  pointer to NULL-terminated ASCII string
// outputs: none
void Display_OutString(char *ptr){
  while(*ptr){
    Display_OutChar(*ptr);
    ptr = ptr + 1;
  }
}
// ************** Display_SetTextColor ************************
// Sets the color in which the characters will be printed
// Background color is fixed at black
// Input:  16-bit packed color
// Output: none
// ********************************************************
void Display_SetTextColor(uint16_t color){
  StTextColor = color;
}

/*// Print a character to Display LCD.
int fputcLCD(int ch, FIL *f){
  Display_OutChar(ch);
  return 1;
}
// No input from Nokia, always return data.
int fgetcLCD(FIL *f){
  return 0;
}*/
// Function called when file error occurs.
void ferrorLCD(char* name){
  Display_DrawString(0, 0, "f_write error ", Display_Color565(0, 0, 255));
	Display_DrawString(14, 0, name, Display_Color565(0, 255, 0));
	while(1){}
}

// Abstraction of general output device
// Volume 2 section 3.4.5

// *************** Output_Init ********************
// Standard device driver initialization function for printf
// Initialize Display LCD
// Inputs: none
// Outputs: none
void Output_Init(void){
  Display_InitR(INITR_REDTAB);
  Display_FillScreen(0);                 // set screen to black
}

// Clear display
void Output_Clear(void){ // Clears the display
  Display_FillScreen(0);    // set screen to black
}
// Turn off display (low power)
void Output_Off(void){   // Turns off the display
  Output_Clear();  // not implemented
}
// Turn on display
void Output_On(void){ // Turns on the display
  Output_Init();      // reinitialize
}
// set the color for future output
// Background color is fixed at black
// Input:  16-bit packed color
// Output: none
void Output_Color(uint32_t newColor){ // Set color of future output
  Display_SetTextColor(newColor);
}

uint8_t convX(uint8_t x){
	return DISPLAY_TFTWIDTH - 1 - x;
}

uint8_t convY(uint8_t y){
	return DISPLAY_TFTHEIGHT - 1 - y;
}
