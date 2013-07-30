//
//  MDDemoDevice.m
//  SDM_MD_Demo
//
//  Created by sam on 7/29/13.
//  Copyright 2013 Sam Marshall. All rights reserved.
//

#import "MDDemoDevice.h"

@implementation MDDemoDevice

@synthesize name;
@synthesize device;

- (id)initWithDevice:(SDMMD_AMDeviceRef)dev {
	self = [super init];
	if (self) {
		self.device = dev;
		SDMMD_AMDeviceConnect(self.device);
		self.name = SDMMD_AMDeviceCopyValue(self.device, NULL, CFSTR(kDeviceName));
		SDMMD_AMDeviceDisconnect(self.device);
		
	}
	return self;
}

- (void)dealloc {
	[super dealloc];
	[name release];
}

@end
