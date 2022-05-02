#include "Game.h"

//GLOBALS SECTION
int screenWipe = 0;
//lawnmower sprite and sound
SpriteType* lmSprite;
Sound* lmSound;
//bitmaps
	
uint16_t menuBackground[1];
uint16_t lawnBackground[1];

//plant sprites
SpriteType* peashooterSprite;
SpriteType* snowPeaSprite;
SpriteType* repeaterSprite;
SpriteType* sunflowerSprite;
SpriteType* cherryBombSprite;
SpriteType* potatoMineSprite;
SpriteType* potatoMineReadySprite;
SpriteType* chomperSprite;
SpriteType* chomperChewSprite;
SpriteType* chomperAttackSprite;
SpriteType* wallNutSprite;


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

SeedPacket* peaSeed = new SeedPacket(FspXpos, SpYpos, peashooterID, LoadTime, peashooterCost);
SeedPacket* sunSeed = new SeedPacket(SspXpos, SpYpos, sunflowerID, LoadTime, sunflowerCost);
SeedPacket* snowSeed = new SeedPacket(TspXpos, SpYpos, snowPeaID, LoadTime, snowPeaCost);
SeedPacket* repSeed = new SeedPacket(FspXpos, SpYpos, repeaterID, LoadTime, repeaterCost);
SeedPacket* chompSeed = new SeedPacket(FispXpos, SpYpos, chomperID, LoadTime, chomperCost);
SeedPacket* mineSeed = new SeedPacket(SispXpos, SpYpos, potatoMineID, LoadTime, potatoMineCost);
SeedPacket* bombSeed = new SeedPacket(SespXpos, SpYpos, cherryBombID, LoadTime, cherryBombCost);
SeedPacket* wallSeed = new SeedPacket(EspXpos, SpYpos, wallNutID, LoadTime, wallNutCost);
	
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

Entity::Entity(SpriteType* sp, Sound* sfx, uint8_t xpos, uint8_t ypos, 
							uint8_t hp, uint8_t anim, uint8_t hostl, uint8_t lane)
							: GameObject(sp, sfx, xpos, ypos, lane)
{
		this->health = hp;
		this->animationTime = anim;
		this->animationTimer = anim;
		this->hostile = hostl;
}
void Entity::tick(){
	if(this->animationTimer > 0) this->animationTimer--;
}
void Entity::advance(){
	if(this->animationTimer == 0){
		this->sprite = this->sprite->nextSprite;
		this->animationTimer = this->animationTime;
		this->redraw = 1;
	}
}
void Entity::hurt(uint8_t damage){
	this->health-=damage;
}


void GameObject::unrender(){
	Display_UnrenderSprite(this->oldx, this->oldy, this->previousSprite->width, this->previousSprite->length, currentScene->retBG());
}
void GameObject::render(){
	Display_RenderSprite(this->x, this->y, this->sprite->bmp, this->sprite->width, this->sprite->length, transparentColor, currentScene->retBG());
}

void GameObject::advance(){
	//does nothing?
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
	this->previousSprite = sp;
}
void GameObject::refresh(){
		this->oldx = this->x;
		this->oldy = this->y;
		this->previousSprite = this->sprite;
		advance();
		if(this->redraw == 1){	
			unrender();
			render();
		}
}

uint8_t GameObject::getX(){
	return this->x;
}
uint8_t GameObject::getY(){
	return this->y;
}
uint8_t GameObject::getLane(){
	return this->lane;
}
//does nothing
void GameObject::tick(){}
//does nothing
void GameObject::collided(){}


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
	for(int j = i; j < indexPtr; j++){
		objects[i] = objects[i+1];
	}
}
void GameObjectList::tryRmv(GameObject* go){
	uint8_t i = 0;
	uint8_t end = this->indexPtr;
	while(i<end){
		if(objects[i] == go){
			this->GORmv(i);
			delete(go);
			return;
		}
	}
}
void GameObjectList::refresh(){
	int16_t i = this->indexPtr-1; 
	while(i>=0){
		this->objects[i]->refresh();
		i--;
	} 
}
void GameObjectList::tick(){
	int16_t i = this->indexPtr-1;
	while(i>=0){
		this->objects[i]->tick();
		i--;
	}
}
uint8_t GameObjectList::getLength(){
	return this->indexPtr;
}

