//
//  Matrix.c
//  Core
//
//  Created by Sam Marshall on 12/1/13.
//  Copyright (c) 2013 Sam Marshall. All rights reserved.
//

#ifndef Core_Matrix_c
#define Core_Matrix_c

#include "Matrix.h"
#include <stdlib.h>

MatrixPtr MatrixCreateFromComponents(VectorPtr xBasis, VectorPtr yBasis, VectorPtr zBasis, VectorPtr origin) {
	MatrixPtr matrix = (MatrixPtr)calloc(0x1, sizeof(struct Matrix));
	if (matrix) {
		matrix->xBasis = VectorCreateFromVector(xBasis);
		matrix->yBasis = VectorCreateFromVector(yBasis);
		matrix->zBasis = VectorCreateFromVector(zBasis);
		matrix->origin = VectorCreateFromVector(origin);
	}
	return matrix;
}

MatrixPtr MatrixCreateFromMatrix(MatrixPtr matrix) {
	return MatrixCreateFromComponents(matrix->xBasis, matrix->yBasis, matrix->zBasis, matrix->origin);
}

MatrixPtr MatrixCreateFromAxisRadians(VectorPtr axis, CGFloat angle) {
	VectorPtr noTranslate = VectorCreateFromComponents(0.f, 0.f, 0.f);
	MatrixPtr matrix = MatrixCreateFromAxisRadiansTranslation(axis, angle, noTranslate);
	VectorRelease(noTranslate);
	return matrix;
}

MatrixPtr MatrixCreateFromAxisRadiansTranslation(VectorPtr axis, CGFloat angle, VectorPtr translate) {
	VectorPtr axisVector = VectorNormalize(axis);
	CGFloat s = CGFloatSin(angle);
	CGFloat c = CGFloatCos(angle);
	CGFloat C = (1-c);
	
	CGFloat xx = VectorAxisTranslateXX(axisVector, s, c, C);
	CGFloat xy = VectorAxisTranslateXY(axisVector, s, c, C);
	CGFloat xz = VectorAxisTranslateXZ(axisVector, s, c, C);
	VectorPtr xBasis = VectorCreateFromComponents(xx, xy, xz);
	
	CGFloat yx = VectorAxisTranslateYX(axisVector, s, c, C);
	CGFloat yy = VectorAxisTranslateYY(axisVector, s, c, C);
	CGFloat yz = VectorAxisTranslateYZ(axisVector, s, c, C);
	VectorPtr yBasis = VectorCreateFromComponents(yx, yy, yz);
	
	CGFloat zx = VectorAxisTranslateZX(axisVector, s, c, C);
	CGFloat zy = VectorAxisTranslateZY(axisVector, s, c, C);
	CGFloat zz = VectorAxisTranslateZZ(axisVector, s, c, C);
	VectorPtr zBasis = VectorCreateFromComponents(zx, zy, zz);
		
	VectorPtr origin = VectorCreateFromComponents(translate->x, translate->y, translate->z);
	MatrixPtr matrix = MatrixCreateFromComponents(xBasis, yBasis, zBasis, origin);
	
	VectorRelease(axisVector);
	VectorRelease(xBasis);
	VectorRelease(yBasis);
	VectorRelease(zBasis);
	VectorRelease(origin);
	return matrix;
}

bool MatrixEqualsMatrix(MatrixPtr matrix, MatrixPtr equalsMatrix) {
	return MatrixCompareEquals(matrix, equalsMatrix);
}

VectorPtr MatrixTransformPoint(MatrixPtr matrix, VectorPtr point) {
	VectorPtr xyz = MatrixTransformDirection(matrix, point);
	VectorPtr xyzo = VectorAddVector(xyz, matrix->origin);
	VectorRelease(xyz);
	return xyzo;
}

VectorPtr MatrixTransformDirection(MatrixPtr matrix, VectorPtr dir) {
	VectorPtr xBasis = VectorMultiplyVector(matrix->xBasis, dir->x);
	VectorPtr yBasis = VectorMultiplyVector(matrix->yBasis, dir->y);
	VectorPtr zBasis = VectorMultiplyVector(matrix->zBasis, dir->z);
	
	VectorPtr xy = VectorAddVector(xBasis, yBasis);
	VectorRelease(xBasis);
	VectorRelease(yBasis);
	
	VectorPtr xyz = VectorAddVector(xy, zBasis);
	VectorRelease(zBasis);
	VectorRelease(xy);
	
	return xyz;
}

MatrixPtr MatrixMultiplyMatrix(MatrixPtr matrix, MatrixPtr mulMatrix) {
	VectorPtr xBasis = MatrixTransformDirection(matrix, mulMatrix->xBasis);
	VectorPtr yBasis = MatrixTransformDirection(matrix, mulMatrix->yBasis);
	VectorPtr zBasis = MatrixTransformDirection(matrix, mulMatrix->zBasis);
	VectorPtr origin = MatrixTransformPoint(matrix, mulMatrix->origin);
	MatrixPtr resultMatrix = MatrixCreateFromComponents(xBasis, yBasis, zBasis, origin);
	VectorRelease(xBasis);
	VectorRelease(yBasis);
	VectorRelease(zBasis);
	VectorRelease(origin);
	return resultMatrix;
}

MatrixPtr MatrixIdentity() {
	VectorPtr xBasis = VectorCreateFromComponents(1.f,0.f,0.f);
	VectorPtr yBasis = VectorCreateFromComponents(0.f,1.f,0.f);
	VectorPtr zBasis = VectorCreateFromComponents(0.f,0.f,1.f);
	VectorPtr origin = VectorCreateFromComponents(0.f,0.f,0.f);
	MatrixPtr matrix = MatrixCreateFromComponents(xBasis, yBasis, zBasis, origin);
	VectorRelease(xBasis);
	VectorRelease(yBasis);
	VectorRelease(zBasis);
	VectorRelease(origin);
	return matrix;
}

MatrixPtr MatrixRigidInverse(MatrixPtr matrix) {
	VectorPtr xBasis = VectorCreateFromComponents(matrix->xBasis->x,matrix->yBasis->x,matrix->zBasis->x);
	VectorPtr yBasis = VectorCreateFromComponents(matrix->xBasis->y,matrix->yBasis->y,matrix->zBasis->y);
	VectorPtr zBasis = VectorCreateFromComponents(matrix->xBasis->z,matrix->yBasis->z,matrix->zBasis->z);
	VectorPtr origin = MatrixTransformDirection(matrix, matrix->origin);
	MatrixPtr inverseMatrix = MatrixCreateFromComponents(xBasis, yBasis, zBasis, origin);
	VectorRelease(xBasis);
	VectorRelease(yBasis);
	VectorRelease(zBasis);
	VectorRelease(origin);
	return inverseMatrix;
}

void MatrixRelease(MatrixPtr matrix) {
	if (matrix->xBasis) {
		VectorRelease(matrix->xBasis);
	}
	if (matrix->yBasis) {
		VectorRelease(matrix->yBasis);
	}
	if (matrix->zBasis) {
		VectorRelease(matrix->zBasis);
	}
	if (matrix->origin) {
		VectorRelease(matrix->origin);
	}
	if (matrix) {
		free(matrix);
	}
}

#endif