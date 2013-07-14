//
//  SDM_MD_DemoAppDelegate.m
//  SDM_MD_Demo
//
//  Created by Sam Marshall on 5/24/13.
//  Copyright 2013 Sam Marshall. All rights reserved.
//

#import "SDM_MD_DemoAppDelegate.h"
#import "MDDemoMainSplitView.h"
#import "SDMMobileDevice/SDMMobileDevice.h"

@implementation SDM_MD_DemoAppDelegate

@synthesize window;

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification {
	MDDemoMainSplitView *splitView = [[MDDemoMainSplitView alloc] initWithFrame:[self.window.contentView bounds]];
	[self.window.contentView addSubview:splitView];
	// Insert code here to initialize your application 
	SDMMD_MCP;
	CFArrayRef devices = SDMMD_AMDCreateDeviceList();
	for (uint32_t i = 0; i < CFArrayGetCount(devices); i++) {
		SDMMD_AMDeviceRef device = (SDMMD_AMDeviceRef)CFArrayGetValueAtIndex(devices, i);
		SDMMD_AMDebugConnectionRef debugConn = NULL;
		uint32_t result = SDMMD_StartDebuggingSessionOnDevice(device, &debugConn);
		printf("debug start: 0x%08x\n",result);
		/*CFStringRef encodedPath = SDMMD_EncodeForDebuggingCommand(CFSTR(""));
		CFStringRef containerPath = SDMMD_EncodeForDebuggingCommand(CFSTR(""));
		sdmmd_debug_return_t dresult;
		CFStringRef commandformat = (CFStringRef)[NSString stringWithFormat:@"%d,0,%@",(uint32_t)CFStringGetLength(encodedPath),encodedPath];
		dresult = SDMMD_DebuggingSend(debugConn, KnownDebugCommands[kDebugA], commandformat);
		CFShow(dresult.data);
		dresult = SDMMD_DebuggingSend(debugConn, KnownDebugCommands[kDebugH], CFSTR("c0"));
		CFShow(dresult.data);*/
		
		result = SDMMD_StopDebuggingSessionOnDevice(device, &debugConn);
		printf("debug stop: 0x%08x\n",result);
		/*uint32_t result = SDMMD_AMDeviceConnect(device);
		printf("connect: 0x%08x\n",result);
		bool paired = SDMMD_AMDeviceIsPaired(device);
		printf("paired status: %s\n",(paired ? "yes" : "no"));
		result = SDMMD_AMDeviceValidatePairing(device);
		printf("validate: 0x%08x\n",result);
		result = SDMMD_AMDeviceStartSession(device);
		printf("start session: 0x%08x\n",result);
		
		CFMutableDictionaryRef dict = NULL;
		SDMMD_AMConnectionRef connection = SDMMD_AMDServiceConnectionCreate(0, NULL, dict);
		result = SDMMD_AMDeviceStartService(device, CFSTR(AMSVC_SPRINGBOARD_SERVICES), NULL, connection);
		printf("service start: 0x%08x\n",result);
		
		result = SDMMD_AMDeviceStopSession(device);
		printf("stop session: 0x%08x\n",result);
		result = SDMMD_AMDeviceDisconnect(device);
		printf("disconnect: 0x%08x\n",result);*/
	}
}

@end
