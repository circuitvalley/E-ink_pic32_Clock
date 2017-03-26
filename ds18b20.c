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
#include "ds18b20.h"
#include "delay.h"
#include <stdint.h>         /* For uint32_t definition                        */
#include <stdbool.h>        /* For true/false definition                      */
#include <sys/attribs.h>
#include "clock_system.h"
#include <xc.h>          /* Defines special funciton registers, CP0 regs  */

void initOneWire()
{
	setOneWireDir(0);
	setOneWireOC(1);
	setOneWire(1);
	T1CONbits.SIDL = 1;
	T1CONbits.ON = 1;

	findDS18B20(&clock_sys.tsensor);



}



/*******************************************************************************
 * FUNCTION:   delay_cycles
 * PURPOSE:    Delay for the given number of micro seconds.
 *
 * INPUT:      dly_us      number of micro seconds to delay
 * OUTPUT:     -
 * RETURN:     -
 ******************************************************************************/


/*******************************************************************************
 * FUNCTION:   DS1820_DelayMs
 * PURPOSE:    Delay for the given number of milliseconds.
 *
 * INPUT:      dly_ms      number of milliseconds to delay
 * OUTPUT:     -
 * RETURN:     -
 ******************************************************************************/
#define DS1820_DelayMs(dly_ms)   delay_ms(dly_ms)

/*******************************************************************************
 * restDS18B20
 * sends reset pluse
 * returns if any device present on bus     
 ******************************************************************************/
__longramfunc__ bool resetDS18B20(void)
{
	bool bPresPulse;
	//disable int
	/* reset pulse */
	setOneWire(0);
	delay_cycles(DS1820_RST_PULSE);
	setOneWire(1);

	/* wait until pullup pull 1-wire bus to high */
	delay_cycles(DS1820_PRESENCE_WAIT);

	/* get presence pulse */
	bPresPulse = getOneWire;

	delay_cycles(DS1820_RST_PULSE); //4244

	//DS1820_EnableInterrupts();
	//enable int
	return bPresPulse;
}

/*******************************************************************************
 * FUNCTION:   DS1820_ReadByte
 * PURPOSE:    Reads a single byte from the DS1820 device.
 *
 * INPUT:      -
 * OUTPUT:     -
 * RETURN:     uint8_t          byte which has been read from the DS1820
 ******************************************************************************/
__longramfunc__ uint8_t readDS18B20(void)
{
	uint8_t i;
	uint8_t value = 0;

	for (i = 0; i < 8; i++) {

		//int    
		setOneWire(0);
		delay_cycles(DS1820_MSTR_BITSTART);
		setOneWire(1);
		delay_cycles(DS1820_BITREAD_DLY);
		//int
		value |= (getOneWire << i);

		delay_cycles(120 * 32);
	}
	return(value);
}

/*******************************************************************************
 * FUNCTION:   DS1820_WriteByte
 * PURPOSE:    Writes a single byte to the DS1820 device.
 *
 * INPUT:      val_u8         byte to be written
 * OUTPUT:     -
 * RETURN:     -
 ******************************************************************************/
__longramfunc__ void writeDS18B20(uint8_t data)
{
	uint8_t i;
	for (i = 0; i < 8; i++) /* writes byte, one bit at a time */ {
		//int dis
		setOneWire(0);
		delay_cycles(DS1820_MSTR_BITSTART);
		setOneWire(data & 0x1);
		delay_cycles(DS1820_BITWRITE_DLY);
		setOneWire(1);
		data = data >> 1;
		delay_cycles(DS1820_BITREAD_DLY);

		//inten
	}
}



/* -------------------------------------------------------------------------- */
/*                             API Interface                                  */
/* -------------------------------------------------------------------------- */


///*******************************************************************************
// * FUNCTION:   DS1820_AddrDevice
// * PURPOSE:    Addresses a single or all devices on the 1-wire bus.
// *
// * INPUT:      nAddrMethod       use DS1820_CMD_MATCHROM to select a single
// *                               device or DS1820_CMD_SKIPROM to select all
// * OUTPUT:     -
// * RETURN:     -
// ******************************************************************************/
//void DS1820_AddrDevice(uint8_t nAddrMethod)
//{
//   uint8_t i;
//   
//   if (nAddrMethod == DS1820_CMD_MATCHROM)
//   {
//      DS1820_WriteByte(DS1820_CMD_MATCHROM);     /* address single devices on bus */
//      for (i = 0; i < DS1820_ADDR_LEN; i ++)
//      {
//         DS1820_WriteByte(nRomAddr_au8[i]);
//      }
//   }
//   else
//   {
//      DS1820_WriteByte(DS1820_CMD_SKIPROM);     /* address all devices on bus */
//   }
//}

