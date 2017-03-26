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

#ifndef _GRAPHICS_H    /* Guard against multiple inclusion */
#define _GRAPHICS_H

#include <stdint.h>
#include <sys/attribs.h>
/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
/* ************************************************************************** */

#define NUMBER_SAMPLE_HISTORY 360

typedef enum {
    NORMAL_THICK_LINE = 0b01, NORMAL_THIN_LINE = 0x00, DOT_THICK_LINE = 0b11, DOT_THIN_LINE = 0b10
} line_property_t;

typedef struct {
    uint16_t data;
    uint16_t attribute;
} samples_t;

typedef struct {
    uint16_t x;
    uint16_t y;
} point_t;

typedef struct {
    uint16_t val;
    uint16_t h;
    uint16_t w;
    point_t point;
} tick_t;

typedef struct {
    samples_t *samples;
    uint16_t size;
    uint16_t asize;
    uint16_t x;
    uint16_t y;
    uint16_t h;
    uint16_t w;
    line_property_t line_property;
    tick_t tick_x[6];
    tick_t tick_y[6];
} plot_area_t;

__longramfunc__ void printstring(unsigned char *ch, uint16_t x, uint16_t y, uint8_t n);
__longramfunc__ void printTick(tick_t *);
__longramfunc__ void showfont(unsigned char ch, uint16_t x, uint16_t y);


#endif /* _GRAPHICS_H */

/* *****************************************************************************
 End of File
 */
