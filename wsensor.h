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

#ifndef _WSENSOR_H    /* Guard against multiple inclusion */
#define _WSENSOR_H

#include <stdint.h>         /* For uint32_t definition                        */
#include <stdbool.h>        /* For true/false definition                      */
#include <sys/attribs.h>
#include "clock_system.h"
#include <xc.h>          /* Defines special funciton registers, CP0 regs  */

typedef struct {
    int16_t temp_12q4;
    int8_t temp_ascii[10];
    int16_t sample_index;

} TEMP_wsensor_t;

void parseTempData(int8_t *rx_message, TEMP_wsensor_t *wsensor);



#endif /* _EXAMPLE_FILE_NAME_H */

/* *****************************************************************************
 End of File
 */
