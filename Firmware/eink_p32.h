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

#ifndef _EINK_P32_H    /* Guard against multiple inclusion */
#define _EINK_P32_H
#include <sys/attribs.h>

#define FRAMEBUFFER_WIDTH 800/8
#define FRAMEBUFFER_HEIGHT 600

extern uint8_t framebuffer[FRAMEBUFFER_HEIGHT][FRAMEBUFFER_WIDTH];


#define EINK_MAXX 799 //800 pixel
#define EINK_MAXY 598 //600 pixel

#define DATA_PORT LATE



#define CL_SET 		LATBbits.LATB2=1
#define CL_CLEAR 	LATBbits.LATB2=0

#define LE_SET 		LATBbits.LATB10=1
#define LE_CLEAR	LATBbits.LATB10=0

#define OE_SET		LATDbits.LATD7=1
#define OE_CLEAR	LATDbits.LATD7=0


#define CKV_SET		LATBbits.LATB0=1
#define CKV_CLEAR	LATBbits.LATB0=0

#define GMOD_SET	LATBbits.LATB3=1
#define	GMOD_CLEAR	LATBbits.LATB3=0

#define SPH_SET		LATDbits.LATD9=1
#define SPH_CLEAR	LATDbits.LATD9=0

#define SHR_SET		LATBbits.LATB11=1
#define SHR_CLEAR	LATBbits.LATB11=0

#define SPV_SET		LATBbits.LATB1=1
#define SPV_CLEAR	LATBbits.LATB1=0

#define REG_SHDN(b)    LATDbits.LATD6=b;

#define GDISP_SCREEN_HEIGHT 600
#define CKV_CLOCK     { CKV_CLEAR;	\
                        delay_cycles(24); \
                        CKV_SET;  \
                        delay_cycles(24);   \
                      }

#define SCLOCK        {  CL_SET;    \
                         CL_CLEAR;  \
                      }


#define ioEinkHscanStop() SPH_SET;               /*marco for little more optimization , inline may not work 
                                                beucase when you haave inline i think you can not have function in RAM , 
                                                and these intruction we need them to be in RAM */

#define ioEinkHscanStart() {	SCLOCK;     \
                                SCLOCK;     \
                                SPH_CLEAR;  \
                           }

void __longramfunc__ initEink(void);
__longramfunc__ void cleanink(unsigned char bw);
__longramfunc__ void refresh_display();
__longramfunc__ void putpixel(uint16_t x, uint16_t y, uint8_t thick);
__longramfunc__ void cleanfb();
__longramfunc__ void flushbuffer();
__longramfunc__ void cleanink(unsigned char bw);
__longramfunc__ void ioEinkVscanStart(void);



#endif /* _EINK_P32_H */

/* *****************************************************************************
 End of File
 */
