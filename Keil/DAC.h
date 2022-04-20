// put prototypes for public functions, explain what it does
// put your names here, date
#ifndef __DAC_H__ // do not include more than once
#define __DAC_H__
#include <stdint.h>
#include "diskio.h"
#include "ff.h"

// Sound stored on SD Card
class Sound{
	protected:
		FIL soundFile;	// Sound file
		char* path;	// Path to file
		uint8_t* soundBuffer;	// Sound buffer
		uint8_t bufferPtr;	// Current location in buffer (offset from soundBuffer)
		uint16_t bufferSize;	// Size of buffer; number of bytes
		
		// Load sound effect into local memory
		void loadFile();
	public:
		// Constructor
		Sound(char* path, uint16_t bufferSize);
		
		// Destructor
		~Sound();
	
		// Go to next sample. If reached end, free buffer and close file but DON'T destroy sound.
		void increment();
		
		// Play sound
		void play();
};

// Music stored on SD Card; inherits from sound
class Music: private Sound{
	private:
		uint64_t chunkPtr;	// Points to chunk in file that buffer is currently at
		uint16_t musicSize;	// Size of total music; number of bytes
		uint64_t musicOfs;	// Offset from original file pointer (use f_lseek)
	public:
		// Constructor
		Music(char* path, uint16_t bufferSize);

		// Go to next sample. If reached end of buffer, change offset and read into local memory. If reached end of music, loop.
		void increment();
};

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

void temp();

#endif
