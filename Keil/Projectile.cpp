#include "Game.h"

Projectile::Projectile(SpriteType* sp, Sound* sfx, uint8_t xpos, uint8_t ypos, uint8_t spd, uint8_t dam, uint8_t lane)
	:	GameObject(sp, sfx, xpos, ypos, lane){
	this->speed = spd;
	this->damage = dam;
	this->collision = 0;
	this->distanceDiff = 0;
}
	
void Projectile::advance(){
	if(this->distanceDiff!=0){
		this->x += this->distanceDiff;
		this->distanceDiff = 0;
		this->redraw = 1;
	}
	if(this->getX()>170){
		this->collided();
	}
}
void Projectile::tick(){
	this->distanceDiff+=this->speed;
}
int Projectile::collided(){
	this->unrender();
	currentScene->Projectiles->tryRmv(this);
	this->soundFX->play();
	return 1;
}
LawnMower::LawnMower(uint8_t x, uint8_t y, uint8_t lane) : Projectile(lmSprite, lmSound, x, y, lawnmowerSpeed, lawnmowerDamage, lane) {
	this->isMoving  = 0;
}
void LawnMower::advance(){
	if(this->x >170 && this->x < 250){
		this->unrender();
		currentScene->Projectiles->tryRmv(this);
		return;
	}
	else if(this->distanceDiff!=0){
		this->x += this->distanceDiff;
		this->distanceDiff = 0;
		this->redraw = 1;
	}
}
int LawnMower::collided(){
	return 0;
}
void LawnMower::tick(){
	if(this->isMoving == 1) this->distanceDiff += this->speed;
}
void LawnMower::reset(){
	this->x = 0;
	this->isMoving = 0;
}

Pea::Pea(uint8_t x, uint8_t y, uint8_t lane) : Projectile(peaSprite, peaSound, x, y, peaSpeed, peaDamage, lane){
	
}

FrozenPea::FrozenPea(uint8_t x, uint8_t y, uint8_t lane) : Projectile(frozenPeaSprite, peaSound, x, y, icePeaSpeed, peaDamage, lane){

}

Ohko::Ohko(uint8_t x, uint8_t y, uint8_t lane) : Projectile(transparentSprite, peaSound, x, y, 0, ohkoDamage, lane){
	
}

Ohko::Ohko(uint8_t x, uint8_t y, uint8_t lane, SpriteType* sprite, Sound* sound) : Projectile(sprite, sound, x, y, 0, ohkoDamage, lane){

}

Explosion::Explosion(uint8_t x, uint8_t y, uint8_t lane) : Ohko(x, y, lane, largeExplosionSprite, explosionSound){
	currentScene->spawnProjectile(ohkoID, x + gridX, y + gridY, lane + 1);
	currentScene->spawnProjectile(ohkoID, x + gridX, y, lane);
	currentScene->spawnProjectile(ohkoID, x + gridX, y - gridY, lane - 1);
	currentScene->spawnProjectile(ohkoID, x, y - gridY, lane - 1);
	currentScene->spawnProjectile(ohkoID, x - gridX, y - gridY, lane - 1);
	currentScene->spawnProjectile(ohkoID, x - gridX, y, lane);
	currentScene->spawnProjectile(ohkoID, x - gridX, y + gridY, lane + 1);
	currentScene->spawnProjectile(ohkoID, x, y + gridY, lane + 1);
	this->explosionTimer = ExplosionTime;
	this->soundFX->play();
}

void Explosion::render(){
	Display_RenderSprite(this->x - 20, this->y - 20, this->sprite->bmp, this->sprite->width, this->sprite->length, transparentColor, currentScene->retBG());
}
void Explosion::unrender(){
	Display_UnrenderSprite(this->oldx - 20, this->oldy - 20, this->previousSprite->width, this->previousSprite->length, currentScene->retBG());
}

void Explosion::advance(){
	if(this->explosionTimer == 0){
		this->unrender();
		currentScene->Projectiles->tryRmv(this);
	}
}

int Explosion::collided(){
	return 0;
}
void Explosion::tick(){
	this->explosionTimer--;
}

SmallExplosion::SmallExplosion(uint8_t x, uint8_t y, uint8_t lane) : Ohko(x, y, lane, smallExplosionSprite, explosionSound){
	this->explosionTimer = ExplosionTime;
	this->soundFX->play();
}
int SmallExplosion::collided(){
	return 0;
}
void SmallExplosion::advance(){
	if(this->explosionTimer == 0){
		this->unrender();
		currentScene->Projectiles->tryRmv(this);
	}
}

void SmallExplosion::tick(){
	this->explosionTimer--;
}

Chomp::Chomp(uint8_t x, uint8_t y, uint8_t lane): Ohko(x, y, lane, transparentSprite, chompSound){
this->soundFX->play();
}

Sun::Sun(uint8_t x, uint8_t y, uint8_t isMoving) : Projectile(sunSprite, sunSound, x, y, sunSpeed, 0, 6){
	this->distance = 80;
	this->isMoving = !isMoving;	//input is flipped
	this->upTimer = sunProductionRate;
}

void Sun::advance(){
	if(currentScene->cursorHit(this->x, this->y)){
		currentScene->changeSun(25);
		this->unrender();
		this->soundFX->play();
		currentScene->Projectiles->tryRmv(this);
		return;
	}
	else if(this->upTimer == 0){
		this->unrender();
		currentScene->Projectiles->tryRmv(this);
		return;
	}
	else if(this->distanceDiff!=0){
		this->y -= this->distanceDiff;
		this->distanceDiff = 0;
		this->redraw = 1;
	}
}
int collided(){
	return 0;
}
void Sun::tick(){
	if(this->isMoving && distance >= 0){
		this->distanceDiff += this->speed;
		this->distance -= this->speed;
	}
	this->upTimer--;
}
