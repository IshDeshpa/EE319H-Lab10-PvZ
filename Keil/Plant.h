#ifndef PLANT_H
#define PLANT_H

#include <stdint.h>
#include "Game.h"

class Plant : public Entity{
	protected:
		uint8_t attackRate;
		Projectile projectile;
		void advance();
};

#endif
