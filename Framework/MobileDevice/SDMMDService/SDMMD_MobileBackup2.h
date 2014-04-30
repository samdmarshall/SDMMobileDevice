//
//  SDMMD_MobileBackup2.h
//  SDMMobileDevice-Framework
//
//  Created by Sam Marshall on 4/30/14.
//
//

#ifndef SDMMobileDevice_Framework_SDMMD_MobileBackup2_h
#define SDMMobileDevice_Framework_SDMMD_MobileBackup2_h

/*
 Open connection
 
 recieve from device
 
 <CFArray>=[
 	<CFStringRef>={DLMessageVersionExchange}
 	<CFNumberRef>=(kCFNumberSInt64Type){300} // major version
	<CFNumberRef>=(kCFNumberSInt64Type){0} // minor version
 ]

 respond with sending
 
 <CFArray>=[
 	<CFStringRef>={DLMessageProcessMessage}
	<CFDictionary>={
 		SupportedProtocolVersions: <CFArray>=[
 			<CFNumberRef>=(kCFNumberFloat64Type){2.100000}
 		]
 		MessageName: <CFStringRef>={Hello}
 	}
 ]
 
 
 if successful, now receive 
 
 <CFArray>=[
	<CFStringRef>={DLMessageDeviceReady}
 ]
 
 now send
 
 
 
 */

#endif
