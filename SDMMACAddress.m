//
//  SDMMACAddress.m
//  SDM_MD_Demo
//
//  Created by sam on 5/30/13.
//  Copyright 2013 Sam Marshall. All rights reserved.
//

#import "SDMMACAddress.h"


@implementation SDMMACAddress

@synthesize MACAddress = _MACAddress;

- (id)initWithMAC:(NSString *)address {
	self = [super init];
	if (self) {
		_MACAddress = address;
		NSScanner *octetScanner = [NSScanner scannerWithString:_MACAddress];
		[octetScanner scanHexInt:&octet0];
		[octetScanner setScanLocation:octetScanner.scanLocation+1];
		[octetScanner scanHexInt:&octet1];
		[octetScanner setScanLocation:octetScanner.scanLocation+1];
		[octetScanner scanHexInt:&octet2];
		[octetScanner setScanLocation:octetScanner.scanLocation+1];
		[octetScanner scanHexInt:&octet5];
		[octetScanner setScanLocation:octetScanner.scanLocation+1];
		[octetScanner scanHexInt:&octet6];
		[octetScanner setScanLocation:octetScanner.scanLocation+1];
		[octetScanner scanHexInt:&octet7];
	}
	return self;
}

- (NSString *)IPV6LocalLink {
	octet3 = 0xFF;
	octet4 = 0xFE;
	uint8_t new_octet = (octet0 & 0x2 ? octet0-0x2 : octet0+0x2);
	return [NSString stringWithFormat:@"fe80::%02x%02x:%02x%02x:%02x%02x:%02x%02x",new_octet,octet1,octet2,octet3,octet4,octet5,octet6,octet7];
}

- (void)dealloc {
	[_MACAddress release];
	[super dealloc];
}

@end
