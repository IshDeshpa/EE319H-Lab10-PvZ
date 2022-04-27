#include "diskio.h"
#include "ff.h"
#include "Display.h"
#include <stdint.h>
#include "inc/tm4c123gh6pm.h"
#include "PLL.h"
#include "DAC.h"
#include "TestFS.h"

/* A test function imported from SDCFile that should ensure the filesystem is running
*
*/
extern "C" void DisableInterrupts(void);
extern "C" void EnableInterrupts(void);
extern "C" void SysTick_Handler(void);

const char inFilename[] = "testFile.txt";   // 8 characters or fewer
const char outFilename[] = "out.txt";   // 8 characters or fewer

#define FILETESTSIZE 10000
static FATFS g_sFatFs;
FIL Handle,Handle2;
FRESULT MountFresult;
FRESULT Fresult;
unsigned char buffer[512];

void FileSystemTest(void){
  UINT successfulreads, successfulwrites;
  uint8_t c, x, y;
  PLL_Init(Bus80MHz);    // 80 MHz
	//	TExaS_Init();
  Output_Init();
	//ST7735_InitR(INITR_REDTAB);
  //ST7735_FillScreen(0);                 // set screen to black
  EnableInterrupts();
//  SimpleUnformattedTest();              // comment this out if continuing to the advanced file system tests
  MountFresult = f_mount(&g_sFatFs, "", 0);
  if(MountFresult){
    ST7735_DrawString(0, 0, "f_mount error", ST7735_Color565(0, 0, 255));
    while(1){};
  }
	
	//int z = 1000;
	//while(--z){}
	
	char d;
  int i; uint32_t n;
  c = 0;
  x = 0;
  y = 10;
  n = 1;    // seed
  Fresult = f_open(&Handle2, "testFile.txt", FA_CREATE_ALWAYS|FA_WRITE);
  if(Fresult){
    ST7735_DrawString(0, 0, "testFile error", ST7735_Color565(0, 0, 255));
    while(1){};
  } else{
    for(i=0; i<FILETESTSIZE; i++){
      n = (16807*n)%2147483647; // pseudo random sequence
      c = ((n>>24)%10)+'0'; // random digit 0 to 9
      Fresult = f_write(&Handle2, &c, 1, &successfulwrites);
      if((successfulwrites != 1) || (Fresult != FR_OK)){
        ST7735_DrawString(0, 0, "f_write error", ST7735_Color565(0, 0, 255));
        while(1){};
      }
    }
    Fresult = f_close(&Handle2);
    if(Fresult){
      ST7735_DrawString(0, 0, "file2 f_close error", ST7735_Color565(0, 0, 255));
      while(1){};
    }
  }
  n = 1;  // reseed, start over to get the same sequence
  Fresult = f_open(&Handle, "testFile.txt", FA_READ);
  if(Fresult == FR_OK){
    ST7735_DrawString(0, 0, "Opened testFile.txt", ST7735_Color565(0, 0, 255));
    for(i=0; i<FILETESTSIZE; i++){
      n = (16807*n)%2147483647; // pseudo random sequence
      d = ((n>>24)%10)+'0'; // expected random digit 0 to 9
      Fresult = f_read(&Handle, &c, 1, &successfulreads);
      if((successfulreads == 1) && (Fresult == FR_OK) && (c == d)){
        ST7735_DrawChar(x, y, c, ST7735_Color565(255, 255, 0), 0, 1);
        x = x + 6;
        if(x > 122){
          x = 0;                          // start over on the next line
          y = y + 10;
        }
        if(y > 150){
          y = 10;                         // the screen is full
        }
      } else{
        ST7735_DrawString(0, 0, "f_read error", ST7735_Color565(0, 0, 255));
        while(1){};
      }

    }
  } else{
    ST7735_DrawString(0, 0, "Error testFile.txt (  )", ST7735_Color565(255, 0, 0));
    ST7735_SetCursor(20, 0);
    ST7735_SetTextColor(ST7735_Color565(255, 0, 0));
    ST7735_OutUDec((uint32_t)Fresult);
    while(1){};
  }
  ST7735_DrawString(0, 0, "file test passed    ", ST7735_Color565(255, 255, 255));
  Fresult = f_close(&Handle);
/*  Fresult = f_open(&Handle,"out.txt", FA_CREATE_ALWAYS|FA_WRITE);
  if(Fresult == FR_OK){
    ST7735_DrawString(0, 0, "Opened out.txt", ST7735_Color565(0, 0, 255));
    c = 'A';
    x = 0;
    y = 10;
    Fresult = f_write(&Handle, &c, 1, &successfulreads);
    ST7735_DrawChar(x, y, c, ST7735_Color565(255, 255, 0), 0, 1);
    while((c <= 'Z') && (Fresult == FR_OK)){
      x = x + 6;
      c = c + 1;
      if(x > 122){
        x = 0;                          // start over on the next line
        y = y + 10;
      }
      if(y > 150){
        break;                          // the screen is full
      }
      Fresult = f_write(&Handle, &c, 1, &successfulreads);
      ST7735_DrawChar(x, y, c, ST7735_Color565(255, 255, 0), 0, 1);
    }
  } else{
    ST7735_DrawString(0, 0, "Error out.txt (  )", ST7735_Color565(255, 0, 0));
    ST7735_SetCursor(15, 0);
    ST7735_SetTextColor(ST7735_Color565(255, 0, 0));
    ST7735_OutUDec((uint32_t)Fresult);
  }*/
	Fresult = f_open(&Handle, inFilename, FA_READ);
  if(Fresult == FR_OK){
    ST7735_DrawString(0, 0, "Opened ", ST7735_Color565(0, 255, 0));
    ST7735_DrawString(7, 0, (char *)inFilename, ST7735_Color565(0, 255, 0));
    // get a character in 'c' and the number of successful reads in 'successfulreads'
    Fresult = f_read(&Handle, &c, 1, &successfulreads);
    x = 0;                              // start in the first column
    y = 10;                             // start in the second row
    while((Fresult == FR_OK) && (successfulreads == 1) && (y <= 130)){
      if(c == '\n'){
        x = 0;                          // go to the first column (this seems implied)
        y = y + 10;                     // go to the next row
      } else if(c == '\r'){
        x = 0;                          // go to the first column
      } else{                           // the character is printable, so print it
        ST7735_DrawChar(x, y, c, ST7735_Color565(255, 255, 255), 0, 1);
        x = x + 6;                      // go to the next column
        if(x > 122){                    // reached the right edge of the screen
          x = 0;                        // go to the first column
          y = y + 10;                   // go to the next row
        }
      }
      // get the next character in 'c'
      Fresult = f_read(&Handle, &c, 1, &successfulreads);
    }
    // close the file
    Fresult = f_close(&Handle);
  } else{
    // print the error code
    ST7735_DrawString(0, 0, "Error          (  )", ST7735_Color565(255, 0, 0));
    ST7735_DrawString(6, 0, (char *)inFilename, ST7735_Color565(255, 0, 0));
    ST7735_SetCursor(16, 0);
    ST7735_SetTextColor(ST7735_Color565(255, 0, 0));
    ST7735_OutUDec((uint32_t)Fresult);
  }

  // open the file to be written
  // Options:
  // FA_CREATE_NEW    - Creates a new file, only if it does not already exist.  If file already exists, the function fails.
  // FA_CREATE_ALWAYS - Creates a new file, always.  If file already exists, it is over-written.
  // FA_OPEN_ALWAYS   - Opens a file, always.  If file does not exist, the function creates a file.
  // FA_OPEN_EXISTING - Opens a file, only if it exists.  If the file does not exist, the function fails.
  Fresult = f_open(&Handle, outFilename, FA_WRITE|FA_OPEN_ALWAYS);
  if(Fresult == FR_OK){
    ST7735_DrawString(0, 14, "Opened ", ST7735_Color565(0, 255, 0));
    ST7735_DrawString(7, 14, (char *)outFilename, ST7735_Color565(0, 255, 0));
    // jump to the end of the file
    Fresult = f_lseek(&Handle, Handle.fsize);
    // write a message and get the number of successful writes in 'successfulwrites'
    Fresult = f_write(&Handle, "Another successful write.\r\n", 27, &successfulwrites);
    if(Fresult == FR_OK){
      // print the number of successful writes
      // expect: third parameter of f_write()
      ST7735_DrawString(0, 15, "Writes:    @", ST7735_Color565(0, 255, 0));
      ST7735_SetCursor(8, 15);
      ST7735_SetTextColor(ST7735_Color565(255, 255, 255));
      ST7735_OutUDec((uint32_t)successfulwrites);
      ST7735_SetCursor(13, 15);
      // print the byte offset from the start of the file where the writes started
      // expect: (third parameter of f_write())*(number of times this program has been run before)
      ST7735_OutUDec((uint32_t)(Handle.fptr - successfulwrites));
    } else{
      // print the error code
      ST7735_DrawString(0, 15, "f_write() error (  )", ST7735_Color565(255, 0, 0));
      ST7735_SetCursor(17, 15);
      ST7735_SetTextColor(ST7735_Color565(255, 0, 0));
      ST7735_OutUDec((uint32_t)Fresult);
    }
    // close the file
    Fresult = f_close(&Handle);
  } else{
    // print the error code
    ST7735_DrawString(0, 14, "Error          (  )", ST7735_Color565(255, 0, 0));
    ST7735_DrawString(6, 14, (char *)outFilename, ST7735_Color565(255, 0, 0));
    ST7735_SetCursor(16, 14);
    ST7735_SetTextColor(ST7735_Color565(255, 0, 0));
    ST7735_OutUDec((uint32_t)Fresult);
  }
  while(1){};
}
