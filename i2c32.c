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
#ifdef __XC32
#include <xc.h>          /* Defines special funciton registers, CP0 regs  */
#endif
#include <stdint.h> 

static uint8_t IdleI2C3(void)
{
	uint16_t waiting = 0;
	/* Wait until I2C Bus is Inactive */
	while (I2C3CONbits.SEN || I2C3CONbits.PEN || I2C3CONbits.RSEN || I2C3CONbits.RCEN || I2C3CONbits.ACKEN || I2C3STATbits.TRSTAT) {
		waiting++;
		if (waiting > 0xFF00)
			return 1;
	}
	return 0;
}

void i2c_init()
{
	I2C3BRG = 0x20; //15 for 400 khz 60 for 100
	I2C3CONbits.ON = 1;
}

uint8_t i2cWrite(unsigned char daddr, unsigned char addr, unsigned char *holder, unsigned char n)
{

	I2C3CONbits.SEN = 1; //send start

	if (IdleI2C3()) //wait till it's sent 
		return 0;
	I2C3TRN = daddr << 1; // send device address
	if (IdleI2C3()) //wait till it's sent
		return 0;
	I2C3TRN = addr; //send reg address
	if (IdleI2C3())
		return 0;
	while (n--) {
		I2C3TRN = *holder++; //keep on sending data
		if (IdleI2C3())
			return 0;

	}
	I2C3CONbits.PEN = 1;
	if (IdleI2C3())
		return 0;
	return 1;
}

uint8_t i2cRead(unsigned char daddr, unsigned char addr, unsigned char *holder, unsigned char n)
{
	I2C3ADD = 0x68 << 1;
	I2C3CONbits.SEN = 1;

	if (IdleI2C3())
		return 0;
	I2C3TRN = daddr << 1;
	if (IdleI2C3())
		return 0;
	I2C3TRN = addr;
	if (IdleI2C3())
		return 0;

	I2C3CONbits.RSEN = 1;
	if (IdleI2C3())
		return 0;


	I2C3TRN = (daddr << 1) | 1;
	if (IdleI2C3())
		return 0;

	I2C3CONbits.ACKDT = 0;

	while (n) {

		I2C3CONbits.RCEN = 1;
		if (IdleI2C3())
			return 0;
		n--;
		if (!n)
			I2C3CONbits.ACKDT = 1;
		I2C3CONbits.ACKEN = 1;
		if (IdleI2C3())
			return 0;

		*(holder++) = I2C3RCV;

	}

	I2C3CONbits.PEN = 1;
	if (IdleI2C3())
		return 0;
	return 1;
}