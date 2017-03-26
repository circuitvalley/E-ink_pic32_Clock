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
#include "graphics.h"
#include <stdint.h>
#include <sys/attribs.h>
#include "font/diedie_270.h"
#include "font/diedie_90_font.h"
#include "font/diedie_32.h"
#include "font/diedie_14.h"

#include "eink_p32.h"
#include "clock_system.h"
#include "bitmap.h"

__longramfunc__ void shiftdata(samples_t *samples, uint16_t size)
{

	uint16_t i = 0;

	while (i < size) {
		samples[i].data = samples[i + 1].data;
		//    samples[i].attribute = samples[i+1].attribute;
		i++;
	}
}

__longramfunc__ uint16_t findMax(samples_t *samples, uint16_t n)
{
	uint16_t m = samples[0].data;
	uint16_t i = 0;
	while (i < n) {
		if (samples[i].data > m)m = samples[i].data;
		i++;
	}
	return m;
}

__longramfunc__ uint16_t findMin(samples_t *samples, uint16_t n)
{
	uint16_t m = samples[0].data;
	uint16_t i = 0;
	while (i < n) {
		if (samples[i].data < m)m = samples[i].data;
		i++;
	}
	return m;
}

__longramfunc__ void plotLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint8_t line_type)
{
	uint16_t tmp;
	uint16_t x, y;
	uint16_t dx, dy;
	int16_t err;
	int16_t ystep;
	uint8_t swapxy = 0;

	uint8_t line_tickness = line_type & 0x1;

	uint8_t skip_pixel = 0;
	if (line_type & 0x2) {
		skip_pixel = 8;
	}

	/* no intersection check at the moment, should be added... */

	if (x1 > x2) dx = x1 - x2;
	else dx = x2 - x1;
	if (y1 > y2) dy = y1 - y2;
	else dy = y2 - y1;

	if (dy > dx) {
		swapxy = 1;
		tmp = dx;
		dx = dy;
		dy = tmp;
		tmp = x1;
		x1 = y1;
		y1 = tmp;
		tmp = x2;
		x2 = y2;
		y2 = tmp;
	}
	if (x1 > x2) {
		tmp = x1;
		x1 = x2;
		x2 = tmp;
		tmp = y1;
		y1 = y2;
		y2 = tmp;
	}
	err = dx >> 1;
	if (y2 > y1) ystep = 1;
	else ystep = -1;
	y = y1;


	if (x2 == 0xffff)
		x2--;

	for (x = x1; x <= x2; x++) {
		if ((skip_pixel > 2) || !(line_type & 0x2)) {
			if (swapxy == 0)
				putpixel(x, y, line_tickness);
			else
				putpixel(y, x, line_tickness);

		} else if (!skip_pixel) {
			skip_pixel = 8;
		}
		skip_pixel--;

		err -= (uint8_t) dy;
		if (err < 0) {
			y += (uint16_t) ystep;
			err += (uint16_t) dx;
		}
	}
}

__longramfunc__ void plot_graph(plot_area_t *pa)
{
	uint16_t w;
	uint16_t pointx, pointy, pointx_old, pointy_old;
	uint32_t y_coff = pa->h;
	uint16_t scale;
	uint16_t y;
	if (pa->asize < 1)
		return;
	uint16_t tickx = ((pa->w) << 5) / 5, ticky = ((pa->h - 1) << 5) / 5;
	uint16_t min = findMin(pa->samples, pa->asize);
	uint16_t max = findMax(pa->samples, pa->asize);
	uint16_t diff;
	diff = max - min;
	for (w = 0; w < 6; w++) {
		pa->tick_x[w].point.x = ((tickx * (w)) >> 5) + pa->x;
		pa->tick_x[w].val = (pa->asize)-(((((pa->asize) << 5) / 5) * w) >> 5);

		pa->tick_y[w].point.y = (pa->y + ((ticky * (w)) >> 5));
		pa->tick_y[w].val = max - ((((diff << 5) / 5)*(w)) >> 5);
	}

	diff = (diff < 1) ? 1 : diff; //+1 to avoid devide by zero 
	//if diff is very big  y_coff will result to zero , Fix this issue ,Re-write this code 
	y_coff = (y_coff << 7) / (diff); // y_coff is step size for y axis 
	y = (pa->y + pa->h) - 2; //available  y axis pixels 
	pointx = (pa->x << 5);

	//calculate  first point  , though , as only once point is present in data buffer , same point will be used for both of the end of line 
	pointx_old = pa->x;
	//we need to subtract  from y because we need to rice upwards and points go downward maximum
	pointy_old = y - ((((pa->samples->data) - min) * y_coff) >> 7);
	//calculate  first point 

	scale = (((uint32_t) (pa->w)) << 5) / (pa->asize); // x axis scale 
	for (w = 0; w < (pa->asize); w++) {
		pointx = pointx + scale;

		pointy = y - ((((pa->samples[w].data) - min) * y_coff) >> 7); //we need to subtract  from y because we need to rice upwards and points go downward maximum
		plotLine(pointx_old, pointy_old, pointx >> 5, pointy, pa->line_property); //first line is always same amplitude
		pointx_old = pointx >> 5;
		pointy_old = pointy;
	}

}

