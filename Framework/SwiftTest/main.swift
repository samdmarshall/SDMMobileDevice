//
//  main.swift
//  SwiftTest
//
//  Created by Sam Marshall on 6/21/14.
//
//

import Foundation


import SDMMobileDevice

func getDeviceAtIndex(array: NSArray, index: Int) -> SDMMD_AMDeviceRef {
	let device: AnyObject = array.objectAtIndex(index) as AnyObject;
	return device as SDMMD_AMDeviceRef;
}

var test: CFArray = SDMMD_AMDCreateDeviceList().takeRetainedValue();
let deviceList = reinterpretCast(test) as NSArray

var device: SDMMD_AMDeviceRef = getDeviceAtIndex(deviceList, 0);
//
//println(convert);
//
//var deviceCount = CFArrayGetCount(convert);
//
//
////let device = CFArrayGetValueAtIndex(deviceList, 0) as? SDMMD_AMDeviceRef;
//
////var result1: sdmmd_return_t = SDMMD_AMDeviceConnect(object as SDMMD_AMDeviceRef);
////println(result1);
////var result2 = SDMMD_AMDeviceStartSession(pointer);
////println(result2)
////var domain: CFStringRef = CFStringCreateWithCString(kCFAllocatorDefault, "NULL", 0);
////var key: CFStringRef = CFStringCreateWithCString(kCFAllocatorDefault, "UniqueDeviceID", 0);
////var udid: CFTypeRef = SDMMD_AMDeviceCopyValue(pointer, domain, key);
////PrintCFType(udid);
////var result3 = SDMMD_AMDeviceStopSession(pointer);
////println(result3);
////var result4 = SDMMD_AMDeviceDisconnect(device);
////println(result4);
//
//for index in 0..deviceCount {
//	var pointer = CFArrayGetValueAtIndex(convert, 0);
//}
//
////var pointer = CFArrayGetValueAtIndex(deviceList, 0).value;
////var device: SDMMD_AMDeviceRef = UnsafePointer(pointer);
////var unsafe: UnsafePointer<sdmmd_am_device> = UnsafePointer.init(pointer);
////println(pointer);
////var result1 = SDMMD_AMDeviceConnect(pointer);
////println(result1);
////var result2 = SDMMD_AMDeviceStartSession(pointer);
////println(result2)
////var domain: CFStringRef = CFStringCreateWithCString(kCFAllocatorDefault, "NULL", 0);
////var key: CFStringRef = CFStringCreateWithCString(kCFAllocatorDefault, "UniqueDeviceID", 0);
////var udid: CFTypeRef = SDMMD_AMDeviceCopyValue(pointer, domain, key);
////PrintCFType(udid);
////var result3 = SDMMD_AMDeviceStopSession(pointer);
////println(result3);
////var result4 = SDMMD_AMDeviceDisconnect(pointer);
////println(result4);
//
//println("testing");
//
//
//
//
//





