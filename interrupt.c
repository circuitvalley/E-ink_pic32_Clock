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
#include "spi32.h"
#include "NRF24L01P.h"
#include "clock_system.h"
#include "interrupt.h"

void __ISR(_EXTERNAL_0_VECTOR, IPL7AUTO) _Externak0Handler(void)
{

	if (nRF24L01_RxPacket(RxBuf)) //then wait for a packet from the peer
	{
		parseTempData(RxBuf, &clock_sys.wtsensor);
		SetRX_Mode();
	}
	clearNRF_INT_FLAG();
	IFS0bits.INT0IF = 0;
}

void __ISR(_EXTERNAL_1_VECTOR, IPL7AUTO) _Externak1Handler(void)
{


	RTC_ISR();

	IFS0bits.INT1IF = 0;

}

void __attribute__((nomips16)) INTConfigureSystem(INT_SYSTEM_CONFIG config)
{
	unsigned int val;

	// set the CP0 cause IV bit high
	asm volatile("mfc0   %0,$13" : "=r"(val));
	val |= 0x00800000;
	asm volatile("mtc0   %0,$13" : "+r"(val));

	if (config == INT_SYSTEM_CONFIG_MULT_VECTOR)
		INTCONSET = _INTCON_MVEC_MASK;
	else
		INTCONCLR = _INTCON_MVEC_MASK;
}

unsigned int __attribute__((nomips16)) INTEnableInterrupts(void)
{
	unsigned int status = 0;
	asm volatile("ei    %0" : "=r"(status));
	return status;
}

void initExternalInterrupts()
{
	IFS0bits.INT0IF = 0;
	IPC0bits.INT0IP = 7;
	IPC1bits.INT1IP = 7;
	INTCONbits.INT0EP = 0; //edge
	INTCONbits.INT1EP = 0;

	IEC0bits.INT0IE = 1; //interrupt enable
	IEC0bits.INT1IE = 1;
	LATDbits.LATD8 = 1;
	LATDbits.LATD1 = 1;

	TRISDbits.TRISD8 = 1; // RTC interrupt
	TRISDbits.TRISD0 = 1; //NRF interrupt

	TRISDbits.TRISD1 = 0;

	IFS1bits.DMA0IF = 0;
	IPC9bits.DMA0IP = 7;
	IEC1bits.DMA0IE = 1;

	IFS1bits.U2RXIF = 0; //uart2 rx interrupt 
	//    IPC8bits.U2IP =7; 
	//    IEC1bits.U2RXIE =1;

	IFS0bits.INT1IF = 1; // needed a interrupt to update screen at startup 

}
/* *****************************************************************************
 End of File
 */
