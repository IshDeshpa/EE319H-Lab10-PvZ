#include <stdint.h>
#include "uDMA.h"
#include "inc/tm4c123gh6pm.h"

static uint32_t udma_control_structure[256] __attribute__ ((aligned(1024)));
static uint16_t udma_buffer_tx[UDMA_BUFFER_SIZE] ;
static uint16_t udma_buffer_rx[UDMA_BUFFER_SIZE] ;



static void SSI0DMAConfiguration(void){
	SYSCTL_RCGCDMA_R = 0x01; 	//Enable clock for DMA
	UDMA_CFG_R = 0x01;	//Enable uDMA controller
	UDMA_CTLBASE_R = (uint32_t)udma_control_structure;//Using Primary control structure
	UDMA_ALTCLR_R = (uint32_t)((1<<10)|(1<<11)); 	// Ch. 10 and 11 for SSI0Rx & Tx
	UDMA_PRIOSET_R = (1<<10);		//Setting high priority for Receiving from SSI0
	UDMA_CHMAP1_R  = (1<<8 /*Map channel 10 to SSI0 Rx*/ )|(1<<12/*Map channel 11 to SSI0 Tx*/);
	UDMA_REQMASKSET_R = ~(uint32_t)((1<<10)|(1<<11));	// Enable Ch. 10 and 11 for request
	
	// Configure channel ten for a source of the SSI0 DR register (receive from SSI) and a destination of the receive buffer declared above
	CfgDMAChSrcAdd(10, (uint32_t)&(SSI0_DR_R));
	CfgDMAChDesAdd(10, (uint32_t)(udma_buffer_rx+(UDMA_BUFFER_SIZE-1)));
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
