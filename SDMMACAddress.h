//
//  SDMMACAddress.h
//  SDM_MD_Demo
//
//  Created by sam on 5/30/13.
//  Copyright 2013 Sam Marshall. All rights reserved.
//

#import <Cocoa/Cocoa.h>


@interface SDMMACAddress : NSObject {
	NSString *_MACAddress;
	uint8_t octet0;
	uint8_t octet1;
	uint8_t octet2;
	uint8_t octet3;
	uint8_t octet4;
	uint8_t octet5;
	uint8_t octet6;
	uint8_t octet7;
}

@property (nonatomic, retain) NSString *MACAddress;

- (id)initWithMAC:(NSString *)address;
- (NSString *)IPV6LocalLink;

@end
