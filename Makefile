LIBRARY_SOURCES = ./Framework/src/SDMMD_AFCConnection_Class.c \
				  ./Framework/src/SDMMD_AFCOperation_Class.c \
				  ./Framework/src/SDMMD_AFC_Types.c \
				  ./Framework/src/SDMMD_AMAuthInstall.c \
				  ./Framework/src/SDMMD_AMAuthInstallApImg3.c \
				  ./Framework/src/SDMMD_AMAuthInstallApImg4.c \
				  ./Framework/src/SDMMD_AMAuthInstallMonet.c \
				  ./Framework/src/SDMMD_AMDevice.c \
				  ./Framework/src/SDMMD_AMDevice_Class.c \
				  ./Framework/src/SDMMD_AppleFileConduit.c \
				  ./Framework/src/SDMMD_Applications.c \
				  ./Framework/src/SDMMD_Connection.c \
				  ./Framework/src/SDMMD_Connection_Class.c \
				  ./Framework/src/SDMMD_Debugger.c \
				  ./Framework/src/SDMMD_Error.c \
				  ./Framework/src/SDMMD_FileRelay.c \
				  ./Framework/src/SDMMD_Functions.c \
				  ./Framework/src/SDMMD_Initialize.c \
				  ./Framework/src/SDMMD_MCP.c \
				  ./Framework/src/SDMMD_MCP_Class.c \
				  ./Framework/src/SDMMD_MDFUModeDevice.c \
				  ./Framework/src/SDMMD_MRUSBDevice.c \
				  ./Framework/src/SDMMD_MRecoveryModeDevice.c \
				  ./Framework/src/SDMMD_MRestorableDevice.c \
				  ./Framework/src/SDMMD_MRestoreModeDevice.c \
				  ./Framework/src/SDMMD_MobileBackup2.c \
				  ./Framework/src/SDMMD_Notification.c \
				  ./Framework/src/SDMMD_SSL_Functions.c \
				  ./Framework/src/SDMMD_Service.c \
				  ./Framework/src/SDMMD_USBMuxListener.c \
				  ./Framework/src/SDMMD_USBMuxListener_Class.c \
				  ./Framework/src/SDMMD_USBMuxListener_Types.c \
				  ./Framework/src/SDMMD_USBMux_Protocol.c

IOSCONSOLE_SOURCES = ./iOSConsole/apps.c \
					 ./iOSConsole/attach.c \
					 ./iOSConsole/main.c \
					 ./iOSConsole/dev.c \
					 ./iOSConsole/install.c \
					 ./iOSConsole/list.c \
					 ./iOSConsole/power.c \
					 ./iOSConsole/query.c \
					 ./iOSConsole/run.c \
					 ./iOSConsole/service.c \
					 ./iOSConsole/springboard.c \
					 ./iOSConsole/syslog.c \
					 ./iOSConsole/test.c

CORE_SOURCES = ./Core/Core/Buffer.c \
			   ./Core/Core/CoreFoundation/CFDataAddition.c \
			   ./Core/Core/CoreFoundation/CFDictionaryAddition.c \
			   ./Core/Core/CoreFoundation/CFStringAddition.c \
			   ./Core/Core/CoreFoundation/CFTypeAddition.c \
			   ./Core/Core/File.c


HEADER_INCLUDES =  -I./Framework/include \
				   -I./Framework/include/SDMMobileDevice \
				   -I./openssl-0.9.8zg/include \
				   -I./Core/Core \
				   -I./Core/Core/CoreFoundation \
				   -I./iOSConsole 

LIBRARY_LINKER_FLAGS = -lssl \
					   -lcrypto \
					   -framework CoreFoundation \
					   -lSystem \
					   -framework IOKit \
					   -lCore \
					   -Lbuild/

IOSCONSOLE_LINKER_FLAGS = -Xlinker -execute \
						  -arch x86_64 \
						  -lssl \
						  -lcrypto \
						  -framework CoreFoundation \
						  -lSystem \
						  -framework IOKit \
						  -lSDMMobileDevice \
						  -lCore \
						  -Lbuild/

LIBRARY_OBJECTS = $(LIBRARY_SOURCES:.c=.o)
IOSCONSOLE_OBJECTS = $(IOSCONSOLE_SOURCES:.c=.o)
CORE_OBJECTS = $(CORE_SOURCES:.c=.o)

framework: core openssl $(LIBRARY_SOURCES) $(LIBRARY_OBJECTS)
	xcrun libtool -dynamic $(LIBRARY_LINKER_FLAGS) $(LIBRARY_OBJECTS) -o build/libSDMMobileDevice.dylib

static: core $(LIBRARY_SOURCES) $(LIBRARY_OBJECTS)
	xcrun libtool -static $(LIBRARY_OBJECTS) -o build/libSDMMobileDevice.a

.c.o:
	xcrun clang -arch x86_64 $(HEADER_INCLUDES) -c $< -o $@

setup:
	mkdir -p build/

iosconsole: static openssl
	xcrun clang $(IOSCONSOLE_SOURCES) $(HEADER_INCLUDES) $(IOSCONSOLE_LINKER_FLAGS) -o build/iOSConsole

core: setup $(CORE_SOURCES) $(CORE_OBJECTS)
	xcrun libtool -static $(CORE_OBJECTS) -o build/libCore.a
	
openssl: setup
	-cd openssl-0.9.8zg/ && $(MAKE) clean
	cd openssl-0.9.8zg/ && ./Configure darwin64-x86_64-cc shared
	cd openssl-0.9.8zg/ && $(MAKE) build_ssl
	mv openssl-0.9.8zg/libcrypto.a build/libcrypto.a
	mv openssl-0.9.8zg/libssl.a build/libssl.a

clean:
	-cd openssl-0.9.8zg/ && $(MAKE) clean
	-rm -rdf build/
	-rm ./Framework/src/*.o
	-rm ./iOSConsole/*.o
	-rm ./Core/Core/*.o
	-rm ./Core/Core/CoreFoundation/*.o

.PHONY: framework static setup iosconsole core openssl clean
