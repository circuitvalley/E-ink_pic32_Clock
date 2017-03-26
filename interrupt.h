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

#ifndef _INTERRUPT_H    /* Guard against multiple inclusion */
#define _INTERRUPT_H

typedef enum {
    // Configure the interrupt module for Mult-vector mode.
    INT_SYSTEM_CONFIG_MULT_VECTOR,
    // Configure the interrupt module for Single Vectored mode.
    INT_SYSTEM_CONFIG_SINGLE_VECTOR
} INT_SYSTEM_CONFIG;
void __attribute__((nomips16)) INTConfigureSystem(INT_SYSTEM_CONFIG config);
unsigned int __attribute__((nomips16)) INTEnableInterrupts(void);

void initExternalInterrupts();

#endif /* _EXAMPLE_FILE_NAME_H */

/* *****************************************************************************
 End of File
 */
