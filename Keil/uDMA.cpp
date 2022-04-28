#include <stdint.h>
#include "uDMA.h"
#include "inc/tm4c123gh6pm.h"

static uint32_t udma_control_structure[256] __attribute__ ((aligned(1024)));
static uint32_t control_word_ch10 = (1<<30)	//destination address increment (increment by 16 bit locations)
									|(1<<28)		//destination data size (16 bit data size)
									|(3<<26)		//source address increment (No increment)
									|(1<<24)		//source data size (16 bit data size)
									|(3<<14)		//Arbitration size ( 8 transfers)
									|((UDMA_BUFFER_SIZE-1)<<4)		//Transfer size (minus 1)
									|(0<<3)		//next useburst
									|(1<<0);		//Basic mode


static void SSI0DMAConfiguration(void){
	if((((uint32_t)udma_control_structure & ~(0x3FF)) == (uint32_t)udma_control_structure)
			&& ((uint32_t)udma_control_structure >= 0x20000000))
	{
		SYSCTL_RCGCDMA_R = 0x01; 	//Enable clock for DMA
		UDMA_CFG_R = 0x01;	//Enable uDMA controller
		UDMA_CTLBASE_R = (uint32_t)udma_control_structure;//Using Primary control structure
		UDMA_ALTCLR_R = (uint32_t)(1<<10); 	// Ch. 10 and 11 for SSI0Rx
		UDMA_PRIOSET_R = (1<<10);		//Setting high priority for Receiving from SSI0
		UDMA_CHMAP1_R  = (1<<8 /*Map channel 10 to SSI0 Rx*/ );
		UDMA_REQMASKSET_R = ~(uint32_t)(1<<10);	// Enable Ch. 10 for request
		
		// Configure channel ten for a source of the SSI0 DR register (receive from SSI) and a destination of the receive buffer declared above
		CfgDMAChSrcAdd(10, (uint32_t)&(SSI0_DR_R));
		CfgDMAChDesAdd(10, (uint32_t)(udma_buffer_rx+(UDMA_BUFFER_SIZE-1)));
		CfgDMAChContrWrd(10, control_word_ch10);
		
		UDMA_ENASET_R = 1<<10;
	}
}

static void CfgDMAChSrcAdd(uint8_t channel, uint32_t end_address)
{
	uint32_t* ptr;
	uint32_t temp = 0;
	temp = (uint32_t)udma_control_structure;	// address of control structure
	temp +=  (uint32_t)(channel<<4);	// add offset of channel
	ptr = (uint32_t*)temp; //point to channel source address container
	*ptr = end_address;
}

static void CfgDMAChDesAdd(uint8_t channel, uint32_t end_address)
{
	uint32_t* ptr;
	uint32_t temp = 0;
	temp = (uint32_t)udma_control_structure;
	temp +=  (((uint32_t)(channel<<4)) + ((uint32_t)0x04));
	ptr = (uint32_t*)temp; //point to channel destination address container
	*ptr = end_address;
}

static void CfgDMAChContrWrd(uint8_t channel, uint32_t control_word){
	uint32_t* ptr;
	uint32_t temp = 0;
	temp = (uint32_t)udma_control_structure;
	temp +=  (((uint32_t)(channel<<4)) + ((uint32_t)0x08));
	ptr = (uint32_t*)temp; //point to channel destination address container
	*ptr = control_word;
}
