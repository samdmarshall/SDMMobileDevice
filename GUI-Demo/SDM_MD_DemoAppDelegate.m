//
//  SDM_MD_DemoAppDelegate.m
//  SDM_MD_Demo
//
//  Created by Sam Marshall on 5/24/13.
//  Copyright 2013 Sam Marshall. All rights reserved.
//

#import "SDM_MD_DemoAppDelegate.h"
#import "MDDemoMainSplitView.h"
#import <SDMMobileDevice/SDMMobileDevice.h>
#import <Anthem/Anthem.h>

@implementation SDM_MD_DemoAppDelegate

@synthesize window;

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification {
	Anthem *test = [[Anthem alloc] init];
	MDDemoMainSplitView *splitView = [[MDDemoMainSplitView alloc] initWithFrame:[self.window.contentView bounds]];
	[self.window.contentView addSubview:splitView];
	// Insert code here to initialize your application 
	//NSArray *keys = [NSArray arrayWithObjects:CFSTR(kActivationPublicKey), CFSTR(kActivationState), CFSTR(kActivationStateAcknowledged), CFSTR(kActiveWirelessTechnology), CFSTR(kActivityURL), CFSTR(kAirplaneMode), CFSTR(kApNonce), CFSTR(kBasebandBoardSerialNumber), CFSTR(kBasebandBootloaderVersion), CFSTR(kBasebandGoldCertId), CFSTR(kBasebandKeyHashInformation), CFSTR(kBasebandNonce), CFSTR(kBasebandSerialNumber), CFSTR(kBasebandStatus), CFSTR(kBasebandVersion), CFSTR(kBoardId), CFSTR(kBluetoothAddress), CFSTR(kBrickState), CFSTR(kBuildVersion), CFSTR(kCarrierBuild), CFSTR(kCarrierBundleInfo), CFSTR(kCarrierBundleInfoArray), CFSTR(kChipID), CFSTR(kCompassCalibration), CFSTR(kCPUArchitecture), CFSTR(kDeviceCertificate), CFSTR(kDeviceClass), CFSTR(kDeviceColor), CFSTR(kDeviceEnclosureColor), CFSTR(kDeviceName), CFSTR(kDevicePublicKey), CFSTR(kDeviceVariant), CFSTR(kDieID), CFSTR(kEthernetAddress), CFSTR(kFirmwarePreflightInfo), CFSTR(kFirmwareVersion), CFSTR(kFirstTime), CFSTR(kHardwareModel), CFSTR(kHardwarePlatform), CFSTR(kHostAttached), CFSTR(kIMLockdownEverRegisteredKey), CFSTR(kIntegratedCircuitCardIdentity), CFSTR(kInternationalMobileEquipmentIdentity), CFSTR(kInternationalMobileSubscriberIdentity), CFSTR(kiTunesHasConnected), CFSTR(kMarketingName), CFSTR(kMLBSerialNumber), CFSTR(kMobileEquipmentIdentifier), CFSTR(kMobileSubscriberCountryCode), CFSTR(kMobileSubscriberNetworkCode), CFSTR(kModelNumber), CFSTR(kNonVolatileRAM), CFSTR(kPartitionType), CFSTR(kPasswordProtected), CFSTR(kPhoneNumber), CFSTR(kProductionSOC), CFSTR(kProductType), CFSTR(kProductVersion), CFSTR(kProtocolVersion), CFSTR(kProximitySensorCalibration), CFSTR(kRegionInfo), CFSTR(kReleaseType), CFSTR(kRentalClockBias), CFSTR(kSBLockdownEverRegisteredKey), CFSTR(kSerialNumber), CFSTR(kShowMarketing), CFSTR(kSIMGID1), CFSTR(kSIMGID2), CFSTR(kSIMStatus), CFSTR(kSIMTrayStatus), CFSTR(kSoftwareBehavior), CFSTR(kSoftwareBundleVersion), CFSTR(kSupportedDeviceFamilies), CFSTR(kTelephonyCapability), CFSTR(kTimeIntervalSince1970), CFSTR(kTimeZone), CFSTR(kTimeZoneOffsetFromUTC), CFSTR(kTrustedHostAttached), CFSTR(kUniqueChipID), CFSTR(kUniqueDeviceID), CFSTR(kUseActivityURL), CFSTR(kUseRaptorCerts), CFSTR(kUses24HourClock), CFSTR(kWeDelivered), CFSTR(kWiFiAddress), nil];
	
	/*SDMMobileDevice;
	CFArrayRef devices = SDMMD_AMDCreateDeviceList();
	for (uint32_t i = 0; i < CFArrayGetCount(devices); i++) {
		SDMMD_AMDeviceRef device = (SDMMD_AMDeviceRef)CFArrayGetValueAtIndex(devices, i);
		uint32_t result;
		//SDMMD_AMDebugConnectionRef debugConn = NULL;
		//uint32_t result = SDMMD_StartDebuggingSessionOnDevice(device, &debugConn);
		//printf("debug start: 0x%08x\n",result);
		CFStringRef encodedPath = SDMMD_EncodeForDebuggingCommand(CFSTR(""));
		CFStringRef containerPath = SDMMD_EncodeForDebuggingCommand(CFSTR(""));
		sdmmd_debug_return_t dresult;
		CFStringRef commandformat = (CFStringRef)[NSString stringWithFormat:@"%d,0,%@",(uint32_t)CFStringGetLength(encodedPath),encodedPath];
		dresult = SDMMD_DebuggingSend(debugConn, KnownDebugCommands[kDebugA], commandformat);
		CFShow(dresult.data);
		dresult = SDMMD_DebuggingSend(debugConn, KnownDebugCommands[kDebugH], CFSTR("c0"));
		CFShow(dresult.data);
		
		//result = SDMMD_StopDebuggingSessionOnDevice(device, &debugConn);
		//printf("debug stop: 0x%08x\n",result);
		
		result = SDMMD_AMDeviceConnect(device);
		printf("connect: 0x%08x\n",result);
		bool paired = SDMMD_AMDeviceIsPaired(device);
		printf("paired status: %s\n",(paired ? "yes" : "no"));
		result = SDMMD_AMDeviceValidatePairing(device);
		printf("validate: 0x%08x\n",result);
		result = SDMMD_AMDeviceStartSession(device);
		printf("start session: 0x%08x\n",result);
		CFDictionaryRef dict = NULL;
		SDMMD_AMConnectionRef conn = SDMMD_AMDServiceConnectionCreate(0, NULL, dict);
		result = SDMMD_AMDeviceStartService(device, CFSTR(AMSVC_AFC), 0x0, &conn);
		printf("service: 0x%08x\n",result);
		
		//for (NSString *key in keys) {
		//	CFTypeRef value = SDMMD_AMDeviceCopyValue(device, NULL, key);
		//	NSLog(@"%@: %@",key, value);
		//}
		
		//CFDictionaryRef response;
		//CFArrayRef values = SDMMD_ApplicationLookupDictionary();
		//CFMutableDictionaryRef optionsDict = SDMMD_create_dict();
		//CFDictionarySetValue(optionsDict, CFSTR("ReturnAttributes"), values);
		
		//result = SDMMD_AMDeviceLookupApplications(device, optionsDict, &response);
		//printf("lookup: 0x%08x\n",result);
		//CFShow(response);
		
		result = SDMMD_AMDeviceStopSession(device);
		printf("stop session: 0x%08x\n",result);
		result = SDMMD_AMDeviceDisconnect(device);
		printf("disconnect: 0x%08x\n",result);
			//CFDictionaryRef app = CFDictionaryGetValue(response, CFSTR("com.samdmarshall.Rdar"));
			//if (app) {
				//SDMMD_AMDebugConnectionRef debugConn;
				//result = SDMMD_StartDebuggingSessionOnDevice(device, &debugConn);
				//printf("debug start: 0x%08x\n",result);
				//CFShow(CFDictionaryGetValue(app, CFSTR("Path")));
				//CFStringRef encodedPath = SDMMD_EncodeForDebuggingCommand(CFDictionaryGetValue(app, CFSTR("Path")));
				//CFStringRef containerPath = SDMMD_EncodeForDebuggingCommand(CFDictionaryGetValue(app, CFSTR("Container")));
				//CFShow(encodedPath);
				//CFShow(containerPath);
				//sdmmd_debug_return_t dresult;
				dresult = SDMMD_DebuggingSend(debugConn, KnownDebugCommands[kDebugQSetMaxPacketSize], SDMMD_EncodeForDebuggingCommand(CFSTR("1024")));
				CFShow(dresult.data);
				dresult = SDMMD_DebuggingSend(debugConn, KnownDebugCommands[kDebugQSetWorkingDir], containerPath);
				CFShow(dresult.data);
				//NSString *commandformat = [NSString stringWithFormat:@"%d,0,%@",(uint32_t)CFStringGetLength(encodedPath),encodedPath];
				//dresult = SDMMD_DebuggingSend(debugConn, KnownDebugCommands[kDebugA], (CFStringRef)commandformat);
				//CFShow(dresult.data);
				dresult = SDMMD_DebuggingSend(debugConn, KnownDebugCommands[kDebugqLaunchSuccess], CFSTR(""));
				CFShow(dresult.data);
				dresult = SDMMD_DebuggingSend(debugConn, KnownDebugCommands[kDebugqGetPid], CFSTR(""));
				CFShow(dresult.data);
				dresult = SDMMD_DebuggingSend(debugConn, KnownDebugCommands[kDebugH], CFSTR("c0"));
				CFShow(dresult.data);
				dresult = SDMMD_DebuggingSend(debugConn, KnownDebugCommands[kDebugc], CFSTR(""));
				CFShow(dresult.data);
				
				//sleep(5);
				//dresult = SDMMD_DebuggingSend(debugConn, KnownDebugCommands[kDebugk], CFSTR(""));
				//CFShow(dresult.data);
				//result = SDMMD_StopDebuggingSessionOnDevice(device, &debugConn);
				//printf("debug stop: 0x%08x\n",result);
				//}
	}*/
}

@end