__longramfunc__ void printstring(unsigned char *ch, uint16_t x, uint16_t y, uint8_t n)
{

	unsigned int w, h;
	unsigned int x_holder, n_holder;
	unsigned char *ho;
	ho = ch;
	x_holder = x >> 3;
	n_holder = n;
	for (h = 0; h < diedie_270[(*ch)&0x0F].size[1]; h++) {
		x = x_holder;
		n = n_holder;
		while (n--) {

			for (w = 0; w < ((diedie_270[((*ch)&0x0F)].size[0] + 7) >> 3); w++) {
				framebuffer[y + h][x + w] = diedie_270[((*ch)&0xF)].data[w + (h * 22)];
			}
			x = x + ((diedie_270[((*ch)&0xF)].size[0] + 7) >> 3);
			ch++;
		}
		ch = ho;
	}
}

__longramfunc__ void printstring_small(unsigned char *ch, uint16_t x, uint16_t y, uint8_t n)
{

	unsigned int w, h, test;
	unsigned int x_holder, n_holder;
	unsigned char *ho;
	ho = ch;
	x_holder = x >> 3;
	n_holder = n;
	for (h = 0; h < diedie_90[(*ch)&0x0F].size[1]; h++) {
		x = x_holder;
		n = n_holder;
		while (n--) {

			for (w = 0; w < ((diedie_90[((*ch)&0x0F)].size[0] + 7) >> 3); w++) {
				framebuffer[y + h][x + w] = diedie_90[((*ch)&0xF)].data[w + (h * 8)];

			}
			x = x + ((diedie_90[((*ch)&0xF)].size[0] + 7) >> 3);
			ch++;
		}
		ch = ho;

	}



}

__longramfunc__ void printstring_vsmall(unsigned char *ch, uint16_t x, uint16_t y, uint8_t n)
{

	unsigned int w, h, test;
	unsigned int x_holder, n_holder;
	unsigned char *ho;
	ho = ch;
	x_holder = x >> 3;
	n_holder = n;
	for (h = 0; h < diedie_14[(*ch)&0x0F].size[1]; h++) {
		x = x_holder;
		n = n_holder;
		while (n--) {

			for (w = 0; w < ((diedie_14[((*ch)&0x0F)].size[0] + 7) >> 3); w++) {
				framebuffer[y + h][x + w] = diedie_14[((*ch)&0xF)].data[w + (h)];
			}
			x = x + ((diedie_14[((*ch)&0xF)].size[0] + 7) >> 3);
			ch++;
		}
		ch = ho;

	}
}

__longramfunc__ void draw_bitmap(bitmap_t *bmp, uint16_t x, uint16_t y)
{
	unsigned int w, h;
	unsigned int w_offset = (bmp->size[0] + 7) >> 3;
	uint16_t x_offset = x >> 3;
	for (h = 0; h < bmp->size[1]; h++) {
		for (w = 0; w < bmp->size[0] >> 3; w++) {
			framebuffer[y + h][x_offset + w] = bmp->data[w + h * w_offset];
		}
	}
}

__longramfunc__ void printTick(tick_t *tick)
{
	uint8_t i;
	uint8_t temp[5];
	uint16_t tt;
	for (i = 0; i < 6; i++) {

		tt = tick[i].val;

		temp[3] = tt % 10;
		tt = tt / 10;
		temp[2] = 14;
		temp[1] = tt % 10;
		tt = tt / 10;
		temp[0] = tt % 10;
		if (temp[0])
			printstring_vsmall(temp, tick[i].point.x, tick[i].point.y, 4);
		else if (temp[1])
			printstring_vsmall(&temp[1], tick[i].point.x, tick[i].point.y, 3);
		else
			printstring_vsmall(&temp[3], tick[i].point.x, tick[i].point.y, 1);

		if (tick[i].w)
			plotLine(tick[i].point.x + 32, tick[i].point.y, tick[i].point.x + 32 + tick[i].w, tick[i].point.y, DOT_THIN_LINE);
		else if (tick[i].h)
			plotLine(tick[i].point.x, tick[i].point.y, tick[i].point.x, tick[i].point.y - tick[i].h, DOT_THIN_LINE);
	}
}

__longramfunc__ void showfont(unsigned char ch, uint16_t x, uint16_t y)
{

	unsigned int w, h;
	unsigned char shift;
	shift = x % 8;
	x = x >> 3;
	for (h = 0; h < diedie_270[ch].size[1]; h++) {
		for (w = 0; w < (diedie_270[ch].size[0] >> 3) + 1; w++) {
			framebuffer[y + h][x + w] = diedie_270[ch].data[w + (h * 12)];

		}
	}
}


/* *****************************************************************************
 End of File
 */
