#ifndef GAME_H
#define GAME_H

#include <stdint.h>
#include "DAC.h"
#include "Display.h"



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

#define CmpButtonXpos 10
#define CmpButtonYpos 50
#define VsButtonXpos 90
#define VsButtonYpos 50
#define LangButtonXpos 80
#define LangButtonYpos 10

#define SpXSize 10 //TO-DO
#define SpYSize 10 
#define SpOffset SpXSize+1
#define SpYpos 115
#define FspXpos 1
#define SspXpos FspXpos + SpOffset * 1
#define TspXpos FspXpos + SpOffset * 2
#define	FospXpos FspXpos + SpOffset * 3
#define FispXpos FspXpos + SpOffset * 4
#define SispXpos FspXpos + SpOffset * 5
#define SespXpos FspXpos + SpOffset * 6
#define EspXpos FspXpos + SpOffset * 7

#define LaneYOffset 20 //change later
#define Lane1Ypos 10
#define Lane2Ypos Lane1Ypos + LaneYOffset * 1
#define Lane3Ypos Lane1Ypos + LaneYOffset * 2
#define Lane4Ypos Lane1Ypos + LaneYOffset * 3
#define Lane5Ypos Lane1Ypos + LaneYOffset * 4

	//packet load times;
#define LoadTime 100

// Sprite contains a pointer to a bitmap, and has a length and width in pixels.
class SpriteType{
	public:
		uint16_t* bmp;	// Bitmap of half words to encode RGB info
		uint8_t length;	// Length
		uint8_t width;	// Width
		SpriteType* nextSprite;	// Sprite pointer for animation FSM (each sprite only has one next sprite)
	public:
		SpriteType(uint16_t* bmp, uint8_t length, uint8_t width, SpriteType* nextSprite);
};



// Any object on the screen that needs to be rendered/unrendered and does something
class GameObject{
	protected:
		//SpriteType* previousSprite; //when we advance, set the previousSprite to the sprite if we change sprites
		SpriteType* sprite;	// Sprite pointer
		uint8_t redraw; //1 or 0, initialize to 1, only render if 0
		Sound* soundFX;	// Sound effect
		uint8_t x;	// X position
		uint8_t y;	// Y position
		uint8_t lane;
		uint8_t oldx;
		uint8_t oldy;
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
		//GameObject();
	
		// Constructor with parameters
		GameObject(SpriteType* sp, Sound* sfx, uint8_t x, uint8_t y);
		GameObject(SpriteType* sp, Sound* sfx, uint8_t x, uint8_t y, uint8_t lane);
	
		
		// Destructor
		//~GameObject();
	
		// Copy Constructor
		//GameObject(const GameObject& other);
	
		// Assignment Operator
		//GameObject operator=(GameObject& other);
		
		// Unrender, advance to next state of the game object and render
		void refresh();
		//game object tick will not do anything
		void tick();
		//game object collide will not do anything
		void collided();
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
		Entity(SpriteType* sp, Sound* sfx, uint8_t xpos, uint8_t ypos, uint8_t hp, uint8_t anim, uint8_t hostl, uint8_t lane);
		//entity tick will decrement animationTime
		void tick();
};

// Projectile
class Projectile: public GameObject{
	protected:
		uint8_t damage;	// Damage
		uint8_t speed;	// Speed
		uint8_t distanceDiff; //increments by speed every tick, advance adds it to xpos and sets redraw to 1;
		uint8_t collision; //1 or 0
		void advance();
	public:
		//constructor with all new parameters, calls parent constructor for first 4
		Projectile(SpriteType* sp, Sound* sfx, uint8_t xpos, uint8_t ypos, uint8_t spd, uint8_t dam, uint8_t lane);
		int collided();
		//projectile tick will change their xpos by their speed and set redraw to 1, if speed > 0
		void tick();
};

