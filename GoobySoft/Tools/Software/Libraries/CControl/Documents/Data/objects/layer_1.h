/*
 * layer_1.h
 *
 * Created on : 2023-11-05 21:16:45
 * Author: <Your name>
 * Purpose: <Classify <your things>
 * Model ID: 27597
 */

#ifndef layer_1_H_
#define layer_1_H_

/* Add the CControl header file */
#include "ccontrol.h"

/*
 * This is a Neural Network - A model that can classify raw data.
 * Most used for images, but you can use it at any data you want as long the data is stored into .pgm files in format P2 or P5.
 * This file contains one matrix called 'layer_1_w' and one vector called 'layer_1_b'.
 * To use this header file, your data that must be a vector of size 'layer_1_column' in row-major.
 * For example, if you have an image with size m * n, where m * n = 'layer_1_column', then you need to turn it into a vector in row-wise, not in column-wise.
 * Multiply your vector together with the matrix 'layer_1_w' and add vector 'layer_1_b'. Then you will get a new vector called y with the size 'layer_1_row'.
 * y = layer_1_w*your_data_vector + layer_1_b
 * The index of the largest number inside the vector y is the class ID of your_data_vector.
 * If you get wrong class ID, then you need to use take account to noise or use better dimension reduction.
 */

#define layer_1_row 4
#define layer_1_column 5

const static float layer_1_w[layer_1_row * layer_1_column] = { 1.650463091e-03f, 3.703328548e-03f, 2.666858491e-04f, 7.719079033e-03f, 3.569646273e-03f, 
3.569646273e-03f, -4.975125194e-02f, -4.975127056e-03f, -4.975128919e-03f, -4.975125194e-02f, 
-4.975125194e-02f, -9.313225746e-10f, 1.582176052e-02f, -7.015913725e-04f, 5.476627499e-02f, 
5.476627499e-02f, -1.101095974e-02f, -8.974594995e-03f, 4.271874577e-02f, -2.417639643e-02f };

const static float layer_1_b[layer_1_row] = { -1.389532804e+00f, 6.467663050e-01f, -1.249911547e+00f, 2.199067175e-02f };

const static ACTIVATION_FUNCTION layer_1_activtion_function = ACTIVATION_FUNCTION_HIGHEST_VALUE_INDEX;

#endif /* !layer_1_H_ */
