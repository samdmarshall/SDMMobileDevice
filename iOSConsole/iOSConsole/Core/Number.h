//
//  Number.h
//  Core
//
//  Created by Sam Marshall on 12/8/13.
//  Copyright (c) 2013 Sam Marshall. All rights reserved.
//

#ifndef Core_Number_h
#define Core_Number_h

#include <math.h>
#include <float.h>

#pragma mark -
#pragma mark Integer

#define GetDigitsOfNumber(num) (num > 0x0 ? (int)log10(num)+0x1 : 0x1)

#define MakeNSIntegerUnsigned(a) a = (a < 0 ? 0 : a)

#pragma mark -
#pragma mark Float

#define FloatCompareEqual(a, b) ((float)fabsf((a) - (b)) < FLT_EPSILON)

#pragma mark -
#pragma mark CGFloat

#ifndef CGFLOAT_DEFINED
	// SDM: copied from CGBase.h in CoreGraphics.framework
	#if defined(__LP64__) && __LP64__
		#define CGFLOAT_TYPE double
		#define CGFLOAT_IS_DOUBLE 1
		#define CGFLOAT_MIN DBL_MIN
		#define CGFLOAT_MAX DBL_MAX
	#else
		#define CGFLOAT_TYPE float
		#define CGFLOAT_IS_DOUBLE 0
		#define CGFLOAT_MIN FLT_MIN
		#define CGFLOAT_MAX FLT_MAX
	#endif
	/* Definition of the `CGFloat' type and `CGFLOAT_DEFINED'. */
	typedef CGFLOAT_TYPE CGFloat;
	#define CGFLOAT_DEFINED 1
#endif

#define CGFloatInternal(func,arg) func arg
#define DoubleMath(func,arg) CGFloatInternal(func,arg)
#define FloatMath(func,arg) CGFloatInternal(func ## f,arg)

#if CGFLOAT_IS_DOUBLE
	#define CGFLOAT_EPSILON DBL_EPSILON
	#define CGFloatMath(func,arg) DoubleMath(func,arg)
#else
	#define CGFLOAT_EPSILON FLT_EPSILON
	#define CGFloatMath(func,arg) FloatMath(func,arg)
#endif

#pragma mark Trigonometric Functions
#define CGFloatCos(a) CGFloatMath(cos,(a))
#define CGFloatSin(a) CGFloatMath(sin,(a))
#define CGFloatTan(a) CGFloatMath(tan,(a))
#define CGFloatACos(a) CGFloatMath(acos,(a))
#define CGFloatASin(a) CGFloatMath(asin,(a))
#define CGFloatATan(a) CGFloatMath(atan,(a))
#define CGFloatATan2(a, b) CGFloatMath(atan2,(a, b))

#pragma mark Hyperbolic Functions
#define CGFloatCosH(a) CGFloatMath(cosh,(a))
#define CGFloatSinH(a) CGFloatMath(sinh,(a))
#define CGFloatTanH(a) CGFloatMath(tanh,(a))
#define CGFloatACosH(a) CGFloatMath(acosh,(a))
#define CGFloatASinH(a) CGFloatMath(asinh,(a))
#define CGFloatATanH(a) CGFloatMath(atanh,(a))

#pragma mark Exponential and Logarithmic Functions
#define CGFloatExp(a) CGFloatMath(exp,(a))
#define CGFloatFrExp(a, b) CGFloatMath(frexp,(a, b))
#define CGFloatLdExp(a, b) CGFloatMath(ldexp,(a, b))
#define CGFloatLog(a) CGFloatMath(log,(a))
#define CGFloatLog10(a) CGFloatMath(log10,(a))
#define CGFloatModf(a, b) CGFloatMath(modfm,(a, b))
#define CGFloatExp2(a) CGFloatMath(exp2,(a))
#define CGFloatExpm1(a) CGFloatMath(expm1,(a))
#define CGFloatiLogb(a) CGFloatMath(ilogb,(a))
#define CGFloatLog1p(a) CGFloatMath(log1p,(a))
#define CGFloatLog2(a) CGFloatMath(log2,(a))
#define CGFloatLogb(a) CGFloatMath(logb,(a))
#define CGFloatScalbn(a, b) CGFloatMath(scalbn,(a, b))
#define CGFloatScalbln(a, b) CGFloatMath(scalbln,(a, b))

#pragma mark Power Functions
#define CGFloatPow(a, b) CGFloatMath(pow,(a, b))
#define CGFloatSqrt(a) CGFloatMath(sqrt,(a))
#define CGFloatCbrt(a) CGFloatMath(cbrt,(a))
#define CGFloatHypot(a) CGFloatMath(hypot,(a))

#pragma mark Error and Gamma Functions
#define CGFloatErf(a) CGFloatMath(erf,(a))
#define CGFloatErfc(a) CGFloatMath(erfc,(a))
#define CGFloatTGamma(a) CGFloatMath(tgamma,(a))
#define CGFloatLGamma(a) CGFloatMath(lgamma,(a))

#pragma mark Rounding and Remainder Functions
#define CGFloatCeil(a) CGFloatMath(ceil,(a))
#define CGFloatFloor(a) CGFloatMath(floor,(a))
#define CGFloatFmod(a, b) CGFloatMath(fmod,(a, b))
#define CGFloatTrunc(a) CGFloatMath(trunc,(a))
#define CGFloatRound(a) CGFloatMath(round,(a))
#define CGFloatLRound(a) CGFloatMath(lround,(a))
#define CGFloatLLRound(a) CGFloatMath(llround,(a))
#define CGFloatRint(a) CGFloatMath(rint,(a))
#define CGFloatLRint(a) CGFloatMath(lrint,(a))
#define CGFloatLLRint(a) CGFloatMath(llrint,(a))
#define CGFloatNearbyInt(a) CGFloatMath(nearbyint,(a))
#define CGFloatRemainder(a, b) CGFloatMath(remainder,(a, b))
#define CGFloatRemQuo(a, b) CGFloatMath(remquo,(a, b))

#pragma mark Floating-Point Manipulation Functions
#define CGFloatCopySign(a, b) CGFloatMath(copysign,(a, b))
#define CGFloatNextAfter(a, b) CGFloatMath(nextafter,(a, b))
#define CGFloatNextToward(a, b) CGFloatMath(nexttoward,(a, b))

#pragma mark Minimum, Maximum, Difference Functions
#define CGFloatFDim(a, b) CGFloatMath(fdim,(a, b))
#define CGFloatFMax(a, b) CGFloatMath(fmax,(a, b))
#define CGFloatFMin(a, b) CGFloatMath(fmin,(a, b))

#pragma Misc Functions
#define CGFloatAbs(a) CGFloatMath(fabs,(a))
#define CGFloatFMA(a, b, c) CGFloatMath(fma,(a, b, c))

#define CGFloatCompareEqual(a, b) (CGFloatAbs((a) - (b)) < CGFLOAT_EPSILON)

#endif