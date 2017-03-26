/******************************************************************************/
/*  Files to Include                                                          */
/******************************************************************************/
#include <xc.h>          /* Defines special funciton registers, CP0 regs  */
#include <stdint.h>         /* For uint32_t definition                        */
#include <stdbool.h>        /* For true/false definition                      */
#include <sys/attribs.h> //longramfunc

#include "delay.h"
#include "ds18b20.h"
#include "interrupt.h"
#include "spi32.h"
#include "NRF24L01P.h"
#include "rtc.h"
#include "clock_system.h"
#include "eink_p32.h"
#include "wsensor.h"
#include "uart32.h"
#include "dma.h"
/******************************************************************************/
/* Global Variable Declaration                                                */
/******************************************************************************/
#define enableIDLEMODE()  SYSKEY=0xAA996655; \
                        SYSKEY=0x556699AA; \
                        OSCCONbits.SLPEN = 0x1; \
                        SYSKEY = 0x0; 

inline void __attribute__((nomips16)) PowerSaveIDLE(void)
{
	OSCCONSET = (1 << _OSCCON_SLPEN_POSITION);
	asm("WAIT");
}
/******************************************************************************/
/* Main Program                                                               */

/******************************************************************************/

int32_t main(void)
{
	enableIDLEMODE();
	DDPCONbits.JTAGEN = 0; //disable JTAG 
	initUART();
	initDMAChannel0();
	initDMA();
	initOneWire();
	i2c_init();
	initRTC();
	spiInit();
	initNRF24L01p();
	clearNRF_INT_FLAG();
	SetRX_Mode();
	setRTCalarm_per_minute();
	initEink(); //initialize Eink 
	initExternalInterrupts();
	INTConfigureSystem(INT_SYSTEM_CONFIG_MULT_VECTOR); // set CPU for multi-vector interrupts
	INTEnableInterrupts(); // set CPU to be able to receive interrupts
	while (1) {
		PowerSaveIDLE();
	}

}
