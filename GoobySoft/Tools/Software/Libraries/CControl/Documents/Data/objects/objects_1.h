/*
 * objects_1.h
 *
 * Created on : 2023-11-05 17:02:08
 * Author: <Your name>
 * Purpose: <Classify <your things>
 * Model ID: 653
 */

#ifndef objects_1_H_
#define objects_1_H_

/* Add the CControl header file */
#include "ccontrol.h"

/*
 * This is a Neural Network - A model that can classify raw data.
 * Most used for images, but you can use it at any data you want as long the data is stored into .pgm files in format P2 or P5.
 * This file contains one matrix called 'objects_1_w' and one vector called 'objects_1_b'.
 * To use this header file, your data that must be a vector of size 'objects_1_column' in row-major.
 * For example, if you have an image with size m * n, where m * n = 'objects_1_column', then you need to turn it into a vector in row-wise, not in column-wise.
 * Multiply your vector together with the matrix 'objects_1_w' and add vector 'objects_1_b'. Then you will get a new vector called y with the size 'objects_1_row'.
 * y = objects_1_w*your_data_vector + objects_1_b
 * The index of the largest number inside the vector y is the class ID of your_data_vector.
 * If you get wrong class ID, then you need to use take account to noise or use better dimension reduction.
 */

#define objects_1_row 4
#define objects_1_column 5

const static float objects_1_w[objects_1_row * objects_1_column] = { 7.530793548e-03f, 3.502847394e-03f, 2.495326102e-03f, 3.458887339e-03f, -8.863362018e-05f, 
-8.863362018e-05f, -4.975125194e-02f, 1.862645149e-09f, -4.975125194e-02f, -4.975122400e-03f, 
-4.975122400e-03f, -4.975119606e-03f, -8.462034166e-03f, -9.588636458e-03f, 1.199423894e-02f, 
1.199423894e-02f, -1.161392778e-03f, 5.830569938e-02f, 8.889935911e-03f, -2.590841614e-02f };

const static float objects_1_b[objects_1_row] = { -1.388593435e+00f, 6.467661262e-01f, -1.261159539e+00f, 5.536140501e-02f };

const static ACTIVATION_FUNCTION objects_1_activtion_function = ACTIVTION_FUNCTION_CLOSEST_VALUE_INDEX;

#endif /* !objects_1_H_ */
