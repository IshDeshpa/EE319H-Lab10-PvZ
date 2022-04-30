#include "Game.h"

Zombie::Zombie(SpriteType* sp, uint8_t xpos, uint8_t ypos, uint8_t hp, 
			uint8_t anim, uint8_t speed, uint8_t lane)
			: Entity(sp, brainsSound, xpos, ypos, hp, anim, 1, lane)
{
	this->speed = speed;
	this->isEating = 0;
	this->wasEating = 0;
	this->damageTimer = 0;
	this->damageTime = ZombieAttackRate;
	this->walkFSM = regularZombieSprite;
	this->eatFSM = regularZombieEatSprite;
	
}

Zombie::Zombie(SpriteType* sp, uint8_t xpos, uint8_t ypos, uint8_t hp, 
			 uint8_t speed, uint8_t lane)
			: Entity(sp, brainsSound, xpos, ypos, hp, DefaultZombieAnimationRate, 1, lane)
{
	this->speed = speed;
	this->isEating = 0;
	this->wasEating = 0;
	this->damageTimer = 0;
	this->damageTime = ZombieAttackRate;
	this->walkFSM = regularZombieSprite;
	this->eatFSM = regularZombieEatSprite;
	
}
	
Zombie::Zombie(uint8_t xpos, uint8_t ypos, uint8_t lane)
			: Entity(regularZombieSprite, brainsSound, xpos, ypos, defaultZombieHealth, DefaultZombieAnimationRate, 1, lane){
	this->speed = zombieSpeed;
	this->isEating = 0;
	this->wasEating = 0;
	this->damageTimer = 0;
	this->damageTime = ZombieAttackRate;
	this->walkFSM = regularZombieSprite;
	this->eatFSM = regularZombieEatSprite;
}

void Zombie::advance(){
	Entity::advance();
	if(this->distanceDiff > 0){
		this->oldx = this->x;
		this->x -= this->distanceDiff;
		this->distanceDiff = 0;
		this->redraw = 1;
	}
	if(this->isEating && !this->wasEating){
		this->sprite = this->eatFSM;
		this->redraw = 1;
	}
	else if(this->wasEating && !this->isEating){
		this->sprite = this->walkFSM;
		this->redraw = 1;
	}
	if(Random32() % 100 == 0) this->soundFX->play();
	
}

void Zombie::stopEating(){
	this->wasEating = this->isEating;
	this->isEating = 0;
}

void Zombie::attack(Plant* plt){
	this->wasEating = this->isEating;
	this->isEating = 1;
	if(this->damageTimer == 0)
	{
		plt->hurt(zombieDamage);
		this->damageTimer = this->damageTime;
	}
}

void Zombie::tick(){
	Entity::tick();
	if(this->damageTimer>0)this->damageTimer--;
	if(!this->isEating && Random32() % (this->speed+1) > 0)this->distanceDiff++;
}

FlagZombie::FlagZombie(uint8_t x, uint8_t y, uint8_t lane)
: Zombie(x, y, lane){
	
}

ConeZombie::ConeZombie(uint8_t x, uint8_t y, uint8_t lane)
: ArmorZombie(x, y, lane){
	
}

NewsZombie::NewsZombie(uint8_t x, uint8_t y, uint8_t lane)
: ArmorZombie(x, y, lane){
	
}

PoleZombie::PoleZombie(uint8_t x, uint8_t y, uint8_t lane)
: Zombie(x, y, lane){
	
}

BucketZombie::BucketZombie(uint8_t x, uint8_t y, uint8_t lane)
: ArmorZombie(x, y, lane){
	
}

FootballZombie::FootballZombie(uint8_t x, uint8_t y, uint8_t lane)
: ArmorZombie(x, y, lane){
	
}

JackZombie::JackZombie(uint8_t x, uint8_t y, uint8_t lane)
: Zombie(x, y, lane){
	
}