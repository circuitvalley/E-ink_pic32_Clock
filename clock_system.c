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
#include "clock_system.h"
#include "graphics.h"

//axis , point to be same, line height , line width,point

#define PLOT_START_X    40
#define PLOT_START_Y    264
#define PLOT_HEIGHT     320
#define PLOT_WIDTH      460         

#define tick_init(b,d,a,c,pt )      .tick_ ## b[0]={.val=0,.h=a,.w=c,.point.d=pt}, \
                              .tick_ ## b[1]={.val=0,.h=a,.w=c,.point.d=pt}, \
                              .tick_ ## b[2]={.val=0,.h=a,.w=c,.point.d=pt}, \
                              .tick_ ## b[3]={.val=0,.h=a,.w=c,.point.d=pt}, \
                              .tick_ ## b[4]={.val=0,.h=a,.w=c,.point.d=pt}, \
                              .tick_ ## b[5]={.val=0,.h=a,.w=c,.point.d=pt}
//tick_init(y,264+320+1),tick_init(x,0)
clock_sys_t clock_sys = {.temp_plot =
	{clock_sys.temp_samples, NUMBER_SAMPLE_HISTORY, 0, PLOT_START_X, PLOT_START_Y, PLOT_HEIGHT, PLOT_WIDTH, NORMAL_THICK_LINE, tick_init(x, y, PLOT_HEIGHT, 0, PLOT_START_Y + PLOT_HEIGHT + 1), tick_init(y, x, 0, PLOT_WIDTH, PLOT_START_X - 32)},
	.wtemp_plot =
	{clock_sys.wtemp_samples, NUMBER_SAMPLE_HISTORY, 0, PLOT_START_X, PLOT_START_Y, PLOT_HEIGHT, PLOT_WIDTH, NORMAL_THIN_LINE, tick_init(x, y, 0, 0, PLOT_START_Y + PLOT_HEIGHT + 1), tick_init(y, x, 0, 0, PLOT_START_X + PLOT_WIDTH + 15)}};

/* *****************************************************************************
 End of File
 */
