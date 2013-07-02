//
//  SDM_MD_DemoAppDelegate.m
//  SDM_MD_Demo
//
//  Created by Sam Marshall on 5/24/13.
//  Copyright 2013 Sam Marshall. All rights reserved.
//

#import "SDM_MD_DemoAppDelegate.h"
#import "MDDemoMainSplitView.h"
#import "MobileDevice/MobileDevice.h"

@implementation SDM_MD_DemoAppDelegate

@synthesize window;

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification {
	MDDemoMainSplitView *splitView = [[MDDemoMainSplitView alloc] initWithFrame:[self.window.contentView bounds]];
	[self.window.contentView addSubview:splitView];
	// Insert code here to initialize your application 
	//dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ^{
		//SDMUSBMuxStartListener(&SDMMDController->usbmuxd);
		//CFArrayRef devices = SDM_AMDCreateDeviceList();
		//dispatch_async(dispatch_get_main_queue(), ^{
			void *createDevice = SDMSTSymbolLookup(SDMMDController->lookupTable, "AMDeviceActivate");
			printf("%x\n",createDevice);
			uint32_t test = SDMSTGetArgumentCount(SDMMDController->lookupTable, createDevice);
			NSLog(@"%i",test);

	//	});
	//});
	
	/*CFArrayRef devices = SDM_AMDCreateDeviceList();
	for (uint32_t i = 0; i < CFArrayGetCount(devices); i++) {
		SDM_AMDeviceRef device = (SDM_AMDeviceRef)CFArrayGetValueAtIndex(devices, i);		
		CFShow(device);
		if (SDM_MD_CallSuccessful(AMDeviceConnect(device))) {
				AMDeviceStartSession(device);
				CFShow(AMDeviceCopyValue(device, CFSTR(kDiskUsageDomain), CFSTR(kCalculateDiskUsage)));
				CFShow(AMDeviceCopyValue(device, CFSTR(kDiskUsageDomain), CFSTR(kTotalDiskCapacity)));
				CFShow(AMDeviceCopyValue(device, CFSTR(kDiskUsageDomain), CFSTR(kTotalSystemCapacity)));
				CFShow(AMDeviceCopyValue(device, CFSTR(kDiskUsageDomain), CFSTR(kTotalSystemAvailable)));
				CFShow(AMDeviceCopyValue(device, CFSTR(kDiskUsageDomain), CFSTR(kTotalDataCapacity)));
				CFShow(AMDeviceCopyValue(device, CFSTR(kDiskUsageDomain), CFSTR(kTotalDataAvailable)));
				CFShow(AMDeviceCopyValue(device, CFSTR(kDiskUsageDomain), CFSTR(kAmountDataReserved)));
				CFShow(AMDeviceCopyValue(device, CFSTR(kDiskUsageDomain), CFSTR(kAmountDataAvailable)));
				
				AMDeviceStopSession(device);
				AMDeviceDisconnect(device);
		}
	}*/
		
	/*struct USBMuxPacket *responseListen = (struct USBMuxPacket *)malloc(sizeof(struct USBMuxPacket));
	SDMUSBMuxListenerReceive(test, responseListen);
	CFShow(responseListen->payload);
	struct USBMuxPacket devicesPacket = SDMUSBMuxCreatePacketType(kSDMUSBMuxPacketListDevicesType, NULL);
	SDMUSBMuxListenerSend(test, devicesPacket);
	struct USBMuxPacket *deviceOne = (struct USBMuxPacket *)malloc(sizeof(struct USBMuxPacket));
	SDMUSBMuxListenerReceive(test, deviceOne);
	CFShow(deviceOne->payload);
	struct USBMuxPacket *deviceTwo = (struct USBMuxPacket *)malloc(sizeof(struct USBMuxPacket));
	SDMUSBMuxListenerReceive(test, deviceTwo);
	CFShow(deviceTwo->payload);
	struct USBMuxPacket *deviceThree = (struct USBMuxPacket *)malloc(sizeof(struct USBMuxPacket));
	SDMUSBMuxListenerReceive(test, deviceThree);
	CFShow(deviceThree->payload);*/
	/*CFArrayRef devices = SDM_AMDCreateDeviceList();
	for (uint32_t i = 0; i < CFArrayGetCount(devices); i++) {
		SDM_AMDeviceRef device = (SDM_AMDeviceRef)CFArrayGetValueAtIndex(devices, i);
		CFShow(device);
		CFDictionaryRef appResults = NULL;
		NSArray *options = [NSArray arrayWithObjects:@kAppLookupKeyCFBundleIdentifier, @kAppLookupKeyPath, @kAppLookupKeyContainer, @kAppLookupKeyCFBundleDisplayName,@kAppLookupKeyCFBundleExecutable, nil];
		SDM_AMDeviceLookupApplications(device, (CFArrayRef)options, &appResults);
		SDM_AMDebugConnectionRef connection = NULL;
		if (SDM_MD_CallSuccessful(SDMMDStartDebuggingSessionOnDevice(device, &connection))) {
			NSDictionary *test = [(NSDictionary *)appResults objectForKey:@"com.tapbots.TweetbotPad"];
			if (test) {
				NSString *path = [test objectForKey:@kAppLookupKeyPath];
				CFStringRef encodedPath = SDMMDEncodeForDebuggingCommand(path);
				CFStringRef containerPath = SDMMDEncodeForDebuggingCommand([test objectForKey:@kAppLookupKeyContainer]);
				sdmmd_debug_return_t result;
				result = SDMMDDebuggingSend(connection, KnownDebugCommands[kDebugQSetMaxPacketSize], SDMMDEncodeForDebuggingCommand(CFSTR("1024")));
				CFShow(result.data);
				result = SDMMDDebuggingSend(connection, KnownDebugCommands[kDebugQSetWorkingDir], containerPath);
				CFShow(result.data);
				//NSString *commandformat = [NSString stringWithFormat:@"%d,0,%@",(uint32_t)CFStringGetLength(encodedPath),encodedPath];
				//result = SDMMDDebuggingSend(connection, KnownDebugCommands[kDebugA], commandformat);
				//CFShow(result.data);
				//result = SDMMDDebuggingSend(connection, KnownDebugCommands[kDebugH], CFSTR("c0"));
				//CFShow(result.data);
				
				//result = SDMMDDebuggingSend(connection, KnownDebugCommands[kDebugvAttach], CFSTR("11B"));
				//CFShow(result.data);
				//result = SDMMDDebuggingSend(connection, KnownDebugCommands[kDebugqGetPid], NULL);
				//CFShow(result.data);
				
			}
			//SDMMDStopDebuggingSessionOnDevice(device, &connection);
		}
	}*/
}

@end
