/*
 *  SDMUSBmuxListener.c
 *  SDM_MD_Demo
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
#ifndef _SDMUSBMUXLISTENER_C_
#define _SDMUSBMUXLISTENER_C_

#include "SDMUSBmuxListener.h"
#include "MobileDevice.h"
#include <sys/socket.h>
#include <sys/ioctl.h>

typedef struct USBMuxResponseCode {
	uint32_t code;
	CFStringRef string;
} __attribute__ ((packed)) USBMuxResponseCode;

static uint32_t transactionId = 0x0; 

void SDMUSBMuxSend(uint32_t sock, struct USBMuxPacket *packet);
void SDMUSBMuxReceive(uint32_t sock, struct USBMuxPacket *packet);

void SDMUSBMuxResponseCallback(void *context, struct USBMuxPacket *packet);
void SDMUSBMuxAttachedCallback(void *context, struct USBMuxPacket *packet);
void SDMUSBMuxDetachedCallback(void *context, struct USBMuxPacket *packet);
void SDMUSBMuxLogsCallback(void *context, struct USBMuxPacket *packet);
void SDMUSBMuxDeviceListCallback(void *context, struct USBMuxPacket *packet);
void SDMUSBMuxListenerListCallback(void *context, struct USBMuxPacket *packet);

struct USBMuxResponseCode SDMUSBMuxParseReponseCode(CFDictionaryRef dict) {
	uint32_t code = 0x0;
	CFNumberRef resultCode = NULL;
	CFStringRef resultString = NULL;
	if (CFDictionaryContainsKey(dict, CFSTR("Number")))
		resultCode = CFDictionaryGetValue(dict, CFSTR("Number"));
	if (CFDictionaryContainsKey(dict, CFSTR("String")))
		resultString = CFDictionaryGetValue(dict, CFSTR("String"));
		
	if (resultCode) {
		CFNumberGetValue(resultCode, CFNumberGetType(resultCode), &code);
		switch (code) {
			case SDMUSBMuxResult_OK: {
				code = 0x0;
					resultString = CFSTR("OK");
				break;
			};
			case SDMUSBMuxResult_BadCommand: {
				code = 0x2d;
				if (!resultString)
					resultString = CFSTR("Bad Command");
				break;
			};
			case SDMUSBMuxResult_BadDevice: {
				code = 0x6;
				if (!resultString)
					resultString = CFSTR("Bad Device");
				break;
			};
			case SDMUSBMuxResult_ConnectionRefused: {
				code = 0x3d;
				if (!resultString)
					resultString = CFSTR("Connection Refused by Device");
				break;
			};
			case SDMUSBMuxResult_Unknown0: {
				code = 0xffffffff;
				break;
			};
			case SDMUSBMuxResult_Unknown1: {
				code = 0x16;
				break;
			};
			case SDMUSBMuxResult_BadVersion: {
				code = 0x49;
				if (!resultString)
					resultString = CFSTR("Bad Protocol Version");
				break;
			};
			case SDMUSBMuxResult_Unknown2: {
				code = 0x4b;
				break;
			};
			default: {
				break;
			};
		}
	}	
	return (struct USBMuxResponseCode){code, resultString};
}

void SDMUSBMuxResponseCallback(void *context, struct USBMuxPacket *packet) {
	if (packet->payload) {
		struct USBMuxResponseCode response = SDMUSBMuxParseReponseCode(packet->payload);
		dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0x0), ^{
			printf(/*"%s:%d */"usbmuxd returned%s: %d - %s.\n", /*"SDMUSBMuxResponseCallback", 0x32a,*/ (response.code ? " error" : ""), response.code, (response.string ? CFStringGetCStringPtr(response.string, CFStringGetFastestEncoding(response.string)) : "Unknown Error Description"));
		});
		dispatch_semaphore_signal(((SDMUSBMuxListenerRef)context)->semaphore);
	}
}

void SDMUSBMuxAttachedCallback(void *context, struct USBMuxPacket *packet) {
	SDM_AMDeviceRef newDevice = AMDeviceCreateFromProperties(packet->payload);
	if (newDevice && !CFArrayContainsValue(SDMMDController->deviceList, CFRangeMake(0x0, CFArrayGetCount(SDMMDController->deviceList)), newDevice)) {
		CFMutableArrayRef updateWithNew = CFArrayCreateMutableCopy(kCFAllocatorDefault, 0x0, SDMMDController->deviceList);
		CFArrayAppendValue(updateWithNew, newDevice);
		CFNotificationCenterPostNotification(CFNotificationCenterGetLocalCenter(), CFSTR("SDMUSBMuxListenerDeviceAttachedNotification"), newDevice, NULL, true);
		CFRelease(SDMMDController->deviceList);
		SDMMDController->deviceList = CFArrayCreateCopy(kCFAllocatorDefault, updateWithNew);
		CFRelease(updateWithNew);
	}
}