SelectCursor::SelectCursor(GameObjectList* gos){
	this->buttonIndex = 0;
	this->targetButtons = gos;
	this->redraw = 0;
	this->button = this->targetButtons->objects[this->buttonIndex];
	this->render();
	this->oldButton = this->button;
}
//TO-DO //on refresh, the cursor will check for button inputs. if
//shoulder buttons, unrender (using seedpacket x, y, w, h, might 
//have to make accessor functions), change buttonIndex,
//then change x and/or y to new button location then rerender
//idk if you need all 3 of these functions
void SelectCursor::refresh(){
	this->updatePos();
	this->render();
		
}
void SelectCursor::updatePos(){
	if(getLB()){
		this->oldButton = this->button;
		this->buttonIndex -= 1;
		if(this->buttonIndex >= this->targetButtons->indexPtr){
			this->buttonIndex = this->targetButtons->indexPtr - 1;
		}
		this->button = this->targetButtons->objects[this->buttonIndex];
		this->redraw = 1;
	}
	else if(getRB()){
		this->oldButton = this->button;
		this->buttonIndex += 1;
		if(this->buttonIndex >= this->targetButtons->indexPtr){
			this->buttonIndex = 0;
		}
		this->button = this->targetButtons->objects[this->buttonIndex];
		this->redraw = 1;
	}
	if(getA()){
		((Button*)this->button)->buttonHit();
	}
}
void SelectCursor::render(){
	if(this->redraw){
		this->redraw = 0;
		Display_UnrenderCursor(this->oldButton->getX() - borderWidth, this->oldButton->getY() - borderWidth, oldButton->sprite->width + 2*borderWidth, oldButton->sprite->length + 2*borderWidth, currentScene->backgroundBMP);
		Display_RenderCursor(this->button->getX() - borderWidth, this->button->getY() - borderWidth, button->sprite->width + 2*borderWidth, button->sprite->length + 2*borderWidth, CURSOR_COLOR);
	}
}
	
	
//gridXpos	and gridYpos start at 1
GridCursor::GridCursor(){
	this->gridXpos = 1;
	this->gridYpos = 1;
	this->oldGridX = 1;
	this->oldGridY = 1;
	this->redraw = 1;
	for(int i = 0; i<5; i++){
		for(int j = 0; j<9; j++){
			this->grid[i][j] = 0;
		}
	}
}
//TO-DO //on refresh, the cursor will check for stick inputs. if 
//stick has changed, unrender, change gridxpos, gridypos 
//then change x and/or y (using calcX/Y) then rerender
//idk if you need all 3 of these functions
//also, check for b button. if b button and gridOpen = 0, somehow
//remove the plant and change grid at position gridXpos - 1, gridYpos - 1 to 1
void GridCursor::refresh(){
	this->updatePos();
	this->render();
}
void GridCursor::updatePos(){
	uint32_t stickPos[2] = {0, 0};
	getJoyXY(stickPos);
	if(stickPos[0] < stickLeftTolerance && this->gridXpos > 1){
		this->oldGridX = this->gridXpos;
		this->gridXpos--;
		this->redraw = 1;
	}
	else if(stickPos[0] < stickRightTolerance && this->gridXpos < 9){
		this->oldGridX = this->gridXpos;
		this->gridXpos++;
		this->redraw = 1;
	}
	if(stickPos[1] < stickDownTolerance && this->gridYpos > 1){
		this->oldGridY = this->gridYpos;
		this->gridYpos--;
		this->redraw = 1;
	}
	else if(stickPos[1] > stickRightTolerance && this->gridYpos < 5){
		this->oldGridY = this->gridYpos;
		this->gridYpos++;
		this->redraw = 1;
	}
	if(getB()){
		if(gridOpen()){
			this->grid[this->gridXpos - 1][this->gridYpos - 1] = 0;
			currentScene->Plants->tryRmv(this->gridXpos, this->gridYpos);
		}
	}
}
//specifically called by shovel mechanic

void GameObjectList::tryRmv(uint8_t col, uint8_t row){
	uint8_t i = 0;
	uint8_t end = this->indexPtr;
	while(i<end){
		if(((Plant*)objects[i])->getCol() == col && objects[i]->getLane() == row){
			GameObject* go = this->objects[i];
			this->GORmv(i);
			go->unrender();
			delete(go);
			return;
		}
	}
}
void GridCursor::render(){
	if(this->redraw){
		this->redraw = 0;
		Display_UnrenderCursor(this->calcX() - borderWidth, this->calcY() - borderWidth, transparentSprite->width + 2*borderWidth, transparentSprite->length + 2*borderWidth, currentScene->backgroundBMP);
		Display_RenderCursor(this->calcX() - borderWidth, this->calcY() - borderWidth, transparentSprite->width + 2*borderWidth, transparentSprite->length + 2*borderWidth, CURSOR_COLOR);
	}
}

