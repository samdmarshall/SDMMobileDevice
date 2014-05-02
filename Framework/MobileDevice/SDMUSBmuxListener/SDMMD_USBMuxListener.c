/*
 *  SDMUSBmuxListener.c
 *  SDMMobileDevice
 *
 * Copyright (c) 2014, Sam Marshall
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification, are permitted provided that the 
 * following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer 
 * 		in the documentation and/or other materials provided with the distribution.
 *
 * 3. Neither the name of Sam Marshall nor the names of its contributors may be used to endorse or promote products derived from this
 * 		software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#ifndef _SDM_MD_USBMUXLISTENER_C_
#define _SDM_MD_USBMUXLISTENER_C_

#include "SDMMD_USBMuxListener.h"
#include "SDMMD_MCP.h"
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <sys/un.h>
#include "Core.h"

const CFStringRef kSDMMD_USBMuxListenerDeviceAttachedNotification = CFSTR("SDMMD_USBMuxListenerDeviceAttachedNotification");
const CFStringRef kSDMMD_USBMuxListenerDeviceAttachedNotificationFinished = CFSTR("SDMMD_USBMuxListenerDeviceAttachedNotificationFinished");
const CFStringRef kSDMMD_USBMuxListenerDeviceDetachedNotification = CFSTR("SDMMD_USBMuxListenerDeviceDetachedNotification");
const CFStringRef kSDMMD_USBMuxListenerDeviceDetachedNotificationFinished = CFSTR("SDMMD_USBMuxListenerDeviceDetachedNotificationFinished");
const CFStringRef kSDMMD_USBMuxListenerStoppedListenerNotification = CFSTR("SDMMD_USBMuxListenerStoppedListenerNotification");

typedef struct USBMuxResponseCode {
	uint32_t code;
	CFStringRef string;
} ATR_PACK USBMuxResponseCode;

static uint32_t transactionId = 0x0; 

void SDMMD_USBMuxSend(uint32_t sock, struct USBMuxPacket *packet);
void SDMMD_USBMuxReceive(uint32_t sock, struct USBMuxPacket *packet);

void SDMMD_USBMuxResponseCallback(void *context, struct USBMuxPacket *packet);
void SDMMD_USBMuxAttachedCallback(void *context, struct USBMuxPacket *packet);
void SDMMD_USBMuxDetachedCallback(void *context, struct USBMuxPacket *packet);
void SDMMD_USBMuxLogsCallback(void *context, struct USBMuxPacket *packet);
void SDMMD_USBMuxDeviceListCallback(void *context, struct USBMuxPacket *packet);
void SDMMD_USBMuxListenerListCallback(void *context, struct USBMuxPacket *packet);

struct USBMuxResponseCode SDMMD_USBMuxParseReponseCode(CFDictionaryRef dict) {
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
			case SDMMD_USBMuxResult_OK: {
				resultString = CFSTR("OK");
				break;
			}
			case SDMMD_USBMuxResult_BadCommand: {
				resultString = CFSTR("Bad Command");
				break;
			}
			case SDMMD_USBMuxResult_BadDevice: {
				resultString = CFSTR("Bad Device");
				break;
			}
			case SDMMD_USBMuxResult_ConnectionRefused: {
				resultString = CFSTR("Connection Refused by Device");
				break;
			}
			case SDMMD_USBMuxResult_Unknown0: {
				break;
			}
			case SDMMD_USBMuxResult_BadMessage: {
				resultString = CFSTR("Incorrect Message Contents");
				break;
			}
			case SDMMD_USBMuxResult_BadVersion: {
				resultString = CFSTR("Bad Protocol Version");
				break;
			}
			case SDMMD_USBMuxResult_Unknown2: {
				break;
			}
			default: {
				break;
			}
		}
	}	
	return (struct USBMuxResponseCode){code, resultString};
}

void SDMMD_USBMuxResponseCallback(void *context, struct USBMuxPacket *packet) {
	if (packet->payload) {
		struct USBMuxResponseCode response = SDMMD_USBMuxParseReponseCode(packet->payload);
		dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0x0), ^{
			if (response.code) {
				printf("usbmuxd returned%s: %d - %s.\n", (response.code ? " error" : ""), response.code, (response.string ? CFStringGetCStringPtr(response.string, kCFStringEncodingUTF8) : "Unknown Error Description"));
			}
		});
		dispatch_semaphore_signal(((SDMMD_USBMuxListenerRef)context)->semaphore);
	}
}

void SDMMD_USBMuxAttachedCallback(void *context, struct USBMuxPacket *packet) {
	SDMMD_AMDeviceRef newDevice = SDMMD_AMDeviceCreateFromProperties(packet->payload);
	if (newDevice && !CFArrayContainsValue(SDMMobileDevice->deviceList, CFRangeMake(0, CFArrayGetCount(SDMMobileDevice->deviceList)), newDevice)) {
		CFMutableArrayRef updateWithNew = CFArrayCreateMutableCopy(kCFAllocatorDefault, 0, SDMMobileDevice->deviceList);
		// give priority to usb over wifi
		if (newDevice->ivars.connection_type == kAMDeviceConnectionTypeUSB) {
			CFArrayAppendValue(updateWithNew, newDevice);
			dispatch_async(dispatch_get_main_queue(), ^{
				CFNotificationCenterPostNotification(CFNotificationCenterGetLocalCenter(), kSDMMD_USBMuxListenerDeviceAttachedNotification, NULL, NULL, true);
			});
			CFSafeRelease(SDMMobileDevice->deviceList);
			SDMMobileDevice->deviceList = CFArrayCreateCopy(kCFAllocatorDefault, updateWithNew);
		}
		else if (newDevice->ivars.connection_type == kAMDeviceConnectionTypeWiFi) {
			// wifi
		}
		CFSafeRelease(updateWithNew);
	}
	dispatch_async(dispatch_get_main_queue(), ^{
		CFNotificationCenterPostNotification(CFNotificationCenterGetLocalCenter(), kSDMMD_USBMuxListenerDeviceAttachedNotificationFinished, NULL, NULL, true);
	});
}

void SDMMD_USBMuxDetachedCallback(void *context, struct USBMuxPacket *packet) {
	uint32_t detachedId;
	CFNumberRef deviceId = CFDictionaryGetValue(packet->payload, CFSTR("DeviceID"));
	CFNumberGetValue(deviceId, kCFNumberSInt64Type, &detachedId);
	CFMutableArrayRef updateWithRemove = CFArrayCreateMutableCopy(kCFAllocatorDefault, 0, SDMMobileDevice->deviceList);
	uint32_t removeCounter = 0x0;
	SDMMD_AMDeviceRef detachedDevice = NULL;
	for (uint32_t i = 0x0; i < CFArrayGetCount(SDMMobileDevice->deviceList); i++) {
		detachedDevice = (SDMMD_AMDeviceRef)CFArrayGetValueAtIndex(SDMMobileDevice->deviceList, i);
		//CFRetain(detachedDevice);
		// add something for then updating to use wifi if available.
		if (detachedId == SDMMD_AMDeviceGetConnectionID(detachedDevice)) {
			CFArrayRemoveValueAtIndex(updateWithRemove, i-removeCounter);
			removeCounter++;
			dispatch_async(dispatch_get_main_queue(), ^{
				CFNotificationCenterPostNotification(CFNotificationCenterGetLocalCenter(), kSDMMD_USBMuxListenerDeviceDetachedNotification, NULL, NULL, true);
			});
		}
	}
	CFSafeRelease(SDMMobileDevice->deviceList);
	SDMMobileDevice->deviceList = CFArrayCreateCopy(kCFAllocatorDefault, updateWithRemove);
	CFSafeRelease(updateWithRemove);
	dispatch_async(dispatch_get_main_queue(), ^{
		CFNotificationCenterPostNotification(CFNotificationCenterGetLocalCenter(), kSDMMD_USBMuxListenerDeviceDetachedNotificationFinished, NULL, NULL, true);
	});
}

void SDMMD_USBMuxLogsCallback(void *context, struct USBMuxPacket *packet) {
	dispatch_semaphore_signal(((SDMMD_USBMuxListenerRef)context)->semaphore);
}

void SDMMD_USBMuxDeviceListCallback(void *context, struct USBMuxPacket *packet) {
	// SDM: used to prune bad records from being retained
	CFMutableArrayRef newList = CFArrayCreateMutable(kCFAllocatorDefault, 0x0, &kCFTypeArrayCallBacks);
	for (uint32_t i = 0x0; i < CFArrayGetCount(SDMMobileDevice->deviceList); i++) {
		SDMMD_AMDeviceRef deviceFromList = (SDMMD_AMDeviceRef)CFArrayGetValueAtIndex(SDMMobileDevice->deviceList, i);
		if (SDMMD_AMDeviceIsValid(deviceFromList)) {
			CFArrayAppendValue(newList, deviceFromList);
		}
	}
	CFSafeRelease(SDMMobileDevice->deviceList);
	SDMMobileDevice->deviceList = CFArrayCreateCopy(kCFAllocatorDefault, newList);
	CFSafeRelease(newList);

	CFArrayRef devices = CFDictionaryGetValue(packet->payload, CFSTR("DeviceList"));
	for (uint32_t i = 0x0; i < CFArrayGetCount(devices); i++) {
		SDMMD_AMDeviceRef deviceFromList = SDMMD_AMDeviceCreateFromProperties(CFArrayGetValueAtIndex(devices, i));
		if (deviceFromList && !CFArrayContainsValue(SDMMobileDevice->deviceList, CFRangeMake(0x0, CFArrayGetCount(SDMMobileDevice->deviceList)), deviceFromList)) {
			struct USBMuxPacket *devicePacket = calloc(1, sizeof(struct USBMuxPacket));
			memcpy(devicePacket, packet, sizeof(struct USBMuxPacket));
			devicePacket->payload = CFArrayGetValueAtIndex(devices, i);
			((SDMMD_USBMuxListenerRef)context)->attachedCallback(context, devicePacket);
		}
	}
	dispatch_semaphore_signal(((SDMMD_USBMuxListenerRef)context)->semaphore);
}

void SDMMD_USBMuxListenerListCallback(void *context, struct USBMuxPacket *packet) {
	dispatch_semaphore_signal(((SDMMD_USBMuxListenerRef)context)->semaphore);
}

void SDMMD_USBMuxUnknownCallback(void *context, struct USBMuxPacket *packet) {
	printf("Unknown response from usbmuxd!\n");
	if (packet->payload) {
		PrintCFType(packet->payload);
	}
	dispatch_semaphore_signal(((SDMMD_USBMuxListenerRef)context)->semaphore);
}

SDMMD_USBMuxListenerRef SDMMD_USBMuxCreate() {
	SDMMD_USBMuxListenerRef listener = (SDMMD_USBMuxListenerRef)calloc(1, sizeof(struct USBMuxListenerClass));
	listener->socket = -1;
	listener->isActive = false;
	listener->socketQueue = dispatch_queue_create("com.samdmarshall.sdmmobiledevice.socketQueue", NULL);
	listener->responseCallback = SDMMD_USBMuxResponseCallback;
	listener->attachedCallback = SDMMD_USBMuxAttachedCallback;
	listener->detachedCallback = SDMMD_USBMuxDetachedCallback;
	listener->logsCallback = SDMMD_USBMuxLogsCallback;
	listener->deviceListCallback = SDMMD_USBMuxDeviceListCallback;
	listener->listenerListCallback = SDMMD_USBMuxListenerListCallback;
	listener->unknownCallback = SDMMD_USBMuxUnknownCallback;
	listener->responses = CFArrayCreateMutable(kCFAllocatorDefault, 0x0, NULL);
	return listener;
}

void SDMMD_USBMuxClose(SDMMD_USBMuxListenerRef listener) {
	listener->isActive = false;
	CFSafeRelease(listener->responses);
	Safe(close,listener->socket);
	Safe(dispatch_release,listener->socketQueue);
	listener->responseCallback = NULL;
	listener->attachedCallback = NULL;
	listener->detachedCallback = NULL;
	listener->logsCallback = NULL;
	listener->deviceListCallback = NULL;
	listener->listenerListCallback = NULL;
	listener->unknownCallback = NULL;
	dispatch_async(dispatch_get_main_queue(), ^{
		CFNotificationCenterPostNotification(CFNotificationCenterGetLocalCenter(), kSDMMD_USBMuxListenerStoppedListenerNotification, NULL, NULL, true);
	});
	Safe(free,listener);
}


/*
 debugging traffic:
 sudo mv /var/run/usbmuxd /var/run/usbmuxx
 sudo socat -t100 -x -v UNIX-LISTEN:/var/run/usbmuxd,mode=777,reuseaddr,fork UNIX-CONNECT:/var/run/usbmuxx
 */