class LawnMower: public Projectile{
	protected:
		uint8_t isMoving; //1 or 0
		//change advance so it only moves if isMoving == 1
		void advance();
		void reset();
	public:
		//constructer calls Projectile with lawnmower sprite, lawnmower sfx, x and y arguments, lawnmower speed, lawnmower damage
		//set isMoving to 0
		LawnMower(uint8_t x, uint8_t y, uint8_t lane);
		//change collided so it doesn't kill lawnmower, sets isMoving to 1 and deals damage to zombies
		int collided();
		//lawnmower tick will change xpos by speed and set redraw to 1 if isMoving == 1
		void tick();
	};	

class Pea : public Projectile{
	public:
		//set projectile variables to defined pea damage, speed, sprite, sfx, collision = 1
		Pea(uint8_t x, uint8_t y);
};

class FrozenPea : public Projectile{
	public:
		//set projectile variables to defined Snowpea damage, sprite, pea sfx, and pea speed, collision = 1
		FrozenPea(uint8_t x, uint8_t y);
		//Change collision so it slows the zombie when hit
		void collided();
};

// One hit KO (e.g. Cherry Bomb, Potato Mine, Chomper)
class Ohko: public Projectile{
	public:
		//set projectile variable to defined OHKO damage, OHKO (transparent) sprite, sfx, speed = 0, collision = 1
		Ohko(uint8_t x, uint8_t y);
		//Special constructor does same as other constructor, but sets sprite to argument
		Ohko(uint8_t x, uint8_t y, SpriteType sprite);	
};

class Explosion : public Ohko{
	protected:	
		uint8_t explosionTimer;
		//change advance so the projectile goes away after explosionTimer 
		void advance();
	
	public:
		//call Ohko constructor in the 8 surrounding squares as well as this one. In this square, use big explosion sprite.
		//initialize
	  Explosion(uint8_t x, uint8_t y);
		//change collided so the projectile does not go away when collision happens
		void collided();
		//tick decrements explosionTimer;
		void tick();
		
};

class SmallExplosion : public Ohko{
	protected:	
		void advance();
	public:
		//call Ohko constructor in this square, use small explosion sprite
		SmallExplosion(uint8_t x, uint8_t y);
		//change collided so the projectile does not go away when collision happens
		void collided();
		
};

class Chomp : public Ohko{
	//I don't know if it needs to be different than Ohko or not
};

class Sun : public Projectile{
	protected:
                uint8_t distance;
		//change advance so if collision with cursor, collect sun
		void advance();
	public:
		//call projectile constructor, use sun sprite, sun sfx, speed as defined sun speed, collision = 0, damage = 0
		Sun(uint8_t x, uint8_t y, uint8_t isMoving);
		//change tick so it changes y pos, not x pos
		void tick();
};

class Zombie: public Entity{
	protected:
		SpriteType* walkFSM;	// Walk animation pointer
		SpriteType* eatFSM; // Eat animation pointer
		uint8_t speed;  // Speed of zombie
		uint8_t isEating;   // Is the zombie eating?
		
		// Advance to the next state of the entity
		void advance();
		
		// do attacking sequence if hostile
		void attack();
	public:
		// Constructor
		Zombie();

		// Constructor
		Zombie(SpriteType* sp, Sound* sfx, uint8_t xpos, uint8_t ypos, uint8_t hp, 
			uint8_t anim, uint8_t speed, uint8_t isEating);
};

// Regular zombie with a flag. Has random zombies in a wave formation following.
// Scene should have a generate function that generates wave
class FlagZombie: public Zombie{
	public:
		FlagZombie(uint8_t x, uint8_t y);
};

// Any zombie with extra health and headwear
class ArmorZombie: public Zombie{
	protected:
		SpriteType* fullWalkFSM;    // Headwear on, undamaged
		SpriteType* fullEatFSM;
		SpriteType* damagedWalkFSM; // Headwear on, damaged
		SpriteType* damagedEatFSM;
		// Redefine advance to change sprite at certain health
		void advance();
	public:
		// Constructor
		ArmorZombie(uint8_t x, uint8_t y);
};

// Conehead zombie. Only thing different is that it has a different sprite and different health.
class ConeZombie: public ArmorZombie{
	public:
		// Constructor
		ConeZombie(uint8_t x, uint8_t y);
};

