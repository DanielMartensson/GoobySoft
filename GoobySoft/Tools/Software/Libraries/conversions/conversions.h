/*
 * conversions.h
 *
 *  Created on: Nov 28, 2024
 *      Author: Daniel Mårtensson
 */

#ifndef SRC_TOOLS_CONVERSIONS_H_
#define SRC_TOOLS_CONVERSIONS_H_

#include <stdint.h>
#include <math.h>

float uint16_to_float(const uint16_t msb, const uint16_t lsb);
void float_to_uint16(const float value, uint16_t* msb, uint16_t* lsb);
float calibrate_value(const uint16_t raw, const uint16_t min_raw_lsb, const uint16_t min_raw_msb, const uint16_t max_raw_lsb, const uint16_t max_raw_msb,
					  const uint16_t min_real_lsb, const uint16_t min_real_msb, const uint16_t max_real_lsb, const uint16_t max_real_msb);

#endif /* SRC_TOOLS_CONVERSIONS_H_ */
