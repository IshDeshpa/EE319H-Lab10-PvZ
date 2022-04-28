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
	
	//TO-DO: find a way to load the scene from here	w/o including main
void CampaignButton::buttonFunction(){
	//loadScene(campaign);
}

LanguageButton::LanguageButton(uint8_t x, uint8_t y) : MenuButton(x, y, languageEnglish, languageSpanish){}
void LanguageButton::buttonFunction(){
	lang ^= lang;
}

SeedPacket::SeedPacket(uint8_t x, uint8_t y, uint8_t plantID, uint8_t loadTime) : Button(x, y, seedPacketSprites[plantID], plantingSound){
	this->ready = seedPacketSprites[plantID];
	this->gray = seedPacketGraySprites[plantID];
	this->loadTime = loadTime;
	this->loadTimer = 0;
	this->isReady = 1;
}
//TO-DO: find a way to spawn a plant from here
void SeedPacket::buttonFunction(){
	this->loadTimer = loadTime;
	this->sprite = gray;	
	//SpawnPlant(plantID);
}
void SeedPacket::buttonHit(){
	if(this->isReady == 1){
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


