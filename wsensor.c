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


#include <stdint.h>         /* For uint32_t definition                        */
#include <stdbool.h>        /* For true/false definition                      */
#include <sys/attribs.h>
#include "clock_system.h"
#include <xc.h>          /* Defines special funciton registers, CP0 regs  */
#include "wsensor.h"

void parseTempData(int8_t *rx_message, TEMP_wsensor_t *wsensor)
{
	uint8_t string_length = 0;
	uint8_t number_start_index = 0;
	while (rx_message[string_length] != '\r' && string_length < 32) {
		wsensor->temp_ascii[string_length] = rx_message[string_length];
		string_length++;
	}
	if (!(string_length < 32)) {
		wsensor->temp_ascii[0] = 0;
		wsensor->temp_12q4 = 0;
		return;
	}
	wsensor->temp_ascii[string_length] = 0x00;
	string_length = 0;
	while (rx_message[string_length] != '+' && rx_message[string_length] != '-' && string_length < 32) {
		string_length++;
		number_start_index++;
	}

	wsensor->temp_12q4 = 0;
	if (!(string_length < 32)) {
		wsensor->temp_ascii[0] = 0;
		return;
	}
	string_length++;

	while (rx_message[string_length] != '.' && string_length < 32) {
		wsensor->temp_12q4 = wsensor->temp_12q4 * 10 + (rx_message[string_length] - 0x30);
		string_length++;
	}
	wsensor->temp_12q4 = (wsensor->temp_12q4 << 4) | ((rx_message[string_length + 1] - 0x30)&0xF);
	if (rx_message[number_start_index] == '-') {
		wsensor->temp_12q4 = ~wsensor->temp_12q4;
	}
	wsensor->sample_index = 1;
}

/* *****************************************************************************
 End of File
 */
