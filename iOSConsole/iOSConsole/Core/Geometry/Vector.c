//
//  Vector.c
//  Core
//
//  Created by Sam Marshall on 12/1/13.
//  Copyright (c) 2013 Sam Marshall. All rights reserved.
//

#ifndef Core_Vector_c
#define Core_Vector_c

#include "Vector.h"
#include <stdlib.h>

static struct Vector VectorDefinedVectors[VectorAxisCount] = {
	{0.f,0.f,0.f},
	{1.f,0.f,0.f},
	{0.f,1.f,0.f},
	{0.f,0.f,1.f},
	{-1.f,0.f,0.f},
	{1.f,0.f,0.f},
	{0.f,-1.f,0.f},
	{0.f,1.f,0.f},
	{0.f,0.f,-1.f},
	{0.f,0.f,1.f}
};

VectorPtr VectorCreateFromComponents(CGFloat x, CGFloat y, CGFloat z) {
	VectorPtr vector = (VectorPtr)calloc(0x1, sizeof(struct Vector));
	if (vector) {
		vector->x = x;
		vector->y = y;
		vector->z = z;
	}
	return vector;
}

VectorPtr VectorCreateFromVector(VectorPtr vector) {
	return VectorCreateFromComponents(vector->x, vector->y, vector->z);
}

bool VectorEqualsVector(VectorPtr vector, VectorPtr equalsVector) {
	return VectorCompareEquals(vector, equalsVector);
}

CGFloat VectorGetMagnitude(VectorPtr vector) {
	return sqrt(VectorMultiX(vector, vector->x) + VectorMultiY(vector,vector->y) + VectorMultiZ(vector,vector->z));
}

CGFloat VectorDistanceToVector(VectorPtr vector, VectorPtr distant) {
	return sqrt(PointDistance(vector->x, distant->x) + PointDistance(vector->y, distant->y) + PointDistance(vector->z, distant->z));
}

CGFloat VectorAngleToVector(VectorPtr vector, VectorPtr distant) {
	CGFloat denom = CGFloatPow(VectorGetMagnitude(vector),2) * CGFloatPow(VectorGetMagnitude(distant),2);
	if (CGFloatCompareEqual(denom,0.f) || denom < 0.f) {
		denom = 0.f;
	} else {
		denom = CGFloatACos(VectorDotVector(vector, distant) / CGFloatSqrt(denom));
	}
	return denom;
}

CGFloat VectorPitch(VectorPtr vector) {
	return CGFloatATan2(vector->y, -vector->z);
}

CGFloat VectorRoll(VectorPtr vector) {
	return CGFloatATan2(vector->x, -vector->y);
}

CGFloat VectorYaw(VectorPtr vector) {
	return CGFloatATan2(vector->x, -vector->z);
}

CGFloat VectorDotVector(VectorPtr vector, VectorPtr dotVector) {
	return VectorDotProduct(vector, dotVector);
}

VectorPtr VectorNegate(VectorPtr vector) {
	CGFloat x = VectorMultiX(vector,FloatNegateConstant);
	CGFloat y = VectorMultiY(vector,FloatNegateConstant);
	CGFloat z = VectorMultiZ(vector,FloatNegateConstant);
	return VectorCreateFromComponents(x, y, z);
}

VectorPtr VectorAddVector(VectorPtr vector, VectorPtr addVector) {
	CGFloat x = VectorAddX(vector, addVector);
	CGFloat y = VectorAddY(vector, addVector);
	CGFloat z = VectorAddZ(vector, addVector);
	return VectorCreateFromComponents(x, y, z);
}

VectorPtr VectorMinusVector(VectorPtr vector, VectorPtr minusVector) {
	CGFloat x = VectorMinusX(vector, minusVector);
	CGFloat y = VectorMinusY(vector, minusVector);
	CGFloat z = VectorMinusZ(vector, minusVector);
	return VectorCreateFromComponents(x, y, z);
}

VectorPtr VectorMultiplyVector(VectorPtr vector, CGFloat mul) {
	CGFloat x = VectorMultiX(vector,mul);
	CGFloat y = VectorMultiY(vector,mul);
	CGFloat z = VectorMultiZ(vector,mul);
	return VectorCreateFromComponents(x, y, z);
}

VectorPtr VectorDivideVector(VectorPtr vector, CGFloat div) {
	CGFloat x = VectorDivideX(vector,div);
	CGFloat y = VectorDivideY(vector,div);
	CGFloat z = VectorDivideZ(vector,div);
	return VectorCreateFromComponents(x, y, z);}

VectorPtr VectorCrossVector(VectorPtr vector, VectorPtr crossVector) {
	CGFloat x = VectorCrossProductX(vector, crossVector);
	CGFloat y = VectorCrossProductY(vector, crossVector);
	CGFloat z = VectorCrossProductZ(vector, crossVector);
	return VectorCreateFromComponents(x, y, z);
}

VectorPtr VectorNormalize(VectorPtr vector) {
	CGFloat denom = CGFloatPow(VectorGetMagnitude(vector),2);
	if (denom <= 0.0f) {
	  return VectorCreateFromVector(VectorAxis(VectorAxisZero));
	}
	denom = 1.0f / CGFloatSqrt(denom);
	VectorPtr resultVector = VectorCreateFromVector(vector);
	return VectorMultiplyVector(resultVector, denom);
}

VectorPtr VectorAxis(enum VectorAxis axis) {
	VectorPtr vector;
	if (axis < VectorAxisCount) {
		vector = VectorCreateFromVector(&(VectorDefinedVectors[axis]));
	} else {
		vector = VectorCreateFromVector(&(VectorDefinedVectors[VectorAxisZero]));
	}
	return vector;
}

void VectorRelease(VectorPtr vector) {
	if (vector) {
		free(vector);
	}
}

#endif