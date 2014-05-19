//
//  main.c
//  iOSConsole
//
//  Created by Sam Marshall on 1/1/14.
//  Copyright (c) 2014 Sam Marshall. All rights reserved.
//

#include <getopt.h>
#include <CoreFoundation/CoreFoundation.h>

#include "SDMMobileDevice.h"
#include "Core.h"

#include "Features.h"

static char *helpArg = "-h";
static char *listArg = "-l,--list";
static char *deviceArg = "-d,--device";
static char *attachArg = "-s,--attach";
static char *queryArg = "-q,--query";
static char *appsArg = "-a,--apps";
static char *infoArg = "-i,--info";
static char *runArg = "-r,--run";
static char *powerArg = "-p,--diag";
static char *devArg = "-x,--develop";
static char *installArg = "-t,--install";
static char *profileArg = "-c,--profile";
static char *testArg = "-z,--test";

enum iOSConsoleOptions {
	OptionsHelp = 0x0,
	OptionsList,
	OptionsDevice,
	OptionsAttach,
	OptionsQuery,
	OptionsApps,
	OptionsInfo,
	OptionsRun,
	OptionsDiag,
	OptionsDev,
	OptionsInstall,
	OptionsConfig,
	OptionsTest,
	OptionsCount
};

static struct option long_options[OptionsCount] = {
	{"help", optional_argument, 0x0, 'h'},
	{"list", no_argument, 0x0, 'l'},
	{"device", required_argument, 0x0, 'd'},
	{"attach", required_argument, 0x0, 's'},
	{"query", required_argument, 0x0, 'q'},
	{"apps", no_argument, 0x0, 'a'},
	{"info", no_argument, 0x0, 'i'},
	{"run", required_argument, 0x0, 'r'},
	{"diag", required_argument, 0x0, 'p'},
	{"develop", no_argument, 0x0, 'x'},
	{"install", required_argument, 0x0, 't'},
	{"profile", required_argument, 0x0, 'c'},
	{"test", no_argument, 0x0, 'z'}
};

static bool optionsEnable[OptionsCount] = {};

