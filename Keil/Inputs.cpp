#include <stdint.h>
#include "inc/tm4c123gh6pm.h"
#include "Inputs.h"

uint8_t LB = 0;
uint8_t RB = 0;
uint8_t A = 0;
uint8_t B = 0;

void Inputs_Init(){
		SYSCTL_RCGCGPIO_R |= 0x1C;
		SYSCTL_RCGCADC_R |= 0x01;
		while((SYSCTL_PRGPIO_R&SYSCTL_PRGPIO_R0) == 0){};
		while((SYSCTL_PRADC_R&SYSCTL_PRADC_R0) == 0){};
		GPIO_PORTC_DEN_R |= 0xC0;	//BR1, BL1
		GPIO_PORTD_DEN_R |= 0xC0;	//A1, B1
		GPIO_PORTE_DEN_R |= 0x03; //Joysticks
		
		GPIO_PORTC_DIR_R &= ~0xC0;
		GPIO_PORTD_DIR_R &= ~0xC0;
		GPIO_PORTE_DIR_R &= ~0x03;
		
		GPIO_PORTC_PDR_R |= 0xC0;	//BR1, BL1
		GPIO_PORTD_PDR_R |= 0xC0;	//A1, B1

		GPIO_PORTC_AMSEL_R &= ~0xC0;          // disable analog functionality
		GPIO_PORTD_AMSEL_R &= ~0xC0;          // disable analog functionality
		GPIO_PORTE_AMSEL_R |= 0x03;          // disable analog functionality
			
		GPIO_PORTC_AFSEL_R &= ~0xC0;          // disable analog functionality
		GPIO_PORTD_AFSEL_R &= ~0xC0;          // disable analog functionality
		GPIO_PORTE_AFSEL_R |= 0x03;          // disable analog functionality
			
		//GPIO_PORTE_PCTL_R = (GPIO_PORTF_PCTL_R&0x000000FF)+0x00000000;
			
		ADC0_PC_R = 0x01;
		ADC0_SSPRI_R = 0x3210;
		ADC0_ACTSS_R &= ~0x0001;	// Sample sequencer 0
		ADC0_SAC_R = 2;	// 4x hardware sampling
		ADC0_EMUX_R &= ~0x000F;
		ADC0_SSMUX0_R = 0x0023;	// Read PE1 and PE0
		ADC0_SSCTL0_R = 0x0060;
		ADC0_IM_R &= ~0x0001;
		ADC0_ACTSS_R |= 0x0001;
}
void getJoyXY(uint32_t data[2]){
	ADC0_PSSI_R = 0x0001;
	while((ADC0_RIS_R&0x01) == 0){};
	data[1] = ADC0_SSFIFO0_R&0xFFF;
	data[0] = ADC0_SSFIFO0_R&0xFFF;
	ADC0_ISC_R = 0x0004;
}

uint8_t getA(){
	if(A && !((GPIO_PORTD_DATA_R & 0x80) >> 7)){
		A = 0;
	}
	return (GPIO_PORTD_DATA_R & 0x80) >> 7;
}

uint8_t getB(){
	if(B && !((GPIO_PORTD_DATA_R & 0x40) >> 6)){
		B = 0;
	}
	return (GPIO_PORTD_DATA_R & 0x40) >> 6;
}

uint8_t getRB(){
	if(RB && !((GPIO_PORTC_DATA_R & 0x80) >> 7)){
		RB = 0;
	}
	return (GPIO_PORTC_DATA_R & 0x80) >> 7;
}

uint8_t getLB(){
	if(LB && !((GPIO_PORTC_DATA_R & 0x40) >> 6)){
		LB = 0;
	}
	return (GPIO_PORTC_DATA_R & 0x40) >> 6;
}
