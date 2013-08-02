/*
 *  SDMMobileDeviceAPI.h
 *  SDMMobileDevice
 *
 *  Copyright (c) 2013, Sam Marshall
 *  All rights reserved.
 * 
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
 *  1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
 *  2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
 *  3. All advertising materials mentioning features or use of this software must display the following acknowledgement:
 *  	This product includes software developed by the Sam Marshall.
 *  4. Neither the name of the Sam Marshall nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.
 * 
 *  THIS SOFTWARE IS PROVIDED BY Sam Marshall ''AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL Sam Marshall BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 */

#ifndef _SDM_MD_API_H_
#define _SDM_MD_API_H_

#include "SDMMobileDevice.h"

/*
 This is an API wrapper created for using this framework.
 */

SDMMobileDeviceRef SDMMobileDeviceManager;

// call this first as it is necessary to see devices
void InitializeSDMMobileDeviceManager();

// call this only when all device communication is finished and app is ready to close
void FinalizeSDMMobileDeviceManager();

// returns data from the specified device based on the domain and key requested. See SDMMD_Keys.h for list of domains and key pairs
CFTypeRef SDMMDeviceGetValue(SDMMD_AMDeviceRef device, CFStringRef domain, CFStringRef key);

// returns the status code of the SIM (tray)
extern sdmmd_sim_return_t SDMMD_GetSIMStatusCode(SDMMD_AMDeviceRef device);

// returns the activation status of a device
extern sdmmd_activation_return_t SDMMD_GetActivationStatus(SDMMD_AMDeviceRef device);

// returns the USB device ID of a device object
extern uint32_t SDMMD_AMDeviceUSBDeviceID(SDMMD_AMDeviceRef device);

// returns the USB Location ID of a device object
extern uint32_t SDMMD_AMDeviceUSBLocationID(SDMMD_AMDeviceRef device);

// returns the USB Product ID of a device object
extern uint16_t SDMMD_AMDeviceUSBProductID(SDMMD_AMDeviceRef device);

// returns CFArray of SDMMD_AMDeviceRef objects
extern CFArrayRef SDMMD_AMDCreateDeviceList();


#endif