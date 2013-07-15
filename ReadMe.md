SDMMobileDevice
===============

## Table of Contents
* [License](./LICENSE)
* [What is this?]()
* [What can this project do for me?]()
* [How do I use this?]()

***

### What is this?
SDMMobileDevice is a framework that enables access to communicating with iOS devices. 

_SDMMobileDevice is a framework that can be used interchangeably with Apple's private framework MobileDevice.framework_

***

### What can this project do for me?


***

### How do I use this?
SDMMobileDevice is an OS X framework and can be used in two ways:

1. Bundled Framework
2. Source Code

#### Bundled Framework
You are already almost done!

1. Add the SDMMobileDevice-Framework.xcodeproj file into your existing Xcode project or workspace. 
2. Add it as a dependency and link against it.
3. Add `#import <SDMMobileDevice/SDMMobileDevice.h>` to your source code

#### Source Code
If you wish you use this as source code, it will require the following libraries:

* CoreFoundation.framework
* libcrypto.dylib
* libssl.dylib

If you have trouble locating any of these, please look at the "-Framework" xcode project file to get the paths for these libraries.