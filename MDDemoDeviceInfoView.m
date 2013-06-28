//
//  MDDemoDeviceInfoView.m
//  SDM_MD_Demo
//
//  Created by sam on 6/21/13.
//  Copyright 2013 Sam Marshall. All rights reserved.
//

#import "MDDemoDeviceInfoView.h"

@implementation MDDemoDeviceInfoView

@synthesize device;
@synthesize space;

- (void)setSelectedDevice:(MDDemoDeviceRef *)deviceref {
	self.device = deviceref;
	dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ^{
		[self.device getValueForKey:@kCalculateDiskUsage fromDomain:@kDiskUsageDomain];
		CFNumberRef systemTotalSpace = [self.device getValueForKey:@kTotalDataCapacity fromDomain:@kDiskUsageDomain];
		uint64_t totalSpace;
		if (systemTotalSpace) {
			CFNumberGetValue(systemTotalSpace, kCFNumberSInt64Type, &totalSpace);
			[space setMinValue:0.0];
			[space setWarningValue:totalSpace*.90];
			[space setCriticalValue:totalSpace*.95];
			[space setMaxValue:totalSpace];
		}
		CFNumberRef systemAvailableSpace = [self.device getValueForKey:@kTotalDataAvailable fromDomain:@kDiskUsageDomain];
		if (systemAvailableSpace) {
			uint64_t availableSpace;
			CFNumberGetValue(systemAvailableSpace, kCFNumberSInt64Type, &availableSpace);
			[space setDoubleValue:totalSpace-availableSpace];			
		}
		
	});
}

- (IBAction)SyncDevice:(id)sender {
	if (device) {
		dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_HIGH, 0x0), ^{
			//AMDeviceUnpair(device.device);
			NSLog(@"Device is Paired: %@",AMDeviceIsPaired(device.device) ? @"Yes" : @"No");
			//NSArray *keys = [NSArray arrayWithObjects:@"ActivationPublicKey", @"ActivationState", @"ActivationStateAcknowledged", @"ActiveWirelessTechnology", @"ActivityURL", @"AirplaneMode", @"ApNonce", @"BasebandBoardSerialNumber", @"BasebandBootloaderVersion", @"BasebandGoldCertId", @"BasebandKeyHashInformation", @"BasebandNonce", @"BasebandSerialNumber", @"BasebandStatus", @"BasebandVersion", @"BoardId", @"BluetoothAddress", @"BrickState", @"BuildVersion", @"CarrierBuild", @"CarrierBundleInfo", @"CarrierBundleInfoArray", @"ChipID", @"CompassCalibration", @"CPUArchitecture", @"DeviceCertificate", @"DeviceClass", @"DeviceColor", @"DeviceEnclosureColor", @"DeviceName", @"DevicePublicKey", @"DeviceVariant", @"DieID", @"EthernetAddress", @"FirmwarePreflightInfo", @"FirmwareVersion", @"FirstTime", @"HardwareModel", @"HardwarePlatform", @"HostAttached", @"IMLockdownEverRegisteredKey", @"IntegratedCircuitCardIdentity", @"InternationalMobileEquipmentIdentity", @"InternationalMobileSubscriberIdentity", @"iTunesHasConnected", @"MarketingName", @"MLBSerialNumber", @"MobileEquipmentIdentifier", @"MobileSubscriberCountryCode", @"MobileSubscriberNetworkCode", @"ModelNumber", @"NonVolatileRAM", @"PartitionType", @"PasswordProtected", @"PhoneNumber", @"ProductionSOC", @"ProductType", @"ProductVersion", @"ProtocolVersion", @"ProximitySensorCalibration", @"RegionInfo", @"ReleaseType", @"RentalClockBias", @"SBLockdownEverRegisteredKey", @"SerialNumber", @"ShowMarketing", @"SIMGID1", @"SIMGID2", @"SIMStatus", @"SIMTrayStatus", @"SoftwareBehavior", @"SoftwareBundleVersion", @"SupportedDeviceFamilies", @"TelephonyCapability", @"TimeIntervalSince1970", @"TimeZone", @"TimeZoneOffsetFromUTC", @"TrustedHostAttached", @"UniqueChipID", @"UniqueDeviceID", @"UseActivityURL", @"UseRaptorCerts", @"Uses24HourClock", @"WeDelivered", @"WiFiAddress",nil];
			//for (NSString *key in keys) {
				CFTypeRef result = SDMDeviceCopyValue(device.device, @"com.apple.iTunes.StoreDevices", @"Volum");
				if (result != NULL) {
					NSLog(@"%@", result);
				}
			//}
			NSLog(@"Device is Paired: %@",AMDeviceIsPaired(device.device) ? @"Yes" : @"No");
			/*if (AMDeviceIsPaired(device.device)) {
				if (SDM_MD_CallSuccessful(AMDeviceConnect(device.device))) {
					if (SDM_MD_CallSuccessful(AMDeviceStartSession(device.device))) {
						uint32_t socket = 0x0;
						if (SDM_MD_CallSuccessful(AMDeviceStartService(device.device, CFSTR(AMSVC_NOTIFICATION_PROXY), &socket, NULL))) {
							AMDPostNotification(socket, CFSTR(kNotificationiTunesSyncWillStart), NULL);
							AMDPostNotification(socket, CFSTR(kNotificationiTunesSyncLock), NULL);
							sleep(5);
							AMDPostNotification(socket, CFSTR(kNotificationiTunesSyncDidStart), NULL);
							AMDPostNotification(socket, CFSTR(kNotificationiTunesSyncDidFinish), NULL);
							AMDShutdownNotificationProxy(socket);
						}
						AMDeviceStopSession(device.device);
					}
				}
			}*/
		});
	} else {
		// error, no device selected
	}
}

@end
