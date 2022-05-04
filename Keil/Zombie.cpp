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
	if(Random32() % 1000000 == 0) 
		this->soundFX->play();
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
	if(!this->isEating && Random32() % (7 - this->speed) == 0)this->distanceDiff++;
}
void Zombie::takeDamage(uint8_t dam){
	this->health -= dam;
}

FlagZombie::FlagZombie(uint8_t x, uint8_t y, uint8_t lane)
: Zombie(x, y, lane){
	this->sprite = flagZombieSprite;
	this->eatFSM = flagZombieEatSprite;
	this->walkFSM = flagZombieSprite;
	this->spawnDelay = 20;
	this->spawnTimer = 20;
	this->numSpawn = bigWaveSize;
}

void FlagZombie::tick(){
	Zombie::tick();
	if(this->spawnTimer > 0)this->spawnTimer--;
}
void FlagZombie::advance(){
	Zombie::advance();
	if(this->spawnTimer == 0)
	{
		currentScene->spawnZombie(Random32()%7, Random32()%5 + 1);
	}
}

ArmorZombie::ArmorZombie(uint8_t x, uint8_t y, uint8_t lane, SpriteType* fullWalk, SpriteType* fullEat)
: Zombie(x, y, lane){
	this->sprite = fullWalk;
	this->fullEatFSM = fullEat;
	this->fullWalkFSM = fullWalk;
}
void ArmorZombie::advance(){
	if(this->health  > defaultZombieHealth){
			Entity::advance();
		if(this->distanceDiff > 0){
			this->oldx = this->x;
			this->x -= this->distanceDiff;
			this->distanceDiff = 0;
			this->redraw = 1;
		}
		if(this->isEating && !this->wasEating){
			this->sprite = this->fullEatFSM;
			this->redraw = 1;
		}
		else if(this->wasEating && !this->isEating){
			this->sprite = this->fullWalkFSM;
			this->redraw = 1;
		}
		if(Random32() % 1000000 == 0) this->soundFX->play();
	}
	else{
		if(this->sprite == this->fullEatFSM){
			this->previousSprite = this->sprite;
			this->sprite = this->eatFSM;
			this->redraw = 1;
		}
		if(this->sprite == this->fullWalkFSM){
			this->previousSprite = this->sprite;
			this->sprite = this->walkFSM;
			this->redraw = 1;
		}
		Zombie::advance();
	}
}
ConeZombie::ConeZombie(uint8_t x, uint8_t y, uint8_t lane)
: ArmorZombie(x, y, lane, coneZombieSprite, coneZombieEatSprite){
	this->health = defaultZombieHealth + coneHealth;
}

NewsZombie::NewsZombie(uint8_t x, uint8_t y, uint8_t lane)
: ArmorZombie(x, y, lane, newspaperZombieSprite, newspaperZombieEatSprite){
	this->health = defaultZombieHealth + newspaperHealth;
}
void NewsZombie::advance(){
	ArmorZombie::advance();
	if(this->health <= defaultZombieHealth){
		this->speed = newspaperAngrySpeed;
	}
}

PoleZombie::PoleZombie(uint8_t x, uint8_t y, uint8_t lane)
: Zombie(x, y, lane){
	this->hasPole = 1;
	this->hadPole = 1;
	this->speed = poleVaultSpeed;
	this->sprite = polevaultZombieRunSprite;
	this->walkFSM = polevaultZombieWalkSprite;
	this->eatFSM = polevaultZombieEatSprite;
	this->jumpSprite = polevaultZombieJumpSprite;
	this->animationTime = PoleVaultAnimationRate;
	this->animationTimer = PoleVaultAnimationRate;
}


void PoleZombie::attack(Plant* plt){
	if(this->hasPole){
		this->previousSprite = this->sprite;
		this->sprite = polevaultZombieJumpSprite;
		this->distanceDiff += 30;
		this->hasPole = 0;
		redraw = 1;
	}
	else{
		Zombie::attack(plt);
	}
}

BucketZombie::BucketZombie(uint8_t x, uint8_t y, uint8_t lane)
: ArmorZombie(x, y, lane, bucketZombieSprite, bucketZombieEatSprite){
	this->health = defaultZombieHealth + bucketHealth;
}

FootballZombie::FootballZombie(uint8_t x, uint8_t y, uint8_t lane)
: ArmorZombie(x, y, lane, footballZombieSprite, footballZombieEatSprite){
	this->speed = footballSpeed;
	this->health = defaultZombieHealth + helmetHealth;
}

JackZombie::JackZombie(uint8_t x, uint8_t y, uint8_t lane)
: Zombie(x, y, lane){
	this->speed = jackZombieSpeed;
	this->walkFSM = jackZombieSprite;
	this->sprite = walkFSM;
}

void JackZombie::attack(Plant* plt){
	this->unrender(); 
	currentScene->spawnProjectile(explosionID, this->x, this->y, this->lane);
	this->health = 0;
	
}
