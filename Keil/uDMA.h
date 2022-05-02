#ifndef _UDMA
#define _UDMA  /* Revision ID */

#include <stdint.h>

#define UDMA_BUFFER_SIZE 8

static void SSI0DMAConfiguration(void);
static void CfgDMAChSrcAdd(uint8_t channel, uint32_t end_address);
static void CfgDMAChDesAdd(uint8_t channel, uint32_t end_address);


#endif

