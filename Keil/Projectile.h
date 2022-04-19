#ifndef PROJECTILE_H
#define PROJECTILE_H

#include <stdint.h>
#include "Game.h"

class Pea : public Projectile{
	public:
		//set entity variables to defined pea damage, speed, sprite, collision = 1
		Pea();
};

class Snowpea : public Projectile{
	public:
		//set entity variables to defined Snowpea damage, sprite, and pea speed collision = 1
};
