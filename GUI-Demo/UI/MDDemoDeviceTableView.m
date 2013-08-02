//
//  MDDemoDeviceTableView.m
//  SDM_MD_Demo
//
//  Created by sam on 6/21/13.
//  Copyright 2013 Sam Marshall. All rights reserved.
//

#import "MDDemoDeviceTableView.h"
#import <SDMMobileDevice/MobileDevice.h>
#import "MDDemoDeviceRef.h"

@implementation MDDemoDeviceTableView

@synthesize deviceList;

- (void)getDevicesFromDeviceController {
	[deviceList removeAllObjects];
	CFArrayRef devices = CFArrayCreateMutableCopy(kCFAllocatorDefault, 0x0, SDMMDController->deviceList);
	for (uint32_t i = 0; i < CFArrayGetCount(devices); i++) {
		MDDemoDeviceRef *addDevice = [[MDDemoDeviceRef alloc] initWithDevice:CFArrayGetValueAtIndex(devices,i)];
		[deviceList addObject:addDevice];
	}
}

- (void)deviceConnectionEvent:(NSNotification *)notification {
	[self getDevicesFromDeviceController];
	[deviceTable reloadData];
}

- (void)awakeFromNib {
	[[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(deviceConnectionEvent:) name:@"SDMUSBMuxListenerDeviceAttachedNotification" object:nil];
	[[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(deviceConnectionEvent:) name:@"SDMUSBMuxListenerDeviceDetachedNotification" object:nil];
}

- (id)init {
	self = [super init];
	if (self) {
		deviceTable.delegate = self;
		SDMMDController;
		SDM_AMDCreateDeviceList();
		deviceList = [NSMutableArray new];
		[self getDevicesFromDeviceController];
	}
	return self;
}

- (void)dealloc {
	[[NSNotificationCenter defaultCenter] removeObserver:self];
	[deviceList release];
	[super dealloc];
}

- (void)updateTableSelection:(NSInteger)index {
	if (index == -1)
		index = [deviceTable selectedRow];
	[infoView setSelectedDevice:[deviceList objectAtIndex:index]];
}

#pragma mark -
#pragma mark NSTableView
- (NSInteger)numberOfRowsInTableView:(NSTableView *)tableView {
	return [deviceList count];
}

- (id)tableView:(NSTableView *)aTableView objectValueForTableColumn:(NSTableColumn *)aTableColumn row:(NSInteger)rowIndex {
	return [[deviceList objectAtIndex:rowIndex] getValueForKey:[aTableColumn identifier] fromDomain:nil];
}

- (void)tableViewSelectionDidChange:(NSNotification *)aNotification {
	[self updateTableSelection:-1];
}

- (BOOL)tableView:(NSTableView *)aTableView shouldSelectRow:(NSInteger)rowIndex {
	[self updateTableSelection:rowIndex];
	return YES;
}

- (BOOL)tableView:(NSTableView *)tableView shouldTrackCell:(NSCell *)cell forTableColumn:(NSTableColumn *)tableColumn row:(NSInteger)row {
	[self updateTableSelection:row];
	return YES;
}

@end
