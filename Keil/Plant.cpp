#include "Game.h"

Plant::Plant(SpriteType* sp, uint8_t xpos, uint8_t ypos, uint8_t hp, 
uint8_t hostile, uint8_t atkRt, uint8_t projID, uint8_t lane) : Entity(sp, biteSound, xpos, ypos, hp, DefaultPlantAnimationRate, hostile, lane){
	this->attackRate = atkRt;
	this->attackTimer = atkRt;
	this->projID = projID;
}

void Plant::tick(){
	Entity::tick();
	if(this->attackTimer>0) this->attackTimer--;
}

void Plant::advance(){
	Entity::advance();
	if(hostile && this->attackTimer == 0) {
		this->attack();
		this->attackTimer = this->attackRate;
	}
}

void Plant::setCol(uint8_t col){
	this->col = col;
}
uint8_t Plant::getCol(){
	return this->col;
}

void Plant::attack(){
	currentScene->spawnProjectile(this->projID, this->x + shootOffsetX, this->y + shootOffsetY, this->lane);
}

void Plant::hurt(uint8_t damage){
	Entity::hurt(damage);
	this->soundFX->play();
}

Peashooter::Peashooter(uint8_t x, uint8_t y, uint8_t lane)
: Plant(peashooterSprite, x, y, defaultPlantHealth, 1, PlantAttackRate, peaID, lane)
{
	
}
Peashooter::Peashooter(uint8_t x, uint8_t y, uint8_t lane, SpriteType* sp)
: Plant(sp, x, y, defaultPlantHealth, 1, PlantAttackRate, peaID, lane)
{
	
}
Peashooter::Peashooter(uint8_t x, uint8_t y, uint8_t lane, SpriteType* sp, uint8_t projID)
: Plant(sp, x, y, defaultPlantHealth, 1, PlantAttackRate, projID, lane)
{
	
}

Repeater::Repeater(uint8_t x, uint8_t y, uint8_t lane) : Peashooter(x, y, lane, repeaterSprite){
	this->repeatTimer = this->attackRate + repeaterRepeatTicks;
}

void Repeater::advance(){
	Plant::advance();
	if(this->repeatTimer == 0) {
		this->attack();
		this->repeatTimer = this->attackRate;
	}
}

void Repeater::tick(){
	Plant::tick();
	if(this->repeatTimer>0)
	{this->repeatTimer--;}
}

Snowpea::Snowpea(uint8_t x, uint8_t y, uint8_t lane) : Peashooter(x, y, lane, snowPeaSprite, frozenPeaID){

}

Wallnut::Wallnut(uint8_t x, uint8_t y, uint8_t lane)
:	Plant(wallNutSprite, x, y, wallNutHealth, 0, 0, 0, lane){
	
}
void Wallnut::advance(){
	Plant::advance();
	if(this->health < wallNutHealth/2){
		this->previousSprite = this->sprite;
		this->sprite = wallNutDamagedSprite;
	}
}
void Wallnut::tick(){
	Plant::tick();
}
CherryBomb::CherryBomb(uint8_t x, uint8_t y, uint8_t lane)
: Plant(cherryBombSprite, x, y, defaultPlantHealth,
	1, DefaultPlantAnimationRate*3, explosionID, lane)
{
	
}


void CherryBomb::attack(){
	Plant::attack();
	this->unrender();
	currentScene->planter->emptyGrid(this->col, this->lane);
	currentScene->Plants->tryRmv(this);
	//TO-DO: have the grid set this plant's spot to 0
	//currentScene->planter->g
}

PotatoMine::PotatoMine(uint8_t x, uint8_t y, uint8_t lane)
: Plant(potatoMineSprite, x, y, defaultPlantHealth,
1, potatoMineSurfaceTime, smallExplosionID, lane)
{
	this->aboveGround = potatoMineReadySprite;
	this->grown = 0;
}
void PotatoMine::attack(){
	if(this->grown == 0){
		this->previousSprite = sprite;
		this->sprite = this->aboveGround;
		this->redraw = 1;
		this->grown = 1;
	}
}
void PotatoMine::hurt(uint8_t dam){
	if(this->grown == 1){
		Plant::attack();
		this->unrender();
		currentScene->planter->emptyGrid(this->col, this->lane);
		currentScene->Plants->tryRmv(this);
	}
	else{
		Plant::hurt(dam);
	}
}

Sunflower::Sunflower(uint8_t x, uint8_t y, uint8_t lane)
: Plant (sunflowerSprite, x, y, defaultPlantHealth, 1, 
sunProductionRate, sunID, lane)
{
	
}

Chomper::Chomper(uint8_t x, uint8_t y, uint8_t lane)
: Plant (chomperSprite, x, y, defaultPlantHealth, 1, 
chomperChewTime, sunID, lane){
	this->empty = chomperSprite;
	this->bite = chomperAttackSprite;
	this->mouthFull = 0;
}
void Chomper::advance(){
	Entity::advance();
	if(attackTimer == 0 && this->mouthFull == 1){
		this->previousSprite = this->sprite;
		this->sprite = this->empty;
		this->redraw = 1;
		this->mouthFull = 0;
	}
}
void Chomper::attack(){
	//do nothing ig
}
void Chomper::hurt(uint8_t dam){
	if(mouthFull == 0) {
		this->previousSprite = this->sprite;
		this->animationTimer = 3*this->animationTime;
		this->sprite = this->bite;
		this->redraw = 1;
		this->attack();
		this->attackTimer = this->attackRate;
		this->mouthFull = 1;
	}
	else Plant::hurt(dam);
}
