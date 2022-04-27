#ifndef BUTTON_H
#define BUTTON_H

#include <stdint.h>
#include "Game.h"

class Button : public GameObject{
	protected:
		//does nothing in Button
		void buttonFunction();
	public:
		//calls gameobject with parameters as passed in
		Button(uint8_t x, uint8_t y, SpriteType* sprite, Sound sfx);
		//plays sound, calls buttonFunction
		void buttonHit();

};

class MenuButton : public Button{
	protected:
		SpriteType* english;
		SpriteType* espanol;
	
	public:
		//calls Button constructor with parameters as passed in, but set to menu button sound
		MenuButton(uint8_t x, uint8_t y, SpriteType* eng, SpriteType* esp);
		//change advance so it checks what the global language (main.cpp) and changes the current sprite and set redraw to 1
};	

class VsButton : public MenuButton{
	protected:
		//change buttonFunction to load VS scene
		void buttonFuntion();
	public:
		//calls MenuButton constructor with parameters as passed in, but set to VS button sprite
		VsButton(uint8_t x, uint8_t y);
		
};

class CampaignButton : public MenuButton{
	protected:
		//change buttonFunction to load Campaign scene
		void buttonFunction();
	public:
		//calls MenuButton constructor with parameters as passed in, but set to Campaign button sprites
		CampaignButton(uint8_t x, uint8_t y);
		
};

class LanguageButton : public MenuButton{
	protected:
		//change buttonFunction will toggle global variable language from 0 to 1 or 1 to 0
		void buttonFunction();
	public:
		//calls MenuButton constructor with parameters as passed in, but set to language button sprites
	  LanguageButton(uint8_t x, uint8_t y);
		
};

class SeedPacket : public Button{
	protected:
                uint8_t isReady; // Tracks if ready to be planted
		SpriteType* ready;
		SpriteType* gray;
		uint8_t loadTime;
		uint8_t plantID; //0 for peashooter, 1 for repeater, 2 for snowpea, 3 for wallnut, 4 for cherry bomb, 5 for mine, 6 for chomper, 7 for sunflower
		//change buttonFunction to call global spawn plant with plantID
		void buttonFunction();
	public:
		//calls Button constructor with parameters, but use planting sound,  and defined seed packet sprite/plantname from scene model
		SeedPacket(uint8_t x, uint8_t y);
		
		
	
};
#endif