uint32_t SDMMD_ConnectToUSBMux() {
	int result = 0x0;
	
	// Initialize socket
	uint32_t sock = socket(AF_UNIX, SOCK_STREAM, 0);
	
	// Set send/receive buffer sizes
	uint32_t bufSize = 0x00010400;
	if (!result) {
		if (setsockopt(sock, SOL_SOCKET, SO_SNDBUF, &bufSize, sizeof(bufSize))) {
			result = 0x1;
			int err = errno;
			printf("%s: setsockopt SO_SNDBUF failed: %d - %s\n", __FUNCTION__, err, strerror(err));
		}
	}
	if (!result) {
		if (setsockopt(sock, SOL_SOCKET, SO_RCVBUF, &bufSize, sizeof(bufSize))) {
			result = 0x2;
			int err = errno;
			printf("%s: setsockopt SO_RCVBUF failed: %d - %s\n", __FUNCTION__, err, strerror(err));
		}
	}
	
	if (!result) {
		// Disable SIGPIPE on socket i/o error
		uint32_t noPipe = 1;
		if (setsockopt(sock, SOL_SOCKET, SO_NOSIGPIPE, &noPipe, sizeof(noPipe))) {
			result = 0x3;
			int err = errno;
			printf("%s: setsockopt SO_NOSIGPIPE failed: %d - %s\n", __FUNCTION__, err, strerror(err));
		}
	}
	
	if (!result) {
		// Create address structure to point to usbmuxd socket
		char *mux = "/var/run/usbmuxd";
		struct sockaddr_un address;
		address.sun_family = AF_UNIX;
		strncpy(address.sun_path, mux, sizeof(address.sun_path));
		address.sun_len = SUN_LEN(&address);
		
		// Connect socket
		if (connect(sock, (const struct sockaddr *)&address, sizeof(struct sockaddr_un))) {
			result = 0x4;
			int err = errno;
			printf("%s: connect socket failed: %d - %s\n", __FUNCTION__, err, strerror(err));
		}
	}
	
	if (!result) {
		// Set socket to blocking IO mode
		uint32_t nonblock = 0;
		if (ioctl(sock, FIONBIO, &nonblock)) {
			result = 0x5;
			int err = errno;
			printf("%s: ioctl FIONBIO failed: %d - %s\n", __FUNCTION__, err, strerror(err));
		}
	}
	
	if (result) {
		// Socket creation failed
		close(sock);
		sock = -1;
	}
	
	return sock;
}