// Buckethead zombie. Only thing different is that it has a different sprite and different health.
class BucketZombie: public ArmorZombie{
	public:
		// Constructor
		BucketZombie(uint8_t x, uint8_t y);
};

// Football zombie. Only thing different is that it has a different sprite, different health, and different speed.
class FootballZombie: public ArmorZombie{
	public:
		// Constructor
		FootballZombie(uint8_t x, uint8_t y);
};

// Newspaper zombie. Only thing different is that it has a different sprite, different health, and conditional speed.
class NewsZombie: public ArmorZombie{
	private:
		void advance();
	public:
		// Constructor
		NewsZombie(uint8_t x, uint8_t y);
};

// Jack in the box zombie. Blows up after certain amount of time.
class JackZombie: public Zombie{
	private:
		uint8_t delay;	// Delay for blowing up
		void advance();	// Make it blow up
	public:
		// Constructor
		JackZombie(uint8_t x, uint8_t y);
};

// Polevault zombie. Jumps over plants.
class PoleZombie: public Zombie{
	private:
		uint8_t hasPole;    // Does the zombie  have its pole?
		
		// Redefine advance with jumping logic
		void advance();

		// Redefine attack to jump over first plant
		void attack();
	public:
		// Constructor
		PoleZombie(uint8_t x, uint8_t y);
};



//Peashooter
//Repeater
//Snow pea
//Wall-nut
//Potato Mine
//Cherry Bomb
//Chomper
//Sunflower

class Plant : public Entity{
	protected:
		uint8_t attackRate;
		uint8_t attackTimer;
		Projectile projectile;
		//create projectile
		void attack();
		//idk why I put this here
		void advance();
	public:
		//call entity constructor with all but attackRate and projectile
		Plant(SpriteType* sp, Sound* sfx, uint8_t xpos, uint8_t ypos, uint8_t hp, 
					uint8_t anim, uint8_t hostile, uint8_t atkRt, Projectile proj);
		
		void takeDamage(uint8_t damage);
		//
		void tick();
		
};

class Peashooter : public Plant{
		
	protected: 
		//check if time>attackRate, then create pea if it is
		void advance();
	
	public:
		//constructor calls Plant constructor with defined peashooter sprite, 
		//defined peashooter sound, x and y arguments, defined generic plant health,
		//generic plant animation time, hostile = 1, atkRt as generic plant attack rate
		//and pea projectile
		Peashooter(uint8_t x, uint8_t y);
		//special constructor calls Plant constructor with Sprite argument rather than defined peashooter
		Peashooter(uint8_t x, uint8_t y, SpriteType* sp);
		//special constructer calls Plant constructor with Sprite argument and pea argument
		Peashooter(uint8_t x, uint8_t y, SpriteType* sp, Projectile p);
};
		
class Repeater : public Peashooter{
	protected:
		//time in between 1st and 2nd pea
		uint8_t repeatTime;
	public:
		//constructor calls peashooter constructor with x and y argument and defined repeater sprite, and sets repeatTime to defined repeater time
		Repeater(uint8_t x, uint8_t y);

};
class Snowpea : public Peashooter{
	public:
		//constructor calls peashooter constructor with x and y arguements, Snowpea sprite and snow pea projectile
		Snowpea(uint8_t x, uint8_t y);
};

class Wallnut : public Plant{
	protected:
		SpriteType* damagedWallnut;
		//new advance function will check health, and change sprite if it reaches defined threshold
		void advance();


	public:
		//constructor calls Plant constructor with defined Walnut sprite, 
		//null sount, x and y arguments, defined walnut health,
		//generic plant animation time (still need to render when it gets hurt), hostile = 0, atkRt as generic plant attack rate (doesn't matter)
		//and null projectile. Set damagedWallnut to the defined damagedWallnut sprite
		Wallnut(uint8_t x, uint8_t y);
};

class CherryBomb : public Plant{
	protected:
		//after attacking, destroy the cherry bomb. also, no range detection
		void advance();
	
	public:
			//constructor calls Plant constructor with defined cherry bomb sprite, explosion sound, x and y arguments, 
		  //defined generic plant health, generic plant animation time, hostile = 1, atkRt as generic plant attack rate maybe, 
			//explosion projectile
			CherryBomb(uint8_t x, uint8_t y);
};

