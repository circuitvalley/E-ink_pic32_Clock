/*
 * NRF24L01P.c
 *
 *  Created on: Feb 26, 2017
 *      Author: Gaurav
 */
#include "NRF24L01P.h"
#include "spi32.h"

//=============================RF24l01 init config=====================================
char TX_ADDRESS[TX_ADR_WIDTH] = {0x31, 0x73, 0x10, 0x10, 0x01}; //TX address
char RX_ADDRESS[RX_ADR_WIDTH] = {0x34, 0x43, 0x10, 0x10, 0x01}; //RX address
char sta;
char tf, RxBuf[32], TxBuf[32];
//===========================MiniDsp Port IO setup ==========================================

void RF24L01_IO_set(void)
{
	SET_CS_DIR_OUT;
	SET_CE_DIR_OUT;

}
//========================delay for about 5ms=============================================

void ms_delay(void)
{
	unsigned int i = 40000;
	while (i != 0) {
		i--;
	}
}
//=======================delay for about 2*s ops ================================
//******************************************************************************************
// Delay for about n us
//******************************************************************************************

void inerDelay_us(int n)
{
	for (; n > 0; n--);
}

/**************************************************
Function: SPI_Read();

Description:
  Read one byte from nRF24L01 register, 'reg'  */

/**************************************************/
char SPI_Read(char reg)
{
	char reg_val;
	setSPICS(0); // CSN low, initialize SPI communication...
	spiWrite(reg); // Select register to read from..
	reg_val = spiRead(); // ..then read registervalue
	setSPICS(1); // CSN high, terminate SPI communication
	return(reg_val); //  return register value
}
/**************************************************
Function: SPI_RW_Reg();

Description:
  Writes value 'value' to register 'reg' */

/**************************************************/
char SPI_RW_Reg(char reg, char value)
{
	char status1;
	setSPICS(0); // CSN low, init SPI transaction
	status1 = spiTxRX(reg); // select register
	spiWrite(value); // ..and write value to it..
	setSPICS(1); // CSN high again
	return(status1); // return nRF24L01 status uchar
}
/**************************************************
Function: SPI_Read_Buf();

Description:
  Reads 'bytes' #of bytes from register 'reg'
  Typically used to read RX payload, Rx/Tx address */

/**************************************************/
char SPI_Read_Buf(char reg, char *pBuf, char chars)
{
	char status2, uchar_ctr;
	setSPICS(0); // Set CSN low, init SPI tranaction
	status2 = spiTxRX(reg); // Select register to write to and read status uchar
	for (uchar_ctr = 0; uchar_ctr < chars; uchar_ctr++) {
		pBuf[uchar_ctr] = spiRead(); //
	}
	setSPICS(1); // Set CSN high
	return(status2); // return nRF24L01 status uchar
}
/**************************************************
Function: SPI_Write_Buf();

Description:
  Writes 'bytes' from pBuff to register 'chars'
  Typically used to write TX payload, Rx/Tx address */

/**************************************************/
char SPI_Write_Buf(char reg, char *pBuf, char chars)
{
	char status1, uchar_ctr;
	setSPICS(0); // Set CSN low, init SPI tranaction
	status1 = spiTxRX(reg); // Select register to write to and read status byte
	for (uchar_ctr = 0; uchar_ctr < chars; uchar_ctr++) // then write all byte in buffer(*pBuf)
	{
		spiWrite(*pBuf++);

	}
	setSPICS(1); // Set CSN high
	return(status1); //
}
//****************************************************************************************************/
// void SetRX_Mode(void)
//功能：数据接收配置
//****************************************************************************************************/

void SetRX_Mode(void)
{
	NRF_ENABLE(0);
	SPI_RW_Reg(WRITE_REG + CONFIG, 0x0f); // Set PWR_UP bit, enable CRC(2 bytes) & Prim:RX. RX_DR enabled..
	NRF_ENABLE(1); // Set CE pin high to enable RX device
	inerDelay_us(1000); //delay for about 1 second
}
//******************************************************************************************************/
// unsigned char nRF24L01_RxPacket(unsigned char* rx_buf)
// RX a packet
//******************************************************************************************************/

char nRF24L01_RxPacket(char* rx_buf)
{
	char revale = 0;
	sta = SPI_Read(STATUS); // read register STATUS's value
	if (sta & 0x40) // success!
	{
		NRF_ENABLE(0); //SPI enable
		SPI_Read_Buf(RD_RX_PLOAD, rx_buf, TX_PLOAD_WIDTH); // read receive payload from RX_FIFO buffer
		revale = 1; //set flag to 1
	}
	SPI_RW_Reg(WRITE_REG + STATUS, sta);
	return revale;
}
//***********************************************************************************************************
//void nRF24L01_TxPacket(char * tx_buf)
//TX a packet
//**********************************************************************************************************/

void nRF24L01_TxPacket(char * tx_buf)
{
	NRF_ENABLE(0); //StandBy
	SPI_Write_Buf(WRITE_REG + RX_ADDR_P0, TX_ADDRESS, TX_ADR_WIDTH); // RX_Addr0 same as TX_Adr for Auto.Ack
	SPI_Write_Buf(WR_TX_PLOAD, tx_buf, TX_PLOAD_WIDTH); // Writes data to TX payload
	SPI_RW_Reg(WRITE_REG + CONFIG, 0x0e); // Set PWR_UP bit, enable CRC(2 bytes) & Prim:TX. MAX_RT & TX_DS enabled..
	NRF_ENABLE(1); // finish
	inerDelay_us(600);
}
//****************************************************************************************
//NRF24L01 init
//***************************************************************************************/

void initNRF24L01p(void)
{
	inerDelay_us(100);
	NRF_ENABLE(0); // chip enable
	setSPICS(1); // Spi disable
	// Spi clock line init high
	SPI_Write_Buf(WRITE_REG + TX_ADDR, TX_ADDRESS, TX_ADR_WIDTH); // Writes TX_Address to nRF24L01
	SPI_Write_Buf(WRITE_REG + RX_ADDR_P0, RX_ADDRESS, RX_ADR_WIDTH); // RX_Addr0 same as TX_Adr for Auto.Ack
	SPI_RW_Reg(WRITE_REG + EN_AA, 0x01); // enable Auto.Ack:Pipe0
	SPI_RW_Reg(WRITE_REG + EN_RXADDR, 0x01); // Enable Pipe0
	SPI_RW_Reg(WRITE_REG + RF_CH, 0); //   Select channel 0
	SPI_RW_Reg(WRITE_REG + RX_PW_P0, RX_PLOAD_WIDTH);
	SPI_RW_Reg(WRITE_REG + RF_SETUP, 0x07); // TX_PWR:0dBm, Datarate:2Mbps, LNA:HCURR
	SPI_RW_Reg(WRITE_REG + CONFIG, 0x0f); // Set PWR_UP bit, enable CRC(2 bytes) & Prim:RX. RX_DR enabled..
}
//=============================================================================


