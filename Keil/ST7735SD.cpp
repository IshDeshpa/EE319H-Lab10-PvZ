// ST7735.c
// Runs on LM4F120/TM4C123
// Low level drivers for the ST7735 160x128 LCD based off of
// the file described above.  Further modified for simultaneous
// use of the SD card (CS on PD7) and ST7735 LCD (CS on PA3).
//    16-bit color, 128 wide by 160 high LCD
// Daniel Valvano
// April 1, 2016
// Augmented 7/17/2014 to have a simple graphics facility
// Tested with LaunchPadDLL.dll simulator 9/2/2014
// added PB0 as a choice for SDC CS


/* This example accompanies the book
   "Embedded Systems: Real Time Interfacing to ARM Cortex M Microcontrollers",
   ISBN: 978-1463590154, Jonathan Valvano, copyright (c) 2016

 Copyright 2016 by Jonathan W. Valvano, valvano@mail.utexas.edu
    You may use, edit, run or distribute this file
    as long as the above copyright notice remains
 THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 VALVANO SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL,
 OR CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 For more information about my classes, my research, and my books, see
 http://users.ece.utexas.edu/~valvano/
 */

// hardware connections
// **********ST7735 TFT and SDC*******************
// ST7735
// Backlight (pin 10) connected to +3.3 V
// MISO (pin 9) connected to PA4 (SSI0Rx)
// SCK (pin 8) connected to PA2 (SSI0Clk)
// MOSI (pin 7) connected to PA5 (SSI0Tx)
// TFT_CS (pin 6) connected to PA3 (GPIO/SSI0Fss)
// CARD_CS (pin 5) connected to PD7 or PB0 (GPIO)
// Data/Command (pin 4) connected to PA6 (GPIO), high for data, low for command
// RESET (pin 3) connected to PA7 (GPIO)
// VCC (pin 2) connected to +3.3 V
// Gnd (pin 1) connected to ground

// **********wide.hk ST7735R*******************
// Silkscreen Label (SDC side up; LCD side down) - Connection
// VCC  - +3.3 V
// GND  - Ground
// !SCL - PA2 Sclk SPI clock from microcontroller to TFT or SDC
// !SDA - PA5 MOSI SPI data from microcontroller to TFT or SDC
// DC   - PA6 TFT data/command
// RES  - PA7 TFT reset
// CS   - PA3 TFT_CS, active low to enable TFT
// *CS  - PD7 or PB0 SDC_CS, active low to enable SDC
// MISO - PA4 MISO SPI data from SDC to microcontroller
// SDA  – (NC) I2C data for ADXL345 accelerometer
// SCL  – (NC) I2C clock for ADXL345 accelerometer
// SDO  – (NC) I2C alternate address for ADXL345 accelerometer
// Backlight + - Light, backlight connected to +3.3 V

#define SDC_CS_PD0 1
#define SDC_CS_PD7 0

// SDC CS is PD7 or PB0 , TFT CS is PA3
// to change CS to another GPIO, change SDC_CS and CS_Init
// **********ST7735 TFT and SDC*******************
// ST7735
// 1  ground
// 2  Vcc +3.3V
// 3  PA7 TFT reset
// 4  PA6 TFT data/command
// 5  PD7/PB0 SDC_CS, active low to enable SDC
// 6  PA3 TFT_CS, active low to enable TFT
// 7  PA5 MOSI SPI data from microcontroller to TFT or SDC
// 8  PA2 Sclk SPI clock from microcontroller to TFT or SDC
// 9  PA4 MOSO SPI data from SDC to microcontroller
// 10 Light, backlight connected to +3.3 V

// **********wide.hk ST7735R*******************
// Silkscreen Label (SDC side up; LCD side down) - Connection
// VCC  - +3.3 V
// GND  - Ground
// !SCL - PA2 Sclk SPI clock from microcontroller to TFT or SDC
// !SDA - PA5 MOSI SPI data from microcontroller to TFT or SDC
// DC   - PA6 TFT data/command
// RES  - PA7 TFT reset
// CS   - PA3 TFT_CS, active low to enable TFT
// *CS  - PD7/PB0 SDC_CS, active low to enable SDC
// MISO - PA4 MISO SPI data from SDC to microcontroller
// SDA  – (NC) I2C data for ADXL345 accelerometer
// SCL  – (NC) I2C clock for ADXL345 accelerometer
// SDO  – (NC) I2C alternate address for ADXL345 accelerometer
// Backlight + - Light, backlight connected to +3.3 V

#include "ST7735SD.h"
#include <stdint.h>
#include "inc/tm4c123gh6pm.h"