class PotatoMine : public Plant{
	protected:
		SpriteType* growing; //this might not be implemented
		SpriteType* aboveGround;
		//can only attack if it's grown
		uint8_t grown; //1 or 0
		//after attacking, destroy the PotatoMine. cannot attack if underground (or transitioning). also, different range detection
		void advance();
	
	public:
			//constructor calls Plant constructor with defined potato mine sprite, explosion sound, x and y arguments,
			//defined generic plant health, generic plant animation time, hostile = 1, atkRt as more instantaneous
			//small explosion projectile
			//set growing and aboveGround to sprites
			//set grown to 0
			PotatoMine(uint8_t x, uint8_t y);
};

class Sunflower : public Plant{
		//to me, it makes sense for the sun to be a projectile, maybe just have sun incorporate different behavior
	protected:
		//no range detection
		void advance();
	public:
		//constructor calls Plant constructor with defined sunflower sprite, null or sun sound, x and y arguments
		//defined generic plant health, generic plant animation time, hostile = 1, atkRt as more slow
		//sun projectile (we can change it from a projectile if we want and just change up the attack function)
		Sunflower(uint8_t x, uint8_t y);
			
};

class Chomper : public Plant{
	protected:	
		SpriteType* full;	//for when zombie in mouth
		SpriteType* empty;	//for when no zombie in mouth
		uint8_t mouthFull; //1 or 0
		//change range detection to one tile
		void advance();
		//if no invisible hitbox projectile, we need to make attack do the collision
		void attack();
		
	public:
			//constructor calls Plant constructor with defined chomper empty sprite, defined chomp sound, x and y arguments
			//defined generic plant health, generic plant animation time, hostile = 1, atkRt as faster than standard
			//null projectile or invisible hitbox?
			//set full sprite to defined full chomper sprite, and empty to empty sprite
			//mouthFull = 0
		Chomper(uint8_t x, uint8_t y);
};


class Button : public GameObject{
	protected:
		//does nothing in Button
		void buttonFunction();
	public:
		//calls gameobject with parameters as passed in
		Button(uint8_t x, uint8_t y, SpriteType* sprite, Sound* sfx);
		//plays sound, calls buttonFunction
		void buttonHit();

};

class MenuButton : public Button{
	protected:
		SpriteType* english;
		SpriteType* espanol;
		void advance();
	public:
		//calls Button constructor with parameters as passed in, but set to menu button sound
		MenuButton(uint8_t x, uint8_t y, SpriteType* eng, SpriteType* esp);
		//change advance so it checks what the global language (main.cpp) and changes the current sprite and set redraw to 1
		
};	

class VsButton : public MenuButton{
	protected:
		//change buttonFunction to load VS scene
		void buttonFunction();
	public:
		//calls MenuButton constructor with parameters as passed in, but set to VS button sprite
		VsButton(uint8_t x, uint8_t y);
		
};

class CampaignButton : public MenuButton{
	protected:
		//change buttonFunction to load Campaign scene
		void buttonFunction();
	public:
		//calls MenuButton constructor with parameters as passed in, but set to Campaign button sprites
		CampaignButton(uint8_t x, uint8_t y);
		
};

class LanguageButton : public MenuButton{
	protected:
		//change buttonFunction will toggle global variable language from 0 to 1 or 1 to 0
		void buttonFunction();
	public:
		//calls MenuButton constructor with parameters as passed in, but set to language button sprites
	  LanguageButton(uint8_t x, uint8_t y);
		
};

