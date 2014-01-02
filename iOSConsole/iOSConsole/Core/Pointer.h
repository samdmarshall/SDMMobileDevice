//
//  Pointer.h
//  Core
//
//  Created by Sam Marshall on 12/8/13.
//  Copyright (c) 2013 Sam Marshall. All rights reserved.
//

#ifndef Core_Pointer_h
#define Core_Pointer_h

#include <stdint.h>

typedef uintptr_t* Pointer;
typedef uintptr_t* (*FunctionPointer)();

#define k32BitMask 0xffffffff
#define k64BitMask 0xffffffffffffffff
#define k64BitMaskHigh 0xffffffff00000000
#define k64BitMaskLow 0x00000000ffffffff
#define PtrCastSmallPointer(a) (*(Pointer)&(a))
#define PtrHighPointer(a) (a & k64BitMaskHigh)
#define PtrLowPointer(a) (a & k64BitMaskLow)

#define Ptr(ptr) PtrCast(ptr,char*)
#define PtrCast(ptr, cast) ((cast)ptr)
#define PtrAdd(ptr, add) (Ptr(ptr) + (uint64_t)add)
#define PtrSub(ptr, sub) (Ptr(ptr) - (uint64_t)sub)

#endif
