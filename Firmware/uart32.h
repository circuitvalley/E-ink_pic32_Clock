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

#ifndef _UART32_H    /* Guard against multiple inclusion */
#define _UART32_H
#include <stdint.h>
#define UART_BUFFER_SIZE 64

typedef struct {
    int8_t rxbuffer[UART_BUFFER_SIZE];
    int8_t txbuffer[UART_BUFFER_SIZE];
} uart_t;



void initUART(void);
void sendStringOverUART(char *str);
void sendOverUART(uint8_t data);

#endif /* _EXAMPLE_FILE_NAME_H */

/* *****************************************************************************
 End of File
 */