void SDMUSBMuxDetachedCallback(void *context, struct USBMuxPacket *packet) {
	uint32_t detachedId;
	CFNumberRef deviceId = CFDictionaryGetValue(packet->payload, CFSTR("DeviceID"));
	CFNumberGetValue(deviceId, kCFNumberSInt64Type, &detachedId);
	CFMutableArrayRef updateWithRemove = CFArrayCreateMutableCopy(kCFAllocatorDefault, 0x0, SDMMDController->deviceList);
	uint32_t removeCounter = 0x0;
	for (uint32_t i = 0x0; i < CFArrayGetCount(SDMMDController->deviceList); i++) {
		SDM_AMDeviceRef device = (SDM_AMDeviceRef)CFArrayGetValueAtIndex(SDMMDController->deviceList, i);
		if (detachedId == AMDeviceGetConnectionID(device)) {
			CFArrayRemoveValueAtIndex(updateWithRemove, i-removeCounter);
			removeCounter++;
			CFNotificationCenterPostNotification(CFNotificationCenterGetLocalCenter(), CFSTR("SDMUSBMuxListenerDeviceDetachedNotification"), device, NULL, true);
		}
	}
	CFRelease(SDMMDController->deviceList);
	SDMMDController->deviceList = CFArrayCreateCopy(kCFAllocatorDefault, updateWithRemove);
	CFRelease(updateWithRemove);	
}

void SDMUSBMuxLogsCallback(void *context, struct USBMuxPacket *packet) {
	dispatch_semaphore_signal(((SDMUSBMuxListenerRef)context)->semaphore);
}

void SDMUSBMuxDeviceListCallback(void *context, struct USBMuxPacket *packet) {
	CFArrayRef devices = CFDictionaryGetValue(packet->payload, CFSTR("DeviceList"));
	for (uint32_t i = 0x0; i < CFArrayGetCount(devices); i++) {
		SDM_AMDeviceRef deviceFromList = AMDeviceCreateFromProperties(CFArrayGetValueAtIndex(devices, i));
		if (deviceFromList && !CFArrayContainsValue(SDMMDController->deviceList, CFRangeMake(0x0, CFArrayGetCount(SDMMDController->deviceList)), deviceFromList)) {
			((SDMUSBMuxListenerRef)context)->attachedCallback(context, packet);
		}
	}
	dispatch_semaphore_signal(((SDMUSBMuxListenerRef)context)->semaphore);
}

void SDMUSBMuxListenerListCallback(void *context, struct USBMuxPacket *packet) {
	dispatch_semaphore_signal(((SDMUSBMuxListenerRef)context)->semaphore);
}

SDMUSBMuxListenerRef SDMUSBMuxCreate() {
	SDMUSBMuxListenerRef listener = (SDMUSBMuxListenerRef)malloc(sizeof(struct USBmuxListenerClass));
	listener->socket = 0x0;
	listener->isActive = false;
	listener->socketQueue = dispatch_queue_create("com.samdmarshall.sdmmobiledevice.socketQueue", NULL);
	listener->responseCallback = SDMUSBMuxResponseCallback;
	listener->attachedCallback = SDMUSBMuxAttachedCallback;
	listener->detachedCallback = SDMUSBMuxDetachedCallback;
	listener->logsCallback = SDMUSBMuxLogsCallback;
	listener->deviceListCallback = SDMUSBMuxDeviceListCallback;
	listener->listenerListCallback = SDMUSBMuxListenerListCallback;
	listener->responses = CFArrayCreateMutable(kCFAllocatorDefault, 0x0, NULL);
	return listener;
}

