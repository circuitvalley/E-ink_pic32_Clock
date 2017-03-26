/* ************************************************************************** */
/** Descriptive File Name

  @Company
    CircuitValley

  @File Name
    spi32.c

  @Summary
    Brief description of the file.

  @Description
    Describe the purpose of this file.
 */
#ifdef __XC32
#include <xc.h>          /* Defines special funciton registers, CP0 regs  */
#endif
#include <stdint.h> 
#include "spi32.h"

void spiInit()
{
	SPI2CONbits.FRMEN = 0; //no framed 
	SPI2CONbits.MSSEN = 0; //slave select disable 
	SPI2CONbits.SPIFE = 0; //Frame synchronization pulse precedes the first bit clock
	SPI2CONbits.ENHBUF = 0; //enhanced buffer mode is disabled 
	SPI2CONbits.SIDL = 1; //stop in idle mode 
	SPI2CONbits.DISSDO = 0; //SDO pin controlled by spi module
	SPI2CONbits.SMP = 0; // sampling edge 
	SPI2CONbits.MSTEN = 1; //master mode 
	SPI2CONbits.STXISEL = 0;
	SPI2CONbits.CKE = 1;
	SPI2CONbits.ON = 1; //on


	SPI2BRG = 1; //32Mhz pbclk 

	SET_CS_DIR_OUT;
	SET_CE_DIR_OUT;
}

uint8_t spiWrite(uint8_t data)
{
	volatile uint8_t zero;

	SPI2BUF = data;
	while (SPI2STATbits.SPIBUSY);
	zero = SPI2BUF;

}

void spiWriteBuffer(uint8_t *holder, uint8_t n)
{
	volatile uint8_t zero;
	while (n--) {
		SPI2BUF = *holder++;
		while (SPI2STATbits.SPIBUSY);
		zero = SPI2BUF;
	}
}

uint8_t spiRead(void)
{
	volatile uint8_t zero;
	SPI2BUF = 0x00;
	//   delay_long(1);

	while (SPI2STATbits.SPIBUSY);
	//  while(!SPI2STATbits.SPIRBF);

	zero = SPI2BUF;
	return zero;
}

uint8_t spiTxRX(uint8_t data)
{
	volatile uint8_t zero;
	SPI2STATCLR = 1 << 6;

	SPI2BUF = data;
	//delay_long(1);
	while (SPI2STATbits.SPIBUSY);

	//  while(!SPI2STATbits.SPIRBF);
	zero = SPI2BUF;

	return zero;
}
