#ifndef PLANT_H
#define PLANT_H

#include <stdint.h>
#include "Game.h"

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
		Projectile projectile;
		void advance();
	public:
		//call entity constructor with all but attackRate and projectile
		Plant(SpriteType* sp, Sound* sfx, uint8_t xpos, uint8_t ypos, uint8_t hp, 
					uint8_t anim, uint8_t hostile, uint8_t atkRt, Projectile proj);
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
};
		
class Repeater : public Peashooter{
	protected:
		//time in between 1st and 2nd pea
		uint8_t repeatTime;
	public:
		//constructor calls peashooter constructor with x and y arguments, and sets repeatTime to defined repeater time
		Repeater(uint8_t x, uint8_t y);

};
class Snowpea : public Peashooter{
	public:
		//constructor calls peashooter constructor with x and y arguements, change projectile to snow pea, change sprite to Snowpea
		Snowpea(uint8_t x, uint8_t y);
};

#endif
