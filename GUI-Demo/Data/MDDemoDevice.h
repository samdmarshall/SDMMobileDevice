//
//  MDDemoDevice.h
//  SDM_MD_Demo
//
//  Created by sam on 7/29/13.
//  Copyright 2013 Sam Marshall. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import <SDMMobileDevice/SDMMobileDevice.h>

@interface MDDemoDevice : NSObject {
	NSString *name;
	SDMMD_AMDeviceRef device;
}

@property (nonatomic, retain) NSString *name;
@property (nonatomic, readwrite) SDMMD_AMDeviceRef device;

- (id)initWithDevice:(SDMMD_AMDeviceRef)dev;
- (void)dealloc;

@end
