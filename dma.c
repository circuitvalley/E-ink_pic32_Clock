/* ************************************************************************** */
/** Descriptive File Name

  @Company
    Company Name

  @File Name
    filename.c

  @Summary
    Brief description of the file.

  @Description
    Describe the purpose of this file.
 */
/* ************************************************************************** */

/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
/* ************************************************************************** */

/* This section lists the other files that are included in this file.
 */
#include <xc.h>          /* Defines special funciton registers, CP0 regs  */
#include <stdint.h> 
#include "dma.h"
#include "clock_system.h"

void initDMA()
{
	DMACONbits.ON = 1;
}

void initDMAChannel0()
{
	DCH0CONbits.CHPRI = 3; //high priority 
	DCH0ECONbits.CHSIRQ = _UART2_RX_IRQ;
	DCH0ECONbits.SIRQEN = 1;
	DCH0ECONbits.PATEN = 1; //pattern match 
	DCH0INTbits.CHBCIF = 0;
	DCH0INTbits.CHDDIF = 0;
	DCH0INTbits.CHBCIE = 1; //enable interrupt on abort transfer
	DCH0INTbits.CHDDIE = 1; //enable interrupt on destination done
	DCH0SSAbits.CHSSA = _VirtToPhys((const void *) &U2RXREG); //address of source 
	DCH0DSAbits.CHDSA = _VirtToPhys((const void *) clock_sys.uart.rxbuffer); // address of destination
	DCH0SSIZ = 1; //source size 
	DCH0DSIZ = UART_BUFFER_SIZE; // destination size
	DCH0SPTR = 0; // source pointer 
	DCH0DPTR = 0; // destination pointer
	DCH0CSIZ = 1; //cell size
	DCH0CPTR = 0; //cell pointer 
	DCH0DAT = '\r'; //pattern match for CR]
	DCH0CONbits.CHEN = 1; //enable channel 

}

extern __inline__ unsigned int __attribute__((always_inline)) _VirtToPhys(const void* p)
{
	return(int) p < 0 ? ((int) p & 0x1fffffffL) : (unsigned int) ((unsigned char*) p + 0x40000000L);
}

void __ISR(_DMA_0_VECTOR, IPL7AUTO) _DMA0Handler(void)
{
	if (DCH0INTbits.CHDDIF) {
		sendStringOverUART("Error\r\n");

	} else if (DCH0INTbits.CHBCIF) {

		if (parse_cmd(clock_sys.uart.rxbuffer) > 0)
			sendStringOverUART(clock_sys.uart.txbuffer);
		else
			sendStringOverUART("Error\r\n");

	}

	initDMAChannel0();
	DCH0INTbits.CHDDIF = 0;
	DCH0INTbits.CHBCIF = 0;
	IFS1bits.DMA0IF = 0;


}
/* *****************************************************************************
 End of File
 */
