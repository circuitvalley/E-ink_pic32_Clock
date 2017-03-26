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
#include <stdint.h>         /* For uint32_t definition                        */
#include <sys/attribs.h> //longramfunc
#include "rtc.h"
#include "i2c32.h"
#include "clock_system.h"
#include "eink_p32.h"

void initRTC(void)
{
	uint8_t buff[2];
	buff[0] = 0x06; //enable osc, set int to alarm , enable alarm 1;
	buff[1] = 0;
	i2cWrite(0x68, 0x0E, buff, 2);
	clearAlarm();
	if (PORTFbits.RF0 == 0) //reset time with switch press on startup if really needed
	{
		clock_sys.time.min = 0;
		clock_sys.time.hour = 0;
		clock_sys.time.sec = 0;
		setRTC(&clock_sys.time);

	}
}

void readRTC(rtc_t *cl)
{
	uint8_t buff[7];
	i2cRead(DS3231_ADDRESS, 0, buff, 7);
	cl->sec = buff[0];
	cl->min = buff[1];
	cl->hour = buff[2];
	cl->day = buff[3];
	cl->date = buff[4];
	cl->month = buff[5];
	cl->year = buff[6];
}

void setalarm1(rtc_t *cl)
{
	uint8_t buff[7];
	buff[0] = cl->sec;
	buff[1] = cl->min;
	buff[2] = cl->hour & (~(1 << 6));
	buff[3] = cl->date & (~(1 << 6));
	i2cWrite(DS3231_ADDRESS, 0x07, buff, 5);

}

void setRTCalarm_per_minute()
{
	uint8_t buff[3];
	buff[0] = 0x80;
	buff[1] = 0x80;
	buff[2] = 0x80;
	i2cWrite(DS3231_ADDRESS, 0x0B, buff, 3);
}

void clearAlarm()
{
	uint8_t buff;
	buff = 0;
	i2cWrite(DS3231_ADDRESS, 0x0F, &buff, 1);

}

uint16_t getTemp()
{
	uint8_t buff[2];
	i2cRead(DS3231_ADDRESS, 0x11, buff, 2);
	return((buff[0] << 2) | buff[1] >> 6);
}

void setRTC(rtc_t *cl)
{
	uint8_t buff[7];
	buff[0] = cl->sec;
	buff[1] = cl->min;
	buff[2] = cl->hour & (~(1 << 6));
	buff[3] = cl->day;
	buff[4] = cl->date;
	buff[5] = cl->month;
	buff[6] = cl->year;
	i2cWrite(DS3231_ADDRESS, 0x00, buff, 7);

}

void RTC_ISR()
{

	readTemp(&clock_sys.tsensor);
	readRTC(&clock_sys.time);
	clearAlarm();
	refresh_display();

}
/* *****************************************************************************
 End of File
 */