int main(int argc, const char * argv[]) {
	SDMMobileDevice;
	
	char *udid = NULL;
	
	char *service = NULL;
	
	char *help = NULL;
	
	char *domain = NULL;
	char *key = NULL;
	
	char *bundle = NULL;
	
	char *diagArg = NULL;
	
	bool searchArgs = true;
	
	char *installPath = NULL;
	
	int c;
	while (searchArgs) {
		int option_index = 0x0;
		c = getopt_long (argc, (char * const *)argv, "lh:d:ais:q:p:t:c:z",long_options, &option_index);
		if (c == -1) {
			break;
		}
		switch (c) {
			case 'h': {
				if (optarg) {
					help = optarg;
				}
				optionsEnable[OptionsHelp] = true;
				searchArgs = false;
				break;
			};
			case 'l': {
				optionsEnable[OptionsList] = true;
				searchArgs = false;
				break;
			};
			case 'd': {
				if (optarg && !optionsEnable[OptionsDevice]) {
					optionsEnable[OptionsDevice] = true;
					udid = optarg;
				}
				break;
			}
			case 's': {
				if (optarg && !optionsEnable[OptionsAttach]) {
					service = optarg;
					optionsEnable[OptionsAttach] = true;
				}
				else {
					printf("please specify a service name to attach");
				}
				break;
			};
			case 'q': {
				if (optarg && !optionsEnable[OptionsQuery]) {
					CFStringRef argValue = CFStringCreateWithCString(kCFAllocatorDefault, optarg, kCFStringEncodingUTF8);
					CFArrayRef argsArray = CFStringCreateArrayBySeparatingStrings(kCFAllocatorDefault, argValue, CFSTR("="));
					CFIndex argsCounter = CFArrayGetCount(argsArray);
					if (argsCounter >= 0x1) {
						domain = (char*)SDMCFStringGetString(CFArrayGetValueAtIndex(argsArray, 0x0));
						if (strncmp(domain, "all", sizeof(char)*0x3) == 0x0) {
							Safe(free, domain);
							domain = calloc(0x1, sizeof(char)*(strlen(kAllDomains)+0x1));
							memcpy(domain, kAllDomains, strlen(kAllDomains));
						}
						optionsEnable[OptionsQuery] = true;
					}
					if (argsCounter == 0x2) {
						key = (char*)SDMCFStringGetString(CFArrayGetValueAtIndex(argsArray, 0x1));
					}
					else {
						key = calloc(0x1, sizeof(char)*(strlen(kAllKeys)+0x1));
						memcpy(key, kAllKeys, strlen(kAllKeys));
					}
					CFSafeRelease(argsArray);
					CFSafeRelease(argValue);
				}
				break;
			};
			case 'a': {
				optionsEnable[OptionsApps] = true;
				break;
			};
			case 'i': {
				optionsEnable[OptionsInfo] = true;
				break;
			};
			case 'r': {
				if (optarg && !optionsEnable[OptionsRun]) {
					bundle = optarg;
					optionsEnable[OptionsRun] = true;
				}
				break;
			};
			case 'p': {
				if (optarg && !optionsEnable[OptionsDiag]) {
					diagArg = optarg;
					optionsEnable[OptionsDiag] = true;
				}
				break;
			};
			case 'x': {
				optionsEnable[OptionsDev] = true;
				break;
			}
			case 't': {
				if (optarg) {
					optionsEnable[OptionsInstall] = true;
					installPath = optarg;
				}
				break;
			}
			case 'c': {
				if (optarg) {
					optionsEnable[OptionsConfig] = true;
					installPath = optarg;
				}
				break;
			}
			case 'z': {
				optionsEnable[OptionsTest] = true;
				break;
			}
			default: {
				printf("--help for help");
				break;
			};
		}
	}
	if (optionsEnable[OptionsHelp]) {
		if (!help) {
			printf("%s [service|query] : list available services or queries\n",helpArg);
			printf("%s : list attached devices\n",listArg);
			printf("%s [UDID] : specify a device\n",deviceArg);
			printf("%s [service] : attach to [service]\n",attachArg);
			printf("%s <domain>=<key> : query value for <key> in <domain>, specify 'null' for global domain\n",queryArg);
			printf("%s : display installed apps\n",appsArg);
			printf("%s : display info of a device\n",infoArg);
			printf("%s [bundle id] : run an application with specified [bundle id]\n",runArg);
			printf("%s [sleep|reboot|shutdown] : perform diag power operations on a device\n",powerArg);
			printf("%s : setup device for development\n",devArg);
			printf("%s [.app path] : install specificed .app to a device\n",installArg);
			printf("%s [.mobileconfig path] : install specificed .mobileconfig to a device\n",profileArg);
		}
		else {
			if (strncmp(help, "service", strlen("service")) == 0x0) {
				printf(" shorthand : service identifier\n--------------------------------\n");
				for (uint32_t i = 0x0; i < SDM_MD_Service_Count; i++) {
					printf("%10s : %s\n",SDMMDServiceIdentifiers[i].shorthand, SDMMDServiceIdentifiers[i].identifier);
				}
			}
			if (strncmp(help, "query", strlen("query")) == 0x0) {
				for (uint32_t i = 0x0; i < SDM_MD_Domain_Count; i++) {
					printf("Domain: %s\n",SDMMDKnownDomain[i].domain);
					for (uint32_t j = 0x0; j < SDMMDKnownDomain[i].keyCount; j++) {
						printf("\t%s\n",SDMMDKnownDomain[i].keys[j]);
					}
					printf("\n\n");
				}
			}
		}
	}
	if (optionsEnable[OptionsList]) {
		ListConnectedDevices();
	}
	if (optionsEnable[OptionsDevice]) {
		if (optionsEnable[OptionsInfo]) {
			
		}
		else if (optionsEnable[OptionsApps]) {
			LookupAppsOnDevice(udid);
		}
		else if (optionsEnable[OptionsAttach]) {
			PerformService(udid, service);
		}
		else if (optionsEnable[OptionsQuery]) {
			PerformQuery(udid, domain, key);
		}
		else if (optionsEnable[OptionsRun]) {
			RunAppOnDeviceWithIdentifier(udid, bundle);
		}
		else if (optionsEnable[OptionsDiag]) {
			if (diagArg) {
				if (strncmp(diagArg, "sleep", strlen("sleep")) == 0x0) {
					SendSleepToDevice(udid);
				} else if (strncmp(diagArg, "reboot", strlen("reboot")) == 0x0) {
					SendRebootToDevice(udid);
				} else if (strncmp(diagArg, "shutdown", strlen("shutdown")) == 0x0) {
					SendShutdownToDevice(udid);
				}
			}
		}
		else if (optionsEnable[OptionsDev]) {
			SetupDeviceForDevelopment(udid);
		}
		else if (optionsEnable[OptionsInstall]) {
			InstallAppToDevice(udid, installPath);
		}
		else if (optionsEnable[OptionsConfig]) {
			InstallProfileToDevice(udid, installPath);
		}
		else if (optionsEnable[OptionsTest]) {
			WhatDoesThisDo(udid);
		}
	}
	Safe(free, domain);
	Safe(free, key);
	
    return 0;
}

