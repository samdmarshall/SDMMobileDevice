//
//  MDDemoInfoView.m
//  SDM_MD_Demo
//
//  Created by Sam Marshall on 6/26/13.
//  Copyright 2013 Sam Marshall. All rights reserved.
//

#import "MDDemoInfoView.h"

@implementation MDDemoInfoView

@synthesize appList;
@synthesize dataSource;
@synthesize device;
@synthesize debugConn;
@synthesize appName;
@synthesize appPath;

- (id)initWithFrame:(NSRect)frame {
    self = [super initWithFrame:frame];
    if (self) {
		[self setAutoresizesSubviews:YES];
		[self setAutoresizingMask:NSViewHeightSizable | NSViewWidthSizable];
		self.dataSource = [[NSArray alloc] init];
        self.appList = [[NSTableView alloc] initWithFrame:CGRectMake(0, 0, 300, self.bounds.size.height)];
		NSTableColumn *column = [[NSTableColumn alloc] initWithIdentifier:@"AppName"];
		[column setWidth:300];
		[self.appList addTableColumn:column];
		[self.appList setAllowsColumnSelection:NO];
		[self.appList setAllowsEmptySelection:YES];
		[self.appList setAllowsMultipleSelection:NO];
		[self.appList setAllowsColumnResizing:NO];
		[self.appList setAllowsColumnReordering:NO];
		[self.appList setUsesAlternatingRowBackgroundColors:YES];
		[self.appList drawGridInClipRect:self.bounds];
		[self.appList drawBackgroundInClipRect:self.bounds];
		[self.appList setGridStyleMask:NSTableViewSolidVerticalGridLineMask];
		[self.appList setTarget:self];
		[self.appList setDoubleAction:@selector(setApplication:)];
		self.appList.delegate = self;
		self.appList.dataSource = self;
		[self addSubview:self.appList];
		[self.appList setFrame:self.bounds];
		[self setNeedsDisplay:YES];
		
		NSButton *run = [[NSButton alloc] initWithFrame:CGRectMake(self.bounds.origin.x+300 + ((self.bounds.size.width-300)/2)-150, 2*(self.bounds.size.height/3), 100, 25)];
		NSButton *quit = [[NSButton alloc] initWithFrame:CGRectMake(self.bounds.origin.x+300 + ((self.bounds.size.width-300)/2)+50, 2*(self.bounds.size.height/3), 100, 25)];
		[run setButtonType:NSMomentaryLightButton];
		[run setBezelStyle:NSRoundedBezelStyle];
		[quit setButtonType:NSMomentaryLightButton];
		[quit setBezelStyle:NSRoundedBezelStyle];
		[run setTitle:@"Run App"];
		[quit setTitle:@"Quit App"];
		[run setAction:@selector(runApp:)];
		[quit setAction:@selector(quitApp:)];
		[run setTarget:self];
		[quit setTarget:self];
		
		[self addSubview:run];
		[self addSubview:quit];
		
		appName = [[NSTextField alloc] initWithFrame:CGRectMake(self.bounds.origin.x+300 + ((self.bounds.size.width-300)/2)-200, (self.bounds.size.height/3), 400, 25)];
		[appName setEditable:NO];
		[appName setBackgroundColor:[NSColor clearColor]];
		[appName setBordered:NO];
		[appName setAlignment:NSCenterTextAlignment];
		[self addSubview:appName];
		
		appPath = [[NSTextField alloc] initWithFrame:CGRectMake(self.bounds.origin.x+300, (self.bounds.size.height/3) + 100, (self.bounds.size.width-300), 25)];
		[appPath setEditable:NO];
		[appPath setBackgroundColor:[NSColor clearColor]];
		[appPath setBordered:NO];
		[appPath setAlignment:NSCenterTextAlignment];
		[self addSubview:appPath];
		
    }
    return self;
}

- (void)setApplication:(id)object {
	int row = [object selectedRow];
	CFDictionaryRef app;
	if (row != -1) {
		app = (CFDictionaryRef)[self.dataSource objectAtIndex:row];
		
		[appPath setStringValue:CFDictionaryGetValue(app, @"Container")];
		[appName setStringValue:CFDictionaryGetValue(app, @"CFBundleDisplayName")];

		sdmmd_return_t result = SDMMD_StartDebuggingSessionOnDevice(self.device.device, &debugConn);
		printf("debug start: 0x%08x\n",result);
		if (result) {
			NSLog(@"Could not connect to developer tools!");
		}
		result = SDMMD_StopDebuggingSessionOnDevice(self.device.device, &debugConn);
		printf("debug stop: 0x%08x\n",result);	
	}
}

