/* ************************************************************************** */
/** Descriptive File Name

  @Company
    Company Name

  @File Name
    filename.h

  @Summary
    Brief description of the file.

  @Description
    Describe the purpose of this file.
 */
/* ************************************************************************** */

#ifndef _SPI32_H_    /* Guard against multiple inclusion */
#define _SPI32_H_

#include <xc.h>          /* Defines special funciton registers, CP0 regs  */
#include <stdint.h>
void spiInit();

uint8_t spiWrite(uint8_t);
uint8_t spiRead(void);
void spiWriteBuffer(unsigned char *holder, unsigned char n);
uint8_t spiTxRX(uint8_t data);
uint8_t spiRead(void);
void spiWriteBuffer(uint8_t *holder, uint8_t n);


#define SET_CS_DIR_OUT	TRISGbits.TRISG9=0
#define setSPICS(b) 	LATGbits.LATG9=b

#define SET_CE_DIR_OUT	TRISEbits.TRISE5=0
#define NRF_ENABLE(b)   LATEbits.LATE5=b 


#endif /* _SPI32_H_ */

/* *****************************************************************************
 End of File
 */
