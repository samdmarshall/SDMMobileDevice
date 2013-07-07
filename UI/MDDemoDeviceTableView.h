//
//  MDDemoDeviceTableView.h
//  SDM_MD_Demo
//
//  Created by sam on 6/21/13.
//  Copyright 2013 Sam Marshall. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "MDDemoDeviceInfoView.h"

@interface MDDemoDeviceTableView : NSObject <NSTableViewDataSource, NSTableViewDelegate> {
	IBOutlet NSTableView *deviceTable;
	IBOutlet MDDemoDeviceInfoView *infoView;
	NSMutableArray *deviceList;
}

@property (nonatomic, retain) NSMutableArray *deviceList;

#pragma mark -
#pragma mark NSTableView
- (NSInteger)numberOfRowsInTableView:(NSTableView *)tableView;
- (id)tableView:(NSTableView *)aTableView objectValueForTableColumn:(NSTableColumn *)aTableColumn row:(NSInteger)rowIndex;

@end
