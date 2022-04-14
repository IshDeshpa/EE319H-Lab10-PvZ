#ifndef PLANT_H
#define PLANT_H





array = [(normal, 36), (bucket, 40), (flag, 40)]

if array[currINdx].time < currtime(
	spawnZombie(array[curINdx}.name);

spawnZombie(zombnum){
	if(zombnum, = 1)
		urrentzombuies[currentzombieslength] = new flagzombie()
		generate(num);
	if(zombnum = 2)
		urrentzombuies[currentzombieslength] = new buscketzombie()
		
	

Flagzombie()
	call zombie

	
generate(num)
	for(int i = 0; i<num; i++)
		spawnZombie(random(0-7));
	
	



#include <stdint.h>
#include "Game.h"

//Peashooter
//Repeater
//Snow pea
//Wall-nut
//Potato Mine
//Cherry Bomb
//Chomper
//Sunflower


class Plant : public Entity{
	protected:
		uint8_t attackRate;
		Projectile projectile;
		//create projectile
		void attack();
		//idk why I put this here
		void advance();
	public:
		//call entity constructor with all but attackRate and projectile
		Plant(SpriteType* sp, Sound* sfx, uint8_t xpos, uint8_t ypos, uint8_t hp, 
					uint8_t anim, uint8_t hostile, uint8_t atkRt, Projectile proj);
		
		
};

class Peashooter : public Plant{
		
	protected: 
		//check if time>attackRate, then create pea if it is
		void advance();
	
	public:
		//constructor calls Plant constructor with defined peashooter sprite, 
		//defined peashooter sound, x and y arguments, defined generic plant health,
		//generic plant animation time, hostile = 1, atkRt as generic plant attack rate
		//and pea projectile
		Peashooter(uint8_t x, uint8_t y);
		//special constructor calls Plant constructor with Sprite argument rather than defined peashooter
		Peashooter(uint8_t x, uint8_t y, SpriteType* sp);
		//special constructer calls Plant constructor with Sprite argument and pea argument
		Peashooter(uint8_t x, uint8_t y, SpriteType* sp, Projectile p);
};
		
class Repeater : public Peashooter{
	protected:
		//time in between 1st and 2nd pea
		uint8_t repeatTime;
	public:
		//constructor calls peashooter constructor with x and y argument and defined repeater sprite, and sets repeatTime to defined repeater time
		Repeater(uint8_t x, uint8_t y);

};
class Snowpea : public Peashooter{
	public:
		//constructor calls peashooter constructor with x and y arguements, Snowpea sprite and snow pea projectile
		Snowpea(uint8_t x, uint8_t y);
};

class Wallnut : public Plant{
	protected:
		SpriteType* damagedWallnut;
		//new advance function will check health, and change sprite if it reaches defined threshold
		void advance();

	public:
		//constructor calls Plant constructor with defined Walnut sprite, 
		//null sount, x and y arguments, defined walnut health,
		//generic plant animation time (still need to render when it gets hurt), hostile = 0, atkRt as generic plant attack rate (doesn't matter)
		//and null projectile. Set damagedWallnut to the defined damagedWallnut sprite
		Wallnut(uint8_t x, uint8_t y);
};

class CherryBomb : public Plant{
	protected:
		//after attacking, destroy the cherry bomb
		void advance();
	
	public:
			//constructor calls Plant constructor with defined cherry bomb sprite, explosion sound, x and y arguments, 
		  //defined cherry bomb health, generic plant animation time, hostile = 1, atkRt as generic plant attack rate maybe, 
			//explosion projectile
			CherryBomb();
};

class PotatoMine : public Plant{
	protected;
		//can only attack if it's grown
		uint8_t grown; //1 or 0
		uint8_t
#endif
