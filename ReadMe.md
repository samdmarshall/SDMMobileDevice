SDMMobileDevice
===============

## Table of Contents
* [License](./LICENSE)
* [What is this?](#what-is-this)
* [What can this project do for me?](#what-can-this-project-do-for-me)
* [Before you begin](#before-you-begin)
* [How do I use this?](#how-do-i-use-this)
* [FAQ](#faq)
* [Help](#help)


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

### Before you begin
1. Run [build-OpenSSL-iOS-Mac.sh](./openssl/build-OpenSSL-iOS-Mac.sh) in order to download and build OpenSSL
2. Make sure both 'openssl/include' and 'openssl/lib/Mac' have been created and populated

### How do I use this?
SDMMobileDevice is an OS X framework and can be used in two ways:

1. [Bundled Framework](#bundled-framework)
2. [Source Code](#source-code)

**Special Note: [Entitlements](#entitlements-where-applicable)**



#### Bundled Framework
You are already almost done!
 
1. Add SDMMobileDevice.framework as a linked library
2. Add a new build phase that copies "SDMMobileDevice.framework" to the "Frameworks" directory of your app.
3. Add "@loader_path/../Frameworks" to "Runtime Search Paths"
4. Add `#include <SDMMobileDevice/SDMMobileDevice.h>` to your source code

#### Source Code
If you wish you use this as source code, it will require the following libraries:

* CoreFoundation.framework
* IOKit.framework

If you have trouble locating any of these, please look at the "-Framework" xcode project file to get the paths for these libraries.


#### Entitlements (where applicable)
See this [file](./Framework/SDMMobileDevice.entitlements)

***

### FAQ

Q. When is the Cocoa version coming?

A. Soon! I am working to provide a few more core features and keeping the whole thing compatible with MobileDevice and keeping it as a completely C library. The Cocoa wrapper for this will come as soon as I have finished the AFC implementation and have working wireless communication.


Q. How do I use this with MobileDevice.framework

A. For the most part SDMMobileDevice is cross-compatible with MobileDevice's function calls and object types. Function calls with matching names (remove the "SDMMD" prefix and underscores), will have matching parameters and types to what MobileDevice uses. Majority of the object types are reconstructed to match in internal layout.


Q. How can I contribute?

A. See the [wiki](https://github.com/samdmarshall/SDMMobileDevice/wiki) for details on submitting pull requests.

### Help
I have created a IRC room on freenode to serve as general discussion and research.

Server: irc.freenode.net

Room: #sdmmobiledevice

Quicklink: [webchat](http://webchat.freenode.net/?channels=sdmmobiledevice)
