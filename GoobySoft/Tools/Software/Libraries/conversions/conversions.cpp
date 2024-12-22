/*
 * conversions.c
 *
 *  Created on: Nov 28, 2024
 *      Author: Daniel Mårtensson
 */

#include "conversions.h"

union {
	uint32_t i;
    float f;
} data;

float uint16_to_float(const uint16_t msb, const uint16_t lsb) {
    data.i = (((uint32_t)msb) << 16) | lsb;
    return data.f;
}

void float_to_uint16(const float value, uint16_t* msb, uint16_t* lsb) {
    data.f = value;
    *msb = (data.i >> 16) & 0xFFFF;
    *lsb = data.i & 0xFFFF;
}

float calibrate_value(const uint16_t raw, const uint16_t min_raw_lsb, const uint16_t min_raw_msb, const uint16_t max_raw_lsb, const uint16_t max_raw_msb,
					  const uint16_t min_real_lsb, const uint16_t min_real_msb, const uint16_t max_real_lsb, const uint16_t max_real_msb){

	/* Compute differences */
	const float min_raw = uint16_to_float(min_raw_msb, min_raw_lsb);
	const float max_raw = uint16_to_float(max_raw_msb, max_raw_lsb);
	const float min_real = uint16_to_float(min_real_msb, min_real_lsb);
	const float max_real = uint16_to_float(max_real_msb, max_real_lsb);

	/* Slope */
	float delta_x = max_raw - min_raw;
	if(fabsf(delta_x) < 0.001f){
		delta_x = 0.001f;
	}
	const float delta_y = max_real - min_real;
	const float slope = delta_y/delta_x;

	/* Bias */
	const float bias = min_real - slope * min_raw;

	/* Calibration */
	const float value = slope*((float)raw) + bias;
	return value;
}
