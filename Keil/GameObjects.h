#ifndef GAMEOBJECTS_H
#define GAMEOBJECTS_H

#include "Game.h"
#include "Plant.h"
#include "Projectile.h"
#include "Zombie.h"
#include "Button.h"


//macros
#define CmpButtonXpos 10
#define CmpButtonYpos 50
#define VsButtonXpos 90
#define VsButtonYpos 50
#define LangButtonXpos 80
#define LangButtonYpos 10

#define SpXSize
#define SpYSize
#define SpOffset SpXSize+1
#define SpYpos 115
#define FspXpos 1
#define SspXpos FspXpos + SpOffset * 1
#define TspXpos FspXpos + SpOffset * 2
#define	FospXpos FspXpos + SpOffset * 3
#define FispXpos FspXpos + SpOffset * 4
#define SispXpos FspXpos + SpOffset * 5
#define SespXpos FspXpos + SpOffset * 6
#define EspXpos FspXpos + SpOffset * 7

#define LaneYOffset = 20 //change later
#define Lane1Ypos 10
#define Lane2Ypos Lane1Ypos + LaneYOffset
#define Lane3Ypos Lane1Ypos + LaneYOffset
#define Lane4Ypos Lane1Ypos + LaneYOffset
#define Lane5Ypos Lane1Ypos + LaneYOffset

//bitmaps
uint16_t menuBackground[] = {};
uint16_t lawnBackground[] = {};
	
//sfx/music filenames or whatever we are doing
//TO-DO
	
//Buttons
CampaignButton* singlePlayer = new CampaignButton(CmpButtonXpos, CmpButtonYpos);
VsButton* multiPlayer = new VsButton(VsButtonXpos, VsButtonYpos);
LanguageButton* language = new LanguageButton(LangButtonXpos, LangButtonYpos);

SeedPacket* peaSeed = new SeedPacket(FspXpos, SpYpos);
SeedPacket* sunSeed = new SeedPacket(SspXpos, SpYpos);
SeedPacket* snowSeed = new SeedPacket(TspXpos, SpYpos);
SeedPacket* repSeed = new SeedPacket(FspXpos, SpYpos);
SeedPacket* chompSeed = new SeedPacket(FispXpos, SpYpos);
SeedPacket* mineSeed = new SeedPacket(SispXpos, SpYpos);
SeedPacket* bombSeed = new SeedPacket(SespXpos, SpYpos);
SeedPacket* wallSeed = new SeedPacket(EspXpos, SpYpos);
	
//LawnMowers
LawnMower* LM1 = new LawnMower(0, Lane1Ypos);
LawnMower* LM2 = new LawnMower(0, Lane1Ypos);
LawnMower* LM3 = new LawnMower(0, Lane1Ypos);
LawnMower* LM4 = new LawnMower(0, Lane1Ypos);
LawnMower* LM5 = new LawnMower(0, Lane1Ypos);

//Music classes
Sound* menuMusic = new Sound(0, 0, 0);
Sound* levelMusic = new Sound(0, 0, 0);
//TO-DO

//Game Object lists
GameObject* btnArr1[4] = {singlePlayer, multiPlayer, language, 0};
GameObject* btnArr2[8] = {peaSeed, sunSeed, snowSeed, repSeed, chompSeed, mineSeed, bombSeed, wallSeed};
//GameObject* btnArr3[16] = {};

GameObject* lmwArr[5] = {LM1, LM2, LM3, LM4, LM5};

GameObjectList* menuButtons = new GameObjectList(btnArr1);	
GameObjectList* singlePlayerButtons = new GameObjectList(btnArr2);
//GameObjectList* multiPlayerButtons = new GameObjectList(btnArr3);

GameObjectList* lawnMowers = new GameObjectList(lmwArr);

//scenes
Scene* menu = new Scene(menuButtons, 0, menuBackground, menuMusic);
Scene* campaign = new Scene(singlePlayerButtons, lawnMowers, lawnBackground, levelMusic);

	





#endif