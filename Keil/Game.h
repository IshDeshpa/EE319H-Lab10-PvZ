#ifndef GAME_H
#define GAME_H

#include <stdint.h>
#include "ff.h"
#include "diskio.h"
#include "DAC.h"


#define MUSIC_BUFFER_SIZE 16
#define FX_BUFFER_SIZE 16

//Main game balancing controls
#define gameMovementSpeed 1 //Based on ticks?
#define gameFPS 10 //could be uncapped?
#define gameTickRate 20 //controls attackRates and animations
#define damageRatio 1
#define healthRatio 1
#define animationRatio 1//general animation speed ratio

#define speedMultiplier 0.1 * gameMovementSpeed
#define peaSpeed 10*speedMultiplier
#define snowPeaSpeed peaSpeed
#define sunSpeed 1*speedMultiplier
#define zombieSpeed 1*speedMultiplier
#define poleVaultSpeed zombieSpeed*2
#define lawnmowerSpeed 5

#define peaDamage 1*damageRatio
#define ohkoDamage 50*damageRatio
#define chompDamage ohkoDamage
#define explosionDamage ohkoDamage
#define smallExplosionDamage ohkoDamage
#define jackInTheBoxDamage ohkoDamage
#define zombieDamage 1*damageRatio
#define lawnmowerDamage 50*damageRatio

#define defaultPlantHealth 8*healthRatio
#define wallNutHealth 100*healthRatio
#define defaultZombieHealth 10*healthRatio
#define coneHealth 10*healthRatio
#define bucketHealth 20*healthRatio
#define newspaperHealth 8*healthRatio
#define helmetHealth 30*healthRatio
//TO-DO

#define	PlantAttackRate 1.5*gameTickRate
#define repeaterRepeatTicks 0.2*gameTickRate
#define ZombieAttackRate 0.4*gameTickRate
#define sunProductionRate 10*gameTickRate


#define defaultAnimationTime gameTickRate*animationRatio
#define DefaultPlantAnimationRate 0.5*defaultAnimationTime
#define CherryBombAnimationRate 0.2*defaultAnimationTime
#define ExplosionAnimationRate 0.2*defaultAnimationTime
#define ChompAnimationRate 0.2*defaultAnimationTime
#define DefaultZombieAnimationRate 0.3*defaultAnimationTime
#define FootballAnimationRate 0.1*defaultAnimationTime
#define PoleVaultAnimationRate 0.1*defaultAnimationTime
#define EatingZombieAnimationRate 0.1*defaultAnimationTime

#define cmpButtonXSize
#define cmpButtonYSize
#define	vsButtonXSize
#define vsButtonYSize
#define langButtonXSize
#define langButtonYSize

// This is a comment leftover from a test issue

// Sprite contains a pointer to a bitmap, and has a length and width in pixels.
typedef struct Sprite SpriteType;
struct{
	uint16_t* bmp;	// Bitmap of half words to encode RGB info
	uint8_t length;	// Length
	uint8_t width;	// Width
	const SpriteType* nextSprite;	// Sprite pointer for animation FSM (each sprite only has one next sprite)
} Sprite;

// Any object on the screen that needs to be rendered/unrendered and does something
class GameObject{
	protected:
		//SpriteType* previousSprite; //when we advance, set the previousSprite to the sprite if we change sprites
		SpriteType* sprite;	// Sprite pointer
		uint8_t redraw; //1 or 0, initialize to 1, only render if 0
		Sound* soundFX;	// Sound effect
		uint8_t x;	// X position
		uint8_t y;	// Y position
		//uint8_t oldx;
		//uint8_t oldy;
		// Clear the current pixels of the game object, replacing them with the background
		void unrender();
		
		// Advance to the next state of the game object (should be overloaded)
		void advance();
	
		// Render the current state of the game object
		void render();
	
		// unrender pixels and replace them one at a time
		void rerender();
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
		void collided();
};

class LawnMower: public Projectile{
	protected:
		uint8_t isMoving; //1 or 0
		//change advance so it only moves if isMoving == 1
	public:
		//constructer calls Projectile with lawnmower sprite, lawnmower sfx, x and y arguments, lawnmower speed, lawnmower damage
		//set isMoving to 0
		LawnMower(uint8_t x, uint8_t y);
		//change collided so it doesn't kill lawnmower, sets isMoving to 1 and deals damage to zombies
		void collided();
};	

class GameObjectList{
	private:
		GameObject* objects[256];
		uint8_t indexPtr;
	public:
		//Constructor
		GameObjectList(GameObject** GOlist);
		//Destructor
		~GameObjectList();
		//Add Object
		void GOAdd(GameObject* add);
		//Access Object at index
		GameObject* operator[](uint8_t i);
		//Remove Object at index
		void GORmv(uint8_t i);
};
// Collection of all game objects, background, music, etc. pertinent to the current area of the game
class Scene{
	private:
		GameObjectList* Zombies;	// List of all objects on the scene these are arrays of pointers
		GameObjectList* Plants;
		GameObjectList* Buttons;
		GameObjectList* Lawnmowers;
		GameObjectList* Projectiles;
		uint16_t* backgroundBMP;	// Background of the scene as a bitmap
		Sound* music;
	public:
		// Constructor
		Scene(GameObjectList* but, GameObjectList* lwm, uint16_t* bg, Sound* msc);
		// Destructor
		//~Scene();
	
		// Copy Constructor
		Scene(const Scene& other);
	
		// Assignment Operator
		Scene operator=(Scene& other);
};


#endif
