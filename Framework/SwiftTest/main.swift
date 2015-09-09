//
//  main.swift
//  SwiftTest
//
//  Created by Samantha Marshall on 6/21/14.
//
//

import Foundation


import SDMMobileDevice

InitializeSDMMobileDevice();

func getDeviceAtIndex(array: NSArray, index: Int) -> SDMMD_AMDeviceRef {
	let device: AnyObject = array.objectAtIndex(index) as AnyObject;
	return device as SDMMD_AMDeviceRef;
}

var deviceList: CFArray = SDMMD_AMDCreateDeviceList();

var device: SDMMD_AMDeviceRef = getDeviceAtIndex(deviceList, 0);

var result1: sdmmd_return_t = SDMMD_AMDeviceConnect(device);
println(result1);
var result2 = SDMMD_AMDeviceStartSession(device);
println(result2)
var domain: CFStringRef = CFStringCreateWithCString(kCFAllocatorDefault, "NULL", 0);
var key: CFStringRef = CFStringCreateWithCString(kCFAllocatorDefault, "UniqueDeviceID", 0);
var udid: CFTypeRef = SDMMD_AMDeviceCopyValue(device, domain, key);
PrintCFType(udid);
var result3 = SDMMD_AMDeviceStopSession(device);
println(result3);
var result4 = SDMMD_AMDeviceDisconnect(device);
println(result4);
//SDMMD_AMDServiceConnectionGetDevice