void findDS18B20(DS_sensor_t *sensor)
{
	uint8_t i;
	resetDS18B20();
	writeDS18B20(DS1820_CMD_READROM);
	for (i = 0; i < 8; i++)
		sensor->ROM[i] = readDS18B20();
}

void readTemp(DS_sensor_t *sensor)
{
	uint8_t i;
	resetDS18B20();
	writeDS18B20(DS1820_CMD_MATCHROM);
	for (i = 0; i < 8; i++)
		writeDS18B20(sensor->ROM[i]);
	writeDS18B20(DS1820_CMD_CONVERTTEMP);
	setOneWire(1);
	setOneWireOC(0);
	//wait hier for 750ms
	delay_long(62500);
	delay_long(31250);
	setOneWireOC(1);
	resetDS18B20();
	writeDS18B20(DS1820_CMD_MATCHROM);

	for (i = 0; i < 8; i++)
		writeDS18B20(sensor->ROM[i]);

	writeDS18B20(DS1820_CMD_READSCRPAD);
	sensor->temp = readDS18B20();
	sensor->temp = sensor->temp | (readDS18B20() << 8);
	for (i = 0; i < 6; i++)
		readDS18B20();



}
//
///*******************************************************************************
// * FUNCTION:   DS1820_FindNextDevice
// * PURPOSE:    Finds next device connected to the 1-wire bus.
// *
// * INPUT:      -
// * OUTPUT:     nRomAddr_au8[]       ROM code of the next device
// * RETURN:     bool                 TRUE if there are more devices on the 1-wire
// *                                  bus, FALSE otherwise
// ******************************************************************************/
//bool DS1820_FindNextDevice(void)
//{
//    uint8_t state_u8;
//    uint8_t byteidx_u8;
//    uint8_t mask_u8 = 1;
//    uint8_t bitpos_u8 = 1;
//    uint8_t nDiscrepancyMarker_u8 = 0;
//    bool bit_b;
//    bool bStatus;
//    bool next_b = FALSE;
//
//    /* init ROM address */
//    for (byteidx_u8=0; byteidx_u8 < 8; byteidx_u8 ++)
//    {
//        nRomAddr_au8[byteidx_u8] = 0x00;
//    }
//
//    bStatus = DS1820_Reset();        /* reset the 1-wire */
//
//    if (bStatus || bDoneFlag)        /* no device found */
//    {
//        nLastDiscrepancy_u8 = 0;     /* reset the search */
//        return FALSE;
//    }
//
//    /* send search rom command */
//    DS1820_WriteByte(DS1820_CMD_SEARCHROM);
//
//    byteidx_u8 = 0;
//    do
//    {
//        state_u8 = 0;
//
//        /* read bit */
//        if ( DS1820_ReadBit() != 0 )
//        {
//            state_u8 = 2;
//        }
//        delay_cycles(120);
//
//        /* read bit complement */
//        if ( DS1820_ReadBit() != 0 )
//        {
//            state_u8 |= 1;
//        }
//        delay_cycles(120);
//
//        /* description for values of state_u8: */
//        /* 00    There are devices connected to the bus which have conflicting */
//        /*       bits in the current ROM code bit position. */
//        /* 01    All devices connected to the bus have a 0 in this bit position. */
//        /* 10    All devices connected to the bus have a 1 in this bit position. */
//        /* 11    There are no devices connected to the 1-wire bus. */
//
//        /* if there are no devices on the bus */
//        if (state_u8 == 3)
//        {
//            break;
//        }
//        else
//        {
//            /* devices have the same logical value at this position */
//            if (state_u8 > 0)
//            {
//                /* get bit value */
//                bit_b = (bool)(state_u8 >> 1);
//            }
//            /* devices have confilcting bits in the current ROM code */
//            else
//            {
//                /* if there was a conflict on the last iteration */
//                if (bitpos_u8 < nLastDiscrepancy_u8)
//                {
//                    /* take same bit as in last iteration */
//                    bit_b = ( (nRomAddr_au8[byteidx_u8] & mask_u8) > 0 );
//                }
//                else
//                {
//                    bit_b = (bitpos_u8 == nLastDiscrepancy_u8);
//                }
//
//                if (bit_b == 0)
//                {
//                    nDiscrepancyMarker_u8 = bitpos_u8;
//                }
//            }
//
//            /* store bit in ROM address */
//           if (bit_b != 0)
//           {
//               nRomAddr_au8[byteidx_u8] |= mask_u8;
//           }
//           else
//           {
//               nRomAddr_au8[byteidx_u8] &= ~mask_u8;
//           }
//
//           DS1820_WriteBit(bit_b);
//
//           /* increment bit position */
//           bitpos_u8 ++;
//
//           /* calculate next mask value */
//           mask_u8 = mask_u8 << 1;
//
//           /* check if this byte has finished */
//           if (mask_u8 == 0)
//           {
//               byteidx_u8 ++;  /* advance to next byte of ROM mask */
//               mask_u8 = 1;    /* update mask */
//           }
//        }
//    } while (byteidx_u8 < DS1820_ADDR_LEN);
//
//
//    /* if search was unsuccessful then */
//    if (bitpos_u8 < 65)
//    {
//        /* reset the last discrepancy to 0 */
//        nLastDiscrepancy_u8 = 0;
//    }
//    else
//    {
//        /* search was successful */
//        nLastDiscrepancy_u8 = nDiscrepancyMarker_u8;
//        bDoneFlag = (nLastDiscrepancy_u8 == 0);
//
//        /* indicates search is not complete yet, more parts remain */
//        next_b = TRUE;
//    }
//
//    return next_b;
//}
//
//
///*******************************************************************************
// * FUNCTION:   DS1820_FindFirstDevice
// * PURPOSE:    Starts the device search on the 1-wire bus.
// *
// * INPUT:      -
// * OUTPUT:     nRomAddr_au8[]       ROM code of the first device
// * RETURN:     bool                 TRUE if there are more devices on the 1-wire
// *                                  bus, FALSE otherwise
// ******************************************************************************/
//bool DS1820_FindFirstDevice(void)
//{
//    nLastDiscrepancy_u8 = 0;
//    bDoneFlag = FALSE;
//
//    return ( DS1820_FindNextDevice() );
//}
//
//
///*******************************************************************************
// * FUNCTION:   DS1820_WriteEEPROM
// * PURPOSE:    Writes to the DS1820 EEPROM memory (2 bytes available).
// *
// * INPUT:      nTHigh         high byte of EEPROM
// *             nTLow          low byte of EEPROM
// * OUTPUT:     -
// * RETURN:     -
// ******************************************************************************/
//void DS1820_WriteEEPROM(uint8_t nTHigh, uint8_t nTLow)
//{
//    /* --- write to scratchpad ----------------------------------------------- */
//    DS1820_Reset();
//    DS1820_AddrDevice(DS1820_CMD_MATCHROM);
//    DS1820_WriteByte(DS1820_CMD_WRITESCRPAD); /* start conversion */
//    DS1820_WriteByte(nTHigh);
//    DS1820_WriteByte(nTLow);
//
//    delay_cycles(10);
//
//    DS1820_Reset();
//    DS1820_AddrDevice(DS1820_CMD_MATCHROM);
//    DS1820_WriteByte(DS1820_CMD_COPYSCRPAD); /* start conversion */
//
//    delay_ms(10);
//}
//
//
///*******************************************************************************
// * FUNCTION:   DS1820_GetTempRaw
// * PURPOSE:    Get temperature raw value from single DS1820 device.
// *
// *             Scratchpad Memory Layout
// *             Byte  Register
// *             0     Temperature_LSB
// *             1     Temperature_MSB
// *             2     Temp Alarm High / User Byte 1
// *             3     Temp Alarm Low / User Byte 2
// *             4     Reserved
// *             5     Reserved
// *             6     Count_Remain
// *             7     Count_per_C
// *             8     CRC
// *
// *             Temperature calculation for DS18S20 (Family Code 0x10):
// *             =======================================================
// *                                             (Count_per_C - Count_Remain)
// *             Temperature = temp_raw - 0.25 + ----------------------------
// *                                                     Count_per_C
// *
// *             Where temp_raw is the value from the temp_MSB and temp_LSB with
// *             the least significant bit removed (the 0.5C bit).
// *
// *
// *             Temperature calculation for DS18B20 (Family Code 0x28):
// *             =======================================================
// *                      bit7   bit6   bit5   bit4   bit3   bit2   bit1   bit0
// *             LSB      2^3    2^2    2^1    2^0    2^-1   2^-2   2^-3   2^-4
// *                      bit15  bit14  bit13  bit12  bit3   bit2   bit1   bit0
// *             MSB      S      S      S      S      S      2^6    2^5    2^4
// *
// *             The temperature data is stored as a 16-bit sign-extended two?s
// *             complement number in the temperature register. The sign bits (S)
// *             indicate if the temperature is positive or negative: for
// *             positive numbers S = 0 and for negative numbers S = 1.
// *
// * RETURN:     int16_t         raw temperature value with a resolution
// *                            of 1/256°C
// ******************************************************************************/
//int16_t DS1820_GetTempRaw(void)
//{   
//    uint8_t i;
//    uint8_t temp_u16;
//    uint8_t highres_u16;
//    uint8_t scrpad[DS1820_SCRPADMEM_LEN];
//
//    /* --- start temperature conversion -------------------------------------- */
//    DS1820_Reset();
//    DS1820_AddrDevice(DS1820_CMD_MATCHROM);     /* address the device */
//    setOneWire(1);
//    DS1820_WriteByte(DS1820_CMD_CONVERTTEMP);   /* start conversion */
//    //DS1820_DelayMs(DS1820_TEMPCONVERT_DLY);   /* wait for temperature conversion */
//    DS1820_DelayMs(750);
//
//
//    /* --- read sratchpad ---------------------------------------------------- */
//    DS1820_Reset();
//    DS1820_AddrDevice(DS1820_CMD_MATCHROM);   /* address the device */
//    DS1820_WriteByte(DS1820_CMD_READSCRPAD);  /* read scratch pad */
//
//    /* read scratch pad data */
//    for (i=0; i < DS1820_SCRPADMEM_LEN; i++)
//    {
//        scrpad[i] = DS1820_ReadByte();
//    }
//
//
//    /* --- calculate temperature --------------------------------------------- */
//    /* Formular for temperature calculation: */
//    /* Temp = Temp_read - 0.25 + ((Count_per_C - Count_Remain)/Count_per_C) */
//
//    /* get raw value of temperature (0.5°C resolution) */
//    temp_u16 = 0;
//    temp_u16 = (uint8_t)((uint8_t)scrpad[DS1820_REG_TEMPMSB] << 8);
//    temp_u16 |= (uint8_t)(scrpad[DS1820_REG_TEMPLSB]);
//
//    if (nRomAddr_au8[0] == DS1820_FAMILY_CODE_DS18S20)
//    {
//        /* get temperature value in 1°C resolution */
//        temp_u16 >>= 1;
//    
//        /* temperature resolution is TEMP_RES (0x100), so 1°C equals 0x100 */
//        /* => convert to temperature to 1/256°C resolution */
//        temp_u16 = ((uint8_t)temp_u16 << 8);
//    
//        /* now substract 0.25°C */
//        temp_u16 -= ((uint8_t)TEMP_RES >> 2);
//    
//        /* now calculate high resolution */
//        highres_u16 = scrpad[DS1820_REG_CNTPERSEC] - scrpad[DS1820_REG_CNTREMAIN];
//        highres_u16 = ((uint8_t)highres_u16 << 8);
//        if (scrpad[DS1820_REG_CNTPERSEC])
//        {
//            highres_u16 = highres_u16 / (uint8_t)scrpad[DS1820_REG_CNTPERSEC];
//        }
//    
//        /* now calculate result */
//        highres_u16 = highres_u16 + temp_u16;
//    }
//    else
//    {
//        /* 12 bit temperature value has 0.0625°C resolution */
//        /* shift left by 4 to get 1/256°C resolution */
//        highres_u16 = temp_u16;
//        highres_u16 <<= 4;
//    }
//
//    return (highres_u16);
//}
//
//
///*******************************************************************************
// * FUNCTION:   DS1820_GetTempFloat
// * PURPOSE:    Converts internal temperature value to string (physical value).
// *
// * INPUT:      none
// * OUTPUT:     none
// * RETURN:     float          temperature value with as float value
// ******************************************************************************/
//float DS1820_GetTempFloat(void)
//{
//    return ((float)DS1820_GetTempRaw() / (float)TEMP_RES);
//}
//
//
///*******************************************************************************
// * FUNCTION:   DS1820_GetTempString
// * PURPOSE:    Converts internal temperature value to string (physical value).
// *
// * INPUT:      tRaw_s16       internal temperature value
// * OUTPUT:     strTemp_pc     user string buffer to write temperature value
// * RETURN:     int16_t         temperature value with an internal resolution
// *                            of TEMP_RES
// ******************************************************************************/
//void DS1820_GetTempString(int16_t tRaw_s16, char *strTemp_pc)
//{
//    int16_t tPhyLow_s16;
//    int16_t tPhy_s8;
//
//    /* convert from raw value (1/256°C resolution) to physical value */
//    tPhy_s8 = (int16_t)(tRaw_s16/TEMP_RES);
//
//    /* convert digits from raw value (1/256°C resolution) to physical value */
//    /*tPhyLow_u16 = tInt_s16 % TEMP_RES;*/
//    tPhyLow_s16 = tRaw_s16 & 0xFF;      /* this operation is the same as */
//                                        /* but saves flash memory tInt_s16 % TEMP_RES */
//    tPhyLow_s16 = tPhyLow_s16 * 100;
//    tPhyLow_s16 = (uint8_t)tPhyLow_s16 / TEMP_RES;
//
//    /* write physical temperature value to string */
//    //sprintf(strTemp_pc, "%d.%02d", tPhy_s8, (int16_t)tPhyLow_s16);
//}

/* *****************************************************************************
 End of File
 */
