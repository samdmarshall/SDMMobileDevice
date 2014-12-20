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
 

 
 it seems that for doing a restore, you need to parse the mbdb
 
 

// for files that are not on the host system
Received:
<CFArray>=[
           <CFStringRef>={DLMessageDownloadFiles}
           <CFArray>=[
                      <CFStringRef>={435f770a03b4ff48d7540f3d85851f845e6e14ac/Info.plist}
                      ]
           <CFDictionary>={
           }
           <CFNumberRef>=(kCFNumberFloat64Type){0.000000}
           <CFNumberRef>=(kCFNumberSInt64Type){0}
           ]


If file doesn't exist...

Send: (length + 0x06)
0000 0001 06

Send: (four zeros raw)
0000 0000

Send Message:
<CFArray>=[
           <CFStringRef>={DLMessageStatusResponse}
           <CFNumberRef>=(kCFNumberSInt64Type){-13}	// magic error number :P
           <CFStringRef>={Multi status}
           <CFDictionary>={
               <CFStringRef>={435f770a03b4ff48d7540f3d85851f845e6e14ac/Info.plist}: <CFDictionary>={
                   <CFStringRef>={DLFileErrorString}: <CFStringRef>=Invalid object referenced in backup image	// this is just an example
                   <CFStringRef>={DLFileErrorCode}: <CFNumberRef>=(kCFNumberSInt64Type){-6}	// this is the error code I've seen for file not found
               }
           }
           ]
 */

#include <CoreFoundation/CoreFoundation.h>
#include "SDMMD_Error.h"
#include "SDMMD_Connection.h"

CF_RETURNS_RETAINED CFMutableArrayRef SDMMD_MB2StatusResponseMessage();
sdmmd_return_t SDMMD_MB2SendFile(SDMMD_AMConnectionRef conn, CFStringRef path, CFDataRef file);
sdmmd_return_t SDMMD_MB2SendFiles(SDMMD_AMConnectionRef conn, CFArrayRef paths, CFArrayRef files);

#endif
