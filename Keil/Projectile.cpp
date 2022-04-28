#include "Game.h"

LawnMower::LawnMower(uint8_t x, uint8_t y, uint8_t lane) : Projectile(lmSprite, lmSound, x, y, lawnmowerSpeed, lawnmowerDamage, lane) {
	this->isMoving  = 0;
}
void LawnMower::advance(){
	if(this->distanceDiff!=0){
		this->x += this->distanceDiff;
		this->distanceDiff = 0;
		this->redraw = 1;
	}
	this->collided();
}
int LawnMower::collided(){
	if(this->x > 170){
		this->unrender();
		return 1;
	}
	return 0;
}
void LawnMower::tick(){
	if(this->isMoving == 1) this->distanceDiff += this->speed;
}
void LawnMower::reset(){
	this->x = 0;
	this->isMoving = 0;
}
