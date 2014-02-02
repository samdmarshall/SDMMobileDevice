//
//  springboard.h
//  iOSConsole
//
//  Created by Sam Marshall on 2/2/14.
//  Copyright (c) 2014 Sam Marshall. All rights reserved.
//

#ifndef iOSConsole_springboard_h
#define iOSConsole_springboard_h

#include <CoreFoundation/CoreFoundation.h>

enum SpringboardIconType {
	SpringboardIconTypeInvalid = 0x0,
	SpringboardIconTypeApp,
	SpringboardIconTypeFolder
};

void SpringboardQuery(char *udid);
CFDictionaryRef CreateSpringboardApp(CFStringRef bundleID);
CFDictionaryRef CreateSpringboardFolder(CFStringRef name, CFArrayRef contents);

#endif
