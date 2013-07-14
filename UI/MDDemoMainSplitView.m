//
//  MDDemoMainSplitView.m
//  SDM_MD_Demo
//
//  Created by Sam Marshall on 6/21/13.
//  Copyright 2013 Sam Marshall. All rights reserved.
//

#import "MDDemoMainSplitView.h"


@implementation MDDemoMainSplitView

@synthesize splitView;
@synthesize deviceList;
@synthesize infoView;

- (id)initWithFrame:(NSRect)frameRect {
	self = [super initWithFrame:frameRect];
	if (self) {
		self.splitView = [[NSSplitView alloc] init];
		[self.splitView setFrame:frameRect];
		[self.splitView setVertical:YES];
		[self.splitView setDividerStyle:NSSplitViewDividerStyleThin];
		self.deviceList = [[MDDemoDeviceTable alloc] initWithFrame:CGRectMake(0, 0, 200, 666)];
		[self.splitView addSubview:self.deviceList];
		self.infoView = [[MDDemoInfoView alloc] initWithFrame:CGRectMake(200, 0, 800, 666)];
		[self.splitView addSubview:self.infoView];
		self.splitView.delegate = self;
		[self.splitView setAutoresizesSubviews:YES];
		[self.splitView adjustSubviews];
		[self addSubview:self.splitView];
		[self setNeedsDisplay:YES];
	}
	return self;
}

- (void)dealloc {
	[splitView release];
	[super dealloc];
}

- (CGFloat)splitView:(NSSplitView *)_splitView constrainMaxCoordinate:(CGFloat)proposedMax ofSubviewAt:(NSInteger)dividerIndex {
	NSView *subview = [[_splitView subviews] objectAtIndex:dividerIndex];
	CGFloat maximumSize = (dividerIndex ? 765 : 275);
	return subview.frame.origin.x + maximumSize;
}

- (CGFloat)splitView:(NSSplitView *)_splitView constrainMinCoordinate:(CGFloat)proposedMin ofSubviewAt:(NSInteger)dividerIndex {
	NSView *subview = [[_splitView subviews] objectAtIndex:dividerIndex];
	CGFloat minimumSize = (dividerIndex ? 690 : 200);
	return subview.frame.origin.x + minimumSize;
}

@end
