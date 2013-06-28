//
//  SDMNetServiceSpoof.h
//  SDM_MD_Demo
//
//  Created by sam on 5/30/13.
//  Copyright 2013 Sam Marshall. All rights reserved.
//

#import <Cocoa/Cocoa.h>


@interface SDMNetServiceSpoof : NSObject <NSNetServiceBrowserDelegate> {
	NSNetServiceBrowser *browser;
}

@end
