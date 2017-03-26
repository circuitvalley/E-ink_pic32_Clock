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

#ifndef _DELAY_H    /* Guard against multiple inclusion */
#define _DELAY_H
#include <stdint.h>         /* For uint32_t definition                        */
#include <xc.h>

void delay_cycles(uint16_t cycles);

void delay_long(uint16_t cycles);

#endif /* _DELAY_H */

/* *****************************************************************************
 End of File
 */
