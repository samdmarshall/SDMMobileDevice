//
//  power.h
//  iOSConsole
//
//  Created by Sam Marshall on 1/25/14.
//  Copyright (c) 2014 Sam Marshall. All rights reserved.
//

#ifndef iOSConsole_power_h
#define iOSConsole_power_h

#include <CoreFoundation/CoreFoundation.h>
#include "SDMMobileDevice.h"

void SendSleepToDevice(char *udid);
void SendRebootToDevice(char *udid);
void SendShutdownToDevice(char *udid);

#endif
