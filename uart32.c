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

#include <xc.h>          /* Defines special funciton registers, CP0 regs  */
#include <stdint.h> 
#include <sys/attribs.h> //longramfunc and __ISR

void initUART()
{
	U2MODEbits.STSEL = 0; //1 stop bit
	U2MODEbits.PDSEL = 0; // 8bit no parity 
	U2MODEbits.BRGH = 1; //  high speed baud
	U2MODEbits.RXINV = 0; //rx idle state is high as normal
	U2MODEbits.ABAUD = 0; //no autobaud
	U2MODEbits.LPBACK = 0; // loopback disable 
	U2MODEbits.WAKE = 1; //wake on start bit detect 
	U2MODEbits.UEN = 0; // only TX and RX enable no RTS or cts
	U2MODEbits.RTSMD = 1; //simplex no flow control
	U2MODEbits.IREN = 0; //irDA disable
	U2MODEbits.SIDL = 0; //will not stop UART when idle
	U2BRG = 68; //4clock cycle of pbclk per bit
	U2STAbits.UTXEN = 1;
	U2STAbits.URXEN = 1;
	U2MODEbits.ON = 1;
}

void sendOverUART(uint8_t data)
{
	while (!U2STAbits.TRMT);
	U2TXREG = data;
}

void sendStringOverUART(char *str)
{
	while (*str)
		sendOverUART(*str++);
}


// not actually used , never called , we use DMA

void __ISR(_UART_2_VECTOR, IPL7AUTO) _UART2RXHandler(void)
{
	sendOverUART(U2RXREG);
	IFS1bits.U2RXIF = 0;
}


/* *****************************************************************************
 End of File
 */
