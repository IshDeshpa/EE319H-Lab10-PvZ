#ifndef INPUTS_H
#define INPUTS_H

#include <stdint.h>

void Inputs_Init();
void getJoyXY(uint32_t data[2]);
uint8_t getA();
uint8_t getB();
uint8_t getRB();
uint8_t getLB();

extern uint8_t LB;
extern uint8_t RB;
extern uint8_t A;
extern uint8_t B;
extern uint8_t JoyX;
extern uint8_t JoyY;

#endif
