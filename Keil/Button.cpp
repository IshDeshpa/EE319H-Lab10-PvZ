#include "Game.h"

Button::Button(uint8_t x, uint8_t y, SpriteType* sprite, Sound* sfx) : GameObject(sprite, sfx, x, y){
	
}
void Button::buttonHit(){
	this->soundFX->play();
	this->buttonFunction();
}
void Button::buttonFunction(){}
	
MenuButton::MenuButton(uint8_t x, uint8_t y, SpriteType* eng, SpriteType* esp) : Button(x, y, eng, menuSound){
	this->english = eng;
	this->espanol = esp;
}	

void MenuButton::advance(){
	if(lang && (this->sprite == this->english)){
		this->sprite = this->espanol;
		this->redraw = 1;
	}
	else if(!lang && (this->sprite == this->espanol)){
		this->sprite = this->english;
		this->redraw = 1;
	}
}

VsButton::VsButton(uint8_t x, uint8_t y) : MenuButton(x, y, vsEnglish, vsSpanish){}
	
void VsButton::buttonFunction(){
	//loadScene(vsmode);
}

CampaignButton::CampaignButton(uint8_t x, uint8_t y) : MenuButton(x, y, campaignEnglish, campaignSpanish){}
	
void CampaignButton::buttonFunction(){
	loadScene(campaign);
}

LanguageButton::LanguageButton(uint8_t x, uint8_t y) : MenuButton(x, y, languageEnglish, languageSpanish){}
void LanguageButton::buttonFunction(){
	lang ^= lang;
}

SeedPacket::SeedPacket(uint8_t x, uint8_t y, uint8_t plantID, uint8_t loadTime, uint8_t sunCost) : Button(x, y, seedPacketSprites[plantID], plantingSound){
	this->ready = seedPacketSprites[plantID];
	this->gray = seedPacketGraySprites[plantID];
	this->loadTime = loadTime;
	this->loadTimer = 0;
	this->isReady = 1;
	this->plantID = plantID;
	this->sunCost = sunCost;
}

void SeedPacket::buttonFunction(){
	
		this->loadTimer = loadTime;
		this->sprite = gray;
		this->isReady = 0;
		currentScene->spawnPlant(plantID);
}
void SeedPacket::buttonHit(){
	if(this->isReady == 1 && currentScene->gridCheck() == 0 
		&& currentScene->changeSun(this->sunCost)){
		buttonFunction();
		this->soundFX->play();
		this->isReady = 0;
	}
}
void SeedPacket::tick(){
	if(this->loadTimer != 0)this->loadTimer--;
}
void SeedPacket::advance(){
	if(this->loadTimer == 0 && this->isReady == 0){
		this->isReady = 1;
		this->sprite = ready;
		this->redraw = 1;	
	}
}


