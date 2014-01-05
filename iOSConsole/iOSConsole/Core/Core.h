//
//  Core.h
//  Core
//
//  Created by Sam Marshall on 12/8/13.
//  Copyright (c) 2013 Sam Marshall. All rights reserved.
//

#ifndef Core_Core_h
#define Core_Core_h

#pragma mark -
#pragma mark Attributes
#define ATR_PACK __attribute__ ((packed))
#define ATR_FUNC(name) __attribute__ ((ifunc(name)))

#pragma mark -
#pragma mark Standard
#include "Pointer.h"
#include "Range.h"
#include "Number.h"
#include "Logging.h"

#ifdef __COREFOUNDATION_COREFOUNDATION__ 
#include "CFDictionaryAddition.h"
#endif

#endif
