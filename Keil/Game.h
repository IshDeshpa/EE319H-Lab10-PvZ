#ifndef GAME_H
#define GAME_H

#include <stdint.h>

// Sprite contains a pointer to a bitmap, and has a length and width in pixels.
typedef struct Sprite SpriteType;
struct{
	uint16_t* bmp;	// Bitmap of half words to encode RGB info
	uint8_t length;	// Length
	uint8_t width;	// Width
	const SpriteType* nextSprite;	// Sprite pointer for animation FSM (each sprite only has one next sprite
} Sprite;


// Any object on the screen that needs to be rendered/unrendered and does something
class GameObject{
	private:
		SpriteType* animationFSM;	// FSM containing frames of animation
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
		
		// Destructor
		~GameObject();
	
		// Copy Constructor
		GameObject(const GameObject&);
		
		// Unrender, advance to next state of the game object and render
		void refresh();
};

// Collection of all game objects, background, music, etc. pertinent to the current area of the game
class Scene{
	private:
		GameObject* objects;	// List of all objects on the scene
		uint16_t* backgroundBMP;	// Background of the scene as a bitmap
		uint8_t* music;	// Music playing in the scene
	public:
};


#endif
