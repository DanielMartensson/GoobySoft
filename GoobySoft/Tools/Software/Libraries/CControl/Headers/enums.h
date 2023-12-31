/*
 * enums.h
 *
 *  Created on: 20 Juli. 2023
 *      Author: Daniel Mårtensson
 */

#ifndef CCONTROL_HEADERS_ENUMS_H_
#define CCONTROL_HEADERS_ENUMS_H_

 /* For lqi.c */
typedef enum {
	ANTI_WINUP_ALWAYS_INTEGRATE,
	ANTI_WINUP_ONLY_INTEGRATE_WHEN_R_GRATER_THAN_Y_ELSE_DELETE,
	ANTI_WINUP_ONLY_INTEGRATE_WHEN_R_Y
}ANTI_WINUP;

/* For norm.c */
typedef enum {
	NORM_METHOD_L1,
	NORM_METHOD_L2,
	NORM_METHOD_FROBENIUS
}NORM_METHOD;

/* For sort.c */
typedef enum {
	SORT_MODE_ROW_DIRECTION_ASCEND,
	SORT_MODE_ROW_DIRECTION_DESCEND,
	SORT_MODE_COLUMN_DIRECTION_ASCEND,
	SORT_MODE_COLUMN_DIRECTION_DESCEND,
}SORT_MODE;

/* For Astar.c */
typedef enum {
	ASTAR_MODE_L1,
	ASTAR_MODE_L2
}ASTAR_MODE;

/* For pdist2.c */
typedef enum {
	PDIST2_METRIC_SQEUCLIDEAN,
	PDIST2_METRIC_EUCLIDEAN,
}PDIST2_METRIC;

/* For find.c */
typedef enum {
	FIND_CONDITION_METOD_E, /* = */
	FIND_CONDITION_METOD_GE, /* >= */
	FIND_CONDITION_METOD_G, /* > */
	FIND_CONDITION_METOD_LE, /* <= */
	FIND_CONDITION_METOD_L /* < */
}FIND_CONDITION_METOD;

/* For pooling.c */
typedef enum {
	POOLING_METHOD_NO_POOLING,
	POOLING_METHOD_MAX,
	POOLING_METHOD_AVERAGE,
	POOLING_METHOD_SHAPE
}POOLING_METHOD;

/* For kernel.c */
typedef enum {
	KERNEL_METHOD_LINEAR,
	KERNEL_METHOD_RBF,
	KERNEL_METHOD_POLY,
	KERNEL_METHOD_SIGMOID,
	KERNEL_METHOD_GAUSSIAN,
	KERNEL_METHOD_EXPONENTIAL
}KERNEL_METHOD;

/* For lbp.c */
typedef enum {
	LBP_BIT_8,
	LBP_BIT_16,
	LBP_BIT_24,
	LBP_BIT_32
}LBP_BIT;

/* For fast.c */
typedef enum {
	FAST_METHOD_9,
	FAST_METHOD_10,
	FAST_METHOD_11,
	FAST_METHOD_12
}FAST_METHOD;

/* For area.c */
typedef enum {
	AREA_METHOD_CIRCLE
}AREA_METHOD;

/* For nn.c */
typedef enum {
	ACTIVATION_FUNCTION_HIGHEST_VALUE_INDEX,
	ACTIVATION_FUNCTION_CLOSEST_VALUE_INDEX
}ACTIVATION_FUNCTION;

/* For data_collect.c */
typedef enum {
	DATA_SETTINGS_CHOICE_FISHERFACES,
	DATA_SETTINGS_CHOICE_ODORP
}DATA_SETTINGS_CHOICE;

#endif /* !CCONTROL_HEADERS_ENUMS_H_ */
