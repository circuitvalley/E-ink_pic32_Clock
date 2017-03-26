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

#ifndef _I2C32_H_    /* Guard against multiple inclusion */
#define _I2C32_H_
void i2c_init();
uint8_t i2cWrite(unsigned char daddr, unsigned char addr, unsigned char *, unsigned char n);
uint8_t i2cRead(unsigned char daddr, unsigned char addr, unsigned char *holder, unsigned char n);

#endif /* _I2C32_H_ */

/* *****************************************************************************
 End of File
 */
