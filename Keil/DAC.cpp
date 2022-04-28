// put implementations for functions, explain how it works
// put your names here, date
#include <stdint.h>
#include "inc/tm4c123gh6pm.h"

#include "ff.h"
#include "diskio.h"
#include "Display.h"
#include "Timer0.h"
#include "DAC.h"

FRESULT file_res;
static FATFS g_sFatFs;
uint8_t br;

Sound* sounds[NUM_SOUNDS] = {0};

extern "C" void DisableInterrupts(void);
extern "C" void EnableInterrupts(void);

Sound::Sound(char* path, uint8_t* soundBuffer, uint16_t bufferSize){
	this->soundBuffer = soundBuffer;
	this->defaultBufferSize = bufferSize;
	this->currentBufferSize = bufferSize;
	this->path = path;
	this->bufferPtr = 0;
	
	Timer0_Arm(0);
	
	// For getting data size
	uint8_t buffSize = 4;	
	uint8_t buff[4];
	
	// Mount filesystem
	file_res = f_mount(&g_sFatFs, "", 1);
	assert();
	
	// Open file
	file_res = f_open(&(this->soundFile), (const TCHAR*)this->path, FA_READ);
	assert();
	
	// Seek to & get data size
	file_res = f_lseek(&(this->soundFile), 0x28);
	assert();
	br = 0;
	file_res = f_read(&(this->soundFile), buff, buffSize, (UINT*)&br);
	assert();
	this->dataSize = (buff[3]<<24) | (buff[2]<<16) | (buff[1]<<8) | (buff[0]);
	
	this->loadFile();
	
	Timer0_Arm(1);
	
	// Close file
	//file_res = f_close(&(this->soundFile));
	//assert();
}

Sound::~Sound(){
	for(int i=0; i<this->defaultBufferSize; i++){
		soundBuffer[i] = 0;
	}
	delete &soundFile;
}

void Sound::increment(){
	if(!this->end){
		if(this->bufferPtr < this->currentBufferSize){	// If not at end of buffer, add to buffer
			this->bufferPtr++;
		}
		else{	// Else, load next buffer
			this->loadFile();
			this->bufferPtr = 0;
		}
	}
	else{
		// Free buffer
		if(this->soundBuffer != 0){
			// Close file
			file_res = f_close(&(this->soundFile));
			assert();

			for(int i=0; i<this->defaultBufferSize; i++){
				soundBuffer[i] = 0;
			}
		}
		
		return;
	}
}

void Sound::loadFile(){
	// Read into buffer
	uint16_t bs = this->currentBufferSize;
	if(chunkPtr*this->defaultBufferSize >= this->dataSize){
		this->end = 1;
	}
	
	if((chunkPtr+1) * this->defaultBufferSize >= this->dataSize){	// If next load of complete buffer size goes over file boundary, we want to shrink buffer to only have remaining data
		//delete [] this->soundBuffer;
		this->currentBufferSize = this->dataSize - this->chunkPtr*bs;
		//this->soundBuffer = new uint8_t[this->dataSize - this->chunkPtr*bs];	// Buffer size is now remaining amount of data
	}
	
	br = 0;
	Timer0_Arm(0);
	file_res = f_read(&(this->soundFile), this->soundBuffer, bs, (UINT*)&br);
	assert();
	Timer0_Arm(1);
	this->chunkPtr++;
}

uint8_t Sound::getSample(){
	return this->soundBuffer[this->bufferPtr];
}

// Takes in a Sound* and adds it to the global array of sounds
void Sound_Init(Sound* s){
	int i=0;
	while(sounds[i]!=0 && i < NUM_SOUNDS){
		i++;	// Loop to next empty position of array
	}
	if(i < NUM_SOUNDS){
		sounds[i] = s;	// Add to array
	}
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
	
	for(int i=0; i<NUM_SOUNDS; i++){
		sounds[i] = 0;	// Initialize sounds array to 0
	}
	
	Timer0_Init(&playAllSounds, 80000000/SAMPLE_RT);
}

// **************DAC_Out*********************
// output to DAC
// Input: 6-bit data, 0 to 63 OR 8-bit data, 0 to 255
// Input=n is converted to n*3.3V/63 OR n*3.3V/255
// Output: none
void DAC_Out(uint32_t data){
	GPIO_PORTB_DATA_R = data&0xFF;
}

void playAllSounds(){
	//uint8_t total = 0;
	//for(int i=0; i<NUM_SOUNDS && sounds[i] != 0; i++){
	uint8_t sample;
	if(sounds[0] != 0){
		sample = sounds[0]->getSample();
		//total += sample;
		sounds[0]->increment();
	}
	//}
	DAC_Out(sample);
}
