//
//  MDDemoDeviceInfoView.h
//  SDM_MD_Demo
//
//  Created by sam on 6/21/13.
//  Copyright 2013 Sam Marshall. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "MDDemoDeviceRef.h"

@interface MDDemoDeviceInfoView : NSView {
	MDDemoDeviceRef *device;
	IBOutlet NSLevelIndicator *space;
}
@property (nonatomic, retain) MDDemoDeviceRef *device;
@property (nonatomic, retain) IBOutlet NSLevelIndicator *space;

- (void)setSelectedDevice:(MDDemoDeviceRef *)deviceref;
- (IBAction)SyncDevice:(id)sender;

@end