sdmmd_return_t SDMMD_USBMuxConnectByPort(SDMMD_AMDeviceRef device, uint32_t port, uint32_t *socketConn) {
	sdmmd_return_t result = kAMDSuccess;
	*socketConn = SDMMD_ConnectToUSBMux();
	if (*socketConn) {
		CFMutableDictionaryRef dict = CFDictionaryCreateMutable(kCFAllocatorDefault, 0x0, &kCFTypeDictionaryKeyCallBacks, &kCFTypeDictionaryValueCallBacks);
		CFNumberRef deviceNum = CFNumberCreate(kCFAllocatorDefault, kCFNumberSInt32Type, &device->ivars.device_id);
		CFDictionarySetValue(dict, CFSTR("DeviceID"), deviceNum);
		CFSafeRelease(deviceNum);
		struct USBMuxPacket *connect = SDMMD_USBMuxCreatePacketType(kSDMMD_USBMuxPacketConnectType, dict);
		if (port != 0x7ef2) {
			uint16_t newPort = htons(port);
			CFNumberRef portNumber = CFNumberCreate(kCFAllocatorDefault, kCFNumberSInt16Type, &newPort);
			CFDictionarySetValue((CFMutableDictionaryRef)connect->payload, CFSTR("PortNumber"), portNumber);
			CFSafeRelease(portNumber);
		}
		SDMMD_USBMuxSend(*socketConn, connect);
		SDMMD_USBMuxReceive(*socketConn, connect);
		CFSafeRelease(dict);
	}
	else {
		result = kAMDMuxConnectError;
	}
	return result;
}

