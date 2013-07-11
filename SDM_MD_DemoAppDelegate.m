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
	CFArrayRef devices = SDMMD_MCP->deviceList;
	SDMMD_AMDeviceRef device;
	for (uint32_t i = 0; i < CFArrayGetCount(devices); i++) {
		device = (SDMMD_AMDeviceRef)CFArrayGetValueAtIndex(devices, i);
		sdmmd_return_t result = SDMMD_AMDeviceConnect(device);
		printf("code: %08x %s\n",result,SDMMD_AMDErrorString(result));
	}
}

@end
