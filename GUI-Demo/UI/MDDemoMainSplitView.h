//
//  MDDemoMainSplitView.h
//  SDM_MD_Demo
//
//  Created by Sam Marshall on 6/21/13.
//  Copyright 2013 Sam Marshall. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "MDDemoDeviceTable.h"
#import "MDDemoInfoView.h"

@interface MDDemoMainSplitView : NSView <NSSplitViewDelegate> {}

@property (nonatomic, retain) NSSplitView *splitView;
@property (nonatomic, retain) MDDemoDeviceTable *deviceList;
@property (nonatomic, retain) MDDemoInfoView *infoView;

- (id)initWithFrame:(NSRect)frameRect;

- (CGFloat)splitView:(NSSplitView *)splitView constrainMaxCoordinate:(CGFloat)proposedMax ofSubviewAt:(NSInteger)dividerIndex;
- (CGFloat)splitView:(NSSplitView *)splitView constrainMinCoordinate:(CGFloat)proposedMin ofSubviewAt:(NSInteger)dividerIndex;

@end
