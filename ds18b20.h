/* ************************************************************************** */
/** Descriptive File Name

  @Company
 CircuitValley

  @File Name
    filename.h

  @Summary
    Brief description of the file.

  @Description
    Describe the purpose of this file.
 */

#ifndef _DS1820_H
#define _DS1820_H

#include <stdint.h>         /* For uint32_t definition                        */
#include <xc.h>
/* check configuration of driver */


#define setOneWire(b) LATDbits.LATD4=b;
#define setOneWireDir(b) TRISDbits.TRISD4=b;
#define getOneWire  PORTDbits.RD4
#define setOneWireOC(b)   { ODCDbits.ODCD4=b;}

#define TEMP_RES              0x100 /* temperature resolution => 1/256°C = 0.0039°C */


/* -------------------------------------------------------------------------- */
/*                         DS1820 Timing Parameters                           */
/* -------------------------------------------------------------------------- */
#define PBCLK 32 //in MHz
#define pic32us(a)  a*32

#define DS1820_RST_PULSE       pic32us(480) //480us   /* master reset pulse time in [us] */
#define DS1820_MSTR_BITSTART   pic32us(2)  //2us     /* delay time for bit start by master */
#define DS1820_PRESENCE_WAIT   pic32us(40)     //40    /* delay after master reset pulse in [us] */
#define DS1820_PRESENCE_FIN    pic32us(480) //480   /* dealy after reading of presence pulse [us] */
#define DS1820_BITREAD_DLY     pic32us(5)     //5     /* bit read delay */
#define DS1820_BITWRITE_DLY    pic32us(80) //100   /* bit write delay */


/* -------------------------------------------------------------------------- */
/*                            DS1820 Registers                                */
/* -------------------------------------------------------------------------- */

#define DS1820_REG_TEMPLSB    0
#define DS1820_REG_TEMPMSB    1
#define DS1820_REG_CNTREMAIN  6
#define DS1820_REG_CNTPERSEC  7
#define DS1820_SCRPADMEM_LEN  9     /* length of scratchpad memory */

#define DS1820_ADDR_LEN       8


/* -------------------------------------------------------------------------- */
/*                            DS1820 Commands                                 */
/* -------------------------------------------------------------------------- */

#define DS1820_CMD_SEARCHROM     0xF0
#define DS1820_CMD_READROM       0x33
#define DS1820_CMD_MATCHROM      0x55
#define DS1820_CMD_SKIPROM       0xCC
#define DS1820_CMD_ALARMSEARCH   0xEC
#define DS1820_CMD_CONVERTTEMP   0x44
#define DS1820_CMD_WRITESCRPAD   0x4E
#define DS1820_CMD_READSCRPAD    0xBE
#define DS1820_CMD_COPYSCRPAD    0x48
#define DS1820_CMD_RECALLEE      0xB8


#define DS1820_FAMILY_CODE_DS18B20      0x28
#define DS1820_FAMILY_CODE_DS18S20      0x10

typedef struct DS18B20 {
    uint16_t temp;
    uint8_t ROM[8];
} DS_sensor_t;

void findDS18B20(DS_sensor_t *sensor);

void readTemp(DS_sensor_t *sensor);
/* -------------------------------------------------------------------------- */
/*                            static variables                                */
/* -------------------------------------------------------------------------- */

//static bool bDoneFlag;
//static uint8 nLastDiscrepancy_u8;
//static uint8 nRomAddr_au8[DS1820_ADDR_LEN];


#endif /* _DS1820_H */

