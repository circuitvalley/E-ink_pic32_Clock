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
#include <xc.h>
#include "uart32.h"
#include <stdint.h>
#include "config_interface.h"
#include <string.h>
#include "clock_system.h"

/* This section lists the other files that are included in this file.
 */

int8_t parse_cmd(int8_t *cmdline)
{
	uint8_t length = 0, param_index = 0;
	execute_command cmd_pointer;

	while (cmdline[length] != ' ' && cmdline[length] != '\t' && cmdline[length] != '\r' && length < UART_BUFFER_SIZE) {
		length++;
	}
	if (!(length < UART_BUFFER_SIZE)) {
		return -1;
	}
	if (length > 2) {
		cmd_pointer = search_cmd(cmdline, length);
		if (cmd_pointer > 0) {
			return cmd_pointer(&cmdline[length + 1]);

		}
	}

	return -1;

}

execute_command search_cmd(int8_t *str, uint8_t length)
{
	uint8_t index = 0;
	while (commands[index].cmd_function) {
		if (!strncmp(str, commands[index].cmd_name, length)) {
			return commands[index].cmd_function;
		}
		index++;
	}
	return commands[index].cmd_function; //null pointer
}
#define timeToBuffer(destination,source,index) destination[index]=((source.hour&0xF0)>>4)+0x30 ; \
                                         destination[index+1]= ((source.hour&0x0F))+0x30;  \
                                         destination[index+2]=':'; \
                                         destination[index+3]=((source.min&0xF0)>>4)+0x30 ; \
                                         destination[index+4]=((source.min&0x0F))+0x30;  \
                                         destination[index+5]=':'; \
                                         destination[index+6]=((source.sec&0xF0)>>4)+0x30 ; \
                                         destination[index+7]=((source.sec&0x0F))+0x30;  \
                                         destination[index+8]='\r'; \
                                         destination[index+9]='\n'; \
                                         destination[index+10]=0; 

#define bufferToTime(desti,source) desti.hour=((source[0]&0xF)<<4)|(source[1]&0xF); \
                                   desti.min=((source[3]&0xF)<<4)|(source[4]&0xF); \
                                   desti.sec=((source[6]&0xF)<<4)|(source[7]&0xF); 

int8_t get_time_cmd(int8_t *parameter)
{
	readRTC(&clock_sys.time);
	timeToBuffer(clock_sys.uart.txbuffer, clock_sys.time, 0);
	return 1;
}

int8_t set_time_cmd(int8_t *parameter)
{
	bufferToTime(clock_sys.time, parameter);
	setRTC(&clock_sys.time);
	timeToBuffer(clock_sys.uart.txbuffer, clock_sys.time, 0);
	return 1;
}

int8_t whoareyou_cmd(int8_t *parameter)
{

	strcpy(clock_sys.uart.txbuffer, "cveink\n\r");
	return 1;
}



/* *****************************************************************************
 End of File
 */