class SeedPacket : public Button{
	protected:
    uint8_t isReady; // Tracks if ready to be planted
		SpriteType* ready;
		SpriteType* gray;
		uint8_t loadTime;
		uint8_t loadTimer;
		uint8_t plantID; //0 for peashooter, 1 for repeater, 2 for snowpea, 3 for wallnut, 4 for cherry bomb, 5 for mine, 6 for chomper, 7 for sunflower
		//change buttonFunction to call global spawn plant with plantID
		void buttonFunction();
		//change advance to check if seed is ready
		void advance();
	public:
		//calls Button constructor with parameters, but use planting sound,  and defined seed packet sprite/plantname from scene model
		SeedPacket(uint8_t x, uint8_t y, uint8_t plantID, uint8_t loadTime);	
		void buttonHit();
		//tick decrements the load timer
		void tick();
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
		void refresh();
		uint8_t getLength();
		//will tick every existing member of objects
		void tick();
};
// Collection of all game objects, background, music, etc. pertinent to the current area of the game
class Scene{
	private:
		GameObjectList* Zombies;	// List of all objects on the scene these are arrays of pointers
		GameObjectList* Plants;
		GameObjectList* Buttons;
		GameObjectList* Lawnmowers;
		GameObjectList* Projectiles;
		const uint16_t* backgroundBMP;	// Background of the scene as a bitmap
		uint8_t sunRate;
		uint8_t sunTimer;
		Sound* music;
	public:
		// Constructor
		Scene(GameObjectList* but, GameObjectList* lwm, const uint16_t* bg, Sound* msc);
		// Destructor
		//~Scene();
	
		// Copy Constructor
		Scene(const Scene& other);
	
		// Assignment Operator
		Scene operator=(Scene& other);
		const uint16_t* retBG();
	  void refresh();
		void collisions();
		void tick();
};

// Basic zombie.



//bitmaps
extern uint16_t menuBackground[1];
extern uint16_t lawnBackground[1];
	


//button sprites
extern SpriteType* vsEnglish;
extern SpriteType* vsSpanish;
extern SpriteType* campaignEnglish;
extern SpriteType* campaignSpanish;
extern SpriteType* languageEnglish;
extern SpriteType* languageSpanish;

extern SpriteType* p0; extern SpriteType* p0g;
extern SpriteType* p1; extern SpriteType* p1g;
extern SpriteType* p2; extern SpriteType* p2g;
extern SpriteType* p3; extern SpriteType* p3g;
extern SpriteType* p4; extern SpriteType* p4g;
extern SpriteType* p5; extern SpriteType* p5g;
extern SpriteType* p6; extern SpriteType* p6g;
extern SpriteType* p7; extern SpriteType* p7g;

extern SpriteType* seedPacketSprites[8];
extern SpriteType* seedPacketGraySprites[8];

//button sounds
extern Sound* menuSound;
extern Sound* plantingSound;

//language
extern uint8_t lang; //0 = eng, 1 = esp	
	

//sfx/music filenames or whatever we are doing
//TO-DO

//LawnMower things
extern SpriteType* lmSprite;
extern Sound* lmSound;

//Buttons
extern CampaignButton* singlePlayer;
extern VsButton* multiPlayer;
extern LanguageButton* language;

extern SeedPacket* peaSeed;
extern SeedPacket* sunSeed;
extern SeedPacket* snowSeed;
extern SeedPacket* repSeed;
extern SeedPacket* chompSeed;
extern SeedPacket* mineSeed;
extern SeedPacket* bombSeed;
extern SeedPacket* wallSeed;
	
//LawnMowers
extern LawnMower* LM1;
extern LawnMower* LM2;
extern LawnMower* LM3;
extern LawnMower* LM4;
extern LawnMower* LM5;


//Music classes
extern Sound* menuMusic;
extern Sound* levelMusic;
//TO-DO

//Game Object lists
extern GameObject* btnArr1[4];
extern GameObject* btnArr2[8];
//GameObject* btnArr3[16] = {};

extern GameObject* lmwArr[5];

extern GameObjectList* menuButtons;
extern GameObjectList* singlePlayerButtons;
//GameObjectList* multiPlayerButtons = new GameObjectList(btnArr3);

extern GameObjectList* lawnMowers;

//scenes
extern Scene* menu;
extern Scene* campaign;
//Scene* vsmode = new Scene(multiPlayerButtons, lawnMowers, lawnBackground, levelMusic);

extern Sound* s;
extern uint8_t soundBuffer[24000];
extern Scene* currentScene;

//global functions
void loadScene(Scene* s);

#endif