void SDMUSBMuxStartListener(SDMUSBMuxListenerRef *listener) {
	dispatch_sync(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0x0), ^{
		uint32_t sock = socket(0x1,0x1,0x0);
		uint32_t mask = 0x10400;
		uint32_t code = 0x0;
		if (setsockopt(sock, 0xffff, 0x1001, &mask, 0x4)) {
			code = 0x1;
		}
		mask = 0x10400;
		if (setsockopt(sock, 0xffff, 0x1002, &mask, 0x4)) {
			code = 0x2;
		}
		mask = 0x1;
		if (setsockopt(sock, 0xffff, 0x1022, &mask, 0x4)) {
			code = 0x3;
		}
		if (!code) {
			char *mux = "/var/run/usbmuxd";
			struct sockaddr address;
			address.sa_family = 0x6a01;
			strlcpy(address.sa_data, mux, 0x68);
			code = connect(sock, &address, 0x6a);
			if (!code) {
				ioctl(sock, 0x8004667e);
				(*listener)->socket = sock;
				(*listener)->socketSource = dispatch_source_create(DISPATCH_SOURCE_TYPE_READ, (*listener)->socket, 0x0, (*listener)->socketQueue);
				dispatch_source_set_event_handler((*listener)->socketSource, ^{
					  	struct USBMuxPacket *packet = (struct USBMuxPacket *)malloc(sizeof(struct USBMuxPacket));
					   	SDMUSBMuxReceive((*listener)->socket, packet);
						if (CFPropertyListIsValid(packet->payload, kCFPropertyListXMLFormat_v1_0))
					   	if (CFDictionaryContainsKey(packet->payload, CFSTR("MessageType"))) {
					   		CFStringRef type = CFDictionaryGetValue(packet->payload, CFSTR("MessageType"));
					   		if (CFStringCompare(type, SDMUSBMuxPacketMessage[kSDMUSBMuxPacketResultType], 0x0) == 0x0) {
					   			(*listener)->responseCallback((*listener), packet);
								CFArrayAppendValue((*listener)->responses, packet);
					   		} else if (CFStringCompare(type, SDMUSBMuxPacketMessage[kSDMUSBMuxPacketAttachType], 0x0) == 0x0) {
					   			(*listener)->attachedCallback((*listener), packet);
					   		} else if (CFStringCompare(type, SDMUSBMuxPacketMessage[kSDMUSBMuxPacketDetachType], 0x0) == 0x0) {
					   			(*listener)->detachedCallback((*listener), packet);
					   		}
					   	} else {
					   		if (CFDictionaryContainsKey(packet->payload, CFSTR("Logs"))) {
					   			(*listener)->logsCallback((*listener), packet);
					   		} else if (CFDictionaryContainsKey(packet->payload, CFSTR("DeviceList"))) {
					   			(*listener)->deviceListCallback((*listener), packet);
					   		} else if (CFDictionaryContainsKey(packet->payload, CFSTR("ListenerList"))) {
					   			(*listener)->listenerListCallback((*listener), packet);
					   		}
							CFArrayAppendValue((*listener)->responses, packet);
					   	}
				});
				dispatch_resume((*listener)->socketSource);
				
				while (!(*listener)->isActive) {
					struct USBMuxPacket *startListen = SDMUSBMuxCreatePacketType(kSDMUSBMuxPacketListenType, NULL);
					SDMUSBMuxListenerSend(*listener, startListen);
					if (startListen->payload) {
						struct USBMuxResponseCode response = SDMUSBMuxParseReponseCode(startListen->payload);
						if (response.code == 0x0)
							(*listener)->isActive = true;
					}
					USBMuxPacketRelease(startListen);
				}
			}
		}
	});
}

void SDMUSBMuxListenerSend(SDMUSBMuxListenerRef listener, struct USBMuxPacket *packet) {
	listener->semaphore = dispatch_semaphore_create(0x0);
	SDMUSBMuxSend(listener->socket, packet);
	dispatch_semaphore_wait(listener->semaphore, packet->timeout);
	
	CFMutableArrayRef updateWithRemove = CFArrayCreateMutableCopy(kCFAllocatorDefault, 0x0, listener->responses);
	struct USBMuxPacket *responsePacket = (struct USBMuxPacket *)calloc(0x1, sizeof(struct USBMuxPacket));
	uint32_t removeCounter = 0x0;
	for (uint32_t i = 0x0; i < CFArrayGetCount(listener->responses); i++) {
		struct USBMuxPacket *response = (struct USBMuxPacket *)CFArrayGetValueAtIndex(listener->responses, i);
		if (packet->body.tag == response->body.tag) {
			responsePacket = response;
			CFArrayRemoveValueAtIndex(updateWithRemove, i-removeCounter);
			removeCounter++;
		}
	}
	CFRelease(listener->responses);
	listener->responses = CFArrayCreateMutableCopy(kCFAllocatorDefault, 0x0, updateWithRemove);
	CFRelease(updateWithRemove);
	*packet = *responsePacket;
	dispatch_release(listener->semaphore);
}

void SDMUSBMuxSend(uint32_t sock, struct USBMuxPacket *packet) {	
	CFDataRef xmlData = CFPropertyListCreateXMLData(kCFAllocatorDefault, packet->payload);
	char *buffer = (char *)CFDataGetBytePtr(xmlData);
	uint32_t result = send(sock, &packet->body, sizeof(struct USBMuxPacketBody), 0x0);
	if (result == sizeof(struct USBMuxPacketBody)) {
		if (packet->body.length > result) {
			uint32_t payloadSize = packet->body.length - result;
			uint32_t remainder = payloadSize;
			while (remainder) {
				result = send(sock, &buffer[payloadSize-remainder], sizeof(char), 0x0);
				if (result != sizeof(char))
					break;
				remainder -= result;
			}
		}
	}
	CFRelease(xmlData);
}

