#include "Game.h"

//GLOBALS SECTION
//lawnmower sprite and sound
SpriteType* lmSprite;
Sound* lmSound;
//bitmaps
	
uint16_t menuBackground[1];
uint16_t lawnBackground[1];

//button sprites
SpriteType* vsEnglish = new SpriteType(0, 0, 0, 0);
SpriteType* vsSpanish = new SpriteType(0, 0, 0, 0);
SpriteType* campaignEnglish = new SpriteType(0, 0, 0, 0);
SpriteType* campaignSpanish = new SpriteType(0, 0, 0, 0);
SpriteType* languageEnglish = new SpriteType(0, 0, 0, 0);
SpriteType* languageSpanish = new SpriteType(0, 0, 0, 0);

SpriteType* p0; SpriteType* p0g;
SpriteType* p1; SpriteType* p1g;
SpriteType* p2; SpriteType* p2g;
SpriteType* p3; SpriteType* p3g;
SpriteType* p4; SpriteType* p4g;
SpriteType* p5; SpriteType* p5g;
SpriteType* p6; SpriteType* p6g;
SpriteType* p7; SpriteType* p7g;


SpriteType* seedPacketSprites[8] = {p0, p1, p2, p3, p4, p5, p6, p7};
SpriteType* seedPacketGraySprites[8] = {p0g, p1g, p2g, p3g, p4g, p5g, p6g, p7g};

//button sounds
Sound* menuSound = new Sound();
Sound* plantingSound = new Sound();

//language
uint8_t lang = 0; //0 = eng, 1 = esp	
	

//sfx/music filenames or whatever we are doing
//TO-DO



//Buttons
CampaignButton* singlePlayer = new CampaignButton(CmpButtonXpos, CmpButtonYpos);
VsButton* multiPlayer = new VsButton(VsButtonXpos, VsButtonYpos);
LanguageButton* language = new LanguageButton(LangButtonXpos, LangButtonYpos);

SeedPacket* peaSeed = new SeedPacket(FspXpos, SpYpos, 0, LoadTime);
SeedPacket* sunSeed = new SeedPacket(SspXpos, SpYpos, 1, LoadTime);
SeedPacket* snowSeed = new SeedPacket(TspXpos, SpYpos, 2, LoadTime);
SeedPacket* repSeed = new SeedPacket(FspXpos, SpYpos, 3, LoadTime);
SeedPacket* chompSeed = new SeedPacket(FispXpos, SpYpos, 4, LoadTime);
SeedPacket* mineSeed = new SeedPacket(SispXpos, SpYpos, 5, LoadTime);
SeedPacket* bombSeed = new SeedPacket(SespXpos, SpYpos, 6, LoadTime);
SeedPacket* wallSeed = new SeedPacket(EspXpos, SpYpos, 7, LoadTime);
	
//LawnMowers
LawnMower* LM1 = new LawnMower(0, Lane1Ypos, 1);
LawnMower* LM2 = new LawnMower(0, Lane2Ypos, 2);
LawnMower* LM3 = new LawnMower(0, Lane3Ypos, 3);
LawnMower* LM4 = new LawnMower(0, Lane4Ypos, 4);
LawnMower* LM5 = new LawnMower(0, Lane5Ypos, 5);


//Music classes
Sound* menuMusic = new Sound();
Sound* levelMusic = new Sound();
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
//Scene* vsmode = new Scene(multiPlayerButtons, lawnMowers, lawnBackground, levelMusic);

Sound* s;
uint8_t soundBuffer[24000];
Scene* currentScene = menu;

void loadScene(Scene* s){
	currentScene = s;
}

SpriteType::SpriteType(uint16_t* bmp, uint8_t length, uint8_t width, SpriteType* nextSprite){
	this->bmp = bmp;
	this->length = length;
	this->width = width;
	this->nextSprite = nextSprite;
}

void GameObject::unrender(){
	Display_UnrenderSprite(this->x, this->y, this->sprite->width, this->sprite->length, currentScene->retBG());
}
void GameObject::render(){
	//TO-DO
}

void GameObject::advance(){
	//TO-DO
}

GameObject::GameObject(SpriteType* sp, Sound* sfx, uint8_t x, uint8_t y){
	this->sprite = sp;
	this->soundFX = sfx;
	this->x = x;
	this->y = y;
	this->redraw = 1;
}
GameObject::GameObject(SpriteType* sp, Sound* sfx, uint8_t x, uint8_t y, uint8_t lane){
	this->sprite = sp;
	this->soundFX = sfx;
	this->x = x;
	this->y = y;
	this->redraw = 1;
	this->lane = lane;
	this->oldx = 0;
	this->oldy = 0;
}
void GameObject::refresh(){
		this->oldx = this->x;
		this->oldy = this->y;
		advance();
		if(this->redraw == 1){	
			unrender();
			render();
		}
}
//does nothing
void GameObject::tick(){}
//does nothing
void GameObject::collided(){}

	
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
}
void Projectile::tick(){
	this->distanceDiff+=this->speed;
}
int Projectile::collided(){
	this->unrender();
	return 1;
}
//Constructor


GameObjectList::GameObjectList(GameObject** GOlist){
	this->indexPtr = 0;
}
		//Destructor
GameObjectList::~GameObjectList(){
	for(int i = 0; i < indexPtr; i++){
		delete objects[i];
	}
}
		//Add Object
void GameObjectList::GOAdd(GameObject* add){
	this->objects[indexPtr] = add;
	this->indexPtr++;
}
	
		//Access Object
GameObject* GameObjectList::operator[](uint8_t i){
	return this->objects[i];
}
		//Remove Object at index
void GameObjectList::GORmv(uint8_t i){
	this->indexPtr--;
	for(int j = i; i < indexPtr; j++){
		objects[i] = objects[i+1];
	}
}
void GameObjectList::refresh(){
	uint8_t i = 0; 
	uint8_t end = this->indexPtr;
	while(i<end){
		this->objects[i]->refresh();
		i++;
	} 
}
void GameObjectList::tick(){
	uint8_t i = 0;
	uint8_t end = this->indexPtr;
	while(i<end){
		this->objects[i]->tick();
		i++;
	}
}
uint8_t GameObjectList::getLength(){
	return this->indexPtr;
}

Scene::Scene(GameObjectList* but, GameObjectList* lwm, const uint16_t* bg, Sound* msc){
		this->Buttons = but;
		this->Plants = new GameObjectList(0);
		this->Zombies = new GameObjectList(0);
		this->Lawnmowers = new GameObjectList(0);
		this->Projectiles = new GameObjectList(0);
		this->backgroundBMP = bg;
		this->music = msc;
		this->sunRate = sunProductionRate;
		this->sunTimer = 0;
}
void Scene::refresh(){
	this->Buttons->refresh();
	this->Plants->refresh();
	this->Zombies->refresh();
	this->Lawnmowers->refresh();
	this->Projectiles->refresh();
}
void Scene::tick(){
	this->Buttons->tick();
	this->Plants->tick();
	this->Zombies->tick();
	this->Lawnmowers->tick();
	this->Projectiles->tick();
	if(this->sunTimer!=0) this->sunTimer--;
	else{
		//TO-DO: somehow spawn sun at top of screen
		//this->Projectiles->GOAdd(new Sun
		sunTimer = sunRate;
	}
}
const uint16_t* Scene::retBG(){
	return this->backgroundBMP;
}
