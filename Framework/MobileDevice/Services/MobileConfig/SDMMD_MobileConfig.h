//
//  SDMMD_MobileConfig.h
//  SDMMobileDevice-Framework
//
//  Created by Sam Marshall on 4/21/14.
//
//

#ifndef SDMMobileDevice_Framework_SDMMD_MobileConfig_h
#define SDMMobileDevice_Framework_SDMMD_MobileConfig_h

/*
 plist style messaging uses "com.apple.mobile.MCInstall"
 
 RequestType : 
 			InstallProfile
 			GetProfileList
 			Flush
 			RemoveProfile
 
 
 InstallProfile
 	Payload : <data object of .mobileconfig>
 
 
 RemoveProfile
 	Identifier : <identifier of the config>
 
 
 Flush
 		// sent between each request
 
 
 responses:
 
 	Status : Acknowledged
 
 
 */


#endif
