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
#include <stdint.h>         /* For uint32_t definition                        */
#include "eink_p32.h"

#include <sys/attribs.h> //longramfunc


#include "font/diedie_270.h"
#include "font/diedie_90_font.h"
#include "font/diedie_32.h"
#include "font/diedie_14.h"
#include "graphics.h"
#include "clock_system.h"
#include "delay.h"
#include "font/home_icon.h"
#include "font/home_temp_icon.h"
#include "font/home_temp_out_icon.h"
/* This section lists the other files that are included in this file.
 */
uint8_t framebuffer[FRAMEBUFFER_HEIGHT][FRAMEBUFFER_WIDTH] = {0};


#define  ioEinkWrite(data) { DATA_PORT=data; \
                            LATDbits.LATD5=data>>2; \
                            SCLOCK; \
                            }

__longramfunc__ void ioEinkClk(int i)
{
	while (i--) {
		SCLOCK
	}
}

__longramfunc__ ioEinkVscanStop()
{
	CKV_CLOCK;
	CKV_CLOCK;
	CKV_CLOCK;
	CKV_CLOCK;
	CKV_CLOCK;
	CKV_CLOCK;
	CKV_CLOCK;
	CKV_CLOCK;
	CKV_CLEAR;
	delay_cycles(375);
	CKV_SET;
	delay_cycles(53);
}

void __longramfunc__ ioEinkVscanWrite()
{
	CL_SET;
	CKV_CLEAR;
	CL_CLEAR;
	//OE_CLEAR; //not happning
	delay_cycles(34); //1.27 us
	//	OE_SET; //not happning
	CKV_SET;
	delay_cycles(24); //1.44 us
	LE_SET;
	LE_CLEAR;
	delay_cycles(24);
}

void __longramfunc__ initEink(void)
{
	TRISE = TRISE & 0xFF00;
	TRISB = TRISB & 0xFFF0;
	TRISBbits.TRISB11 = 0;
	TRISDbits.TRISD5 = 0;
	TRISDbits.TRISD6 = 0;
	TRISDbits.TRISD7 = 0;
	TRISDbits.TRISD8 = 1;
	TRISBbits.TRISB10 = 0;
	TRISDbits.TRISD9 = 0;

	T1CONbits.ON = 1; //enable timer used for delay 

	REG_SHDN(1);
	delay_cycles(1000);
	SHR_SET;


	delay_cycles(65530);
	cleanink(1); //cleanup 
	cleanink(1);
	cleanink(0);
	cleanink(0);
	cleanink(1);
	cleanink(1);
	cleanink(0);
	cleanink(0);
	cleanink(1);
	cleanink(1);

	cleanink(1);
	cleanink(1);

	cleanink(1);
	cleanink(1);

	OE_CLEAR;
	GMOD_CLEAR;
}

__longramfunc__ void ioEinkVscanStart(void)
{
	GMOD_SET;
	CKV_SET;
	OE_SET;
	LE_CLEAR;
	delay_cycles(800); //100 us
	SPV_SET;
	delay_cycles(800); //100 us



	SPV_CLEAR;
	delay_cycles(24); //1 us //1us
	CKV_CLEAR;
	delay_cycles(671); //25 us //28 us noted
	CKV_SET;
	delay_cycles(24); //1 us //1us
	SPV_SET;
	delay_cycles(671); //25 us //26 noted
	CKV_CLOCK;
	delay_cycles(671); //25 us //26 noted
	CKV_CLOCK;
	delay_cycles(671); //25 us //27.8
	CKV_CLOCK;


}

__longramfunc__ void cleanink(unsigned char bw)
{
	unsigned int j, i, data;

	data = bw ? 0xAA : 0x55;
	DATA_PORT = data;
	LATDbits.LATD5 = data >> 2;
	for (j = 0; j < 5; j++) {
		ioEinkVscanStart();
		for (i = 0; i < 600; i++) {
			ioEinkHscanStart();
			ioEinkClk(800 / 4);
			ioEinkHscanStop();
			ioEinkVscanWrite();
		}
		ioEinkVscanStop();
	}
}
unsigned char transform[16] = {//nibble to 4 pixel each pixel 2bits 0b10 -->black 0b01-->white
	0b10101010,
	0b10101001,
	0b10100110,
	0b10100101,
	0b10011010,
	0b10011001,
	0b10010110,
	0b10010101,
	0b01101010,
	0b01101001,
	0b01100110,
	0b01100101,
	0b01011010,
	0b01011001,
	0b01010110,
	0b01010101
};

__longramfunc__ void flushbuffer()
{
	unsigned int h;
	unsigned char w;
	ioEinkVscanStart();
	for (h = 0; h < 600; h++) {
		ioEinkHscanStart();
		for (w = 0; w < 100; w++) {
			ioEinkWrite(transform[(framebuffer[h][w]&0xF0) >> 4]);
			ioEinkWrite(transform[(framebuffer[h][w]&0x0F)]);
		}
		ioEinkHscanStop();
		ioEinkVscanWrite();
	}
	ioEinkVscanStop();
}

