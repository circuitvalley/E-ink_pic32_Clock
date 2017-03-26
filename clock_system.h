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

#ifndef _CLOCK_SYSTEM_H    /* Guard against multiple inclusion */
#define _CLOCK_SYSTEM_H

#include "graphics.h"
#include "rtc.h"
#include "ds18b20.h"
#include "wsensor.h"
#include "uart32.h"

typedef struct {
    samples_t temp_samples[NUMBER_SAMPLE_HISTORY];
    samples_t wtemp_samples[NUMBER_SAMPLE_HISTORY];
    plot_area_t temp_plot;
    plot_area_t wtemp_plot;
    rtc_t time;
    DS_sensor_t tsensor;
    TEMP_wsensor_t wtsensor;
    uart_t uart;
} clock_sys_t;

extern clock_sys_t clock_sys;
#endif /* _CLOCK_SYSTEM_H */

/* *****************************************************************************
 End of File
 */
