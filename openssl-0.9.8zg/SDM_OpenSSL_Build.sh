#!/bin/sh

#  SDM_OpenSSL_Build.sh
#  OpenSSL
#
#  Created by Samantha Marshall on 11/12/15.
#  Copyright (c) 2015 Samantha Marshall. All rights reserved.

echo "$ACTION"

if [ "$ACTION" = "clean" ]; then
	make clean
	if -e $BUILD_CONFIGURATION_DIR ]; then
		rm -rdf $BUILD_CONFIGURATION_DIR
	fi
fi

if [ "$ACTION" = "build" ] || [ "$ACTION" = "" ]; then
	mkdir -p $BUILD_CONFIGURATION_DIR

	if [ -f Configure ]; then

		if [ ! -f "$BUILD_CONFIGURATION_DIR/libcrypto-32.a" ] || [ ! -f "$BUILD_CONFIGURATION_DIR/libssl-32.a" ]; then
			make clean
			./Configure darwin-i386-cc shared
			make build_ssl
			mv libcrypto.a $BUILD_CONFIGURATION_DIR/libcrypto-32.a
			mv libssl.a $BUILD_CONFIGURATION_DIR/libssl-32.a
		fi

		if [ ! -f "$BUILD_CONFIGURATION_DIR/libcrypto-64.a" ] || [ ! -f "$BUILD_CONFIGURATION_DIR/libssl-64.a" ]; then
			make clean
			./Configure darwin64-x86_64-cc shared
			make build_ssl
			mv libcrypto.a $BUILD_CONFIGURATION_DIR/libcrypto-64.a
			mv libssl.a $BUILD_CONFIGURATION_DIR/libssl-64.a
		fi

		pushd $BUILD_CONFIGURATION_DIR
		lipo -create -arch x86_64 libcrypto-64.a -arch i386 libcrypto-32.a -output libcrypto.a
		lipo -create -arch x86_64 libssl-64.a -arch i386 libssl-32.a -output libssl.a
		popd

	else
		echo "Cannot find the configure file!"
		exit 1
	fi
fi