//
//  MDDemoDeviceTable.m
//  SDM_MD_Demo
//
//  Created by Sam Marshall on 6/26/13.
//  Copyright 2013 Sam Marshall. All rights reserved.
//

#import "MDDemoDeviceTable.h"


@implementation MDDemoDeviceTable

@synthesize deviceTable;
@synthesize dataSource;

- (id)initWithFrame:(NSRect)frame {
    self = [super initWithFrame:frame];
    if (self) {
		[self setAutoresizesSubviews:YES];
		[self setAutoresizingMask:NSViewHeightSizable | NSViewWidthSizable];
		self.dataSource = [[NSArray alloc] init];
        self.deviceTable = [[NSTableView alloc] initWithFrame:self.bounds];
		NSTableColumn *column = [[NSTableColumn alloc] initWithIdentifier:@"DeviceName"];
		[column setWidth:self.bounds.size.width];
		[self.deviceTable addTableColumn:column];
		[self.deviceTable setAllowsColumnSelection:NO];
		[self.deviceTable setAllowsEmptySelection:NO];
		[self.deviceTable setAllowsMultipleSelection:NO];
		[self.deviceTable setAllowsColumnResizing:NO];
		[self.deviceTable setAllowsColumnReordering:NO];
		[self.deviceTable setUsesAlternatingRowBackgroundColors:YES];
		self.deviceTable.delegate = self;
		self.deviceTable.dataSource = self;
		[self addSubview:self.deviceTable];
		[self setNeedsDisplay:YES];
    }
    return self;
}

- (void)dealloc {
	[deviceTable release];
	[super dealloc];
}

- (BOOL)isFlipped {
	return YES;
}

#pragma mark -
#pragma mark NSTableViewDelegate + NSTableViewDataSource
- (NSInteger)numberOfRowsInTableView:(NSTableView *)tableView {
	return [dataSource count];
}

- (NSCell *)tableView:(NSTableView *)tableView dataCellForTableColumn:(NSTableColumn *)tableColumn row:(NSInteger)row {
	NSCell *newCell = [[NSCell alloc] initTextCell:@"Testing"];
	return newCell;
}

@end