uint8_t GridCursor::calcX(){
	return ZeroX + (this->gridXpos-1)*gridX;
}
uint8_t GridCursor::calcY(){
	return ZeroY + (this->gridYpos-1)*gridY;
}

uint8_t GridCursor::gridOpen(){
	return this->grid[this->gridXpos-1][this->gridYpos-1];
}
void GridCursor::fillGrid(){
	this->grid[this->gridXpos-1][this->gridYpos-1] = 1;
}
void GridCursor::emptyGrid(uint8_t col, uint8_t row){
	this->grid[col-1][row-1] = 0;	
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
		this->sunAmount = 0;
}
void Scene::refresh(){
	if(this->select != 0)
	{
			this->select->refresh();
	}
	if(this->planter != 0){
			this->planter->refresh();
	}
	
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
		this->spawnProjectile(sunID, 50, 125, 0);
		sunTimer = sunRate;
	}
}
const uint16_t* Scene::retBG(){
	return this->backgroundBMP;
}

uint8_t Scene::changeSun(int16_t amount){
	if(this->sunAmount+amount>=0){
		this->sunAmount += amount;
		this->renderSun();
		return 1;
	}
	return 0;
}

void Scene::renderSun(){
	if(this->sunAmount>=0){	//>= 0 because it's signed, don't wanna do negatives ig
		//TO-DO
				Display_RenderSprite(2, 110, sunSprite->bmp, sunSprite->width, sunSprite->length, transparentColor, currentScene->backgroundBMP);
				Display_SetCursor(4, 114);
				Display_OutUDec(sunAmount, 0x0000);
	}
}

//0 = Pea
//1 = FrozenPea
//2 = Chomp
//3 = Ohko
//4 = Small explosion
//5 = large explosion
//6 = Sun (set lane to 1 if from sky, 0 if from sunflower)


void Scene::spawnProjectile(uint8_t projID, uint8_t x, uint8_t y, uint8_t lane){
	if(lane<1 || lane > 5){
		return;
	}
	switch (projID){
		case peaID:
			this->Projectiles->GOAdd(new Pea(x, y, lane));
			break;
		case frozenPeaID:
			this->Projectiles->GOAdd(new FrozenPea(x, y, lane));
			break;
		case chompID:
			this->Projectiles->GOAdd(new Chomp(x, y, lane));
			break;
		case ohkoID:
			this->Projectiles->GOAdd(new Ohko(x, y, lane));
			break;
		case smallExplosionID:
			this->Projectiles->GOAdd(new SmallExplosion(x, y, lane));
			break;
		case explosionID:
			this->Projectiles->GOAdd(new Explosion(x, y, lane));
			break;
		case sunID:
			this->Projectiles->GOAdd(new Sun(x, y, lane));
			break;
		default:
			break;
			
	}
}

uint8_t Scene::gridCheck(){
	return this->planter->gridOpen();
}

void Scene::spawnPlant(uint8_t plantID){
	switch (plantID) {
		case peashooterID:
		{
			Plant* pt = new Peashooter(this->planter->calcX(), this->planter->calcY(), this->planter->gridYpos);
			this->Plants->GOAdd(pt);
			pt->setCol(planter->gridXpos);
			break;
		}
		case snowPeaID:
		{
			Plant* pt = new Snowpea(this->planter->calcX(), this->planter->calcY(), this->planter->gridYpos);
			this->Plants->GOAdd(pt);
			pt->setCol(planter->gridXpos);
			break;
		}
		case repeaterID:
		{
			Plant* pt = new Repeater(this->planter->calcX(), this->planter->calcY(), this->planter->gridYpos);
			this->Plants->GOAdd(pt);
			pt->setCol(planter->gridXpos);
			break;
		}
		case sunflowerID:
		{
			Plant* pt = new Sunflower(this->planter->calcX(), this->planter->calcY(), this->planter->gridYpos);
			this->Plants->GOAdd(pt);
			pt->setCol(planter->gridXpos);
			break;
		}
		case chomperID:
		{
			Plant* pt =new Chomper(this->planter->calcX(), this->planter->calcY(), this->planter->gridYpos);
			this->Plants->GOAdd(pt);
			pt->setCol(planter->gridXpos);
			break;
		}
		case potatoMineID:
		{
			Plant* pt =new PotatoMine(this->planter->calcX(), this->planter->calcY(), this->planter->gridYpos);
			this->Plants->GOAdd(pt);
			pt->setCol(planter->gridXpos);
			break;
		}
		case cherryBombID:
		{
			Plant* pt = new CherryBomb(this->planter->calcX(), this->planter->calcY(), this->planter->gridYpos);
			this->Plants->GOAdd(pt);
			pt->setCol(planter->gridXpos);
			break;
		}
		case wallNutID:
		{
			Plant* pt = new Wallnut(this->planter->calcX(), this->planter->calcY(), this->planter->gridYpos);
			this->Plants->GOAdd(pt);
			pt->setCol(planter->gridXpos);
			break;
		}
		default:
			break;
		
	}
	this->planter->fillGrid();
}


