#ifndef ZOMBIE_H
#define ZOMBIE_H

#include <stdint.h>
#include "Game.h"

// Basic zombie.
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


#endif
