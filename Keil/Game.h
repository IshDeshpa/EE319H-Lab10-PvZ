#ifndef GAME_H
#define GAME_H

#include <stdint.h>
#include "ff.h"
#include "diskio.h"

#define MUSIC_BUFFER_SIZE 16
#define FX_BUFFER_SIZE 16

// This is a comment leftover from a test issue

// Sprite contains a pointer to a bitmap, and has a length and width in pixels.
typedef struct Sprite SpriteType;
struct{
	uint16_t* bmp;	// Bitmap of half words to encode RGB info
	uint8_t length;	// Length
	uint8_t width;	// Width
	const SpriteType* nextSprite;	// Sprite pointer for animation FSM (each sprite only has one next sprite)
} Sprite;

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
		Sound();
	
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
		// Go to next sample. If reached end of buffer, change offset and read into local memory. If reached end of music, loop.
		void increment();
};

// Any object on the screen that needs to be rendered/unrendered and does something
class GameObject{
	protected:
		SpriteType* sprite;	// Sprite pointer
		Sound* soundFX;	// Sound effect
		uint8_t x;	// X position
		uint8_t y;	// Y position
		// Clear the current pixels of the game object
		void unrender();
		
		// Advance to the next state of the game object
		void advance();
	
		// Render the current state of the game object
		void render();
	public:
		// Constructor
		GameObject();
	
		// Constructor with parameters
		GameObject(SpriteType* sp, Sound* sfx, uint8_t x, uint8_t y);
		
		// Destructor
		~GameObject();
	
		// Copy Constructor
		GameObject(const GameObject& other);
	
		// Assignment Operator
		GameObject operator=(GameObject& other);
		
		// Unrender, advance to next state of the game object and render
		void refresh();
};

// GameObject with health (plants and zombies)
class Entity: public GameObject{
	protected:
		uint8_t health;	// Health
		uint8_t animationTime;	// time to switch animation sprites
		uint8_t hostile; //0 or 1
		
		// Advance to the next state of the entity and call attack?
		void advance();
		// do attacking sequence if hostile
		void attack();
	public:
		//constructor with all new parameters, calls parent constructor for first 4
		Entity(SpriteType* sp, Sound* sfx, uint8_t xpos, uint8_t ypos, uint8_t hp, uint8_t anim, uint8_t hostl);
};

// Projectile
class Projectile: public GameObject{
	protected:
		uint8_t damage;	// Damage
		uint8_t speed;	// Speed
		uint8_t collision; //1 or 0
		void advance();
	public:
		//constructor with all new parameters, calls parent constructor for first 4
		Projectile(SpriteType* sp, Sound* sfx, uint8_t xpos, uint8_t ypos, uint8_t spd, uint8_t dam);
};

// Collection of all game objects, background, music, etc. pertinent to the current area of the game
class Scene{
	private:
		GameObject* objects;	// List of all objects on the scene
		uint16_t* backgroundBMP;	// Background of the scene as a bitmap
	public:
		// Constructor
		Scene();
		
		// Destructor
		~Scene();
	
		// Copy Constructor
		Scene(const Scene& other);
	
		// Assignment Operator
		Scene operator=(Scene& other);
};


#endif
