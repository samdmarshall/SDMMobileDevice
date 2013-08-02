//
//  MDDemoDeviceTable.m
//  SDM_MD_Demo
//
//  Created by Sam Marshall on 6/26/13.
//  Copyright 2013 Sam Marshall. All rights reserved.
//

#import "MDDemoDeviceTable.h"
#import "MDDemoDevice.h"
#import <SDMMobileDevice/SDMMobileDevice.h>
#import "MDDemoInfoView.h"
#import "MDDemoMainSplitView.h"

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
		[self.deviceTable setAllowsEmptySelection:YES];
		[self.deviceTable setAllowsMultipleSelection:NO];
		[self.deviceTable setAllowsColumnResizing:NO];
		[self.deviceTable setAllowsColumnReordering:NO];
		[self.deviceTable setUsesAlternatingRowBackgroundColors:YES];
		[self.deviceTable drawGridInClipRect:self.bounds];
		[self.deviceTable drawBackgroundInClipRect:self.bounds];
		[self.deviceTable setGridStyleMask:NSTableViewSolidVerticalGridLineMask];
		[self.deviceTable setTarget:self];
		[self.deviceTable setDoubleAction:@selector(updateAppList:)];
		[self updateDevices];
		self.deviceTable.delegate = self;
		self.deviceTable.dataSource = self;
		[self addSubview:self.deviceTable];
		[self.deviceTable setFrame:self.bounds];
		[self setNeedsDisplay:YES];
		
		[[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(updateDeviceTable:) name:@"SDMMD_USBMuxListenerDeviceAttachedNotificationFinished" object:nil];
		[[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(updateDeviceTable:) name:@"SDMMD_USBMuxListenerDeviceDetachedNotificationFinished" object:nil];

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

- (void)updateDeviceTable:(NSNotification *)notification {
	[self updateDevices];
}

- (void)updateDevices {
	SDMMobileDevice;
	CFArrayRef devices = SDMMD_AMDCreateDeviceList();
	NSMutableArray *list = [NSMutableArray new];
	for (uint32_t i = 0; i < CFArrayGetCount(devices); i++) {
		MDDemoDevice *device = [[MDDemoDevice alloc] initWithDevice:(SDMMD_AMDeviceRef)CFArrayGetValueAtIndex(devices, i)];
		[list addObject:device];
	}
	if (dataSource != nil)
		[dataSource release];
	dataSource = [list copy];
	[list release];
	int row = [self.deviceTable selectedRow];
	if (row != -1)
		[self.deviceTable deselectRow:row];
	[self.deviceTable reloadData];
}

- (void)updateAppList:(id)object {
	int row = [self.deviceTable selectedRow];
	if (row != -1)
		[(MDDemoInfoView*)[(MDDemoMainSplitView*)[[self superview] superview] infoView] setActiveDevice:[dataSource objectAtIndex:row]]; 
}

#pragma mark -
#pragma mark NSTableViewDelegate + NSTableViewDataSource
- (NSInteger)numberOfRowsInTableView:(NSTableView *)tableView {
	return [dataSource count] + ((self.bounds.size.height - ([dataSource count]*[tableView rowHeight]))/[tableView rowHeight]);
}

- (id)tableView:(NSTableView *)aTableView objectValueForTableColumn:(NSTableColumn *)aTableColumn row:(NSInteger)rowIndex {
	return (rowIndex < dataSource.count ? [[dataSource objectAtIndex:rowIndex] name] : nil);
}

- (BOOL)tableView:(NSTableView *)aTableView shouldSelectRow:(NSInteger)rowIndex {
	return (rowIndex < dataSource.count ? true : false);
}

@end
