#ifndef _BITMAP_H_
#define _BITMAP_H_

typedef struct {
    uint16_t size[2]; /* Symbol header */
    uint8_t data[]; /* Symbol data, "variable length" */
} bitmap_t;

#endif