void SDMMD_USBMuxStartListener(SDMMD_USBMuxListenerRef *listener) {
	__block uint64_t bad_packet_counter = 0;
	dispatch_sync(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0x0), ^{
		(*listener)->socket = SDMMD_ConnectToUSBMux();
		(*listener)->socketSource = dispatch_source_create(DISPATCH_SOURCE_TYPE_READ, (*listener)->socket, 0x0, (*listener)->socketQueue);
		dispatch_source_set_event_handler((*listener)->socketSource, ^{
            //printf("socketSourceEventHandler: fired\n");
			struct USBMuxPacket *packet = (struct USBMuxPacket *)calloc(0x1, sizeof(struct USBMuxPacket));
			SDMMD_USBMuxReceive((*listener)->socket, packet);
			if (CFPropertyListIsValid(packet->payload, kCFPropertyListXMLFormat_v1_0)) {
				if (CFDictionaryContainsKey(packet->payload, CFSTR("MessageType"))) {
					CFStringRef type = CFDictionaryGetValue(packet->payload, CFSTR("MessageType"));
					if (CFStringCompare(type, SDMMD_USBMuxPacketMessage[kSDMMD_USBMuxPacketResultType], 0x0) == 0x0) {
						(*listener)->responseCallback((*listener), packet);
						CFArrayAppendValue((*listener)->responses, packet);
					}
					else if (CFStringCompare(type, SDMMD_USBMuxPacketMessage[kSDMMD_USBMuxPacketAttachType], 0x0) == 0x0) {
						(*listener)->attachedCallback((*listener), packet);
					}
					else if (CFStringCompare(type, SDMMD_USBMuxPacketMessage[kSDMMD_USBMuxPacketDetachType], 0x0) == 0x0) {
						(*listener)->detachedCallback((*listener), packet);
					}
				}
				else {
					if (CFDictionaryContainsKey(packet->payload, CFSTR("Logs"))) {
						(*listener)->logsCallback((*listener), packet);
					}
					else if (CFDictionaryContainsKey(packet->payload, CFSTR("DeviceList"))) {
						(*listener)->deviceListCallback((*listener), packet);
					}
					else if (CFDictionaryContainsKey(packet->payload, CFSTR("ListenerList"))) {
						(*listener)->listenerListCallback((*listener), packet);
					}
					else {
						(*listener)->unknownCallback((*listener), packet);
					}
					CFArrayAppendValue((*listener)->responses, packet);
				}
			}
			else {
				bad_packet_counter++;
                printf("socketSourceEventHandler: failed to decodeCFPropertyList from packet payload\n");
				if (bad_packet_counter > 10) {
					printf("eating bad packets, exiting...\n");
					exit(EXIT_FAILURE);
				}
            }
		});
        dispatch_source_set_cancel_handler((*listener)->socketSource, ^{
            printf("socketSourceEventCancelHandler: source canceled\n");
        });
		dispatch_resume((*listener)->socketSource);
				
		while (!(*listener)->isActive) {
			struct USBMuxPacket *startListen = SDMMD_USBMuxCreatePacketType(kSDMMD_USBMuxPacketListenType, NULL);
			SDMMD_USBMuxListenerSend(*listener, &startListen);
			if (startListen->payload) {
				struct USBMuxResponseCode response = SDMMD_USBMuxParseReponseCode(startListen->payload);
				if (response.code == 0x0){
					(*listener)->isActive = true;
                }
				else {
                    printf("%s: non-zero response code. trying again. code:%i string:%s\n", __FUNCTION__, response.code, response.string ? CFStringGetCStringPtr(response.string, kCFStringEncodingUTF8):"");
                }
			}
			else {
                printf("%s: no response payload. trying again.\n",__FUNCTION__);
            }
			USBMuxPacketRelease(startListen);
		}
	});
}