- (IBAction)runApp:(id)sender {
	dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0x0), ^{
		int row = [self.appList selectedRow];
		CFDictionaryRef app;
		if (row != -1) {
			app = (CFDictionaryRef)[self.dataSource objectAtIndex:row];
			
			sdmmd_return_t result = SDMMD_StartDebuggingSessionOnDevice(self.device.device, &debugConn);
			printf("debug start: 0x%08x\n",result);
			
			CFShow(CFDictionaryGetValue(app, CFSTR("Path")));
			CFStringRef encodedPath = SDMMD_CreateEncodeForDebuggingCommand(CFDictionaryGetValue(app, CFSTR("Path")));
			CFStringRef containerPath = SDMMD_CreateEncodeForDebuggingCommand(CFDictionaryGetValue(app, CFSTR("Container")));
			CFShow(encodedPath);
			CFShow(containerPath);
			sdmmd_debug_return_t dresult;
			dresult = SDMMD_DebuggingSend(self.debugConn, KnownDebugCommands[kDebugQSetMaxPacketSize], SDMMD_CreateEncodeForDebuggingCommand(CFSTR("1024")));
			CFShow(dresult.data);
			dresult = SDMMD_DebuggingSend(self.debugConn, KnownDebugCommands[kDebugQSetWorkingDir], containerPath);
			CFShow(dresult.data);
			NSString *commandformat = [NSString stringWithFormat:@"%d,0,%@",(uint32_t)CFStringGetLength(encodedPath),encodedPath];
			dresult = SDMMD_DebuggingSend(self.debugConn, KnownDebugCommands[kDebugA], (CFStringRef)commandformat);
			CFShow(dresult.data);
			//dresult = SDMMD_DebuggingSend(self.debugConn, KnownDebugCommands[kDebugqLaunchSuccess], CFSTR(""));
			//CFShow(dresult.data);
			//dresult = SDMMD_DebuggingSend(self.debugConn, KnownDebugCommands[kDebugqGetPid], CFSTR(""));
			//CFShow(dresult.data);
			dresult = SDMMD_DebuggingSend(self.debugConn, KnownDebugCommands[kDebugH], CFSTR("c0"));
			CFShow(dresult.data);
			dresult = SDMMD_DebuggingSend(self.debugConn, KnownDebugCommands[kDebugc], CFSTR(""));
			CFShow(dresult.data);
		}
	});
}

- (IBAction)quitApp:(id)sender {
	dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0x0), ^{
		sdmmd_debug_return_t dresult = SDMMD_DebuggingSend(self.debugConn, KnownDebugCommands[kDebugk], CFSTR(""));
		CFShow(dresult.data);
		sdmmd_return_t result = SDMMD_StopDebuggingSessionOnDevice(self.device.device, &debugConn);
		printf("debug stop: 0x%08x\n",result);
	});
}

- (void)setActiveDevice:(MDDemoDevice *)sentdevice {
	self.device = sentdevice;
	sdmmd_return_t result = 0x0;
	result = SDMMD_AMDeviceConnect(self.device.device);
	printf("connect: 0x%08x\n",result);
	bool paired = SDMMD_AMDeviceIsPaired(self.device.device);
	printf("paired status: %s\n",(paired ? "yes" : "no"));
	result = SDMMD_AMDeviceValidatePairing(self.device.device);
	printf("validate: 0x%08x\n",result);
	result = SDMMD_AMDeviceStartSession(self.device.device);
	printf("start session: 0x%08x\n",result);
	
	CFDictionaryRef response;
	CFArrayRef values = SDMMD_ApplicationLookupDictionary();
	CFMutableDictionaryRef optionsDict = SDMMD_create_dict();
	CFDictionarySetValue(optionsDict, CFSTR("ReturnAttributes"), values);
	
	NSMutableArray *apps = [NSMutableArray new];
	result = SDMMD_AMDeviceLookupApplications(self.device.device, optionsDict, &response);
	printf("lookup: 0x%08x\n",result);
	for (id item in (NSDictionary*)response) {
		NSDictionary *appInfo = [(NSDictionary*)response objectForKey:item];
		if (CFDictionaryContainsKey((CFDictionaryRef)appInfo, CFSTR("Container")))
			[apps addObject:appInfo];
	}
	result = SDMMD_AMDeviceStopSession(self.device.device);
	printf("stop session: 0x%08x\n",result);
	result = SDMMD_AMDeviceDisconnect(self.device.device);
	
	self.dataSource = apps;
	[self.appList reloadData];
}

- (BOOL)isFlipped {
	return YES;
}

#pragma mark -
#pragma mark NSTableViewDelegate + NSTableViewDataSource
- (NSInteger)numberOfRowsInTableView:(NSTableView *)tableView {
	return [self.dataSource count] + ((self.bounds.size.height - ([dataSource count]*[tableView rowHeight]))/[tableView rowHeight]);
}

- (id)tableView:(NSTableView *)aTableView objectValueForTableColumn:(NSTableColumn *)aTableColumn row:(NSInteger)rowIndex {
	return (rowIndex < self.dataSource.count ? [[self.dataSource objectAtIndex:rowIndex] objectForKey:@"CFBundleDisplayName"] : nil);
}

- (BOOL)tableView:(NSTableView *)aTableView shouldSelectRow:(NSInteger)rowIndex {
	return (rowIndex < self.dataSource.count ? true : false);
}

@end
