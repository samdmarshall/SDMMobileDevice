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
		uint32_t result = SDMMD_AMDeviceConnect(device);
		printf("connect: 0x%08x\n",result);
		bool paired = SDMMD_AMDeviceIsPaired(device);
		printf("paired status: %s\n",(paired ? "yes" : "no"));
		result = SDMMD_AMDeviceValidatePairing(device);
		printf("validate: 0x%08x\n",result);
		result = SDMMD_AMDeviceStartSession(device);
		printf("start session: 0x%08x\n",result);
		CFShow(device->ivars.session);
		result = SDMMD_AMDeviceStopSession(device);
		printf("stop session: 0x%08x\n",result);
	}
}

@end