void SDMMD_USBMuxListenerSend(SDMMD_USBMuxListenerRef listener, struct USBMuxPacket **packet) {
	listener->semaphore = dispatch_semaphore_create(0x0);
	SDMMD_USBMuxSend(listener->socket, *packet);
	dispatch_semaphore_wait(listener->semaphore, (*packet)->timeout);
	
	CFMutableArrayRef updateWithRemove = CFArrayCreateMutableCopy(kCFAllocatorDefault, 0x0, listener->responses);
	struct USBMuxPacket *responsePacket = NULL;
	uint32_t removeCounter = 0x0;
	for (uint32_t i = 0x0; i < CFArrayGetCount(listener->responses); i++) {
		struct USBMuxPacket *response = (struct USBMuxPacket *)CFArrayGetValueAtIndex(listener->responses, i);
		if ((*packet)->body.tag == response->body.tag) {
			responsePacket = response;
			CFArrayRemoveValueAtIndex(updateWithRemove, i-removeCounter);
			removeCounter++;
		}
	}
	CFSafeRelease(listener->responses);
	listener->responses = CFArrayCreateMutableCopy(kCFAllocatorDefault, 0x0, updateWithRemove);
	CFSafeRelease(updateWithRemove);
	USBMuxPacketRelease(*packet);
	if (!responsePacket) {
		responsePacket = (struct USBMuxPacket *)calloc(0x1, sizeof(struct USBMuxPacket));
	}
	*packet = responsePacket;
	dispatch_release(listener->semaphore);
}

