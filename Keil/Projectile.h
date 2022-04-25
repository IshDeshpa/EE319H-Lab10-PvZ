#ifndef PROJECTILE_H
#define PROJECTILE_H

#include <stdint.h>
#include "Game.h"

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
		
};

class SmallExplosion : public Ohko{
	protected:	
		uint8_t explosionTimer;
		//change advance so the projectile goes away after explosionTimer 
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
};

#endif
