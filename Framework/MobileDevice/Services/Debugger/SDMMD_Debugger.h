/*
 *  SDMMD_Debugger.h
 *  SDMMobileDevice
 *
 * Copyright (c) 2014, Sam Marshall
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification, are permitted provided that the 
 * following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer 
 * 		in the documentation and/or other materials provided with the distribution.
 *
 * 3. Neither the name of Sam Marshall nor the names of its contributors may be used to endorse or promote products derived from this
 * 		software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#ifndef _SDM_MD_DEBUGGER_H_
#define _SDM_MD_DEBUGGER_H_

#include "SDMMD_AMDevice.h"
#include "SDMMD_Error.h"
#include "SDMMD_Connection.h"
#include "SDMMD_Service.h"

#pragma mark -
#pragma mark TYPES
#pragma mark -

typedef enum DebuggerCommandType {
	kDebugACK = 0,
	kDebugNACK = 1,
	kDebugConC = 2,
	kDebugBang = 3,
	kDebugQuest = 4,
	kDebugA = 5,
	kDebugB = 6,
	kDebugc = 7,
	kDebugC = 8,
	kDebugD = 9,
	kDebugg = 10,
	kDebugG = 11,
	kDebugH = 12,
	kDebugi = 13,
	kDebugI = 14,
	kDebugk = 15,
	kDebugm = 16,
	kDebugM = 17,
	kDebug_M = 18,
	kDebug_m = 19,
	kDebugp = 20,
	kDebugP = 21,
	kDebugR = 22,
	kDebugs = 23,
	kDebugS = 24,
	kDebugt = 25,
	kDebugT = 26,
	kDebugvAttach = 27,
	kDebugvAttachWait = 28,
	kDebugvContsm = 29,
	kDebugvContqs = 30,
	kDebugX = 31,
	kDebugZ0 = 32,
	kDebugz0 = 33,
	kDebugZ1 = 34,
	kDebugz1 = 35,
	kDebugZ2 = 36,
	kDebugz2 = 37,
	kDebugZ3 = 38,
	kDebugz3 = 39,
	kDebugZ4 = 40,
	kDebugz4 = 41,
	kDebugqC = 42,
	kDebugqGetPid = 43,
	kDebugqCRC = 44,
	kDebugqfThreadInfo = 45,
	kDebugqsThreadInfo = 46,
	kDebugqThreadExtraInfo = 47,
	kDebugqThreadStopInfo = 48,
	kDebugqOffsets = 49,
	kDebugqLaunchSuccess = 50,
	kDebugqShlibInfoAddr = 51,
	kDebugqStepPacketSupported = 52,
	kDebugqHostInfo = 53,
	kDebugqSymbol = 54,
	kDebugQStartNoAckMode = 55,
	kDebugqRegisterInfo = 56,
	kDebugvAttachName = 57,
	kDebugqProcessInfo = 58,
	kDebugQSetWorkingDir = 59,
	kDebugQSetMaxPacketSize = 60,
	
	kDebugCUSTOMCOMMAND,
	kDebugCommandCount
} DebuggerCommandType;


struct DebuggerCommand {
	DebuggerCommandType commandCode; // enum DebuggerCommandType
	CFStringRef command; // only use if custom command
	CFArrayRef arguments; // command arguments as CFStringRef <- these are encoded
};

typedef struct DebuggerCommand* DebuggerCommandRef;

struct SDMMD_AMDebugConnection {
	SDMMD_AMDeviceRef device;
	SDMMD_AMConnectionRef connection;
	bool ackEnabled;
} __attribute__ ((packed));

typedef struct SDMMD_AMDebugConnection* SDMMD_AMDebugConnectionRef;

typedef struct SDMMD_DebugCommandType {
	char *code;
	CFStringRef description;
} SDMMD_DebugCommandType;

static struct SDMMD_DebugCommandType KnownDebugCommands[kDebugCommandCount] = {
	{"+", CFSTR("ACK")},
	{"-", CFSTR("!ACK")},
	{"\x03", CFSTR("^C")},
	{"!", CFSTR("Use extended mode")},
	{"?", CFSTR("Why did target halt")},
	{"A", CFSTR("Set argv")}, // arglen, argnumber, arg
	{"B", CFSTR("Set/clear breakpoint")},
	{"c", CFSTR("continue")}, // "c [addr]"
	{"C", CFSTR("Continue with signal")},
	{"D", CFSTR("Detach debugger from remote system")},
	{"g", CFSTR("Read registers")},
	{"G", CFSTR("Write registers")},
	{"H", CFSTR("Set thread")}, // followed by 'c' (set continue thread) or 'g' (set current thread) then the thread number
	{"i", CFSTR("Step inferior by one clock cycle")},
	{"I", CFSTR("Signal inferior, then step one clock cycle")},
	{"k", CFSTR("Kill")},
	{"m", CFSTR("Read Memory")},
	{"M", CFSTR("Write Memory")},
	{"_M", CFSTR("Allocate memory")},
	{"_m", CFSTR("Deallocate memory")}, // _mXXXXXX where "XXXXXX" is the address previously allocated
	{"p", CFSTR("Read one register")}, // "p XX" where XX is the register
	{"P", CFSTR("Write one register")}, // "Pnn=rrrrr" nn=register, rrrrr=value
	{"R", CFSTR("Restart inferior")}, 
	{"s", CFSTR("Single step")},
	{"S", CFSTR("Single step with signal")}, // "S sig [;addr]"
	{"t", CFSTR("Search memory backwards")},
	{"T", CFSTR("Is thread alive")}, // "T XX"
	{"vAttach;", CFSTR("Attach to a new process")}, // vAttach;pid
	{"vAttachWait;", CFSTR("Wait for a new process to start up then attach to it")},
	{"vCont;", CFSTR("Verbose resume with thread actions")},
	{"vCont?", CFSTR("List valid continue-with-thread-actions actions")},
	{"X", CFSTR("Write data to memory")},
	{"Z0", CFSTR("Insert memory breakpoint")},
	{"z0", CFSTR("Remove memory breakpoint")},
	{"Z1", CFSTR("Insert hardware breakpoint")},
	{"z1", CFSTR("Remove hardware breakpoint")},
	{"Z2", CFSTR("Insert write watchpoint")},
	{"z2", CFSTR("Remove write watchpoint")},
	{"Z3", CFSTR("Insert read watchpoint")},
	{"z3", CFSTR("Remove read watchpoint")},
	{"Z4", CFSTR("Insert access watchpoint")},
	{"z4", CFSTR("Remove access watchpoint")},
	{"qC", CFSTR("Query current thread ID")}, // returns "QC" then the current thread of attached process as a string in hex
	{"qGetPid", CFSTR("Query process id")}, // returns the attached pid number as a string in hex
	{"qCRC", CFSTR("Compute CRC of memory region")},
	{"qfThreadInfo", CFSTR("Get list of active threads (first req)")},
	{"qsThreadInfo", CFSTR("Get list of active threads (subsequent req)")},
	{"qThreadExtraInfo", CFSTR("Get printable status of thread")},
	{"qThreadStopInfo", CFSTR("Get stop reason and other info about a thread")},
	{"qOffsets", CFSTR("Report offset of loaded program")},
	{"qLaunchSuccess", CFSTR("Report the success or failure of the launch attempt")},
	{"qShlibInfoAddr", CFSTR("Returns the address that contains info needed for getting shared library notifications")},
	{"qStepPacketSupported", CFSTR("Replies with OK if the 's' packet is supported.")},
	{"qHostInfo", CFSTR("Replies with multiple 'key:value;' tuples appended to each other.")},
	{"qSymbol", CFSTR("Notify that host debugger is ready to do symbol lookups")},
	{"QStartNoAckMode", CFSTR("Request that debugserver stop acking remote protocol packets")},
	{"qRegisterInfo", CFSTR("Requests the register information")},
	{"vAttachName:", CFSTR("Attach to process by name")},
	{"qProcessInfo", CFSTR("Get process info")},
	{"QSetWorkingDir:", CFSTR("Setting working directory, only do so when we have no process")},
	{"QSetMaxPacketSize:", CFSTR("Setting max packet size")},
	
	{"", CFSTR("")}
};

#pragma mark -
#pragma mark FUNCTIONS
#pragma mark -

SDMMD_AMDebugConnectionRef SDMMD_AMDebugConnectionCreateForDevice(SDMMD_AMDeviceRef device);
void SDMMD_AMDebugConnectionClose(SDMMD_AMDebugConnectionRef dconn);

sdmmd_return_t SDMMD_AMDebugConnectionStart(SDMMD_AMDebugConnectionRef dconn);
sdmmd_return_t SDMMD_AMDebugConnectionStop(SDMMD_AMDebugConnectionRef dconn);

sdmmd_return_t SDMMD_AMDeviceMountDeveloperImage(SDMMD_AMDeviceRef device); // use me instead of the one below for convenience
sdmmd_return_t SDMMD_AMDeviceMountImage(SDMMD_AMDeviceRef device, CFStringRef path, CFDictionaryRef dict, CallBack handle, void* unknown);

DebuggerCommandRef SDMMD_CreateDebuggingCommand(DebuggerCommandType commandCode, CFStringRef command, CFArrayRef arguments);
void SDMMD_DebuggingCommandRelease(DebuggerCommandRef command);

//BufferRef SDMMD_EncodeDebuggingString(CFStringRef command);

sdmmd_return_t SDMMD_DebuggingSend(SDMMD_AMDebugConnectionRef dconn, DebuggerCommandRef command, CFDataRef *response);
sdmmd_return_t SDMMD_DebuggingReceive(SDMMD_AMDebugConnectionRef dconn, CFDataRef *response);

bool SDMMD_device_os_is_at_least(SDMMD_AMDeviceRef device, CFStringRef version);
sdmmd_return_t SDMMD_stream_image(SDMMD_AMConnectionRef connection, CFStringRef path, CFStringRef image_type);

//sdmmd_debug_return_t SDMMD_DebuggingSend(SDMMD_AMDebugConnectionRef connection, SDMMD_DebugCommandType commandType, CFStringRef command);
//sdmmd_debug_return_t SDMMD_DebuggingReceive(SDMMD_AMDebugConnectionRef connection, CFDataRef *data);

#endif