void SDMMD_USBMuxSend(uint32_t sock, struct USBMuxPacket *packet) {	
	CFDataRef xmlData = CFPropertyListCreateXMLData(kCFAllocatorDefault, packet->payload);
	char *buffer = (char *)CFDataGetBytePtr(xmlData);
	ssize_t result = send(sock, &packet->body, sizeof(struct USBMuxPacketBody), 0x0);
	if (result == sizeof(struct USBMuxPacketBody)) {
		if (packet->body.length > result) {
			ssize_t payloadSize = packet->body.length - result;
			ssize_t remainder = payloadSize;
			while (remainder) {
				result = send(sock, &buffer[payloadSize-remainder], sizeof(char), 0x0);
				if (result != sizeof(char)) {
					break;
				}
				remainder -= result;
			}
		}
	}
	CFSafeRelease(xmlData);
}

void SDMMD_USBMuxListenerReceive(SDMMD_USBMuxListenerRef listener, struct USBMuxPacket *packet) {
	SDMMD_USBMuxReceive(listener->socket, packet);
}

void SDMMD_USBMuxReceive(uint32_t sock, struct USBMuxPacket *packet) {
	ssize_t result = recv(sock, &packet->body, sizeof(struct USBMuxPacketBody), 0x0);
	if (result == sizeof(struct USBMuxPacketBody)) {
		ssize_t payloadSize = packet->body.length - result;
		if (payloadSize) {
			char *buffer = calloc(0x1, payloadSize);
			ssize_t remainder = payloadSize;
			while (remainder) {
				result = recv(sock, &buffer[payloadSize-remainder], sizeof(char), 0x0);
				if (result != sizeof(char)) {
					break;
				}
				remainder -= result;
			}
			CFDataRef xmlData = CFDataCreate(kCFAllocatorDefault, (UInt8 *)buffer, payloadSize);
			packet->payload = CFPropertyListCreateFromXMLData(kCFAllocatorDefault, xmlData, kCFPropertyListImmutable, NULL);
			Safe(free,buffer);
			CFSafeRelease(xmlData);
		}
	}
}

