//
//  Vector.h
//  Core
//
//  Created by Sam Marshall on 12/1/13.
//  Copyright (c) 2013 Sam Marshall. All rights reserved.
//

#ifndef Core_Vector_h
#define Core_Vector_h

#include "Number.h"
#include "GeometryMaths.h"
#include <stdbool.h>

enum VectorAxis {
	VectorAxisZero = 0x0,
	VectorAxisX,
	VectorAxisY,
	VectorAxisZ,
	VectorAxisLeft,
	VectorAxisRight,
	VectorAxisDown,
	VectorAxisUp,
	VectorAxisForward,
	VectorAxisBackward,
	VectorAxisCount
};

struct Vector {
	CGFloat x;
	CGFloat y;
	CGFloat z;
};

typedef struct Vector* VectorPtr;

VectorPtr VectorCreateFromComponents(CGFloat x, CGFloat y, CGFloat z);
VectorPtr VectorCreateFromVector(VectorPtr vector);

bool VectorEqualsVector(VectorPtr vector, VectorPtr equalsVector);
CGFloat VectorGetMagnitude(VectorPtr vector);
CGFloat VectorDistanceToVector(VectorPtr vector, VectorPtr distant);
CGFloat VectorAngleToVector(VectorPtr vector, VectorPtr distant);
CGFloat VectorPitch(VectorPtr vector);
CGFloat VectorRoll(VectorPtr vector);
CGFloat VectorYaw(VectorPtr vector);
CGFloat VectorDotVector(VectorPtr vector, VectorPtr dotVector);
VectorPtr VectorNegate(VectorPtr vector);
VectorPtr VectorAddVector(VectorPtr vector, VectorPtr addVector);
VectorPtr VectorMinusVector(VectorPtr vector, VectorPtr minusVector);
VectorPtr VectorMultiplyVector(VectorPtr vector, CGFloat mul);
VectorPtr VectorDivideVector(VectorPtr vector, CGFloat div);
VectorPtr VectorCrossVector(VectorPtr vector, VectorPtr crossVector);
VectorPtr VectorNormalize(VectorPtr vector);
VectorPtr VectorAxis(enum VectorAxis axis);

void VectorRelease(VectorPtr vector);

#endif