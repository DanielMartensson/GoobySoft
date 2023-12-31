/*
 * Enum_Auxiliary_And_General_Purpose_Valves.h
 *
 *  Created on: 14 juli 2021
 *      Author: Daniel Mårtensson
 */

#ifndef ISO_11783_ISO_11783_ENUMS_ISO_11783_ENUM_AUXILIARY_AND_GENERAL_PURPOSE_VALVES_H_
#define ISO_11783_ISO_11783_ENUMS_ISO_11783_ENUM_AUXILIARY_AND_GENERAL_PURPOSE_VALVES_H_

/* Enums for valve commands */
typedef enum {
	VALVE_STATE_NEUTRAL = 0x00U,
	VALVE_STATE_EXTEND = 0x1U,
	VALVE_STATE_RETRACT = 0x2U,
	VALVE_STATE_FLOATING = 0x3U,
	VALVE_STATE_INITIALISATION = 0xAU,
	VALVE_STATE_ERROR = 0xEU
}ENUM_VALVE_STATE_CODES;

typedef enum {
	EXIT_CODE_NOT_USED = 0x1FU
}ENUM_EXIT_CODE_CODES;

typedef enum {
	FAIL_SAFE_MODE_BLOCKED = 0x0U,
	FAIL_SAFE_MODE_ACTIVATED = 0x1U
}ENUM_FAIL_SAFE_MODE_CODES;

typedef enum {
	LIMIT_NOT_USED = 0x7U
}ENUM_LIMIT_CODES;

#endif /* ISO_11783_ISO_11783_ENUMS_ISO_11783_ENUM_AUXILIARY_AND_GENERAL_PURPOSE_VALVES_H_ */
