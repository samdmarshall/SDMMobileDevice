//
//  MDDemoDeviceRef.m
//  SDM_MD_Demo
//
//  Created by sam on 6/21/13.
//  Copyright 2013 Sam Marshall. All rights reserved.
//

#import "MDDemoDeviceRef.h"


@implementation MDDemoDeviceRef

@synthesize device;
@synthesize deviceInfo;

- (id)initWithDevice:(SDM_AMDeviceRef)deviceref {
	self = [super init];
	if (self) {
		self.device = deviceref;
		self.deviceInfo = [NSMutableDictionary new];
	}
	return self;
}

- (CFTypeRef)getValueForKey:(NSString *)key fromDomain:(NSString *)domain {
	CFTypeRef result;
	if (!domain) {
		domain = @"NoDomain";
	}
	NSMutableDictionary *keyDomain;
	keyDomain = [self.deviceInfo objectForKey:domain];
	if (!keyDomain) {
		keyDomain = [NSMutableDictionary new];
	}
	
	result = [keyDomain objectForKey:key];
	if (!result) {
		result = SDMDeviceCopyValue(self.device, ([domain isEqualToString:@"NoDomain"] ? NULL : domain), key);
		[keyDomain setObject:(result ? result : CFSTR("")) forKey:key];
		[self.deviceInfo setObject:keyDomain forKey:domain];
	}
	return result;
}

- (void)dealloc {
	[self.deviceInfo release];
	[super dealloc];
}

@end
