// put prototypes for public functions, explain what it does
// put your names here, date
#ifndef __DAC_H__ // do not include more than once
#define __DAC_H__
#include <stdint.h>
#include "diskio.h"
#include "ff.h"
#include "Display.h"
#include "Timer0.h"

#define assert(); if(file_res!=FR_OK){ferrorLCD(this->path);}
#define SAMPLE_RT 8000
#define BIT_DEPTH 8
#define NUM_SOUNDS 3

// Sound stored on SD Card
class Sound{
	protected:
		FIL soundFile;	// Sound file
		char* path;	// Path to file
		uint32_t chunkPtr;	// Number of buffers read so far
		uint32_t dataSize;	// Size of data section
		
		uint8_t* soundBuffer;	// Sound buffer
		uint16_t bufferPtr;	// Current location in buffer (offset from soundBuffer)
		uint16_t defaultBufferSize;	// Size of a default buffer; number of bytes
		uint16_t currentBufferSize;	// Size of current buffer; number of bytes
		
		uint8_t end;	// Reached end of file
		uint8_t loop;	// True if we need to loop, false otherwise (if it's music, we loop)
	
		// Load sound effect into local memory
		void loadFile();
	public:
		// Constructor
		Sound(char* path, uint8_t* soundBuffer, uint16_t bufferSize);
		
		// Destructor
		~Sound();
	
		// Go to next sample. If reached end, free buffer and close file but DON'T destroy sound.
		void increment();
	
		// Get current sample
		uint8_t getSample();
	
		//plays the sound (I referenced this in a lot of classes, so impement it as play lol)
		void play();
};

// Takes in a Sound* and adds it to the global array of sounds
void Sound_Init(Sound* s);

// **************DAC_Init*********************
// Initialize 6-bit DAC, called once 
// Input: none
// Output: none
void DAC_Init(void);

// **************DAC_Out*********************
// output to DAC
// Input: 6-bit data, 0 to 63 OR 8-bit data, 0 to 255
// Input=n is converted to n*3.3V/63 OR n*3.3V/255
// Output: none
void DAC_Out(uint32_t data);

// Play all sounds in the array
void playAllSounds();

#endif
