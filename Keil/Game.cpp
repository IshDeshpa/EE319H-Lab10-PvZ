#include "Game.h"

//Constructor
GameObjectList::GameObjectList(GameObject** GOlist){
	indexPtr = 0;
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
	indexPtr++;
}
	
		//Access Object
GameObject* GameObjectList::operator[](uint8_t i){
	return this->objects[i];
}
		//Remove Object at index
void GameObjectList::GORmv(uint8_t i){
	indexPtr--;
	for(int j = i; i < indexPtr; j++){
		objects[i] = objects[i+1];
	}
}

Scene::Scene(GameObjectList* but, GameObjectList* lwm, uint16_t* bg, Music* msc){
		this->Buttons = but;
		this->Plants = new GameObjectList(0);
		this->Zombies = new GameObjectList(0);
		this->Lawnmowers = new GameObjectList(0);
		this->Projectiles = new GameObjectList(0);
		this->backgroundBMP = bg;
		this->music = msc;
}