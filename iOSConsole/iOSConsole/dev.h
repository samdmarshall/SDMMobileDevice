//
//  dev.h
//  iOSConsole
//
//  Created by Sam Marshall on 4/17/14.
//  Copyright (c) 2014 Sam Marshall. All rights reserved.
//

#ifndef iOSConsole_dev_h
#define iOSConsole_dev_h

#include <CoreFoundation/CoreFoundation.h>

void SetupDeviceForDevelopment(char *udid);
void TranferAppForInstallOnDevice(char *udid, char *app_path);
void InstallApplicationOnDevice(char *udid, char *app_path);
void StartDebuggingAndDetach(char *udid, char *app_path);

#endif
