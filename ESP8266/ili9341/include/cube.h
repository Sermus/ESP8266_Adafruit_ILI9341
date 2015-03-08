#ifndef INCLUDE_CUBE_H_
#define INCLUDE_CUBE_H_

#define AMOUNT_NODE 	8

#include <math.h>
#include <osapi.h>

void cube_calculate(int16_t _pix[AMOUNT_NODE][3], double degreeX, double degreeY, double degreeZ, double scale, int16_t shiftX, int16_t shiftY, int16_t shiftZ);
void cube_draw(int16_t _pix[AMOUNT_NODE][3], uint16_t color);
void cube_draw_init();

#endif /* INCLUDE_CUBE_H_ */