void SDMUSBMuxListenerReceive(SDMUSBMuxListenerRef listener, struct USBMuxPacket *packet) {
	SDMUSBMuxReceive(listener->socket, packet);
}

void SDMUSBMuxReceive(uint32_t sock, struct USBMuxPacket *packet) {
	uint32_t result = recv(sock, &packet->body, sizeof(struct USBMuxPacketBody), 0x0);
	if (result == sizeof(struct USBMuxPacketBody)) {
		uint32_t payloadSize = packet->body.length - result;
		if (payloadSize) {
			char *buffer = malloc(payloadSize);
			uint32_t remainder = payloadSize;
			while (remainder) {
				result = recv(sock, &buffer[payloadSize-remainder], sizeof(char), 0x0);
				if (result != sizeof(char))
					break;
				remainder -= result;
			}
			CFDataRef xmlData = CFDataCreate(kCFAllocatorDefault, (UInt8 *)buffer, payloadSize);
			packet->payload = CFPropertyListCreateFromXMLData(kCFAllocatorDefault, xmlData, kCFPropertyListImmutable, NULL);
			free(buffer);
			CFRelease(xmlData);
		}
	}
}

struct USBMuxPacket * SDMUSBMuxCreatePacketType(SDMUSBMuxPacketMessageType type, CFDictionaryRef dict) {
	struct USBMuxPacket *packet = (struct USBMuxPacket *)malloc(sizeof(struct USBMuxPacket));
	if (/*type == kSDMUSBMuxPacketListenType || */type == kSDMUSBMuxPacketConnectType) {
		packet->timeout = dispatch_time(DISPATCH_TIME_NOW, NSEC_PER_SEC*0x1e);
	} else {
		packet->timeout = dispatch_time(DISPATCH_TIME_NOW, NSEC_PER_SEC*0x5);
	}
	packet->body = (struct USBMuxPacketBody){0x10, 0x1, 0x8, transactionId};
	transactionId++;
	packet->payload = CFDictionaryCreateMutable(kCFAllocatorDefault, 0x0, &kCFTypeDictionaryKeyCallBacks, &kCFTypeDictionaryValueCallBacks);
	CFDictionarySetValue((CFMutableDictionaryRef)packet->payload, CFSTR("BundleID"), CFSTR("com.samdmarshall.sdmmobiledevice"));
	CFDictionarySetValue((CFMutableDictionaryRef)packet->payload, CFSTR("ClientVersionString"), CFSTR("1.0"));
	CFDictionarySetValue((CFMutableDictionaryRef)packet->payload, CFSTR("ProgName"), CFSTR("SDMMobileDevice"));
	if (dict) {
		CFIndex count = CFDictionaryGetCount(dict);
		void *keys[count];
		void *values[count];
		CFDictionaryGetKeysAndValues(dict, (const void **)keys, (const void **)values);
		for (uint32_t i = 0x0; i < count; i++) {
			CFDictionarySetValue((CFMutableDictionaryRef)packet->payload, keys[i], values[i]);
		}
	}
	CFDictionarySetValue((CFMutableDictionaryRef)packet->payload, CFSTR("MessageType"), SDMUSBMuxPacketMessage[type]);
	if (type == kSDMUSBMuxPacketConnectType) {
		uint16_t port = htons(0x7ef2);
		CFNumberRef portNumber = CFNumberCreate(kCFAllocatorDefault, 0x2, &port);
		CFDictionarySetValue((CFMutableDictionaryRef)packet->payload, CFSTR("PortNumber"), portNumber);
		CFRelease(portNumber);
	}
	uint32_t connection = 0x0;
	CFNumberRef connectionType = CFNumberCreate(kCFAllocatorDefault, 0x9, &connection);
	CFDictionarySetValue((CFMutableDictionaryRef)packet->payload, CFSTR("ConnType"), connectionType);
	CFRelease(connectionType);

	CFDataRef xmlData = CFPropertyListCreateXMLData(kCFAllocatorDefault, packet->payload);
	packet->body.length = 0x10 + CFDataGetLength(xmlData);
	CFRelease(xmlData);
	return packet;
}

void USBMuxPacketRelease(struct USBMuxPacket *packet) {
	if (CFPropertyListIsValid(packet->payload, kCFPropertyListXMLFormat_v1_0))
		CFRelease(packet->payload);
	free(packet);
}

#endif