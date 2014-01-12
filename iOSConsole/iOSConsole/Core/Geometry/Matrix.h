//
//  Matrix.h
//  Core
//
//  Created by Sam Marshall on 12/1/13.
//  Copyright (c) 2013 Sam Marshall. All rights reserved.
//

#ifndef Core_Matrix_h
#define Core_Matrix_h

#include "GeometryMaths.h"
#include "Vector.h"

struct Matrix {
	VectorPtr xBasis;
	VectorPtr yBasis;
	VectorPtr zBasis;
	VectorPtr origin;
};

typedef struct Matrix* MatrixPtr;

MatrixPtr MatrixCreateFromComponents(VectorPtr xBasis, VectorPtr yBasis, VectorPtr zBasis, VectorPtr origin);
MatrixPtr MatrixCreateFromMatrix(MatrixPtr matrix);
MatrixPtr MatrixCreateFromAxisRadians(VectorPtr axis, CGFloat angle);
MatrixPtr MatrixCreateFromAxisRadiansTranslation(VectorPtr axis, CGFloat angle, VectorPtr translate);

bool MatrixEqualsMatrix(MatrixPtr matrix, MatrixPtr equalsMatrix);
VectorPtr MatrixTransformPoint(MatrixPtr matrix, VectorPtr point);
VectorPtr MatrixTransformDirection(MatrixPtr matrix, VectorPtr dir);
MatrixPtr MatrixMultiplyMatrix(MatrixPtr matrix, MatrixPtr mulMatrix);
MatrixPtr MatrixIdentity();
MatrixPtr MatrixRigidInverse(MatrixPtr matrix);

void MatrixRelease(MatrixPtr matrix);

#endif