#define TFT_CS           (*((volatile uint32_t *)0x40004020))
#define TFT_CS_LOW       0 // CS controlled by software PA3
#define TFT_CS_HIGH      0x08
#if SDC_CS_PD7
// PD7 output used for SDC CS
#define SDC_CS           (*((volatile uint32_t *)0x40007200))
#define SDC_CS_LOW       0           // CS controlled by software
#define SDC_CS_HIGH      0x80
void SDC_CS_Init(void){
  SYSCTL_RCGCGPIO_R |= 0x08;            // activate clock for Port D
  while((SYSCTL_PRGPIO_R&0x08) == 0){}; // allow time for clock to stabilize
  GPIO_PORTD_LOCK_R = 0x4C4F434B;       // unlock GPIO Port D
  GPIO_PORTD_CR_R = 0xFF;               // allow changes to PD7-0
  // only PD7 needs to be unlocked, other bits can't be locked
  GPIO_PORTD_DIR_R |= 0x80;             // make PD7 out
  GPIO_PORTD_AFSEL_R &= ~0x80;          // disable alt funct on PD7
  GPIO_PORTD_DR4R_R |= 0x80;            // 4mA drive on outputs
  GPIO_PORTD_PUR_R |= 0x80;             // enable weak pullup on PD7
  GPIO_PORTD_DEN_R |= 0x80;             // enable digital I/O on PD7
                                        // configure PD7 as GPIO
  GPIO_PORTD_PCTL_R = (GPIO_PORTD_PCTL_R&0x0FFFFFFF)+0x00000000;
  GPIO_PORTD_AMSEL_R &= ~0x80;          // disable analog functionality on PD7
  SDC_CS = SDC_CS_HIGH;
}
#endif
// PD0 output used for SDC CS
#if SDC_CS_PD0
#define SDC_CS           (*((volatile uint32_t *)0x40007004))
#define SDC_CS_LOW       0           // CS controlled by software
#define SDC_CS_HIGH      0x01
void SDC_CS_Init(void){
  SYSCTL_RCGCGPIO_R |= 0x08;            // activate clock for Port D
  while((SYSCTL_PRGPIO_R&0x08) == 0){}; // allow time for clock to stabilize
  GPIO_PORTD_LOCK_R = 0x4C4F434B;       // unlock GPIO Port D
  GPIO_PORTD_CR_R = 0xFF;               // allow changes to PD7-0
  // only PD7 needs to be unlocked, other bits can't be locked
  GPIO_PORTD_DIR_R |= 0x01;             // make PD0 out
  GPIO_PORTD_AFSEL_R &= ~0x01;          // disable alt funct on PD0
  GPIO_PORTD_DR4R_R |= 0x01;            // 4mA drive on outputs
  GPIO_PORTD_PUR_R |= 0x01;             // enable weak pullup on PD0
  GPIO_PORTD_DEN_R |= 0x01;             // enable digital I/O on PD0
                                        // configure PD0 as GPIO
  GPIO_PORTD_PCTL_R = (GPIO_PORTD_PCTL_R&0xFFFFFFF0)+0x00000000;
  GPIO_PORTD_AMSEL_R &= ~0x01;          // disable analog functionality on PD0
  SDC_CS = SDC_CS_HIGH;
}
#endif

extern "C" void DisableInterrupts(void);
extern "C" void EnableInterrupts(void);
uint8_t xchg_spi(unsigned char c, uint8_t dc){
	DisableInterrupts();
	volatile uint8_t response;
                                        // wait until SSI0 not busy/transmit FIFO empty
  while((SSI0_SR_R&SSI_SR_BSY)==SSI_SR_BSY){};
	D_C(dc);
	SSI0_DR_R = c;                        // data out
  while((SSI0_SR_R&SSI_SR_RNE)==0){};   // wait until response
		
	response = SSI0_DR_R;
	EnableInterrupts();
	return response;
}

void Delay(uint32_t u1Count){	
	Delay:
		__asm("subs u1Count, #1");
		__asm("bne Delay");
	return;
}

// Subroutine to wait 1 msec
// Inputs: None
// Outputs: None
// Notes: This assumes 80 MHz system clock.
void Delay1ms(uint32_t n){
  while(n){
    Delay(26667);
    n--;
  }
}

// Companion code to the above tables.  Reads and issues
// a series of LCD commands stored in ROM byte array.
void commandList(const cmdList* c) {
  //uint8_t numCommands, numArgs;
  //uint16_t ms;
	uint8_t i = 0;
	uint8_t numCmds = c->numCmds;
	
  while(i<numCmds) {                 // For each command...
    xchg_spi(c->cmds[i].id, DC_COMMAND);             //   Read, issue command
    uint8_t j = 0;
		while(j<c->cmds[i].numArgs) {                   //   For each argument...
			xchg_spi(c->cmds[i].args[j++], DC_DATA);              //     Read, issue argument
    }
		
		uint16_t ms = c->cmds[i].delay;
    if(ms) {
      if(ms == 255) ms = 500;     // If 255, delay for 500 ms
      Delay1ms(ms);
    }
		i++;
  }
}

