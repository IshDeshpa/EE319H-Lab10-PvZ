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
	
};

class Peashooter : public Plant{
		
	protected: 
		//check if time>attackRate, then create pea if it is
		void advance();
	
	public:
		//Initializes with correct projectile, hostile = 1, attackrate
		Peashooter();
};
		

#endif
