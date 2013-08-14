SDMMobileDevice
===============

## Table of Contents
* [License](./LICENSE)
* [What is this?](#what-is-this)
* [What can this project do for me?](#what-can-this-project-do-for-me)
* [How do I use this?](#how-do-i-use-this)
* [FAQ](#faq)


***

### What is this?
SDMMobileDevice is a framework that enables access to communicating with iOS devices. This framework serves as a public and documented way to interact with iOS devices and the services that exist on iOS devices. 

_SDMMobileDevice is a framework that can be used interchangeably with Apple's private framework MobileDevice.framework_

***

### What can this project do for me?
This framework gives access to a number of technologies that have previously been "off limits" to developers. Due to being a private and undocumented API, MobileDevice.framework has not been something that can be safely interacted with. This framework provides access to:

1. Detecting attached iOS devices
2. Querying iOS devices for software and hardware configurations
3. Communication with device services
4. Sandbox access to applications installed on iOS devices
5. Installation of applications onto iOS devices
6. File transfers



***

### How do I use this?
SDMMobileDevice is an OS X framework and can be used in two ways:

1. [Bundled Framework](#bundled-framework)
2. [Source Code](#source-code)

**Special Note: [Entitlements](#entitlements-where-applicable)**



#### Bundled Framework
You are already almost done!

1. Add the SDMMobileDevice-Framework.xcodeproj file into your existing Xcode project or workspace. 
2. Add it as a dependency and link against it.
3. Add `#include <SDMMobileDevice/SDMMobileDevice.h>` to your source code

#### Source Code
If you wish you use this as source code, it will require the following libraries:

* CoreFoundation.framework
* libcrypto.dylib
* libssl.dylib

If you have trouble locating any of these, please look at the "-Framework" xcode project file to get the paths for these libraries.


#### Entitlements (where applicable)
This framework will require entitlements to access the following locations:  
`/var/run/usbmuxd` (read/write access to this file)  
`/var/db/lockdown/` (read/write access to this directory)


***

### FAQ

Q. Why do I get so many warnings when compiling?

	A. The warnings are from the openssl and libcrypto calls that Apple has deprecated starting in 10.7, I am looking to replace these with CommonCrypto or bundling openssl and libcrypto so these errors don't happen.

Q. When is the Cocoa version coming?

	A. Soon! I am working to provide a few more core features and keeping the whole thing compatible with MobileDevice and keeping it as a completely C library. The Cocoa wrapper for this will come as soon as I have finished the AFC implementation and have working wireless communication.
