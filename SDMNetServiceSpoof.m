//
//  SDMNetServiceSpoof.m
//  SDM_MD_Demo
//
//  Created by sam on 5/30/13.
//  Copyright 2013 Sam Marshall. All rights reserved.
//

#import "SDMNetServiceSpoof.h"


@implementation SDMNetServiceSpoof

- (id)init {
	self = [super init];
	if (self) {
		browser = [[NSNetServiceBrowser alloc] init];
		[browser setDelegate:self];
		[browser searchForServicesOfType:@"_apple-mobdev._tcp." inDomain:@"local."];
	}
	return self;
}

- (void)netServiceBrowser:(NSNetServiceBrowser *)netServiceBrowser didFindService:(NSNetService *)netService moreComing:(BOOL)moreServicesComing {
	//NSLog(@"%@ --- %i",netService.name, netService.port);
	
	//NSNetService *test = [[NSNetService alloc] initWithDomain:@"local." type:@"_apple-mobdev._tcp." name:@"88:1f:a1:5b:3f:7f" port:62078];
	//[test publish];
}

@end