__longramfunc__ void cleanfb()
{
	unsigned int h;
	unsigned char w;
	for (h = 0; h < 600; h++) {
		for (w = 0; w < 100; w++)
			framebuffer[h][w] = 0x00;

	}
}

__longramfunc__ void putpixel(uint16_t x, uint16_t y, uint8_t thick)
{

	if (thick) {
		if (y > 0)
			framebuffer[y - 1][x >> 3] = (framebuffer[y - 1][x >> 3]) | ((0x80)>>(x & 0x7));
		if (y < EINK_MAXY)
			framebuffer[y + 1][x >> 3] = (framebuffer[y + 1][x >> 3]) | ((0x80)>>(x & 0x7));
	}
	framebuffer[y][x >> 3] = (framebuffer[y][x >> 3]) | ((0x80)>>(x & 0x7));

}

__longramfunc__ void refresh_display()
{
	unsigned char buffer[5];
	uint32_t i;

	buffer[0] = (clock_sys.time.hour & 0xF0) >> 4;
	buffer[1] = (clock_sys.time.hour & 0x0F);
	buffer[2] = 10;
	buffer[3] = (clock_sys.time.min & 0xF0) >> 4;
	buffer[4] = (clock_sys.time.min & 0x0F);
	cleanfb();
	if (buffer[0] == 0) {
		buffer[0] = buffer[1];
		buffer[1] = buffer[2];
		buffer[2] = buffer[3];
		buffer[3] = buffer[4];
		printstring(buffer, 120, 0, 4);
	} else {
		printstring(buffer, 5, 0, 5);
	}


	buffer[0] = clock_sys.wtsensor.temp_ascii[2]; //TODO fix me 
	buffer[1] = clock_sys.wtsensor.temp_ascii[3];
	buffer[2] = 11;
	buffer[3] = clock_sys.wtsensor.temp_ascii[5];
	buffer[4] = 10;
	draw_bitmap(&home_outdoor_temp, 560, 425);
	if ((buffer[0]&0xF) > 9)
		printstring_small(&buffer[1], 555, 495, 4);
	else
		printstring_small(buffer, 555, 495, 5);

	i = clock_sys.tsensor.temp;

	i = (i * 625) / 1000;
	if (clock_sys.temp_plot.asize > (clock_sys.temp_plot.size - 1)) {
		shiftdata(clock_sys.temp_plot.samples, clock_sys.temp_plot.asize);
		clock_sys.temp_plot.asize = clock_sys.temp_plot.size - 1;
	}
	clock_sys.temp_plot.samples[clock_sys.temp_plot.asize].attribute = (clock_sys.temp_plot.asize) + 1;
	clock_sys.temp_plot.samples[clock_sys.temp_plot.asize++].data = i;


	if (clock_sys.wtemp_plot.asize > (clock_sys.wtemp_plot.size - 1)) {
		shiftdata(clock_sys.wtemp_plot.samples, clock_sys.wtemp_plot.asize);
		clock_sys.wtemp_plot.asize = clock_sys.wtemp_plot.size - 1;
	}
	clock_sys.wtemp_plot.samples[clock_sys.wtemp_plot.asize].attribute = (clock_sys.wtemp_plot.asize) + 1;
	clock_sys.wtemp_plot.samples[clock_sys.wtemp_plot.asize++].data = ((clock_sys.wtsensor.temp_12q4 >> 4)*10)+(clock_sys.wtsensor.temp_12q4 & 0xF); //convert from Q12.4 to simple decimal for the sake of plotting

	buffer[4] = 10;

	buffer[3] = i % 10;

	buffer[2] = 11;
	i = i / 10;
	buffer[1] = i % 10;
	i = i / 10;
	buffer[0] = i % 10;
	draw_bitmap(&home_indoor_temp, 560, 250);

	printstring_small(buffer, 555, 320, 5);
	plot_graph(&clock_sys.temp_plot);
	plot_graph(&clock_sys.wtemp_plot);

	printTick(clock_sys.temp_plot.tick_x);
	printTick(clock_sys.temp_plot.tick_y);
	printTick(clock_sys.wtemp_plot.tick_y);
	REG_SHDN(1);
	OE_SET;
	GMOD_SET;

	cleanink(1);
	cleanink(1);
	cleanink(1);
	cleanink(0);
	cleanink(0);
	cleanink(1);
	cleanink(1);
	cleanink(0);
	cleanink(0);
	cleanink(1);
	cleanink(1); //eink need some to get completely into color , so how many time you clean eink to be white will decide what is the contrast  
	cleanink(1);
	cleanink(1);
	cleanink(1);
	cleanink(1);
	cleanink(1);
	cleanink(1);
	for (i = 0; i < 5; i++) {
		flushbuffer();
	}
	OE_CLEAR;
	GMOD_CLEAR;
	REG_SHDN(0);
}


/* *****************************************************************************
 End of File
 */