void SSI0_Init(uint32_t CPSDVSR){
	SDC_CS_Init();
	                                      // activate clock for Port A
  SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R0;
                                        // allow time for clock to stabilize
  while((SYSCTL_PRGPIO_R&SYSCTL_PRGPIO_R0) == 0){};
                                        // activate clock for SSI0
  SYSCTL_RCGCSSI_R |= SYSCTL_RCGCSSI_R0;
                                        // allow time for clock to stabilize
  while((SYSCTL_PRSSI_R&SYSCTL_PRSSI_R0) == 0){};

  // toggle RST low to reset; CS low so it'll listen to us
  // SSI0Fss is used as GPIO
  GPIO_PORTA_DIR_R |= 0xC8;             // make PA3,6,7 out
  GPIO_PORTA_AFSEL_R &= ~0xC8;          // disable alt funct on PA3,6,7
  GPIO_PORTA_DEN_R |= 0xC8;             // enable digital I/O on PA3,6,7
                                        // configure PA3,6,7 as GPIO
  GPIO_PORTA_PCTL_R = (GPIO_PORTA_PCTL_R&0x00FF0FFF)+0x00000000;
  GPIO_PORTA_AMSEL_R &= ~0xC8;          // disable analog functionality on PA3,6,7
  SwitchToDisplay();
  Reset(RESET_HIGH);
  Delay1ms(500);
  Reset(RESET_LOW);
  Delay1ms(500);
  Reset(RESET_HIGH);
  Delay1ms(500);

  // initialize SSI0
  GPIO_PORTA_AFSEL_R |= 0x34;           // enable alt funct on PA2,4,5
  GPIO_PORTA_PUR_R |= 0x3C;             // enable weak pullup on PA2,3,4,5
	GPIO_PORTA_DR4R_R |= 0xFC;            // 4mA drive on all outputs
	GPIO_PORTA_DEN_R |= 0x34;             // enable digital I/O on PA2,4,5
                                        // configure PA2,4,5 as SSI
  GPIO_PORTA_PCTL_R = (GPIO_PORTA_PCTL_R&0xFF00F0FF)+0x00220200;
  GPIO_PORTA_AMSEL_R &= ~0x34;          // disable analog functionality on PA2,4,5
  SDC_CS_Init();                        // SDC CS on PB0 or PD7
  TurnDisplay(OFF);
  SSI0_CR1_R &= ~SSI_CR1_SSE;           // disable SSI
  SSI0_CR1_R &= ~SSI_CR1_MS;            // master mode
                                        // configure for clock from source PIOSC for baud clock source
  //SSI0_CC_R = (SSI0_CC_R&~SSI_CC_CS_M)+SSI_CC_CS_PIOSC;
	SSI0_CC_R = (SSI0_CC_R&~SSI_CC_CS_M)+SSI_CC_CS_SYSPLL;
                                        // clock divider for 8 MHz SSIClk (16 MHz PIOSC/2)
                                        // PIOSC/(CPSDVSR*(1+SCR))
                                        // 16/(2*(1+0)) = 8 MHz
  SSI0_CPSR_R = (SSI0_CPSR_R&~SSI_CPSR_CPSDVSR_M)+CPSDVSR; // must be even number
  SSI0_CR0_R &= ~(SSI_CR0_SCR_M |       // SCR = 0 (8 Mbps data rate)
                  SSI_CR0_SPH |         // SPH = 0
                  SSI_CR0_SPO);         // SPO = 0
                                        // FRF = Freescale format
  SSI0_CR0_R = (SSI0_CR0_R&~SSI_CR0_FRF_M)+SSI_CR0_FRF_MOTO;
                                        // DSS = 8-bit data
  SSI0_CR0_R = (SSI0_CR0_R&~SSI_CR0_DSS_M)+SSI_CR0_DSS_8;
  SSI0_CR1_R |= SSI_CR1_SSE;            // enable SSI
	
	//SSI0_DMACTL_R |= 0x01;
}

void SSI0_Handler(){


}

void TurnDisplay(uint8_t state){	// ON or OFF
	TFT_CS = !state;
}

void TurnSD(uint8_t state){	// ON or OFF
	SDC_CS = !state;
}

void SwitchToDisplay(){
	TurnDisplay(ON);
	TurnSD(OFF);
}

void SwitchToSD(){
	TurnDisplay(OFF);
	TurnSD(ON);
}

void D_C(uint8_t b){
	DC = b;
}

void Reset(uint8_t b){
	RESET = b;
}

