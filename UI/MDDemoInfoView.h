//
//  MDDemoInfoView.h
//  SDM_MD_Demo
//
//  Created by Sam Marshall on 6/26/13.
//  Copyright 2013 Sam Marshall. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "MDDemoDevice.h"

@interface MDDemoInfoView : NSView <NSTableViewDelegate, NSTableViewDataSource> {}

@property (nonatomic, retain) NSTableView *appList;
@property (nonatomic, retain) NSArray *dataSource;
@property (nonatomic, retain) MDDemoDevice *device;
@property (nonatomic, readwrite) SDMMD_AMDebugConnectionRef debugConn;
@property (nonatomic, retain) NSTextField *appName;
@property (nonatomic, retain) NSTextField *appPath;

- (void)setActiveDevice:(MDDemoDevice *)device;

@end
