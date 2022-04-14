#ifndef ZOMBIE_H
#define ZOMBIE_H

#include <stdint.h>
#include "Game.h"

// Basic zombie.
class Zombie: public Entity{
    protected:
        uint8_t speed;  // Speed of zombie
        uint8_t isEating;   // Is the zombie eating?
        
        // Advance to the next state of the entity
		void advance();
		
        // do attacking sequence if hostile
		void attack();
    public:
        // Constructor
        Zombie();
};

// Conehead zombie. Only thing different is that it has a different sprite and different health.
class ConeZombie: public Zombie{
    protected:
        // Redefine advance to change sprite at certain health
        void advance();
    public:
        // Constructor
        ConeZombie(uint8_t x);
};

// Buckethead zombie. Only thing different is that it has a different sprite and different health.
class BucketZombie: public Zombie{
    protected:
        // Redefine advance to change sprites at certain health
        void advance();
    public:
        // Constructor
        BucketZombie();
};

// Polevault zombie. Jumps over plants.
class PoleZombie: public Zombie{
    private:
        uint8_t hasPole;    // Does the zombie  have its pole?
        
        // Redefine advance with jumping logic
        void advance();

        // Redefine attack to jump over first plant
        void attack();
    public:
        // Constructor
        PoleZombie();
};


#endif