void Scene::spawnZombie(uint8_t zombieID, uint8_t lane){
	switch(zombieID){
		case regularZombieID:
			this->Zombies->GOAdd(new Zombie(170, Lane1Ypos + (lane-1)*LaneYOffset, lane));
			break;
		case coneZombieID:
			this->Zombies->GOAdd(new ConeZombie(170, Lane1Ypos + (lane-1)*LaneYOffset, lane));
			break;
		case bucketZombieID:
			this->Zombies->GOAdd(new BucketZombie(170, Lane1Ypos + (lane-1)*LaneYOffset, lane));
			break;
		case footballZombieID:
			this->Zombies->GOAdd(new FootballZombie(170, Lane1Ypos + (lane-1)*LaneYOffset, lane));
			break;
		case newspaperZombieID:
			this->Zombies->GOAdd(new NewsZombie(170, Lane1Ypos + (lane-1)*LaneYOffset, lane));
			break;
		case flagZombieID:
			this->Zombies->GOAdd(new FlagZombie(170, Lane1Ypos + (lane-1)*LaneYOffset, lane));
			break;
		case poleVaultZombieID:
			this->Zombies->GOAdd(new PoleZombie(170, Lane1Ypos + (lane-1)*LaneYOffset, lane));
			break;
		case jackZombieID:
			this->Zombies->GOAdd(new JackZombie(170, Lane1Ypos + (lane-1)*LaneYOffset, lane));
			break;
		default:
				break;
	}
}

int Scene::cursorHit(int16_t x, int16_t y){
	if((int16_t)this->planter->calcX() - x < collectTolerance && this->planter->calcX() - x > -collectTolerance
		&& this->planter->calcY()- y < collectTolerance && this->planter->calcY() - y > collectTolerance)
		return 1;
	return 0;
}

void Scene::wipe(){
	for(int i = this->Zombies->indexPtr; i>=0; i--){
		Zombies->objects[i]->unrender();
		Zombies->GORmv(i);
	}
	for(int i = this->Plants->indexPtr; i>=0; i--){
		Zombies->objects[i]->unrender();
		Zombies->GORmv(i);
	}
	screenWipe = 1;
}

void Scene::collisions(){
	GameObject** zmList = this->Zombies->objects;
	GameObject** ptList = this->Plants->objects;
	GameObject** prList = this->Projectiles->objects;
	for(int i = this->Zombies->indexPtr - 1; i >=0; i--) {
		Zombie* zm = (Zombie*)zmList[i];
		uint8_t eatingFlag = 0;
		for(int j = this->Plants->indexPtr - 1; j >= 0; j--){
			Plant* pt = (Plant*)ptList[i];
			if(zm->getLane() == pt->getLane()
				&& zm->getX() > pt->getX() 
				&& zm->getX() < pt->getX() + pt->sprite->width)
			{
				eatingFlag = 1;
				zm->attack(pt);
				if(screenWipe){
					break;
				}
				if(pt->health <=0){
					pt->unrender();
					this->planter->emptyGrid(pt->getCol(), pt->getLane());
					this->Plants->tryRmv(pt);
				}
				if(zm->health<=0){
					zm->unrender();
					this->Zombies->tryRmv(zm);
					zm = 0;
					break;
				}
			}
		}
		if(screenWipe){
			screenWipe = 0;
			break;
		}
		if(zm != 0){
			if(eatingFlag == 0){
				zm->stopEating();
			}
		}
	}
	for(int i = this->Zombies->indexPtr - 1; i >=0; i--) {
		for(int j = this->Projectiles->indexPtr - 1; j >= 0; j--){
			Zombie* zm = (Zombie*)zmList[i];
			Projectile* pr = (Projectile*)prList[i];
			if(pr->getLane() == zm->getLane()
				&& pr->getX() > zm->getX() 
				&& pr->getX() < zm->getX() + zm->sprite->width)
			{
				zm->takeDamage(pr->damage);
				pr->collided();
				if(zm->health<=0){
					zm->unrender();
					this->Zombies->tryRmv(zm);
					break;
				}
			}
		}
	}
}