struct USBMuxPacket * SDMMD_USBMuxCreatePacketType(SDMMD_USBMuxPacketMessageType type, CFDictionaryRef dict) {
	struct USBMuxPacket *packet = (struct USBMuxPacket *)calloc(1, sizeof(struct USBMuxPacket));
	if (type == kSDMMD_USBMuxPacketListenType || type == kSDMMD_USBMuxPacketConnectType) {
		packet->timeout = dispatch_time(DISPATCH_TIME_NOW, NSEC_PER_SEC*0x1e);
	}
	else {
		packet->timeout = dispatch_time(DISPATCH_TIME_NOW, NSEC_PER_SEC*0x5);
	}
	packet->body = (struct USBMuxPacketBody){0x10, 0x1, 0x8, transactionId};
	transactionId++;
	packet->payload = CFDictionaryCreateMutable(kCFAllocatorDefault, 0x0, &kCFTypeDictionaryKeyCallBacks, &kCFTypeDictionaryValueCallBacks);
	CFDictionarySetValue((CFMutableDictionaryRef)packet->payload, CFSTR("BundleID"), CFSTR("com.samdmarshall.sdmmobiledevice"));
	CFDictionarySetValue((CFMutableDictionaryRef)packet->payload, CFSTR("ClientVersionString"), CFSTR("usbmuxd-323"));
	CFDictionarySetValue((CFMutableDictionaryRef)packet->payload, CFSTR("ProgName"), CFSTR("SDMMobileDevice"));
    uint32_t version = 3;
    CFNumberRef versionNumber = CFNumberCreate(kCFAllocatorDefault, kCFNumberIntType, &version);
	CFDictionarySetValue((CFMutableDictionaryRef)packet->payload, CFSTR("kLibUSBMuxVersion"), versionNumber);
    CFSafeRelease(versionNumber);
    
	if (dict) {
		CFIndex count = CFDictionaryGetCount(dict);
		void *keys[count];
		void *values[count];
		CFDictionaryGetKeysAndValues(dict, (const void **)keys, (const void **)values);
		for (uint32_t i = 0x0; i < count; i++) {
			CFDictionarySetValue((CFMutableDictionaryRef)packet->payload, keys[i], values[i]);
		}
	}
	CFDictionarySetValue((CFMutableDictionaryRef)packet->payload, CFSTR("MessageType"), SDMMD_USBMuxPacketMessage[type]);
	if (type == kSDMMD_USBMuxPacketConnectType) {
		uint16_t port = 0x7ef2;
		CFNumberRef portNumber = CFNumberCreate(kCFAllocatorDefault, kCFNumberSInt16Type, &port);
		CFDictionarySetValue((CFMutableDictionaryRef)packet->payload, CFSTR("PortNumber"), portNumber);
		CFSafeRelease(portNumber);
	}
	if (type == kSDMMD_USBMuxPacketListenType) {
		uint32_t connection = 0x0;
		CFNumberRef connectionType = CFNumberCreate(kCFAllocatorDefault, kCFNumberIntType, &connection);
		CFDictionarySetValue((CFMutableDictionaryRef)packet->payload, CFSTR("ConnType"), connectionType);
		CFSafeRelease(connectionType);
	}

	CFDataRef xmlData = CFPropertyListCreateXMLData(kCFAllocatorDefault, packet->payload);
	packet->body.length = 0x10 + (uint32_t)CFDataGetLength(xmlData);
	CFSafeRelease(xmlData);
	return packet;
}

void USBMuxPacketRelease(struct USBMuxPacket *packet) {
	CFSafeRelease(packet->payload);
	Safe(free,packet);
}

#endif