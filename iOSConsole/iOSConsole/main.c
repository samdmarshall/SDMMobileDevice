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

enum iOSConsoleOptions {
	OptionsHelp = 0x0,
	OptionsList,
	OptionsDevice,
	OptionsAttach,
	OptionsQuery,
	OptionsApps,
	OptionsInfo,
	OptionsRun,
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
	{"run", required_argument, 0x0, 'r'}
};

static bool optionsEnable[OptionsCount] = {};

int main(int argc, const char * argv[]) {
	SDMMobileDevice;
	
	char *udid;
	
	char *service;
	
	char *help = NULL;
	
	char *domain = "NULL";
	char *key = NULL;
	
	bool searchArgs = true;
	
	int c;
	while (searchArgs) {
		int option_index = 0x0;
		c = getopt_long (argc, (char * const *)argv, "lh:d:ais:q:",long_options, &option_index);
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
				if (optarg) {
					optionsEnable[OptionsDevice] = true;
					udid = optarg;
				}
				break;
			}
			case 's': {
				if (optarg) {
					service = optarg;
					optionsEnable[OptionsAttach] = true;
				} else {
					printf("please specify a service name to attach");
				}
				break;
			};
			case 'q': {
				if (optarg) {
					CFStringRef argValue = CFStringCreateWithCString(kCFAllocatorDefault, optarg, kCFStringEncodingUTF8);
					CFArrayRef argsArray = CFStringCreateArrayBySeparatingStrings(kCFAllocatorDefault, argValue, CFSTR("="));
					domain = (char*)CFStringGetCStringPtr(CFArrayGetValueAtIndex(argsArray, 0x0),kCFStringEncodingMacRoman);
					key = (char*)CFStringGetCStringPtr(CFArrayGetValueAtIndex(argsArray, 0x1),kCFStringEncodingMacRoman);
					optionsEnable[OptionsQuery] = true;
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
				optionsEnable[OptionsRun] = true;
				break;
			};
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
			printf("%s [path] : run an application at specified [path]\n",runArg);
		} else {
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
			
		} else if (optionsEnable[OptionsApps]) {
			LookupAppsOnDevice(udid);
		} else if (optionsEnable[OptionsAttach]) {
			PerformService(udid, service);
		} else if (optionsEnable[OptionsQuery]) {
			PerformQuery(udid, domain, key);
		} else if (optionsEnable[OptionsRun]) {
			
		}
	}
	
    return 0;
}

