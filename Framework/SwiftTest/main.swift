//
//  main.swift
//  SwiftTest
//
//  Created by Samantha Marshall on 6/21/14.
//
//

import Foundation


import SDMMobileDevice

InitializeSDMMobileDevice()

func getDeviceAtIndex(array: NSArray, index: Int) -> SDMMD_AMDeviceRef {
	let device = array.objectAtIndex(index)
    return unsafeBitCast(device, SDMMD_AMDeviceRef.self)
}

var deviceList = SDMMD_AMDCreateDeviceList().takeRetainedValue()

var device = getDeviceAtIndex(deviceList, index: 0)

var result1: sdmmd_return_t = SDMMD_AMDeviceConnect(device)
print(result1)
var result2 = SDMMD_AMDeviceStartSession(device)
print(result2)
var domain = CFStringCreateWithCString(kCFAllocatorDefault, "NULL", 0)
var key = CFStringCreateWithCString(kCFAllocatorDefault, "UniqueDeviceID", 0)
var udid = SDMMD_AMDeviceCopyValue(device, domain, key)
print(udid.takeUnretainedValue())
var result3 = SDMMD_AMDeviceStopSession(device)
print(result3)
var result4 = SDMMD_AMDeviceDisconnect(device)
print(result4)
//SDMMD_AMDServiceConnectionGetDevice
