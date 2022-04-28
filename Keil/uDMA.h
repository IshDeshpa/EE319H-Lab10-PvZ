#ifndef _UDMA
#define _UDMA  /* Revision ID */

#include <stdint.h>

#define UDMA_BUFFER_SIZE 8

static uint16_t udma_buffer_rx[UDMA_BUFFER_SIZE];

static void SSI0DMA_Init(void);
static void CfgDMAChSrcAdd(uint8_t channel, uint32_t end_address);
static void CfgDMAChDesAdd(uint8_t channel, uint32_t end_address);
static void CfgDMAChContrWrd(uint8_t channel, uint32_t control_word);


#endif

