// put implementations for functions, explain how it works
// put your names here, date
#include <stdint.h>
#include "inc/tm4c123gh6pm.h"

//#include "ff.h"
//#include "diskio.h"
#include "Timer0.h"
#include "DAC.h"

uint16_t i=0;

const uint8_t Sine[256] = {32,32,33,34,35,35,36,37,38,38,39,40,41,41,42,43,
    44,44,45,46,46,47,48,48,49,50,50,51,51,52,53,53,54,54,55,55,56,56,57,57,
    58,58,59,59,59,60,60,60,61,61,61,61,62,62,62,62,62,63,63,63,63,63,63,63,
    63,63,63,63,63,63,63,63,62,62,62,62,62,61,61,61,61,60,60,60,59,59,59,58,
    58,57,57,56,56,55,55,54,54,53,53,52,51,51,50,50,49,48,48,47,46,46,45,44,
    44,43,42,41,41,40,39,38,38,37,36,35,35,34,33,32,32,31,30,29,28,28,27,26,
    25,25,24,23,22,22,21,20,19,19,18,17,17,16,15,15,14,13,13,12,12,11,10,10,
    9,9,8,8,7,7,6,6,5,5,4,4,4,3,3,3,2,2,2,2,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,1,1,1,1,1,2,2,2,2,3,3,3,4,4,4,5,5,6,6,7,7,8,8,9,9,10,10,11,12,12,
    13,13,14,15,15,16,17,17,18,19,19,20,21,22,22,23,24,25,25,26,27,28,28,29,
    30,31};

Sound::Sound(char* path, uint16_t bufferSize){
	this->soundBuffer = new uint8_t[bufferSize];
	this->bufferSize = bufferSize;
	this->path = path;
	this->bufferPtr = 0;

	// Open file
//	f_open(&(this->soundFile), (const TCHAR*)this->path, FA_READ);
	
	// Read file
	//uint8_t br;
//	f_read(&(this->soundFile), this->soundBuffer, this->bufferSize, (UINT*)&br);
}

Sound::~Sound(){
	delete [] soundBuffer;
	delete &soundFile;
}

void Sound::increment(){
	this->bufferPtr++;
	this->play();
}

void Sound::play(){
	DAC_Out(this->soundBuffer[bufferPtr]);
}

// **************DAC_Init*********************
// Initialize 6-bit DAC, called once 
// Input: none
// Output: none
void DAC_Init(void){
	SYSCTL_RCGCGPIO_R |= 0x02;
	volatile uint32_t delay = SYSCTL_RCGCGPIO_R;
	GPIO_PORTB_DIR_R |= 0xFF;	// 3F for 6-bit DAC, FF for 8-bit DAC
	GPIO_PORTB_DEN_R |= 0xFF;
	GPIO_PORTB_PDR_R = 0x00;
	GPIO_PORTB_PUR_R = 0x00;
	GPIO_PORTB_DR8R_R |= 0xFF;
	//void (*dacout)(void) = &temp;
	Timer0_Init(temp, 355);
}

// **************DAC_Out*********************
// output to DAC
// Input: 6-bit data, 0 to 63 OR 8-bit data, 0 to 255
// Input=n is converted to n*3.3V/63 OR n*3.3V/255
// Output: none
void DAC_Out(uint32_t data){
	GPIO_PORTB_DATA_R = data&0xFF;
}

void temp(){
	
	if(i > 255){
		i = 0;
	}
	
	DAC_Out(Sine[i]);
	
	i++;
}


