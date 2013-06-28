//
//  MDDemoDeviceRef.h
//  SDM_MD_Demo
//
//  Created by sam on 6/21/13.
//  Copyright 2013 Sam Marshall. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import <SDMMobileDevice/MobileDevice.h>

@interface MDDemoDeviceRef : NSObject {
	SDM_AMDeviceRef device;
	NSMutableDictionary *deviceInfo;
}

@property (nonatomic, readwrite) SDM_AMDeviceRef device;
@property (nonatomic, retain) NSMutableDictionary *deviceInfo;

- (id)initWithDevice:(SDM_AMDeviceRef)deviceref;
- (CFTypeRef)getValueForKey:(NSString *)key fromDomain:(NSString *)domain;

@end
