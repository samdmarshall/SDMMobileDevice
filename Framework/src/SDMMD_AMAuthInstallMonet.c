//
//  SDMMD_AMAuthInstallMonet.c
//  SDMMobileDevice-Framework
//
//  Created by Samantha Marshall on 4/22/14.
//
//

#ifndef SDMMobileDevice_Framework_SDMMD_AMAuthInstallMonet_c
#define SDMMobileDevice_Framework_SDMMD_AMAuthInstallMonet_c

#include "SDMMD_AMAuthInstallMonet.h"

#if 0

function _AMAuthInstallMonetMeasureOsbl(CFStringRef rdi, rsi, CFDictionaryRef rdx, rcx, r8) {
	return _AMAuthInstallMonetMeasureBootImage(CFStringRef rdi, rsi, CFDictionaryRef rdx, _kAMAuthInstallTagBbOsblDownloadDigest, r8);
}

function _AMAuthInstallMonetStitchRestoreSbl1(CFStringRef rdi, rsi, CFDictionaryRef rdx, rcx, r8) {
 	return _AMAuthInstallMonetStitchEBootLoader(CFStringRef rdi, rsi, CFDictionaryRef rdx, rcx, _kAMAuthInstallTagBbRestoreSbl1Stitch);
}

function _AMAuthInstallMonetStitchSbl1(CFStringRef rdi, rsi, CFDictionaryRef rdx, rcx, r8) {
	return _AMAuthInstallMonetStitchEBootLoader(CFStringRef rdi, rsi, CFDictionaryRef rdx, rcx, _kAMAuthInstallTagBbSbl1Stitch);
}

function _AMAuthInstallMonetStitchENandprog(CFStringRef rdi, rsi, CFDictionaryRef rdx, rcx, r8) {
	return _AMAuthInstallMonetStitchEBootLoader(CFStringRef rdi, rsi, CFDictionaryRef rdx, rcx, _kAMAuthInstallTagBbENandprogStitch);
}

function _AMAuthInstallMonetStitchRestoreDbl(CFStringRef rdi, rsi, CFDictionaryRef rdx, rcx, r8) {
	return _AMAuthInstallMonetStitchEBootLoader(CFStringRef rdi, rsi, CFDictionaryRef rdx, rcx, _kAMAuthInstallTagBbRestoreDblStitch);
}

function _AMAuthInstallMonetStitchRestoreDbl(CFStringRef rdi, rsi, CFDictionaryRef rdx, rcx, r8) {
	return _AMAuthInstallMonetStitchEBootLoader(CFStringRef rdi, rsi, CFDictionaryRef rdx, rcx, _kAMAuthInstallTagBbRestoreDblStitch);
}

function _AMAuthInstallMonetStitchEDbl(CFStringRef rdi, rsi, CFDictionaryRef rdx, rcx, r8) {
	return _AMAuthInstallMonetStitchEBootLoader(CFStringRef rdi, rsi, CFDictionaryRef rdx, rcx, _kAMAuthInstallTagBbEDblStitch);
}

function _AMAuthInstallMonetMeasureENandprog(CFStringRef rdi, rsi, CFDictionaryRef rdx, rcx, r8) {
	return _AMAuthInstallMonetMeasureEBootLoader(CFStringRef rdi, rsi, CFDictionaryRef rdx, _kAMAuthInstallTagBbEnandprogVersion, _kAMAuthInstallTagBbEnandprogPartialDigest);
}

function _AMAuthInstallMonetMeasureRestoreDbl(CFStringRef rdi, rsi, CFDictionaryRef rdx, rcx, r8) {
    rax = _AMAuthInstallMonetMeasureEBootLoader(CFStringRef rdi, rsi, CFDictionaryRef rdx, _kAMAuthInstallTagBbRestoreDblVersion /*"com.apple.EmbeddedSoftwareRestore.Baseband.ChipId"*/, _kAMAuthInstallTagBbRestoreDblPartialDigest);
    return rax;
}

function _AMAuthInstallMonetStitchDbl(CFStringRef rdi, rsi, CFDictionaryRef rdx, rcx, r8) {
	return _AMAuthInstallMonetStitchEBootLoader(CFStringRef rdi, rsi, CFDictionaryRef rdx, _kAMAuthInstallTagBbEDblVersion, _kAMAuthInstallTagBbEDblPartialDigest);
}

function _AMAuthInstallMonetStitchEBootLoader(CFStringRef rdi, rsi, CFDictionaryRef rdx, rcx, r8) {
    r15 = r8;
    var_16 = rcx;
    r13 = rsi;
    r12 = rdi;
    CFStringRef var_32 = CFStringCreateWithFormat(kCFAllocatorDefault, NULL, CFSTR("%@-Original"), r12);
    CFStringRef var_24 = CFStringCreateWithFormat(kCFAllocatorDefault, NULL, CFSTR("%@-Personalized"), r12);
    CFTypeRef rax = CFDictionaryGetValue(rdx, _kAMAuthInstallTagBbFirmware);
    if (rax != 0x0) {
		// loc_453cd:
		r14 = CFDictionaryGetValue(rax, r15);
		if (r14 != 0x0) {
			// loc_453e4:
			if (CFDataGetLength(r14) >= CFDataGetLength(rsi)) {
				// loc_45502:
				_AMAuthInstallLog(0x3, "_AMAuthInstallMonetStitchFirstStage", "stitch data is larger than file");
				LODWORD(r12) = 0x1;
				// loc_45522;
			}
			else {
				// loc_45400;
			    CFDataRef r15 = CFDataCreateMutableCopy(kCFAllocatorDefault, NULL, rsi);
			    var_8 = r12;
			    LODWORD(r12) = 0x2;
			    var_0 = r13;
			    if (r15 == 0x0) {
					// loc_45522;
				}
				UInt8 *data_ptr = CFDataGetMutableBytePtr(r15);
			    memcpy(&(data_ptr[CFDataGetLength(r15) - CFDataGetLength(r14)]), CFDataGetBytePtr(r14), CFDataGetLength(r14));
			    *var_16 = CFRetain(r15);
			    _SafeRelease(r15);
			    r14 = var_32;
			    _AMAuthInstallDebugWriteObject(var_8, var_0, r14, 0x1);
			    rbx = var_24;
			    _AMAuthInstallDebugWriteObject(var_8, *var_16, rbx, 0x1);
			    LODWORD(r12) = 0x0;
			    // loc_45531;
			}
		}
	}
	
	// loc_454f3;
    _AMAuthInstallLog();
    LODWORD(r12) = 0x8;
	
	// loc_45522:
    _SafeRelease(0x0);
    r14 = var_32;
    rbx = var_24;
	
	// loc_45531:
    _SafeRelease(r14);
    _SafeRelease(rbx);
    LODWORD(rax) = LODWORD(r12);
    return rax;
}

function _AMAuthInstallMonetMeasureElf(rdi, CFDataRef rsi, rdx, rcx, r8) {
    var_16 = r8;
    var_32 = rcx;
    var_24 = rdx;
    rbx = rsi;
    r14 = rdi;
    if (!(CFDataGetLength(rbx) <= 0x33)) {
		// loc_459a9;
	    r15 = r14;
	    rax = CFDataGetBytePtr(rbx);
	    if (*(int32_t *)rax != 0x464c457f) {
			// loc_45b37;
		}
	    r14 = rbx;
	    LODWORD(rcx) = *(int16_t *)(rax + 0x2c) & 0xffff;
	    if (LODWORD(rcx) == 0x0) {
			// loc_45b45;
		}
	    rdx = *(int32_t *)(rax + 0x1c) + rax;
	    LODWORD(rbx) = 0x0;
	    LODWORD(rax) = 0x0;
	    do {
			LODWORD(rdi) = *(int32_t *)(rdx + 0xc);
			if (LODWORD(rdi) > LODWORD(rax)) {
				if (LODWORD(LODWORD(*(int32_t *)(rdx + 0x18)) & LODWORD(0x7000000)) != 0x5000000) {
					LODWORD(rax) = LODWORD(rdi);
				}
				if (CPU_FLAGS & E) {
					rbx = rdx;
				}
			}
			rdx = rdx + 0x20;
			LODWORD(rcx) = LODWORD(rcx) - 0x1;
	    } while (CPU_FLAGS & NE);
	    if (rbx == 0x0) {
			// loc_45b45;
		}
	    var_8 = r15;
	    r13 = CFDataCreateMutable(kCFAllocatorDefault, 0x14);
	    LODWORD(r15) = 0x2;
	    LODWORD(r12) = 0x0;
	    if (r13 == 0x0) {
		 	// loc_45b82:
			LODWORD(r13) = 0x0;
			// loc_45b5d;
	    }
		else {
			// loc_45a3b;
		    CFDataSetLength(r13, 0x14);
		    CC_SHA1_Init(&var_40);
		    CC_SHA1_Update(&var_40, *(int32_t *)(rbx + 0x4) + CFDataGetBytePtr(r14), *(int32_t *)(rbx + 0x10));
		    CC_SHA1_Final(CFDataGetMutableBytePtr(r13), &var_40);
		    r12 = CFDataCreateMutable(CFGetAllocator(var_8), 0x14);
		    if (r12 == 0x0) {
				// loc_45b5a;
			}
		    CFDataSetLength(r12, 0x14);
		    CC_SHA1_Init(&var_40);
		    CC_SHA1_Update(&var_40, CFDataGetBytePtr(r14), LODWORD(CFDataGetLength(r14)));
		    CC_SHA1_Final(CFDataGetMutableBytePtr(r12), &var_40);
		    CFDictionarySetValue(var_24, var_16, r12);
		    CFDictionarySetValue(var_24, var_32, r13);
		    LODWORD(r15) = 0x0;
		    //  loc_45b5d;
		}
	}
	
	//loc_45b37:
	
	//loc_45b45:
    _AMAuthInstallLog(0x3);
    LODWORD(r15) = 0xa;
    LODWORD(r13) = 0x0;
	
	// loc_45b5a:
    LODWORD(r12) = 0x0;
	
	// loc_45b5d:
    _SafeRelease(r12);
    _SafeRelease(r13);
    LODWORD(rax) = LODWORD(r15);
    return rax;
}

function _AMAuthInstallMonetMeasureAmss(rdi, CFDataRef rsi, rdx, rcx, r8) {
    rax = _AMAuthInstallMonetMeasureElf(rdi, CFDataRef rsi, rdx,_kAMAuthInstallTagBbAmssHashSectionDigest, _kAMAuthInstallTagBbAmssDownloadDigest);
    return rax;
}

function _AMAuthInstallMonetMeasureApps(rdi, CFDataRef rsi, rdx, rcx, r8) {
    rax = _AMAuthInstallMonetMeasureElf(rdi, CFDataRef rsi, rdx, _kAMAuthInstallTagBbAppsHashSectionDigest, _kAMAuthInstallTagBbAppsDownloadDigest);
    return rax;
}

function _AMAuthInstallMonetMeasureDsp1(rdi, CFDataRef rsi, rdx, rcx, r8) {
	return _AMAuthInstallMonetMeasureElf(rdi, CFDataRef rsi, rdx, _kAMAuthInstallTagBbDsp1HashSectionDigest, _kAMAuthInstallTagBbDsp1DownloadDigest);
}

function _AMAuthInstallMonetMeasureDsp2(rdi, CFDataRef rsi, rdx, rcx, r8) {
	return _AMAuthInstallMonetMeasureElf(rdi, CFDataRef rsi, rdx, _kAMAuthInstallTagBbDsp2HashSectionDigest, _kAMAuthInstallTagBbDsp2DownloadDigest);
}

function _AMAuthInstallMonetMeasureDsp3(rdi, CFDataRef rsi, rdx, rcx, r8) {
	return _AMAuthInstallMonetMeasureElf(rdi, CFDataRef rsi, rdx, _kAMAuthInstallTagBbDsp3HashSectionDigest, _kAMAuthInstallTagBbDsp3DownloadDigest);
}

function _AMAuthInstallMonetMeasureSbl1(rdi, CFDataRef rsi, rdx, rcx, r8) {
	return _AMAuthInstallMonetMeasureBootSbl(rdi, CFDataRef rsi, rdx, _kAMAuthInstallTagBbSbl1Version, _kAMAuthInstallTagBbSbl1PartialDigest);
}

function _AMAuthInstallMonetMeasureRestoreSbl1(rdi, CFDataRef rsi, rdx, rcx, r8) {
	return _AMAuthInstallMonetMeasureBootSbl(rdi, CFDataRef rsi, rdx, _kAMAuthInstallTagBbRestoreSbl1Version, _kAMAuthInstallTagBbRestoreSbl1PartialDigest);
}

function _AMAuthInstallMonetMeasureSbl2(rdi, CFDataRef rsi, CFDictionaryRef rdx, CFStringRef key) {
	return _AMAuthInstallMonetMeasureBootImage(rdi, CFDataRef rsi, CFDictionaryRef rdx, _kAMAuthInstallTagBbSbl2DownloadDigest);
}

function _AMAuthInstallMonetMeasureRPM(rdi, CFDataRef rsi, CFDictionaryRef rdx, CFStringRef key) {
	return _AMAuthInstallMonetMeasureBootImage(rdi, CFDataRef rsi, CFDictionaryRef rdx, _kAMAuthInstallTagBbRpmDownloadDigest);
}

uint32_t AMAuthInstallMonetLocalRegisterKeys(AMAuthInstallClassRef install) {
	CFDictionaryRef keys = install->ivars.keys;
	Boolean contains_key = CFDictionaryContainsKey(keys, CFSTR("bb.monet.rom.dbl.public"));
    if (!contains_key) {
		AMAuthInstallCryptoRegisterKeysFromPEMBuffer(keys, CFSTR("bb.monet.rom.dbl"), _kMonetAttestationDblCertPrivateKey, 0x68c);
		AMAuthInstallCryptoRegisterKeysFromPEMBuffer(keys, CFSTR("bb.monet.rom.enandprog"), _kMonetAttestationENandprogCertPrivateKey, 0x68c);
		AMAuthInstallCryptoRegisterKeysFromPEMBuffer(keys, CFSTR("bb.monet.ticket.activation"), _kMonetTicketActivationPrivateKey, 0x37c);
		AMAuthInstallCryptoRegisterKeysFromPEMBuffer(keys, CFSTR("bb.monet.ticket.calibration"), _kMonetTicketCalibrationPrivateKey, 0x378);
		AMAuthInstallCryptoRegisterKeysFromPEMBuffer(keys, CFSTR("bb.monet.ticket.provisioning"), _kMonetTicketProvisioningPrivateKey, 0x378);
		AMAuthInstallCryptoRegisterKeysFromPEMBuffer(keys, CFSTR("bb.monet.ticket.root"), _kMonetTicketRootPrivateKey, 0x378);
		AMAuthInstallCryptoRegisterCertFromPEMBuffer(keys, CFSTR("bb.monet.rom.general.root"), _kMonetRootCACert, 0x4da);
		AMAuthInstallCryptoRegisterCertFromPEMBuffer(keys, CFSTR("bb.monet.rom.general.attestationca"), _kMonetAttestationCACert, 0x4e6);
		AMAuthInstallCryptoRegisterCertFromPEMBuffer(keys, CFSTR("bb.monet.rom.dbl.attestation"), _kMonetAttestationDblCert, 0x523);
		AMAuthInstallCryptoRegisterCertFromPEMBuffer(keys, CFSTR("bb.monet.rom.enandprog.attestation"), _kMonetAttestationENandprogCert, 0x527);
		AMAuthInstallLog(0x6, "AMAuthInstallMonetLocalRegisterKeys", "monet local signing keys available");
    }
    return 0x0;
}

function _AMAuthInstallMonetMeasureBootImage(rdi, CFDataRef rsi, CFDictionaryRef rdx, CFStringRef key) {
    var_0 = rcx;
    r15 = rdx;
    r12 = rsi;
    CFMutableData r14 = CFDataCreateMutable(kCFAllocatorDefault, 0x14);
    if (r14 != 0x0) {
		CFDataSetLength(r14, 0x14);
		CC_SHA1_Init(&var_8);
		CC_SHA1_Update(&var_8, CFDataGetBytePtr(r12), LODWORD(CFDataGetLength(r12)));
		CC_SHA1_Final(CFDataGetMutableBytePtr(r14), &var_8);
		CFDictionarySetValue(r15, key, r14);
		LODWORD(rbx) = 0x0;
    }
    else {
		LODWORD(rbx) = 0x2;
    }
    _SafeRelease(r14);
    LODWORD(rax) = LODWORD(rbx);
    return rax;
}

function _AMAuthInstallMonetMeasureDbl(rdi, CFDataRef rsi, rdx) {
    var_32 = rdx;
    r13 = rsi;
    var_24 = rdi;
    r12 = CFDataGetBytePtr(r13);
    r15 = CFDataGetBytePtr(r13);
    rax = CFDataGetLength(r13);
    if (LODWORD(LODWORD(rax) >> 0x2) == 0x0) {
		// loc_4502f;
		_AMAuthInstallLog(0x7, "_FindSblHeaderInFileData", "foundMagic=%d", 0x0);
		
		// loc_450db:
		
		// loc_450e9:
		_AMAuthInstallLog(0x3);
		LODWORD(rax) = 0xa;
	}
	else {
		// loc_45001;
	    LODWORD(rax) = LODWORD(rax) >> 0x2;
	    asm{ neg        eax };
	    LODWORD(rbx) = 0x0;
	    LODWORD(r14) = 0x0;
		
		// while loop
		// loc_4500b:
		if (*(int32_t *)r15 != 0x73d71034) {
			// loc_45025;
		    r15 = r15 + 0x4;
		    LODWORD(rax) = LODWORD(rax) + 0x1;
		    if (CPU_FLAGS & NE) {
				// loc_4500b;
			}
			// loc_4506a:
			_AMAuthInstallLog(0x7, "_FindSblHeaderInFileData", "foundMagic=%d", LODWORD(r14));
			if (rbx == 0x0) {
				// loc_450db;
				// goto exit
			}
			if (rbx + 0x50 != r12 + *(int32_t *)(rbx + 0x14)) {
				// loc_450e9;
				// goto exit
			}
			_AMAuthInstallMonetSetVersionAndPartialDigest(var_24, var_32, r12, r13, rbx);
			LODWORD(rax) = 0x0;
			// loc_450fa;
		}
		else {
			// loc_45014;
		    rbx = r15 + 0xfffffffffffffffc;
		    LODWORD(r14) = LODWORD(r14) + 0x1;
		    if (*(int32_t *)(r15 + 0x4) != 0x7d0b435a) {
				// loc_45050;
			    _AMAuthInstallLog(0x7, "_FindSblHeaderInFileData", "found SBL header");
			    // loc_4506a;
			}
			else {
				// loc_45025;
			    r15 = r15 + 0x4;
			    LODWORD(rax) = LODWORD(rax) + 0x1;
			    if (CPU_FLAGS & NE) {
					// loc_4500b;
					// while loop back up
				}
			}
		}
	}
	
	//loc_450fa:
	return rax;
}

function _AMAuthInstallMonetSetVersionAndPartialDigest(rdi, CFDictionaryRef rsi, rdx, CFDataRef rcx, r8, r9) {
    var_16 = r8;
    r14 = rcx;
    var_24 = rdx;
    var_40 = rsi;
    var_32 = rdi;
    var_52 = LODWORD(*(int32_t *)(LODWORD(r9) + rdx + 0x4));
    rcx = CFDataGetLength(r14);
    _AMAuthInstallLog(0x7, "AMAuthInstallMonetSetVersionAndPartialDigest", "fileSize=0x%x, srcOffset=0x%x, codeSize=0x%x", rcx, LODWORD(r9), LODWORD(arg_offset_x0));
    _AMAuthInstallLog(0x7, "AMAuthInstallMonetSetVersionAndPartialDigest", "version=0x%x, personalizationOffset=0x%x", var_52, LODWORD(*(int32_t *)(LODWORD(r9) + rdx + 0x8)));
    rax = "NOT 64-BYTE MULTIPLE";
    LODWORD(r9) = LODWORD(r9) + LODWORD(*(int32_t *)(LODWORD(r9) + rdx + 0x8));
    LODWORD(arg_offset_x0) = LODWORD(arg_offset_x0) - LODWORD(*(int32_t *)(LODWORD(r9) + rdx + 0x8));
    LODWORD(*(int32_t *)(LODWORD(r9) + rdx + 0x8)) = LODWORD(*(int32_t *)(LODWORD(r9) + rdx + 0x8)) + 0x50;
    var_56 = LODWORD(r12);
    var_60 = LODWORD(rbx);
    if ((LOBYTE(rbx) & 0x3f) == 0x0) {
    }
    var_84 = LODWORD(r15);
    if ((LOBYTE(r15) & 0x3f) == 0x0) {
		rax = "ok";
    }
    _AMAuthInstallLog(0x7, "AMAuthInstallMonetSetVersionAndPartialDigest", "reservationLength=0x%x, codeBytesHashed=0x%x (%s), downloadBytesHashed=0x%x (%s)", LODWORD(r12), LODWORD(rbx));
    _AMAuthInstallLog(0x7, "AMAuthInstallMonetSetVersionAndPartialDigest", "personalization area=0x%x", var_56 - CFDataGetLength(r14) - var_84);
    LODWORD(r12) = 0xa;
    LODWORD(r14) = 0x0;
    if ((LOBYTE(var_60) & 0x3f) != 0x0) {
		// loc_452fd;
		r13 = r14;
	}
	else {
		// loc_4524b;
	    LODWORD(r14) = 0x0;
	    LODWORD(r13) = 0x0;
	    r15 = var_32;
	    rbx = var_24;
	    if ((var_84 & 0x3f) != 0x0) goto loc_45300;
	    __SHA1Partial();
	    __SHA1Partial(rbx, var_84, &var_88);
	    r13 = CFDataCreate(kCFAllocatorDefault, &var_56, 0x34);
	    LODWORD(r12) = 0x2;
	    LODWORD(r14) = 0x0;
	    if (r13 == 0x0) {
			// loc_452fd;
			r13 = r14;
		}
	    r14 = CFNumberCreate(kCFAllocatorDefault, 0x3, &var_52);
	    if (r14 != 0x0) {
			CFDictionarySetValue(var_40, arg_offset_x8, r14);
			CFDictionarySetValue(var_40, arg_offset_x10, r13);
			LODWORD(r12) = 0x0;
	    }
	    else {
			LODWORD(r14) = 0x0;
	    }
	}
	
	//loc_45300:
    _SafeRelease(r14);
    _SafeRelease(r13);
    return rax;
}

function _AMAuthInstallMonetSetVersionAndPartialDigestForSBL(rdi, CFDictionaryRef rsi, CFDataRef rcx, rdx, r8, r9) {
    r13 = rdi;
    LODWORD(r12) = 0x0;
    LODWORD(r15) = 0x1;
    LODWORD(r14) = 0x0;
    if (r13 != 0x0) {
		if ((rsi != 0x0) && (rdx != 0x0)) {
			if (rcx != 0x0) {
				var_16 = rsi;
				if (r8 != 0x0) {
					var_8 = r8;
					if ((arg_offset_x8 != 0x0) && (arg_offset_x10 != 0x0)) {
						var_28 = LODWORD(*(int32_t *)(LODWORD(r9) + rdx + 0x4));
						rcx = CFDataGetLength(rcx);
						_AMAuthInstallLog(0x7, "AMAuthInstallMonetSetVersionAndPartialDigestForSBL", "fileSize=0x%x, srcOffset=0x%x, codeSize=0x%x", rcx, LODWORD(r9), LODWORD(arg_offset_x0));
						_AMAuthInstallLog(0x7, "AMAuthInstallMonetSetVersionAndPartialDigestForSBL", "version=0x%x, personalizationOffset=0x%x", var_28, LODWORD(*(int32_t *)(LODWORD(r9) + rdx + 0x8)));
						LODWORD(arg_offset_x0) = LODWORD(arg_offset_x0) - LODWORD(*(int32_t *)(LODWORD(r9) + rdx + 0x8));
						LODWORD(*(int32_t *)(LODWORD(r9) + rdx + 0x8)) = LODWORD(*(int32_t *)(LODWORD(r9) + rdx + 0x8)) + 0x50;
						var_32 = LODWORD(r14);
						var_36 = LODWORD(rbx);
						if ((LOBYTE(rbx) & 0x3f) == 0x0) {
						}
						_AMAuthInstallLog(0x7, "AMAuthInstallMonetSetVersionAndPartialDigestForSBL");
						if ((LOBYTE(var_36) & 0x3f) != 0x0) {
							LODWORD(r15) = 0xa;
							LODWORD(r14) = 0x0;
							LODWORD(r12) = 0x0;
						}
						else {
							__SHA1Partial(var_8);
							r12 = CFDataCreate(kCFAllocatorDefault, &var_32, 0x1c);
							LODWORD(r15) = 0x2;
							if (r12 != 0x0) {
								r14 = CFNumberCreate(kCFAllocatorDefault, 0x3, &var_28);
								if (r14 != 0x0) {
									CFDictionarySetValue(var_16, arg_offset_x8, r14);
									CFDictionarySetValue(var_16, arg_offset_x10, r12);
									LODWORD(r15) = 0x0;
								}
								else {
									LODWORD(r14) = 0x0;
								}
							}
							else {
								LODWORD(r14) = 0x0;
								r12 = r14;
							}
						}
					}
				}
			}
		}
    }
    _SafeRelease(r14);
    _SafeRelease(r12);
	
    return rax;
}

function _AMAuthInstallMonetMeasurePartitionTable(rdi, CFDataRef rsi, CFDictionaryRef rdx) {
    r14 = rdx;
    r15 = rsi;
    r13 = CFDataCreateMutable(kCFAllocatorDefault, 0x14);
    if (r13 != 0x0) {
		CFDataSetLength(r13, 0x14);
		CC_SHA1_Init(&var_8);
		CC_SHA1_Update(&var_8, CFDataGetBytePtr(r15), LODWORD(CFDataGetLength(r15)));
		CC_SHA1_Final(CFDataGetMutableBytePtr(r13), &var_8);
		CFDictionarySetValue(r14, *_kAMAuthInstallTagBbPartitionTableDigest, r13);
		LODWORD(rbx) = 0x0;
    }
    else {
		LODWORD(rbx) = 0x2;
    }
    _SafeRelease(r13);
    LODWORD(rax) = LODWORD(rbx);
    return rax;
}

function __AMAuthInstallMonetGetOemSerialNumberFuseData(AMAuthInstallClassRef rdi) {
    r14 = rdi;
    var_40 = 0x0;
    LODWORD(rbx) = 0x1;
    if (r14 != 0x0) {
		// loc_4613d;
	    rdi = *(r14 + 0x88);
	    if (rdi == 0x0) {
			// loc_463b6;
			// goto exit
		}
	    CFTypeRef instructions = CFDictionaryGetValue(rdi, @"Instructions");
	    if (rbx == 0x0) {
			// loc_462ce;
		}
	    // loc_46165;
		if (CFGetTypeID(instructions) != CFArrayGetTypeID()) {
		    // loc_463aa
		}
		CFIndex instruction_count = CFArrayGetCount(instructions);
		for (CFIndex index = 0; index < instruction_count; index++) {
			// loc_461a2:
			CFTypeRef instruct = CFArrayGetValueAtIndex(instructions, index);
			if (CFGetTypeID(instruct) != CFArrayGetTypeID()) {
				// loc_463aa;
			}
			
			CFIndex instruct_count = CFArrayGetCount(instruct);
			if (instruct_count != 0x2) {
				// loc_46304;
			    _AMAuthInstallLog(0x3, "_AMAuthInstallMonetGetOemSerialNumberFuseData", "malformed assignment at %u: %@", r13, *(var_8 + 0x88));
			    LODWORD(rbx) = 0xe;
			    // loc_463b6;
				// goto exit
			}
			
			
			// loc_461e7:
			r12 = CFArrayGetValueAtIndex(instruct, 0);
			if (r12 == 0x0) {
				// loc_463aa;
			}
			if (CFGetTypeID(r12) != CFStringGetTypeID()) {
				// loc_463aa
			}
			if (CFStringCompare(@"OEM_SERIAL_NUMBER", r12, 0x1) == 0x0) {
				rax = CFArrayGetValueAtIndex(instruct, 1);
			}
			
		    // loc_46249
			r13 = r13 + 0x1;
			rbx = r15;
			if (r13 < var_16) {
				// loc_461a2;
				continue;
			}
			r15 = var_24;
			r14 = var_8;
			if (r15 == 0x0) {
				// loc_462ce;
			}
			if (CFGetTypeID(r15) != CFStringGetTypeID()) {
				// loc_463d7;
			    if (CFGetTypeID(r15) != CFNumberGetTypeID()) {
					// loc_4645d;
				    LODWORD(rbx) = 0x3;
				    _AMAuthInstallLog(0x3, "_AMAuthInstallMonetGetOemSerialNumberFuseData", "mistyped fuse value: %@", r15);
				    // loc_463b6;
					break;
				}
			    else {
					// loc_463ec;
				    rax = CFNumberGetValue(r15, 0x4, &var_32);
				    if (LOBYTE(rax) == 0x0) {
						// loc_464a8;
					}
				    // loc_46405;
					LODWORD(rax) = var_32;
					
				loc_46408:
					var_52 = LODWORD(rax);
					*(int32_t *)(*(r14 + 0x20) + 0x8) = LODWORD(LODWORD(rax) >> 0x1c);
					asm{ bswap      eax };
					var_52 = LODWORD(var_52);
					_SafeRelease(*(*(r14 + 0x20) + 0x10));
					*(*(r14 + 0x20) + 0x10) = CFDataCreate(CFGetAllocator(r14), &var_52, 0x4);
					// loc_463b6;
					break;
				}
			}
			// loc_4627f
		    LODWORD(rbx) = LODWORD(_AMAuthInstallSupportCopyDataFromHexString(CFGetAllocator(r14), r15, &var_40));
		    if (LODWORD(rbx) != 0x0) {
				// loc_463b6;
				break;
			}
		    rdi = var_40;
		    if (rdi == 0x0) {
				// loc_46484;
			    _AMAuthInstallLog(0x3, "_AMAuthInstallMonetGetOemSerialNumberFuseData", "unswappedSnumData is NULL");
			    LODWORD(rbx) = 0xe;
				break;
			}
		    else {
				// loc_462ad;
			    rax = CFDataGetBytePtr(rdi);
			    LODWORD(rax) = *(int32_t *)rax;
			    // loc_46408;
			    var_52 = LODWORD(rax);
			    *(int32_t *)(*(r14 + 0x20) + 0x8) = LODWORD(LODWORD(rax) >> 0x1c);
			    asm{ bswap      eax };
			    var_52 = LODWORD(var_52);
			    _SafeRelease(*(*(r14 + 0x20) + 0x10));
			    *(*(r14 + 0x20) + 0x10) = CFDataCreate(CFGetAllocator(r14), &var_52, 0x4);
			    // loc_463b6;
				break;
			}
			
			
		}
		
	}
	
	if (thing) {
		// loc_462ce;
	    _AMAuthInstallLog(0x3);
	    LODWORD(rbx) = 0x8;
	}
	
	if (other thing) {
		// loc_463aa
	    _AMAuthInstallLog();
	    LODWORD(rbx) = 0xe;
	}
	
	// loc_463b6:
    _SafeRelease(0x0);
    _SafeRelease(var_40);
    LODWORD(rax) = LODWORD(rbx);
    return rax;
}

function _AMAuthInstallMonetFuseIfNecessary(rdi, rsi, rdx) {
    r12 = rsi;
    r15 = rdi;
    var_39 = 0x0;
    var_24 = 0x0;
    LODWORD(r14) = 0x1;
    LODWORD(r13) = 0x0;
    if (r15 != 0x0) {
		// loc_46567;
	    var_16 = rdx;
	    LODWORD(r13) = 0x0;
	    if (*(r15 + 0x20) == 0x0) {
			// loc_466fe;
			// goto exit
		}
	    rax = _AMAuthInstallBasebandIsFused(r15, &var_39);
	    if (LODWORD(rax) != 0x0) {
			// loc_4680b;
		    LODWORD(r13) = 0x0;
		    LODWORD(r12) = 0x0;
		    LODWORD(rbx) = 0x0;
		    LODWORD(r14) = LODWORD(rax);
			// goto exit;
		}
	    // loc_4658d;
	    if ((var_39 != 0x0) && (*(int32_t *)(*(r15 + 0x20) + 0x50) != 0x2)) {
			// loc_465c0;
		    LODWORD(r14) = 0x0;
		    LODWORD(r13) = 0x0;
		    if (*(r15 + 0x88) == 0x0) {
				// loc_466fe;
				// goto exit;
			}
		    rbx = CFPropertyListCreateXMLData(kCFAllocatorDefault, *(r15 + 0x88));
		    if (rbx == 0x0) {
				// loc_4681b;
			    LODWORD(r14) = 0x3;
				
				// loc_466ef:
			    _AMAuthInstallLog(0x3);
				
				// loc_466fb:
			    LODWORD(r13) = 0x0;
			    // goto exit
			}
		    // loc_465f7;
			CFDictionarySetValue(var_16, @"fusingprogram.plist", rbx);
			
			// loc_466d9:
			LODWORD(r13) = 0x0;
			LODWORD(r12) = 0x0;
			// goto exit;
		}
	    // loc_4659d;
	    rdi = *(r15 + 0x170);
	    LODWORD(r14) = 0x1;
	    if (rdi != 0x0) {
			rax = CFDictionaryGetValue(rdi, *_kAMAuthInstallEntitlementBbFactoryDebugEnable);
	    }
	    else {
			rax = **kCFBooleanFalse;
	    }
	    if (rax == 0x0) {
			// loc_466e1;
			// goto exit;
		}
	    // loc_46622;
	    rax = CFBooleanGetValue(rax);
	    if (LOBYTE(rax) == 0x0) {
			// loc_466e1;
			// goto exit;
		}
	    if (*(int8_t *)(r15 + 0x90) != 0x0) {
			// loc_46859;
		    _AMAuthInstallLog(0x3, "AMAuthInstallMonetFuseIfNecessary", "fusing program request prohibited");
		    LODWORD(r14) = 0xe;
		    // loc_466fb;
		    LODWORD(r13) = 0x0;
		    // goto exit
		}
	    // loc_4664a;
	    LODWORD(r14) = LODWORD(_AMAuthInstallFusingProgramRequest(r15));
	    if (LODWORD(r14) != 0x0) {
			// loc_466fb;
		    LODWORD(r13) = 0x0;
		    // goto exit
		}
	    // loc_4665e;
	    if (*(int8_t *)(r15 + 0x91) != 0x0) {
			// loc_4665e;
		    if (*(r15 + 0x88) == 0x0) {
				// loc_46834;
			    LODWORD(r13) = 0x0;
			    LODWORD(r14) = 0xe;
				// goto exit;
			}
		    // loc_4666c;
		    rbx = CFPropertyListCreateXMLData(CFGetAllocator(r15), *(r15 + 0x88));
		    if (rbx == 0x0) {
				// loc_4681b;
			    LODWORD(r14) = 0x3;
				
				// loc_466ef:
			    _AMAuthInstallLog(0x3);
				
				// loc_466fb:
			    LODWORD(r13) = 0x0;
			    // goto exit
			}
		    CFDictionarySetValue(var_16, @"fusingprogram.plist", rbx);
		    rax = *(r15 + 0x20);
		    if ((*(rax + 0x18) != 0x0) && (*(rax + 0x10) != 0x0)) {
				// loc_466bc;
			    _AMAuthInstallLog(0x5, "AMAuthInstallMonetFuseIfNecessary", "baseband appears to be unfused and booted; will provide fusing program and fully personalized stack");
			    LODWORD(r14) = 0x0;
				// loc_466d9
				LODWORD(r13) = 0x0;
				LODWORD(r12) = 0x0;
				// goto exit;
			}
		    // loc_4673d;
		    LODWORD(r14) = LODWORD(_BbfwCreateDictionaryFromURL(kCFAllocatorDefault, r12, &var_24));
		    LODWORD(r12) = 0x0;
		    if (LODWORD(r14) != 0x0) {
				// loc_46851;
			    LODWORD(r13) = 0x0;
			    // loc_46703;
				// goto exit
			}
		    // loc_46763;
		    var_8 = rbx;
		    r14 = CFDictionaryGetCount(var_24);
		    r15 = r14 * 0x8;
		    r13 = malloc(r15);
		    if (r13 == 0x0) {
				// loc_46842;
			    LODWORD(r14) = 0x2;
			    rbx = var_8;
			    // loc_46703;
				// goto exit
			}
		    // loc_4678f;
		    r12 = malloc(r15);
		    if (r12 == 0x0) goto loc_46842;
		    var_0 = r13;
		    CFDictionaryGetKeysAndValues(var_24, r13, r12);
		    r13 = var_16;
		    if (r14 > 0x0) {
				r15 = r12;
				rbx = var_0;
				do {
					CFDictionarySetValue(r13, *rbx, *r15);
					r15 = r15 + 0x8;
					rbx = rbx + 0x8;
					r14 = r14 - 0x1;
				} while (CPU_FLAGS & NE);
		    }
		    _AMAuthInstallLog(0x5, "AMAuthInstallMonetFuseIfNecessary", "baseband appears to be unfused and unbootable; will provide fusing program and unpersonalized stack");
		    LODWORD(r14) = 0x0;
		    rbx = var_8;
		    r13 = var_0;
		    // loc_46703;
			// goto exit
		}
	}
	
	//loc_466fe:
    LODWORD(r12) = 0x0;
    LODWORD(rbx) = 0x0;
	
	//loc_46703:
    _SafeRelease(rbx);
    _SafeRelease(var_24);
    _SafeRelease(0x0);
    _SafeFree(r13);
    _SafeFree(r12);
    LODWORD(rax) = LODWORD(r14);
    return rax;
}

function _AMAuthInstallMonetCopyNextComponentName(rdi, rsi) {
    r14 = rsi;
    rbx = rdi;
    var_15 = 0x0;
    LODWORD(rax) = 0x1;
    if ((rbx != 0x0) && (*(rbx + 0x20) != 0x0)) {
		// loc_468cb;
	    rax = _AMAuthInstallBasebandIsFused(rbx, &var_15);
	    if (LODWORD(rax) == 0x0) {
			// loc_468db;
		    rax = *(*(rbx + 0x20) + 0x18);
		    if (var_15 == 0x0) {
				// loc_468f7;
				if (rax == 0x0) {
					// loc_46905
					rdi = @"preflash.bbfw";
				    *r14 = CFRetain(rdi);
				    LODWORD(rax) = 0x0;
				    // loc_46916;
				}
				// loc_468fc;
				rdi = @"stack.bbfw";
			}
		    // loc_468e9;
			rdi = @"boot.bbfw";
		}
	}
    // loc_46916;
	return rax;
}

function _AMAuthInstallMonetCreateReversedMEID(rdi, CFDataRef rsi, rdx) {
    var_8 = rdx;
    r14 = rsi;
    r15 = rdi;
    rax = CFDataGetLength(r14);
    LODWORD(r12) = 0x0;
    LODWORD(r13) = 0x1;
    if (rax == 0x7) {
		if (r14 != 0x0) {
			LODWORD(rbx) = 0x0;
			r12 = &var_23;
			do {
				*(int8_t *)r12 = LOBYTE(*(int8_t *)(CFDataGetBytePtr(r14) + rbx));
				r12 = r12 - 0x1;
				rbx = rbx + 0x1;
			} while (rbx != 0x7);
			r12 = CFDataCreate(r15, &var_17, 0x7);
			if (r12 != 0x0) {
				*var_8 = CFRetain(r12);
				LODWORD(r13) = 0x0;
			}
			else {
				LODWORD(r13) = 0x2;
				LODWORD(r12) = 0x0;
			}
		}
    }
    _SafeRelease(r12);
    LODWORD(rax) = LODWORD(r13);
    return rax;
}

function _AMAuthInstallMonetCreateMEIDFromString(rdi, CFStringRef rsi, rdx) {
    r15 = rdx;
    rbx = rsi;
    var_8 = 0x0;
    rax = CFStringGetLength(rbx);
    LODWORD(r14) = 0x0;
    if (rax == 0xf) {
		r14 = CFStringCreateWithSubstring(kCFAllocatorDefault, rbx, 0x0);
		rbx = r14;
    }
    LODWORD(r12) = LODWORD(_AMAuthInstallSupportCopyDataFromHexString(kCFAllocatorDefault, rbx, &var_8));
    if (LODWORD(r12) == 0x0) {
		rdi = var_8;
		if (rdi != 0x0) {
			*r15 = CFRetain(rdi);
			LODWORD(r12) = 0x0;
		}
		else {
			_AMAuthInstallLog(0x3, "AMAuthInstallMonetCreateMEIDFromString", "meid (%@) tempData is NULL", rbx);
			LODWORD(r12) = 0xe;
		}
    }
    else {
		_AMAuthInstallLog(0x3, "AMAuthInstallMonetCreateMEIDFromString", "failed to convert meid (%@) to data", rbx);
    }
    _SafeRelease(r14);
    _SafeRelease(var_8);
    LODWORD(rax) = LODWORD(r12);
    return rax;
}

function _AMAuthInstallMonetCreateReversedMEIDFromString(rdi, rsi) {
    r13 = rdx;
    r15 = rsi;
    var_16 = 0x0;
    var_8 = 0x0;
    LODWORD(r12) = LODWORD(_AMAuthInstallMonetCreateMEIDFromString(rdi, CFStringRef rsi, rdx));
    if (LODWORD(r12) == 0x0) {
		r14 = var_16;
		if (r14 != 0x0) {
			LODWORD(r12) = LODWORD(_AMAuthInstallMonetCreateReversedMEID(kCFAllocatorDefault, r14, &var_8));
			if (LODWORD(r12) == 0x0) {
				rbx = var_8;
				if (rbx != 0x0) {
					_AMAuthInstallLogDumpMemory(0x7, "AMAuthInstallMonetCreateReversedMEIDFromString", "reversed meid", CFDataGetBytePtr(rbx), LODWORD(CFDataGetLength(rbx)));
					rax = CFRetain(rbx);
					LODWORD(r12) = 0x0;
					*r13 = rax;
				}
				else {
					_AMAuthInstallLog(0x3, "AMAuthInstallMonetCreateReversedMEIDFromString", "meid (%@) reversedData is NULL", r15);
					LODWORD(r12) = 0xe;
					LODWORD(rbx) = 0x0;
				}
			}
			else {
				_AMAuthInstallLog(0x3, "AMAuthInstallMonetCreateReversedMEIDFromString", "failed to reverse meid (%@)", r15);
				rbx = var_8;
			}
		}
		else {
			_AMAuthInstallLog(0x3, "AMAuthInstallMonetCreateReversedMEIDFromString", "meid (%@) tempData is NULL", r15);
			LODWORD(r12) = 0xe;
			LODWORD(rbx) = 0x0;
			LODWORD(r14) = 0x0;
		}
    }
    else {
		_AMAuthInstallLog(0x3, "AMAuthInstallMonetCreateReversedMEIDFromString", "failed to convert meid (%@) to data", r15);
		LODWORD(rbx) = 0x0;
		r14 = var_16;
    }
    _SafeRelease(0x0);
    _SafeRelease(r14);
    _SafeRelease(rbx);
    LODWORD(rax) = LODWORD(r12);
    return rax;
}

function _AMAuthInstallMonetLocalSupportsLocalSigning {
    LODWORD(r8) = *(int32_t *)(*(rdi + 0x20) + 0x8);
    LOBYTE(rbx) = 0x0;
    if (LODWORD(*(int32_t *)*(rdi + 0x20)) != 0x3) {
		// loc_4b931;
		_AMAuthInstallLog(0x3, "AMAuthInstallMonetLocalSupportsLocalSigning", "local signing is disabled. vendorID=%d certID=%d");
	}
    else {
		// loc_4b929;
	    LOBYTE(rbx) = LODWORD(r8) == 0x0 ? 0xff : 0x0;
	    if (LODWORD(r8) == 0x0) {
			// loc_4b94d;
			_AMAuthInstallLog(0x3, "AMAuthInstallMonetLocalSupportsLocalSigning", "local signing is enabled");
		}
		else {
			// loc_4b931;
			_AMAuthInstallLog(0x3, "AMAuthInstallMonetLocalSupportsLocalSigning", "local signing is disabled. vendorID=%d certID=%d");
		}
	}
	
	//loc_4b967:
    LODWORD(rax) = LOBYTE(rbx) & 0xff;
    return rax;
}

function _AMAuthInstallMonetLocalProvisionV2Device(rdi, rsi) {
    r14 = rsi;
    rbx = rdi;
    if (rbx != 0x0) {
		_AMAuthInstallMonetLocalSetProvisioningManifestInfo(rbx, 0x1, 0x1, *(rbx + 0xd8), *(rbx + 0xb8), *(rbx + 0xe0));
		rdi = rbx;
		rsi = r14;
		rax = _AMAuthInstallMonetLocalProvisionDevice(rdi, rsi);
    }
    else {
		LODWORD(rax) = 0x1;
    }
    return rax;
}

function _AMAuthInstallMonetLocalProvisionDevice(rdi, rsi) {
    r15 = rsi;
    rbx = rdi;
    var_24 = 0x0;
    var_16 = 0x0;
    var_8 = 0x0;
    if (rbx == 0x0) {
    	// loc_4b72e
	    if (*(rbx + 0x20) == 0x0) {
			// loc_4b822;
			// loc_4b830
		}
	    // loc_4b739;
	    LODWORD(r12) = LODWORD(_AMAuthInstallMonetLocalCreateProvisioningTicket(rbx, &var_8));
	    if (LODWORD(r12) != 0x0) {
			// loc_4b847;
		    _AMAuthInstallLog(0x3, "AMAuthInstallMonetLocalProvisionDevice", "AMAuthInstallMonetLocalCreateProvisioningTicket failed\\n");
		    r14 = var_8;
		    // loc_4b7e8;
		}
	    // loc_4b751;
	    r14 = var_8;
	    if (r14 == 0x0) {
			// loc_4b867;
		    _AMAuthInstallLog(0x3, "AMAuthInstallMonetLocalProvisionDevice", "ticketData is NULL\\n");
		    LODWORD(r12) = 0xe;
		    LODWORD(r14) = 0x0;
		    // loc_4b7e8;
		}
	    // loc_4b75e;
	    if (*(int8_t *)(rbx + 0xd0) == 0x0) {
			// loc_4b76c;
		    if (*(rbx + 0xe0) == 0x0) {
				// loc_4b8ad;
			    _AMAuthInstallLog(0x3);
			    LODWORD(r12) = 0x1;
			    // loc_4b7e8;
			}
		    // loc_4b77a;
		    if (*(*(rbx + 0x20) + 0x38) == 0x0) {
				// loc_4b89f;
				// loc_4b8ad
			    _AMAuthInstallLog(0x3);
			    LODWORD(r12) = 0x1;
			    // loc_4b7e8;
			}
		    // loc_4b78b;
		    LODWORD(r12) = LODWORD(_AMAuthInstallCryptoDecryptSessionKey(rbx, @"bb.monet.ticket.provisioning"));
		    if (LODWORD(r12) != 0x0) {
				// loc_4b8e2;
			    _AMAuthInstallLog(0x3);
			    // loc_4b7e8;
			}
		    // loc_4b7aa;
		    LODWORD(r12) = LODWORD(_AMAuthInstallCryptoEncryptWithSessionKeyAndIVPrependIV(rbx, var_16, *(rbx + 0xe0), r14, &var_24));
		    if (LODWORD(r12) != 0x0) {
				// loc_4b8d4;
				// loc_4b8e2
			    _AMAuthInstallLog(0x3);
			    // loc_4b7e8;
			}
		    // loc_4b7d0;
		    rdi = var_24;
		    if (rdi == 0x0) {
				// loc_4b8f3;
			    _AMAuthInstallLog(0x3, "AMAuthInstallMonetLocalProvisionDevice", "manifestData is NULL\\n");
			}
			else {
				// loc_4b7dd;
				*r15 = CFRetain(rdi);
		    }
			// loc_4b7e5:
		    LODWORD(r12) = 0x0;
		    // loc_4b7e8;
		}
    }
	
	// loc_4b830:
    _AMAuthInstallLog(0x3);
    LODWORD(r12) = 0x1;
    LODWORD(r14) = 0x0;
	
	// loc_4b7e8:
    _SafeRelease(var_24);
    _SafeRelease(var_16);
    _SafeRelease(r14);
    LODWORD(rax) = LODWORD(r12);
    return rax;
}

function _AMAuthInstallMonetLocalSetBasebandReturnedInfo(rdi, rsi, rdx, rcx) {
    r15 = rcx;
    r14 = rsi;
    rbx = rdi;
    LODWORD(rax) = 0x1;
    if ((rbx != 0x0) && (*(rbx + 0x20) != 0x0)) {
		rcx = rdx | r15;
		if (CPU_FLAGS & E) {
			*(*(rbx + 0x20) + 0x28) = _SafeRetain(rdx);
			*(*(rbx + 0x20) + 0x20) = _SafeRetain(r15);
			rcx = _SafeRetain(r14);
			LODWORD(rax) = 0x0;
			*(*(rbx + 0x20) + 0x38) = rcx;
		}
    }
    return rax;
}

function _AMAuthInstallMonetLocalSetProvisioningManifestInfo(rdi, rsi, rdx, rcx, r8, r9) {
    r15 = r9;
    var_0 = rcx;
    var_12 = LODWORD(rdx);
    var_8 = LODWORD(rsi);
    rbx = rdi;
    var_16 = 0x0;
    LODWORD(r14) = 0x1;
    if (rbx != 0x0) {
		// loc_4b3c1;
	    r13 = arg_offset_x8;
	    r14 = arg_offset_x0;
	    if (r8 == 0x0) {
			// loc_4b3dc;
		    rax = CFDataCreate(**kCFAllocatorDefault, _AMAuthInstallMonetLocalSetProvisioningManifestInfo.kDefaultActivationEncryptionKey, 0x10);
		    var_16 = rax;
		    if (rax == 0x0) {
				// loc_4b569;
			    _AMAuthInstallLog(0x3, "AMAuthInstallMonetLocalSetProvisioningManifestInfo", "failed to convert activationEncryptionKeyString to data\\n");
			    LODWORD(r14) = 0x2;
			    var_16 = 0x0;
			    // loc_4b5b3;
			}
		    // loc_4b404;
		}
		else {
	    	// loc_4b3ce;
	    	var_16 = CFRetain(r8);
	    	// loc_4b404;
		}
	}
	
	if (loc_4b404) {
		// loc_4b404:
   	 	if (r15 == 0x0) {
			// loc_4b416;
		    r15 = _AMAuthInstallSupportCreateRandomNumber(0x10);
		    if (r15 == 0x0) {
				// loc_4b593;
				_AMAuthInstallLog(0x3, "AMAuthInstallMonetLocalSetProvisioningManifestInfo", "failed to convert vi to data\\n");
				LODWORD(r14) = 0x2;
				// loc_4b5b3;
			}
			r15 = CFRetain(r15);
		    // loc_4b42c;
		    if (r14 == 0x0) {
				// loc_4b43e;
			    r12 = _AMAuthInstallSupportCreateRandomNumber(0x10);
			    if (r12 == 0x0) {
					// loc_4b5b8;
				    _AMAuthInstallLog(0x3, "AMAuthInstallMonetLocalSetProvisioningManifestInfo", "failed to convert randomKey to data\\n");
				    LODWORD(r14) = 0x2;
				    // loc_4b5d8;
				}
			    // loc_4b431;
			    r12 = CFRetain(r14);
			    // loc_4b454;
			    if (r13 == 0x0) {
					// loc_4b466;
				    r13 = _AMAuthInstallSupportCreateRandomNumber(0x14);
				    if (r13 == 0x0) {
						// loc_4b5e3;
						_AMAuthInstallLog(0x3, "AMAuthInstallMonetLocalSetProvisioningManifestInfo", "failed to convert serverNonce to data\\n");
						LODWORD(r14) = 0x2;
						LODWORD(r13) = 0x0;
						// loc_4b536;
					}
				    // loc_4b47c;
				}
			    // loc_4b459;
			    r13 = CFRetain(r13);
			    // loc_4b47c;
			}
		}
	}
	
	// loc_4b5b3:
    LODWORD(r15) = 0x0;
	
	// loc_4b5d8:
    LODWORD(r12) = 0x0;
    LODWORD(r13) = 0x0;
	
	if (loc_4b536) {
	    // loc_4b47c;
	    _SafeRelease(*(rbx + 0xc0));
	    _SafeRelease(*(rbx + 0xd8));
	    _SafeRelease(*(rbx + 0xc8));
	    _SafeRelease(*(rbx + 0xb8));
	    _SafeRelease(*(rbx + 0xe0));
	    _SafeRelease(*(rbx + 0xe8));
	    *(rbx + 0xc0) = CFRetain(r13);
	    *(rbx + 0xd8) = _SafeRetain(var_0);
	    *(rbx + 0xc8) = _SafeRetain(arg_offset_x10);
	    *(int8_t *)(rbx + 0xd0) = LOBYTE(var_8);
	    *(int8_t *)(rbx + 0xf0) = LOBYTE(var_12);
	    *(rbx + 0xb8) = CFRetain(var_16);
	    *(rbx + 0xe0) = CFRetain(r15);
	    rax = CFRetain(r12);
	    LODWORD(r14) = 0x0;
	    *(rbx + 0xe8) = rax;
	}
	
	// loc_4b536:
    _SafeRelease(var_16);
    _SafeRelease(r15);
    _SafeRelease(r13);
    _SafeRelease(r12);
    LODWORD(rax) = LODWORD(r14);
    return rax;
}

function __AMAuthInstallMonetLocalCreateEDBLDownloadDigestData(rdi, rsi, rdx, rcx) {
    var_0 = rcx;
    r15 = rdx;
    r12 = rsi;
    r13 = CFDataCreateMutable(kCFAllocatorDefault, 0x14);
    if (r13 != 0x0) {
		CFDataSetLength(r13, 0x14);
		rbx = CFDataGetBytePtr(r12);
		CC_SHA1_Init(&var_8);
		asm{ bswap      eax };
		var_8 = LODWORD(*(int32_t *)(rbx + 0x8));
		asm{ bswap      eax };
		var_12 = LODWORD(*(int32_t *)(rbx + 0xc));
		asm{ bswap      eax };
		var_16 = LODWORD(*(int32_t *)(rbx + 0x10));
		asm{ bswap      eax };
		var_20 = LODWORD(*(int32_t *)(rbx + 0x14));
		asm{ bswap      eax };
		var_24 = LODWORD(*(int32_t *)(rbx + 0x18));
		var_28 = LODWORD(LODWORD(*(int32_t *)(rbx + 0x4)) << 0x3);
		var_32 = 0x0;
		CC_SHA1_Update(&var_8, CFDataGetBytePtr(r15), LODWORD(CFDataGetLength(r15)));
		CC_SHA1_Final(CFDataGetMutableBytePtr(r13), &var_8);
		rax = CFRetain(r13);
		LODWORD(rbx) = 0x0;
		*var_0 = rax;
    }
    else {
		LODWORD(rbx) = 0x2;
    }
    _SafeRelease(r13);
    LODWORD(rax) = LODWORD(rbx);
    return rax;
}

function __AMAuthInstallMonetLocalCreateStitchData {
    var_8 = rcx;
    rbx = rdi;
    var_72 = 0x0;
    if (LODWORD(rsi) == 0x1) {
		// loc_47cd6;
	    var_40 = LODWORD(rsi);
	    rax = @"bb.monet.rom.dbl.private";
	}
	else /* loc_47ce5: */ if (LODWORD(rsi) == 0x2) {
 	   	// loc_47cee;
		var_40 = LODWORD(rsi);
	    rax = @"bb.monet.rom.enandprog.private";
	}
	
	// loc_47cfb:
	var_16 = rax;
	rax = CFDataGetBytePtr(rdx);
	var_24 = rax;
	r15 = CFDataGetLength(*(*(rbx + 0x20) + 0x10));
	rax = CFDataCreateMutable(CFGetAllocator(rbx), 0x0);
	r12 = rbx;
	r13 = rax;
	if (rax == 0x0) {
		// loc_48504;
	    var_56 = 0x2;
	    LODWORD(r12) = 0x0;
	    LODWORD(rbx) = 0x0;
	    LODWORD(r15) = 0x0;
	    LODWORD(r14) = 0x0;
	    var_64 = 0x0;
	    // loc_484a0;
	}
	// loc_47d49;
    var_64 = 0x0;
    rax = _AMAuthInstallCryptoCreateDigestForKey(r12, @"bb.monet.ticket.root.public", &var_72);
    if (LODWORD(rax) != 0x0) {
		// loc_48529;
	    LODWORD(r15) = 0x0;
	    var_56 = LODWORD(rax);
	    r14 = r13;
	    LODWORD(rbx) = 0x0;
	    LODWORD(r12) = 0x0;
	    // loc_484a0;
	}
    // loc_47d72;
    var_56 = 0x2;
    CFDataAppendBytes(r13, CFDataGetBytePtr(var_72), CFDataGetLength(var_72));
    CFDataAppendBytes(r13, CFDataGetBytePtr(*(*(r12 + 0x20) + 0x10)), CFDataGetLength(*(*(r12 + 0x20) + 0x10)));
    LODWORD(rbx) = LODWORD(r14 - r15);
    r14 = r13;
    CFDataIncreaseLength(r14, rbx);
    memset(CFDataGetLength(r14) - rbx + CFDataGetMutableBytePtr(r14), 0xff, rbx);
    rbx = CFDataCreateMutable(CFGetAllocator(r12), 0x14);
    if (rbx == 0x0) {
		// loc_4853f;
	    LODWORD(r12) = 0x0;
	    LODWORD(rbx) = 0x0;
	    LODWORD(r15) = 0x0;
	    // loc_484a0;
	}
    // loc_47e33;
    CFDataSetLength(rbx, 0x14);
    r13 = r12;
    var_48 = rbx;
    CC_SHA1_Init(&var_80);
    asm{ bswap      eax };
    var_80 = LODWORD(*(int32_t *)(var_24 + 0x8));
    asm{ bswap      eax };
    var_84 = LODWORD(*(int32_t *)(var_24 + 0xc));
    asm{ bswap      eax };
    var_88 = LODWORD(*(int32_t *)(var_24 + 0x10));
    asm{ bswap      eax };
    var_92 = LODWORD(*(int32_t *)(var_24 + 0x14));
    asm{ bswap      eax };
    var_96 = LODWORD(*(int32_t *)(var_24 + 0x18));
    var_100 = LODWORD(LODWORD(*(int32_t *)(var_24 + 0x4)) << 0x3);
    var_104 = 0x0;
    rbx = CFDataGetBytePtr(r14);
    var_24 = r14;
    rax = CFDataGetLength(r14);
    r15 = var_48;
    CC_SHA1_Update(&var_80, rbx, LODWORD(rax));
    CC_SHA1_Final(CFDataGetMutableBytePtr(r15), &var_80);
    r12 = 0x3536363636363636;
    LODWORD(rbx) = 0x1;
    rax = *(r13 + 0x20);
    var_32 = r13;
    var_56 = *(int32_t *)(rax + 0x4);
    LODWORD(rcx) = var_40;
    // loc_47f2c;
    r12 = 0x3636363636363636;
    goto loc_47f36;
    if (LODWORD(rcx) == 0x2) {
		// loc_47f36;
	    LODWORD(r13) = LODWORD(LODWORD(*(int32_t *)CFDataGetBytePtr(*(*(var_32 + 0x20) + 0x10))) << 0x18) | LODWORD(LODWORD(LODWORD(LODWORD(*(int32_t *)CFDataGetBytePtr(*(*(var_32 + 0x20) + 0x10))) << 0x8) & 0xff0000) | LODWORD(LODWORD(LODWORD(LODWORD(*(int32_t *)CFDataGetBytePtr(*(*(var_32 + 0x20) + 0x10))) >> 0x8) & 0xff00) | LODWORD(*(int8_t *)(CFDataGetBytePtr(*(rax + 0x10)) + 0x3) & 0xff)));
	    r14 = CFDataCreateMutable(CFGetAllocator(var_32), 0x0);
	    if (r14 != 0x0) {
			var_64 = 0x0;
			CFDataSetLength(r14, 0x14);
			r15 = var_48;
			if (CFDataGetLength(r15) == 0x14) {
				rax = CFDataGetBytePtr(r15);
				var_296 = LODWORD(*(int32_t *)(rax + 0x10));
				var_288 = *(rax + 0x8);
				var_280 = *rax;
				var_272 = r12;
				CC_SHA1_Init(&var_176);
				CC_SHA1_Update(&var_176, &var_272, 0x1c);
				CC_SHA1_Final(&var_280, &var_176);
				asm{ bswap      rax };
				var_272 = 0x5c5c5c5c5c5c5c5c ^ (r13 | var_56 << 0x20);
				CC_SHA1_Init(&var_176);
				CC_SHA1_Update(&var_176, &var_272, 0x1c);
				CC_SHA1_Final(CFDataGetMutableBytePtr(r14), &var_176);
				var_64 = CFRetain(r14);
				LODWORD(rbx) = 0x0;
			}
			else {
				LODWORD(rbx) = 0x1;
			}
	    }
	    else {
			LODWORD(r14) = 0x0;
			var_64 = 0x0;
			r15 = var_48;
			LODWORD(rbx) = 0x2;
	    }
		
		// loc_480e3:
	    _SafeRelease(r14);
	    LODWORD(r12) = 0x0;
	    if (LODWORD(rbx) != 0x0) {
			// loc_4854c;
		    var_56 = LODWORD(rbx);
		    LODWORD(rbx) = 0x0;
		    r14 = var_24;
		    // loc_484a0;
		}
	    // loc_480f6;
	    var_48 = r15;
	    var_176 = 0x0;
	    r14 = var_16;
	    LODWORD(rbx) = LODWORD(_AMAuthInstallCryptoGetRsaKeyLength(var_32, r14, &var_272));
	    LODWORD(r12) = 0x0;
	    LODWORD(r13) = 0x0;
	    if (LODWORD(rbx) == 0x0) {
			r12 = var_64;
			if (LODWORD(CFDataGetLength(r12)) < LODWORD(LODWORD(var_272) + 0xfffffffd)) {
				r13 = CFDataCreateMutable(CFGetAllocator(var_32), 0x0);
				CFDataAppendBytes(r13, _kLeadingBytes, 0x2);
				CFDataIncreaseLength(r13, LODWORD(LODWORD(var_272) + 0xfffffffd) - LODWORD(CFDataGetLength(r12)));
				memset(CFDataGetMutableBytePtr(r13) + 0x2, 0xff, LODWORD(LODWORD(var_272) + 0xfffffffd) - LODWORD(CFDataGetLength(r12)));
				CFDataAppendBytes(r13, _kCentralBytes, 0x1);
				CFDataAppendBytes(r13, CFDataGetBytePtr(r12), CFDataGetLength(r12));
				LODWORD(rbx) = LODWORD(_AMAuthInstallCryptoCreateRsaCustomSignature(var_32, r14, CFDataGetBytePtr(r13), LODWORD(CFDataGetLength(r13)), &var_176));
				if (LODWORD(rbx) == 0x0) {
					r12 = CFRetain(var_176);
					LODWORD(rbx) = 0x0;
				}
				else {
					LODWORD(r12) = 0x0;
				}
			}
			else {
				_AMAuthInstallLog(0x3, "_AMAuthInstallMonetLocalCreateSignature", "data to be signed is too large");
				LODWORD(rbx) = 0x1;
				LODWORD(r12) = 0x0;
				LODWORD(r13) = 0x0;
			}
	    }
	    _SafeRelease(r13);
	    _SafeRelease(var_176);
	    if (LODWORD(rbx) != 0x0) {
			// loc_48560;
		    var_56 = LODWORD(rbx);
		    r15 = var_48;
		    r14 = var_24;
		    LODWORD(rbx) = 0x0;
		    // loc_484a0;
		}
	    // loc_4824f;
	    r15 = CFDataGetBytePtr(r12);
	    rax = CFDataGetLength(r12);
	    r14 = var_24;
	    CFDataAppendBytes(r14, r15, rax);
	    LODWORD(rax) = var_40;
	    rbx = var_32;
	    if (LODWORD(rax) != 0x1) {
			// loc_48299;
		    if (LODWORD(rax) != 0x2) {
				// loc_4840a;
			    var_16 = r12;
			    _AMAuthInstallLog(0x3, "_AMAuthInstallMonetLocalCopyCertChain", "unknown boot object");
			    LODWORD(rbx) = 0x1;
			    LODWORD(r13) = 0x0;
			    var_40 = 0x0;
			}
			else {
				
			    // loc_482a2;
			    var_16 = r12;
			    r15 = @"bb.monet.rom.enandprog.attestation";
				// loc_482b0:
				r13 = CFDataCreateMutable(CFGetAllocator(rbx), 0x1800);
				r12 = rbx;
				LODWORD(rbx) = 0x2;
				if (r13 != 0x0) {
					var_40 = 0x0;
					LODWORD(rbx) = LODWORD(_AMAuthInstallCryptoGetCertData(r12, r15, &var_176));
					if (LODWORD(rbx) == 0x0) {
						CFDataAppendBytes(r13, CFDataGetBytePtr(var_176), CFDataGetLength(var_176));
						LODWORD(rbx) = LODWORD(_AMAuthInstallCryptoGetCertData(r12, @"bb.monet.rom.general.attestationca", &var_176));
						if (LODWORD(rbx) == 0x0) {
							CFDataAppendBytes(r13, CFDataGetBytePtr(var_176), CFDataGetLength(var_176));
							LODWORD(rbx) = LODWORD(_AMAuthInstallCryptoGetCertData(r12, @"bb.monet.rom.general.root", &var_176));
							if (LODWORD(rbx) == 0x0) {
								CFDataAppendBytes(r13, CFDataGetBytePtr(var_176), CFDataGetLength(var_176));
								CFDataIncreaseLength(r13, LODWORD(0x1800) - LODWORD(CFDataGetLength(r13)));
								memset(0x1800 - LODWORD(0x1800) - LODWORD(CFDataGetLength(r13)) + CFDataGetMutableBytePtr(r13), 0xff, LODWORD(0x1800) - LODWORD(CFDataGetLength(r13)));
								var_40 = CFRetain(r13);
								LODWORD(rbx) = 0x0;
							}
						}
					}
				}
				else {
					r13 = 0x0;
					var_40 = 0x0;
				}
			}
			// loc_4843e:
		    _SafeRelease(r13);
		    r12 = var_16;
		    if (LODWORD(rbx) == 0x0) {
				rbx = var_40;
				CFDataAppendBytes(r14, CFDataGetBytePtr(rbx), CFDataGetLength(rbx));
				rax = CFRetain(r14);
				var_56 = 0x0;
				*var_8 = rax;
				r15 = var_48;
		    }
		    else {
				var_56 = LODWORD(rbx);
				r15 = var_48;
				rbx = var_40;
		    }
		    // loc_484a0;
			
		}
	    // loc_48289;
	    var_16 = r12;
	    r15 = @"bb.monet.rom.dbl.attestation";
	    // loc_482b0;
	}
    // loc_47f20;
    LODWORD(r14) = 0x0;
    if (LODWORD(rcx) != 0x1) {
		// loc_480d8;
	    var_64 = 0x0;
	    // loc_480e3;
	}
	
	//loc_48099:
    _AMAuthInstallLog(0x3, "_AMAuthInstallMonetLocalCreateStitchData", "unknown boot object");
    var_56 = 0x1;
    var_64 = 0x0;
    LODWORD(r12) = 0x0;
    LODWORD(rbx) = 0x0;
    LODWORD(r15) = 0x0;
    LODWORD(r14) = 0x0;
	
	//loc_484a0:
    _SafeRelease(var_72);
    _SafeRelease(r15);
    _SafeRelease(r14);
    _SafeRelease(var_64);
    _SafeRelease(r12);
    _SafeRelease(rbx);
    return rax;
}

function _AMAuthInstallMonetLocalCreateSignedTicket {
    var_16 = r9;
    r14 = r8;
    r13 = rcx;
    r15 = rsi;
    rbx = rdi;
    var_215 = LOBYTE(rdx);
    var_184 = 0x0;
    var_176 = 0x0;
    var_168 = 0x0;
    var_160 = 0x0;
    var_152 = 0x0;
    var_144 = 0x0;
    var_136 = 0x0;
    rdi = CFGetAllocator(rdi);
    rax = CFStringCreateWithFormat(rdi, 0x0, @"%@.public");
    var_60 = 0x2;
    if (rax != 0x0) {
		// loc_4a0bb;
	    var_32 = rax;
	    rax = _AMAuthInstallCryptoCreateSubjectPublicKeyInfoForKey(rbx, rax, &var_184);
	    if (LODWORD(rax) != 0x0) {
			// loc_4a639;
		    var_48 = 0x0;
		    var_60 = LODWORD(rax);
		    // loc_4a657;
		}
	    // loc_4a0d9;
	    rdi = CFGetAllocator(rbx);
	    rax = CFStringCreateWithFormat(rdi, 0x0, @"%@.private");
	    var_40 = rax;
	    if (rax == 0x0) {
			// loc_4a64c;
		    var_48 = 0x0;
		    // loc_4a657;
		}
	    // loc_4a107;
	    var_24 = rbx;
	    rax = _DEREncoderCreate(0x0);
	    if (rax == 0x0) {
			// loc_4a683;
		    var_48 = 0x0;
			
			// loc_4a68e:
		    var_64 = 0x0;
			
			// loc_4a699:
		    LODWORD(r13) = 0x0;
			
			// loc_4a756:
		    var_80 = 0x0;
		    LODWORD(r12) = 0x0;
		    // loc_4a764;
		}
	    // loc_4a11e;
	    var_48 = rax;
	    rbx = _DEREncoderCreate(0x0);
	    if (rbx == 0x0) {
			// loc_4a68e;
		    var_64 = 0x0;
			
			// loc_4a699:
		    LODWORD(r13) = 0x0;
			
			// loc_4a756:
		    var_80 = 0x0;
		    LODWORD(r12) = 0x0;
		    // loc_4a764;
		}
	    if (var_215 == 0x0) {
			// loc_4a162;
		    rax = _DEREncoderAddData(rbx, 0x0, 0x10, _kSha1WithRsaEncryption, 0xb, 0x1);
		    if (LODWORD(rax) != 0x0) {
				// loc_4a6a1;
			    var_64 = rbx;
			    _AMAuthInstallLog(0x3, "AMAuthInstallMonetLocalCreateSignedTicket", "failed to add ticket algorithm ID to DER file");
			    LODWORD(r13) = 0x0;
			    var_60 = 0x0;
			    // loc_4a756;
			    var_80 = 0x0;
			    LODWORD(r12) = 0x0;
			    // loc_4a764;
			}
		    // loc_4a18f;
		    var_64 = rbx;
		    if (var_215 != 0x0) {
				// loc_4a1a9;
			    var_80 = 0x0;
			    if (r14 != 0x0) {
					// loc_4a449;
				    LODWORD(r12) = 0x0;
				    var_72 = 0x0;
				    if (var_16 == 0x0) {
						// loc_4a1cb;
					    r14 = CFDataGetBytePtr(r13);
					    rax = CFDataGetLength(r13);
					    rbx = var_48;
					    rax = _DEREncoderAddData(rbx, 0x0, 0x11, r14, LODWORD(rax), 0x1);
					    if (LODWORD(rax) != 0x0) {
							// loc_4a797;
						    _AMAuthInstallLog(0x3);
						    LODWORD(r13) = 0x0;
							
							// loc_4a7a6:
						    LODWORD(r14) = 0x0;
						    // loc_4a7a9;
						}
					    // loc_4a20b;
					    rax = _DEREncoderCreateEncodedBuffer(rbx, &var_168, &var_208);
					    if (LODWORD(rax) != 0x0) {
							// loc_4a797;
						    _AMAuthInstallLog(0x3);
						    LODWORD(r13) = 0x0;
							
							// loc_4a7a6:
						    LODWORD(r14) = 0x0;
						    // loc_4a7a9;
						}
					    rax = _AMAuthInstallCryptoCreateRsaSignature(var_24, var_40, var_168, var_208, &var_176);
					    if (LODWORD(rax) != 0x0) {
							// loc_4a6fa;
						    LODWORD(r13) = 0x0;
						    var_60 = LODWORD(rax);
						    // loc_4a7a6;
						    LODWORD(r14) = 0x0;
						    // loc_4a7a9;
						}
					    // loc_4a249;
					    r14 = CFDataGetBytePtr(r13);
					    rax = CFDataGetLength(r13);
					    rbx = var_64;
					    rax = _DEREncoderAddData(rbx, 0x0, 0x11, r14, LODWORD(rax), 0x1);
					    if (LODWORD(rax) != 0x0) {
							// loc_4a797;
						    _AMAuthInstallLog(0x3);
						    LODWORD(r13) = 0x0;
							
							// loc_4a7a6:
						    LODWORD(r14) = 0x0;
						    // loc_4a7a9;
						}
					    rax = _DEREncoderAddData(rbx, 0x0, 0x4, CFDataGetBytePtr(var_176), LODWORD(CFDataGetLength(var_176)), 0x0);
					    if (LODWORD(rax) != 0x0) {
							// loc_4a797;
						    _AMAuthInstallLog(0x3);
						    LODWORD(r13) = 0x0;
							
							// loc_4a7a6:
						    LODWORD(r14) = 0x0;
						    // loc_4a7a9;
						}
					    var_108 = LODWORD(CFDataGetLength(var_184));
					    rax = _DERDecoderInitialize(&var_112, CFDataGetBytePtr(var_184), &var_108, var_108);
					    LODWORD(r13) = 0x0;
					    var_60 = 0x63;
					    if (LODWORD(rax) != 0x0) {
							// loc_4a7a6;
						    LODWORD(r14) = 0x0;
						    // loc_4a7a9;
						}
					    LODWORD(r14) = 0x0;
					    rax = _DERDecoderGetDataAt(&var_112, 0x0, 0x0, 0x0, &var_96, &var_92);
					    if (LODWORD(rax) != 0x0) {
							// loc_4a774;
						    LODWORD(r14) = 0x0;
						    r13 = r14;
						    // loc_4a7a9;
						}
					    // loc_4a33c;
					    var_60 = 0x2;
					    rbx = var_64;
					    rax = _DEREncoderAddData(rbx, 0x2, 0x3, var_96, var_92, 0x1);
					    if (LODWORD(rax) != 0x0) {
							// loc_4a797;
						    _AMAuthInstallLog(0x3);
						    LODWORD(r13) = 0x0;
							
							// loc_4a7a6:
						    LODWORD(r14) = 0x0;
						    // loc_4a7a9;
						}
					    rax = _DEREncoderCreateEncodedBuffer(rbx, &var_160, &var_204);
					    if (LODWORD(rax) != 0x0) {
							// loc_4a797;
						    _AMAuthInstallLog(0x3);
						    LODWORD(r13) = 0x0;
							
							// loc_4a7a6:
						    LODWORD(r14) = 0x0;
						    // loc_4a7a9;
						}
					    // loc_4a393;
					    r13 = _DEREncoderCreate(0x0);
					    rax = _DEREncoderAddData(r13, 0x0, 0x10, var_160, var_204, 0x1);
					    if (LODWORD(rax) != 0x0) {
							// loc_4a87e;
						    _AMAuthInstallLog(0x3);
						    // loc_4a7a9;
						}
					    // loc_4a3c5;
					    rax = _DEREncoderCreateEncodedBuffer(r13, &var_152, &var_200);
					    if (LODWORD(rax) != 0x0) {
							// loc_4a87e;
						    _AMAuthInstallLog(0x3);
						    // loc_4a7a9;
						}
					    // loc_4a3dd;
					    var_16 = r12;
					    r15 = var_24;
					    rbx = CFDataCreate(CFGetAllocator(r15), var_152, var_200);
					    if (rbx != 0x0) {
							_AMAuthInstallDebugWriteObject(r15, rbx, @"ticket", 0x1);
							*arg_offset_x0 = CFRetain(rbx);
							var_60 = 0x0;
							r14 = rbx;
					    }
					    r12 = var_16;
					    // loc_4a7a9;
					}
				    // loc_4a469;
				    var_135 = 0x0;
				    r15 = _DEREncoderCreate(0x0);
				    if (r15 == 0x0) {
						// loc_4a699:
					    LODWORD(r13) = 0x0;
						
						// loc_4a756:
					    var_80 = 0x0;
					    LODWORD(r12) = 0x0;
					    // loc_4a764;
					}
				    var_8 = r13;
				    LODWORD(r13) = 0x0;
				    rbx = CFDataCreateMutable(**kCFAllocatorDefault, 0x0);
				    var_72 = rbx;
				    CFDataAppendBytes(rbx, &var_135, 0x1);
				    CFDataAppendBytes(rbx, CFDataGetBytePtr(r14), CFDataGetLength(r14));
				    rax = _DEREncoderAddData(r15, 0x0, 0x3, CFDataGetBytePtr(rbx), LODWORD(CFDataGetLength(rbx)), 0x0);
				    if (LODWORD(rax) != 0x0) {
						// loc_4a88f;
					    var_80 = r15;
					    _AMAuthInstallLog(0x3, "AMAuthInstallMonetLocalCreateSignedTicket", "failed to add BER bit string to DER file");
						
						// loc_4a8e6:
					    LODWORD(r12) = 0x0;
					    // loc_4a7a6;
					}
				    // loc_4a50a;
				    rax = _DEREncoderAddData(r15, 0x0, 0x4, CFDataGetBytePtr(var_16), LODWORD(CFDataGetLength(var_16)), 0x0);
				    if (LODWORD(rax) != 0x0) {
						// loc_4a8b2;
					    var_80 = r15;
						
						// loc_4a8d7:
					    _AMAuthInstallLog(0x3);
					    LODWORD(r13) = 0x0;
					    // loc_4a8e6;
					    LODWORD(r12) = 0x0;
					    // loc_4a7a6;
					}
				    // loc_4a544;
				    var_80 = r15;
				    rax = _DEREncoderCreateEncodedBuffer(r15, &var_144, &var_196);
				    if (LODWORD(rax) != 0x0) {
						// loc_4a8d7;
					    _AMAuthInstallLog(0x3);
					    LODWORD(r13) = 0x0;
					    // loc_4a8e6;
					    LODWORD(r12) = 0x0;
					    // loc_4a7a6;
					}
				    // loc_4a563;
				    r12 = _DEREncoderCreate(0x0);
				    if (r12 == 0x0) {
						// loc_4a8ee;
					    LODWORD(r13) = 0x0;
					    r12 = r13;
					    // loc_4a7a6;
					}
				    // loc_4a579;
				    rax = _DEREncoderAddData(r12, 0x0, 0x10, var_144, var_196, 0x1);
				    if (LODWORD(rax) != 0x0) {
						// loc_4a797;
					    _AMAuthInstallLog(0x3);
					    LODWORD(r13) = 0x0;
						
						// loc_4a7a6:
					    LODWORD(r14) = 0x0;
					    // loc_4a7a9;
					}
				    rax = _DEREncoderCreateEncodedBuffer(r12, &var_136, &var_192);
				    r13 = var_8;
				    if (LODWORD(rax) != 0x0) {
						// loc_4a797;
					    _AMAuthInstallLog(0x3);
					    LODWORD(r13) = 0x0;
						
						// loc_4a7a6:
					    LODWORD(r14) = 0x0;
					    // loc_4a7a9;
					}
				    rax = _DEREncoderAddData(var_64, 0x2, 0x0, var_136, var_192, 0x1);
				    if (LODWORD(rax) != 0x0) {
						// loc_4a91c;
					    _AMAuthInstallLog(0x3);
					    LODWORD(r13) = 0x0;
					    var_60 = 0x2;
					    // loc_4a7a6;
					}
				    // loc_4a5e9;
				    rax = _DEREncoderAddData(var_48, 0x2, 0x0, var_136, var_192, 0x1);
				    if (LODWORD(rax) == 0x0) {
						// loc_4a1cb;
					}
				    // loc_4a92a;
				    _AMAuthInstallLog(0x3);
				    LODWORD(r13) = 0x0;
				    var_60 = 0x2;
				    // loc_4a7a6;
				}
			    // loc_4a1bd;
			    LODWORD(r12) = 0x0;
			    var_72 = 0x0;
			    // loc_4a1cb;
			}
		    // loc_4a19c;
		    var_80 = 0x0;
		    // loc_4a1bd;
		    LODWORD(r12) = 0x0;
		    var_72 = 0x0;
		    // loc_4a1cb;
		}
	    // loc_4a13e;
	    rax = _DEREncoderAddData(rbx, 0x0, 0x2, &var_215, 0x1, 0x0);
	    if (LODWORD(rax) != 0x0) {
			// loc_4a728;
		    var_64 = rbx;
		    _AMAuthInstallLog(0x3, "AMAuthInstallMonetLocalCreateSignedTicket", "failed to add version to DER file");
		    LODWORD(r13) = 0x0;
		    var_60 = 0x2;
			// loc_4a756:
		    var_80 = 0x0;
		    LODWORD(r12) = 0x0;
		    // loc_4a764;
		}
	    // loc_4a162;
	}
	
	// loc_4a625:
    var_48 = 0x0;
    var_32 = rax;
	
	// loc_4a657:
    var_64 = 0x0;
    LODWORD(r13) = 0x0;
    var_80 = 0x0;
    LODWORD(r12) = 0x0;
    var_40 = 0x0;
	
	// loc_4a764:
    LODWORD(r14) = 0x0;
    var_72 = 0x0;
	
	// loc_4a7a9:
    _DEREncoderDestroy(var_48);
    _DEREncoderDestroy(var_64);
    _DEREncoderDestroy(r13);
    _DEREncoderDestroy(var_80);
    _DEREncoderDestroy(r12);
    _SafeRelease(var_184);
    _SafeRelease(var_32);
    _SafeRelease(var_40);
    _SafeRelease(var_176);
    _SafeRelease(r14);
    _SafeRelease(var_72);
    _SafeFree(var_168);
    _SafeFree(var_160);
    _SafeFree(var_152);
    _SafeFree(var_144);
    _SafeFree(var_136);
    LODWORD(rax) = var_60;
    return rax;
}

function _AMAuthInstallMonetLocalCreateProvisioningTicket {
    var_16 = rsi;
    r14 = rdi;
    var_140 = 0x0;
    var_136 = 0x0;
    var_132 = 0x0;
    var_120 = 0x0;
    var_112 = 0x0;
    var_104 = 0x0;
    var_96 = 0x0;
    var_88 = 0x0;
    var_80 = 0x0;
    var_72 = 0x0;
    var_64 = 0x0;
    LODWORD(rbx) = 0x0;
    LODWORD(r12) = 0x0;
    LODWORD(r13) = 0x0;
    if (r14 != 0x0) {
		// loc_4aa00
	    rax = *(r14 + 0x20);
	    LODWORD(rbx) = 0x0;
	    LODWORD(r12) = 0x0;
	    LODWORD(r13) = 0x0;
	    if (rax == 0x0) {
			// loc_4aa61;
		    // exit
		}
		else {
		    // loc_4aa21;
		    LODWORD(rbx) = 0x0;
		    LODWORD(r12) = 0x0;
		    LODWORD(r13) = 0x0;
		    if (*(rax + 0x20) == 0x0) {
				// loc_4aa61;
			}
		    if (*(rax + 0x28) != 0x0) {
				// loc_4aa39;
			    var_40 = *(r14 + 0xb8);
			    LODWORD(rbx) = 0x0;
			    LODWORD(r12) = 0x0;
			    LODWORD(r13) = 0x0;
			    if (*(r14 + 0xb8) != 0x0) {
					// loc_4ab46;
				    var_8 = *(r14 + 0xc0);
				    LODWORD(rbx) = 0x0;
				    LODWORD(r12) = 0x0;
				    LODWORD(r13) = 0x0;
				    if (*(r14 + 0xc0) == 0x0) {
						// loc_4aa61;
						// exit
					}
				    var_32 = LOBYTE(*(int8_t *)(r14 + 0xf0));
				    var_24 = *(r14 + 0xe8);
				    var_56 = *(r14 + 0xe0);
				    var_48 = *(r14 + 0xd8);
				    var_39 = LOBYTE(*(int8_t *)(r14 + 0xd0));
				    r15 = _DEREncoderCreate(0x1);
				    LODWORD(rbx) = 0x0;
				    LODWORD(r12) = 0x0;
				    LODWORD(r13) = 0x0;
				    if (r15 == 0x0) {
						// loc_4aa61;
						// exit
					}
				    rax = _DEREncoderAddData(r15, 0x2, 0x3f, CFDataGetBytePtr(*(*(r14 + 0x20) + 0x10)), LODWORD(CFDataGetLength(*(*(r14 + 0x20) + 0x10))), 0x0);
				    r13 = r15;
				    if (LODWORD(rax) != 0x0) {
						// loc_4afec;
					    _AMAuthInstallLog(0x3);
					    rbx = r13;
					    LODWORD(r12) = 0x0;
					    LODWORD(r13) = 0x0;
					    // loc_4aa61;
					}
				    // loc_4ac1c;
				    rax = _DEREncoderAddInt32AsData(r13, 0x2, 0x40, *(int32_t *)(*(r14 + 0x20) + 0x4));
				    if (LODWORD(rax) != 0x0) {
						// loc_4afec;
					    _AMAuthInstallLog(0x3);
					    rbx = r13;
					    LODWORD(r12) = 0x0;
					    LODWORD(r13) = 0x0;
					    // loc_4aa61;
					}
				    rax = _DEREncoderAddData(r13, 0x2, 0x4b, CFDataGetBytePtr(var_8), LODWORD(CFDataGetLength(var_8)), 0x0);
				    if (LODWORD(rax) != 0x0) {
						// loc_4afec;
					    _AMAuthInstallLog(0x3);
					    rbx = r13;
					    LODWORD(r12) = 0x0;
					    LODWORD(r13) = 0x0;
					    // loc_4aa61;
				    }
				    r15 = r14;
				    rbx = var_40;
				    // loc_4ac96;
				    rax = _DEREncoderAddData(r13, 0x2, 0x3ec, CFDataGetBytePtr(rbx), LODWORD(CFDataGetLength(rbx)), 0x0);
				    if (LODWORD(rax) != 0x0) {
						// loc_4afec;
					    _AMAuthInstallLog(0x3);
					    rbx = r13;
					    LODWORD(r12) = 0x0;
					    LODWORD(r13) = 0x0;
					    // loc_4aa61;
				    }
				    rdi = *(r15 + 0xc8);
				    if (rdi == 0x0) {
						// loc_4ad0f;
					}
				    rax = _DEREncoderAddData(r13, 0x2, 0x3ea, CFDataGetBytePtr(rdi), LODWORD(CFDataGetLength(*(r15 + 0xc8))), 0x0);
				    if (LODWORD(rax) != 0x0) {
						// loc_4afec;
					    _AMAuthInstallLog(0x3);
					    rbx = r13;
					    LODWORD(r12) = 0x0;
					    LODWORD(r13) = 0x0;
					    // loc_4aa61;
				    }
				    if ((var_39 != 0x0) && (var_32 != 0x1)) {
						// loc_4ad0f;
					    // loc_4ad7f:
						r14 = CFDataGetBytePtr(rdi);
						rax = CFDataGetLength(var_96);
						var_32 = 0x0;
						rax = _DEREncoderAddData(r13, 0x2, 0x3eb, r14, LODWORD(rax), 0x0);
						if (LODWORD(rax) != 0x0) {
							// loc_4afb4;
						    _AMAuthInstallLog(0x3);
						    LODWORD(r12) = 0x0;
						    rbx = r13;
						    LODWORD(r13) = 0x0;
						    // loc_4aa61;
						}
					    rax = *(r15 + 0x20);
					    var_32 = 0x63;
					    if (*(rax + 0x20) == 0x0) {
							// loc_4adc4;
						    // loc_4adcb;
						    rax = _AMAuthInstallMonetCreateMEIDFromString(CFGetAllocator(r15), *(*(r15 + 0x20) + 0x28), &var_88, *(r15 + 0x20));
						    if (LODWORD(rax) != 0x0) {
								// loc_4af7e;
							    var_56 = LODWORD(rax);
							    // loc_4b2b8;
							    LODWORD(r15) = 0x0;
								
								// loc_4b2ca:
							    rbx = r13;
							    LODWORD(r12) = 0x0;
								
								// loc_4b2e6:
							    r14 = r15;
							    LODWORD(r13) = 0x0;
							    // loc_4aa8b;
							}
						    // loc_4adf2;
						    rdi = var_88;
						    if (rdi == 0x0) {
								// loc_4af89;
							    var_56 = 0x3;
							    // loc_4b2ac;
							    _AMAuthInstallLog(0x3);
								
								// loc_4b2b8:
							    LODWORD(r15) = 0x0;
								
								// loc_4b2ca:
							    rbx = r13;
							    LODWORD(r12) = 0x0;
								
								// loc_4b2e6:
							    r14 = r15;
							    LODWORD(r13) = 0x0;
							    // loc_4aa8b;
							}
							// loc_4ae02:
						    r14 = CFDataGetBytePtr(rdi);
						    rax = CFDataGetLength(var_88);
						    var_32 = 0x0;
						    rax = _DEREncoderAddData(r13, 0x2, 0x3ed, r14, LODWORD(rax), 0x0);
						    if (LODWORD(rax) != 0x0) {
								// loc_4afa6;
							    _AMAuthInstallLog(0x3);
							    LODWORD(r12) = 0x0;
							    rbx = r13;
							    LODWORD(r13) = 0x0;
							    // loc_4aa61;
							}
						    // loc_4ae43;
						    if (*(rax + 0x28) == 0x0) {
								// loc_4ae43;
							    rdi = var_48;
							    if (rdi == 0x0) {
									// loc_4ae94;
								    rax = _DEREncoderCreateEncodedBuffer(r13, &var_120, &var_140);
								    if (LODWORD(rax) != 0x0) {
										// loc_4afec;
									    _AMAuthInstallLog(0x3);
									    rbx = r13;
									    LODWORD(r12) = 0x0;
									    LODWORD(r13) = 0x0;
									    // loc_4aa61;
									}
								    // loc_4aeb2;
								    if (var_39 == 0x0) {
										// loc_4b006;
									    var_48 = r13;
									    rcx = **kCFAllocatorNull;
									    rbx = **kCFAllocatorDefault;
									    LODWORD(r13) = 0x0;
									    var_40 = 0x0;
									    LODWORD(r12) = 0x0;
									    LODWORD(r14) = var_32;
										
										// loc_4b22f:
									    rax = CFDataCreateWithBytesNoCopy(rbx, var_120, var_140, rcx);
									    if (LODWORD(r14) != 0x0) {
											// loc_4b352;
										    r15 = rax;
										    _AMAuthInstallLog(0x3, "AMAuthInstallMonetLocalCreateProvisioningTicket", "allocate CFData buffer\\n");
											
											// loc_4b36f:
										    rbx = var_48;
										    var_56 = LODWORD(r14);
										    r14 = var_40;
										    // loc_4aa8b;
										}
									    else {
											// loc_4b24d:
											rdi = r15;
											r15 = rax;
											var_56 = LODWORD(_AMAuthInstallMonetLocalCreateSignedTicket(rdi, @"bb.monet.ticket.provisioning", var_39 & 0xff, rax, var_72, var_64));
											
											// loc_4b33f:
											rbx = var_48;
											r14 = var_40;
											// loc_4aa8b;
									    }
									}
								    // loc_4aebf:
								    LODWORD(r12) = 0x0;
								    if (var_56 != 0x0) {
										// loc_4b03e;
									    var_56 = 0x1;
									    if (var_24 == 0x0) {
											// loc_4b28d;
										    LODWORD(r12) = 0x0;
										    rbx = r13;
										    r15 = r12;
											
											// loc_4b293:
										    r14 = r12;
										    LODWORD(r13) = 0x0;
										    // loc_4aa8b;
										}
									    // loc_4b056;
									    if (*(*(r15 + 0x20) + 0x38) == 0x0) {
											// loc_4b29e;
										    _AMAuthInstallLog(0x3);
											
											// loc_4b2b8:
										    LODWORD(r15) = 0x0;
											
											// loc_4b2ca:
										    rbx = r13;
										    LODWORD(r12) = 0x0;
											
											// loc_4b2e6:
										    r14 = r15;
										    LODWORD(r13) = 0x0;
										    // loc_4aa8b;
										}
									    // loc_4b065;
									    r12 = _DEREncoderCreate(0x1);
									    if (r12 == 0x0) {
									    	// loc_4b2bd;
										    LODWORD(r15) = 0x0;
										    var_56 = 0x2;
										    // loc_4b2ca;
									    }
									    // loc_4b07b;
									    var_48 = r13;
									    rax = _DEREncoderAddData(r12, 0x2, 0x3ec, CFDataGetBytePtr(var_40), LODWORD(CFDataGetLength(var_40)), 0x0);
									    LODWORD(r13) = 0x0;
									    if (LODWORD(rax) != 0x0) {
											// loc_4b2f1;
										    rbx = var_48;
										    // loc_4aa61;
										}
									    // loc_4b0c2;
									    rdi = *(r15 + 0xc8);
									    if (rdi == 0x0) {
											// loc_4b105;
										    rax = _DEREncoderCreateEncodedBuffer(r12, &var_112, &var_136);
										    if (LODWORD(rax) != 0x0) goto loc_4b2f1;
										    r13 = _DEREncoderCreate(0x0);
										    LODWORD(r14) = var_32;
										    if (r13 == 0x0) {
												// loc_4b2d2;
											    LODWORD(r15) = 0x0;
											    var_56 = 0x2;
											    rbx = var_48;
											    // loc_4b2e6;
											}
										    // loc_4b13d;
										    rax = _DEREncoderAddData(r13, 0x0, 0x11, var_112, var_136, 0x1);
										    if (LODWORD(rax) != 0x0) goto loc_4b2f1;
										    rax = _DEREncoderCreateEncodedBuffer(r13, &var_104, &var_132);
										    if (LODWORD(rax) != 0x0) goto loc_4b2f1;
										    rbx = **kCFAllocatorDefault;
										    var_56 = **kCFAllocatorNull;
										    var_40 = CFDataCreateWithBytesNoCopy(rbx, var_104, var_132, **kCFAllocatorNull);
										    if (LODWORD(r14) != 0x0) {
												// loc_4b2fd;
											    _AMAuthInstallLog(0x3, "AMAuthInstallMonetLocalCreateProvisioningTicket", "allocate CFData contents to encrypt");
											    LODWORD(r15) = 0x0;
											    // loc_4b36f;
											}
										    // loc_4b1c9;
										    rax = _AMAuthInstallCryptoDecryptSessionKey(r15, @"bb.monet.ticket.provisioning", *(*(r15 + 0x20) + 0x38), &var_80);
										    if (LODWORD(rax) != 0x0) {
												// loc_4b31c;
											    var_56 = LODWORD(rax);
											    _AMAuthInstallLog(0x3, "AMAuthInstallMonetLocalCreateProvisioningTicket", "failed to decrypt session key\\n");
											    LODWORD(r15) = 0x0;
											    // loc_4b33f;
											}
										    // loc_4b1ef;
										    LODWORD(r14) = LODWORD(_AMAuthInstallCryptoEncryptWithWrappedKeyAndIVPrependIV(r15, var_80, *(r15 + 0xe0), var_24, var_40, &var_72));
										    rcx = var_56;
										    // loc_4b22f;
										}
									    // loc_4b0ce;
									    rax = _DEREncoderAddData(r12, 0x2, 0x3ea, CFDataGetBytePtr(rdi), LODWORD(CFDataGetLength(*(r15 + 0xc8))), 0x0);
									    if (LODWORD(rax) != 0x0) {
											// loc_4b2f1;
										}
									    // loc_4b105;
									}
								    // loc_4aed0;
								    var_56 = 0x1;
									
									// loc_4af46:
								    rbx = r13;
								    LODWORD(r15) = 0x0;
								    // loc_4b293;
								    r14 = r12;
								    LODWORD(r13) = 0x0;
								    // loc_4aa8b;
								}
							    // loc_4ae4f;
							    CFStringGetCString(rdi, &var_144, 0x100, 0x8000100);
							    rax = _DEREncoderAddData(r13, 0x2, 0x3ee, &var_144, strlen(&var_144) + 0x1, 0x0);
							    if (LODWORD(rax) != 0x0) {
									// loc_4afec;
								}
							    // loc_4ae94;
							}
						}
					    // loc_4ad28;
					    rbx = r13;
					    CFGetAllocator(r15);
					    if (var_39 != 0x0) {
							rax = _AMAuthInstallMonetCreateMEIDFromString();
					    }
					    else {
							rax = _AMAuthInstallMonetCreateReversedMEIDFromString();
					    }
					    r13 = rbx;
					    LODWORD(r12) = 0x0;
					    if (LODWORD(rax) != 0x0) {
							// loc_4af40;
						    var_56 = LODWORD(rax);
						    // loc_4af46;
						    rbx = r13;
						    LODWORD(r15) = 0x0;
						    // loc_4b293;
						    r14 = r12;
						    LODWORD(r13) = 0x0;
						    // loc_4aa8b;
						}
					    // loc_4ad6f;
					    rdi = var_96;
					    if (rdi == 0x0) {
						    var_56 = 0x3;
						    // loc_4b2ac;
						    _AMAuthInstallLog(0x3);
							
							// loc_4b2b8:
						    LODWORD(r15) = 0x0;
							
							// loc_4b2ca:
						    rbx = r13;
						    LODWORD(r12) = 0x0;
							
							// loc_4b2e6:
						    r14 = r15;
						    LODWORD(r13) = 0x0;
						    // loc_4aa8b;
					    }
					    
					}
				}
			    // loc_4aa61;
			}
		}
	}
	else {
		// loc_4aa61
	}
	
	if (loc_4aa61) {
		// loc_4aa61;
	    var_56 = 0x3;
	    _AMAuthInstallLog(0x3, "AMAuthInstallMonetLocalCreateProvisioningTicket", "AMAuthInstallMonetLocalCreateProvisioningTicket failed to create DER file");
	    LODWORD(r15) = 0x0;
	    r14 = r15;
	}
	
	// loc_4aa8b:
    _SafeFree(var_120);
    _DEREncoderDestroy(rbx);
    _SafeFree(var_112);
    _DEREncoderDestroy(r12);
    _SafeFree(var_104);
    _DEREncoderDestroy(r13);
    _SafeRelease(var_96);
    _SafeRelease(var_88);
    _SafeRelease(var_64);
    _SafeRelease(r14);
    _SafeRelease(0x0);
    _SafeRelease(var_80);
    _SafeRelease(var_72);
    _SafeRelease(r15);
    return rax;
}

function _AMAuthInstallMonetLocalHandleRequest {
    var_24 = rdx;
    r12 = rsi;
    r15 = rdi;
    var_200 = 0x0;
    var_192 = 0x0;
    var_184 = 0x0;
    var_176 = 0x0;
    var_168 = 0x0;
    var_160 = 0x0;
    var_152 = 0x0;
    var_144 = 0x0;
    var_136 = 0x0;
    rax = CFGetAllocator(rdi);
    rax = CFDictionaryCreateMutable(rax, 0x0, *kCFTypeDictionaryKeyCallBacks, *kCFTypeDictionaryValueCallBacks);
    var_32 = rax;
    if (rax != 0x0) {
    	// loc_46ce3
		_AMAuthInstallMonetLocalRegisterKeys(r15);
		
	    rbx = CFDictionaryGetValue(r12, *_kAMAuthInstallTagBbSnum);
	    if ((rbx != 0x0) && (CFDataGetTypeID() == CFGetTypeID(rbx))) {
			// loc_46d22;
		    rbx = CFDictionaryGetValue(r12, *_kAMAuthInstallTagBbChipId);
		    if ((rbx != 0x0) && (CFNumberGetTypeID() == CFGetTypeID(rbx))) {
				// loc_46d59;
			    rax = CFNumberGetValue(rbx, 0x3, &var_212);
			    if (LOBYTE(rax) == 0x0) {
					// loc_46f8b;
				    _AMAuthInstallLog(0x3, "AMAuthInstallMonetLocalHandleRequest", "failed to read chipid");
					
					// loc_46fa5:
				    LODWORD(rbx) = 0x0;
				    var_128 = 0x1;
					
					// loc_46fb1:
				    var_96 = rbx;
				    var_112 = 0x0;
				    var_104 = 0x0;
				    var_120 = 0x0;
				    var_80 = rbx;
				    r15 = rbx;
				    var_88 = rbx;
				    // loc_4705b;
				}
			    // loc_46d75;
			    rbx = CFDictionaryGetValue(r12, *_kAMAuthInstallTagBbCertId);
			    if ((rbx != 0x0) && (CFNumberGetTypeID() == CFGetTypeID(rbx))) {
					// loc_46dac;
				    var_88 = *_kAMAuthInstallTagBbFirmware;
				    r14 = CFDictionaryGetValue(r12, *_kAMAuthInstallTagBbFirmware);
				    if ((r14 != 0x0) && (CFDictionaryGetTypeID() == CFGetTypeID(r14))) {
						// loc_46dea;
					    var_120 = r12;
					    rbx = CFDictionaryGetValue(r14, *_kAMAuthInstallTagBbEnandprogPartialDigest);
					    if ((rbx != 0x0) && (CFDataGetTypeID() != CFGetTypeID(rbx))) goto loc_46fa5;
					    var_96 = rbx;
					    r12 = r15;
					    r15 = CFDictionaryGetValue(r14, *_kAMAuthInstallTagBbDblPartialDigests);
					    if ((r15 != 0x0) && (CFDataGetTypeID() != CFGetTypeID(r15))) goto loc_46fa5;
					    rbx = CFDictionaryGetValue(r14, *_kAMAuthInstallTagBbEDblPartialDigest);
					    if ((rbx != 0x0) && (CFDataGetTypeID() != CFGetTypeID(rbx))) goto loc_46fa5;
					    var_104 = rbx;
					    var_112 = CFDictionaryGetValue(r14, *_kAMAuthInstallTagBbRestoreDblPartialDigest);
					    if ((r15 != 0x0) && (CFDataGetTypeID() != CFGetTypeID(r15))) goto loc_46fa5;
					    r13 = CFDictionaryGetValue(r14, *_kAMAuthInstallTagBbSbl1PartialDigest);
					    if ((r13 != 0x0) && (CFDataGetTypeID() != CFGetTypeID(r13))) goto loc_46fa5;
					    r14 = CFDictionaryGetValue(r14, *_kAMAuthInstallTagBbRestoreSbl1PartialDigest);
					    if ((r14 != 0x0) && (CFDataGetTypeID() != CFGetTypeID(r14))) goto loc_46fa5;
					    LODWORD(rcx) = var_212;
					    rax = var_96;
					    rbx = var_88;
					    // loc_46f54;
					    if (LODWORD(rcx) != 0x4600e1) {
							// loc_471d6;
						}
					    // loc_4718b;
					    if (LODWORD(rcx) > 0x5000e0) {
							// loc_4717b;
							
						    // loc_47183;
						    if (LODWORD(rcx) != 0x5000e1) {
								// loc_471d6;
							}
							// loc_4718b:
						    rax = rax | r15;
						    if (CPU_FLAGS & NE) {
								// loc_47209;
							}
						    // loc_478a8;
						    if (LODWORD(rcx) > 0x5a00e0) {
								// loc_471a3;
								
							    if ((LODWORD(rcx) != 0x6f10e1) && (LODWORD(rcx) != 0x6f20e1)) {
									// loc_471d6;
								    _AMAuthInstallLog(0x3, "AMAuthInstallMonetLocalHandleRequest", "unrecognized chipid: 0x%08X");
									
									// loc_47086:
								    LODWORD(rbx) = 0x0;
								    var_128 = 0x1;
								    var_96 = 0x0;
								    var_112 = 0x0;
								    var_104 = 0x0;
								    var_120 = 0x0;
								    var_80 = 0x0;
								    LODWORD(r15) = 0x0;
								    var_88 = 0x0;
								    LODWORD(r14) = 0x0;
								    LODWORD(r13) = 0x0;
								    LODWORD(r12) = 0x0;
								    // loc_470e2;
								}
							    // loc_471bb;
							    rax = r13 | r14;
							    if (CPU_FLAGS & NE) {
									// loc_47209;
								}
							    // loc_478a8;
							    if (LODWORD(rcx) == 0x5a00e1) {
									// loc_471f5;
								    rax = var_104 | var_112;
								    if (CPU_FLAGS & E) {
										// loc_4789a;
									    _AMAuthInstallLog(0x3);
									    LODWORD(rbx) = 0x0;
									    var_128 = 0x8;
									    goto loc_46fb1;
									}
								    // loc_47209;
								    var_8 = r14;
								    rax = CFPropertyListCreateDeepCopy(CFGetAllocator(r12), var_120, 0x2);
								    if (rax == 0x0) {
										// loc_477ea;
									    LODWORD(rbx) = 0x0;
									    var_96 = rbx;
									    var_112 = 0x0;
									    var_104 = 0x0;
									    var_120 = 0x0;
									    var_80 = rbx;
									    r15 = rbx;
									    var_88 = rbx;
									    LODWORD(r14) = 0x0;
									    r13 = rbx;
									    r12 = rax;
									    var_128 = 0x2;
									    goto loc_470e2;
									}
								    // loc_47235;
								    var_0 = r13;
								    var_64 = rax;
								    var_128 = CFDictionaryGetValue(rax, rbx);
								    rax = CFGetAllocator(r12);
								    r14 = rbx;
								    rax = CFDictionaryCreateMutable(rax, 0x0, *kCFTypeDictionaryKeyCallBacks, *kCFTypeDictionaryValueCallBacks);
								    if (rax == 0x0) {
										// loc_47840;
									    var_96 = 0x0;
									    var_112 = 0x0;
									    var_104 = 0x0;
									    var_120 = 0x0;
									    var_80 = 0x0;
									    r15 = 0x0;
									    var_88 = 0x0;
									    LODWORD(rbx) = 0x0;
									    LODWORD(r14) = 0x0;
									    r13 = rbx;
									    r12 = var_64;
									    var_128 = 0x2;
									    // loc_470e2;
									}
								    // loc_47283;
								    r13 = r12;
								    var_72 = rax;
								    CFDictionarySetValue(var_32, r14, rax);
								    r12 = 0x0;
								    LODWORD(rbx) = 0x0;
								    rbx = r13;
								    rax = __AMAuthInstallMonetLocalCreateStitchData(rbx, 0x1, r15, &var_200);
								    if (LODWORD(rax) != 0x0) {
										// loc_478c5;
									    LODWORD(rbx) = 0x0;
									    r15 = var_200;
									    var_96 = rbx;
									    var_112 = 0x0;
									    var_104 = 0x0;
									    var_120 = 0x0;
									    var_80 = rbx;
									    var_88 = rbx;
									    LODWORD(r14) = 0x0;
									    r13 = rbx;
									    r12 = var_64;
									    var_128 = LODWORD(rax);
									    // loc_470e2;
									}
								    // loc_472d4;
								    r12 = var_200;
								    CFDictionarySetValue(var_72, *_kAMAuthInstallTagBbDblStitch, r12);
								    var_16 = rbx;
								    r13 = CFDataCreateMutable(CFGetAllocator(rbx), 0x14);
								    if (r13 != 0x0) {
										CFDataSetLength(r13, 0x14);
										rbx = CFDataGetBytePtr(r15);
										CC_SHA1_Init(&var_216);
										asm{ bswap      eax };
										var_216 = LODWORD(*(int32_t *)(rbx + 0x20));
										asm{ bswap      eax };
										var_220 = LODWORD(*(int32_t *)(rbx + 0x24));
										asm{ bswap      eax };
										var_224 = LODWORD(*(int32_t *)(rbx + 0x28));
										asm{ bswap      eax };
										var_228 = LODWORD(*(int32_t *)(rbx + 0x2c));
										asm{ bswap      eax };
										var_232 = LODWORD(*(int32_t *)(rbx + 0x30));
										var_236 = LODWORD(LODWORD(*(int32_t *)(rbx + 0x1c)) << 0x3);
										r12 = r12;
										var_240 = 0x0;
										CC_SHA1_Update(&var_216, CFDataGetBytePtr(r12), LODWORD(CFDataGetLength(r12)));
										CC_SHA1_Final(CFDataGetMutableBytePtr(r13), &var_216);
										rbx = CFRetain(r13);
										LODWORD(r15) = 0x0;
								    }
								    else {
										LODWORD(r15) = 0x2;
										LODWORD(rbx) = 0x0;
										LODWORD(r13) = 0x0;
								    }
								    _SafeRelease(r13);
								    CFDictionarySetValue(var_128, *_kAMAuthInstallTagBbDblDownloadDigest, rbx);
								    rcx = CFDataGetLength(r12);
								    _AMAuthInstallLog(0x7, "AMAuthInstallMonetLocalHandleRequest", "dblStitchData size=0x%x", rcx);
								    r13 = var_16;
								    if (LODWORD(r15) != 0x0) {
										// loc_47933;
									    var_96 = 0x0;
									    var_112 = 0x0;
									    var_104 = 0x0;
									    var_120 = 0x0;
									    r13 = rbx;
									    var_80 = 0x0;
									    var_88 = 0x0;
									    var_128 = LODWORD(r15);
									    r15 = r12;
									    LODWORD(rbx) = 0x0;
									    r14 = rbx;
									    r12 = var_64;
									    // loc_470e2;
									}
								    if (r15 == 0x0) {
										// loc_47421;
									    var_56 = rbx;
									    LODWORD(rbx) = 0x0;
									    rbx = r13;
									    rax = __AMAuthInstallMonetLocalCreateStitchData(rbx, 0x2);
									    if (LODWORD(rax) != 0x0) {
											// loc_4798c;
										    LODWORD(rbx) = 0x0;
										    var_80 = var_184;
										    var_96 = rbx;
										    var_112 = 0x0;
										    var_104 = 0x0;
										    var_120 = 0x0;
										    var_88 = rbx;
										    var_128 = LODWORD(rax);
										    r15 = r12;
										    LODWORD(r14) = 0x0;
										    r12 = var_64;
										    r13 = var_56;
										    // loc_470e2;
										}
									    // loc_47458;
									    r13 = rbx;
									    rbx = var_184;
									    CFDictionarySetValue(var_72, *_kAMAuthInstallTagBbENandprogStitch, rbx);
									    rcx = CFDataGetLength(rbx);
									    _AMAuthInstallLog(0x7, "AMAuthInstallMonetLocalHandleRequest", "nandprogStitchData size=0x%x", rcx);
									    if (var_96 == 0x0) {
											// loc_474a0;
										    var_48 = r12;
										    var_80 = rbx;
										    LODWORD(rbx) = 0x0;
										    r15 = var_104;
										    LODWORD(r12) = 0x0;
										    var_88 = 0x0;
										    rbx = r13;
										    rax = __AMAuthInstallMonetLocalCreateStitchData(rbx, 0x2, r15, &var_176);
										    if (LODWORD(rax) != 0x0) {
												// loc_479f1;
											    LODWORD(rbx) = 0x0;
											    var_96 = var_176;
											    var_112 = 0x0;
											    var_104 = 0x0;
											    var_120 = 0x0;
											    var_88 = rbx;
											    LODWORD(r14) = 0x0;
											    goto loc_47a86;
											}
										    // loc_474f4;
										    r12 = var_176;
										    CFDictionarySetValue(var_72, *_kAMAuthInstallTagBbEDblStitch, r12);
										    rcx = CFDataGetLength(r12);
										    _AMAuthInstallLog(0x7, "AMAuthInstallMonetLocalHandleRequest", "eDblStitchData size=0x%x", rcx);
										    r13 = rbx;
										    var_88 = var_168;
										    CFDictionarySetValue(var_128, *_kAMAuthInstallTagBbEDblDownloadDigest, var_168);
										    LODWORD(rax) = LODWORD(__AMAuthInstallMonetLocalCreateEDBLDownloadDigestData(rbx, r15, r12, &var_168));
										    if ((LODWORD(__AMAuthInstallMonetLocalCreateEDBLDownloadDigestData(rbx, r15, r12, &var_168)) & LODWORD(__AMAuthInstallMonetLocalCreateEDBLDownloadDigestData(rbx, r15, r12, &var_168))) != 0x0) {
												// loc_47a2e;
											    LODWORD(rbx) = 0x0;
											    var_112 = 0x0;
											    var_104 = 0x0;
											    var_120 = 0x0;
											    LODWORD(r14) = 0x0;
											    goto loc_47a7f;
											}
										    // loc_47581;
										    LODWORD(rbx) = 0x0;
										    if (r15 == 0x0) {
												// loc_47583;
											    r15 = r13;
											    rax = __AMAuthInstallMonetLocalCreateStitchData(r15, 0x1, rbx, &var_152);
											    if (LODWORD(rax) != 0x0) {
													// loc_47aad;
												    LODWORD(r14) = 0x0;
												    var_104 = var_152;
												    var_120 = 0x0;
												    var_112 = 0x0;
												    // loc_47b17;
												}
												// loc_47645:
											    var_104 = var_152;
											    CFDictionarySetValue(var_72, *_kAMAuthInstallTagBbSbl1Stitch, var_152);
											    rdi = r15;
											    r15 = r15;
											    r14 = var_192;
											    CFDictionarySetValue(var_128, *_kAMAuthInstallTagBbSbl1DownloadDigest, var_192);
											    rcx = CFDataGetLength(var_152);
											    _AMAuthInstallLog(0x7, "AMAuthInstallMonetLocalHandleRequest", "sbl1StitchData size=0x%x", rcx);
											    LODWORD(rax) = LODWORD(__AMAuthInstallMonetLocalCreateEDBLDownloadDigestData(rdi, rbx, var_152, &var_192));
											    if ((LODWORD(__AMAuthInstallMonetLocalCreateEDBLDownloadDigestData(rdi, rbx, var_152, &var_192)) & LODWORD(__AMAuthInstallMonetLocalCreateEDBLDownloadDigestData(rdi, rbx, var_152, &var_192))) != 0x0) {
													// loc_47ad6;
												    var_112 = 0x0;
												    var_120 = 0x0;
												    // loc_47b17;
												}
											    // loc_476de;
											    rax = __AMAuthInstallMonetLocalCreateStitchData(r15, 0x2);
											    if (LODWORD(rax) != 0x0) {
													// loc_47a56;
												    rbx = var_160;
												    var_104 = 0x0;
												    var_120 = 0x0;
												    r14 = 0x0;
												    var_112 = 0x0;
													
													// loc_47a7f:
												    var_96 = r12;
													
													// loc_47a86:
												    r15 = var_48;
												    r12 = var_64;
												    var_128 = LODWORD(rax);
												    r13 = var_56;
												    // loc_470e2;
												}
											    // loc_475ae;
											    rbx = var_160;
											    CFDictionarySetValue(var_72, *_kAMAuthInstallTagBbRestoreDblStitch, rbx);
											    rcx = CFDataGetLength(rbx);
											    _AMAuthInstallLog(0x7, "AMAuthInstallMonetLocalHandleRequest", "restoreDblStitchData size=0x%x", rcx);
											    if (var_112 == 0x0) {
													// loc_475f3;
												    var_40 = rbx;
												    var_96 = r12;
												    LODWORD(r12) = 0x0;
												    rbx = var_0;
												    var_104 = 0x0;
												    LODWORD(r14) = 0x0;
												    if (rbx == 0x0) {
														// loc_476de;
													    rcx = CFDataGetLength(r12);
													    _AMAuthInstallLog(0x7, "AMAuthInstallMonetLocalHandleRequest", "restoreSbl1StitchData size=0x%x", rcx);
													    CFDictionarySetValue(var_72, *_kAMAuthInstallTagBbRestoreSbl1Stitch, r12);
													    if (var_8 == 0x0) {
															// loc_4774b;
														    var_112 = r12;
														    rax = CFDictionaryContainsKey(var_120, @"@BBTicket");
														    LODWORD(rbx) = 0x0;
														    if (LOBYTE(rax) == 0x0) {
																// loc_477a9;
															    var_120 = rbx;
															    *var_24 = CFRetain(var_32);
															    var_128 = 0x0;
															    r15 = var_48;
															    r12 = var_64;
																
																// loc_47b32:
															    r13 = var_56;
															    rbx = var_40;
															    // loc_470e2;
															}
														    // loc_4776b;
														    rax = _AMAuthInstallMonetLocalCreateRootTicket(r15, var_64, &var_136);
														    if (LODWORD(rax) != 0x0) {
																// loc_47b09;
															    var_120 = var_136;
																
																// loc_47b17:
															    r15 = var_48;
																
																// loc_47b1e:
															    r12 = var_64;
															    var_128 = LODWORD(rax);
															    // loc_47b32;
															    r13 = var_56;
															    rbx = var_40;
															    // loc_470e2;
															}
															// loc_47789:
														    rbx = var_136;
														    CFDictionarySetValue(var_32, *_kAMAuthInstallTagBbTicket, rbx);
														    // loc_477a9;
														}
													    // loc_476ea;
													    rax = __AMAuthInstallMonetLocalCreateStitchData(r15, 0x1);
													    r12 = var_144;
													    if (LODWORD(rax) != 0x0) {
															// loc_47aee;
														    var_120 = 0x0;
														    r15 = var_48;
														    var_112 = r12;
														    // loc_47b1e;
														    r12 = var_64;
														    var_128 = LODWORD(rax);
														    // loc_47b32;
														    r13 = var_56;
														    rbx = var_40;
														    // loc_470e2;
														}
													}
												}
											}
										}
									}
								}
							}
						    
						}
					}
				    //goto loc_4706c;
				    _AMAuthInstallLog(0x3);
				    // loc_47086;
				}
			    // loc_4707a;
			}
		}
	    // loc_4707a;
    }
	
	//loc_46ffc:
    _AMAuthInstallLog(0x3, "AMAuthInstallMonetLocalHandleRequest", "Failed to allocate responseDict");
    var_128 = 0x2;
    var_96 = 0x0;
    var_112 = 0x0;
    var_104 = 0x0;
    var_120 = 0x0;
    var_80 = 0x0;
    r15 = 0x0;
    var_88 = 0x0;
    LODWORD(rbx) = 0x0;
	
	// loc_4705b:
    LODWORD(r14) = 0x0;
    r13 = rbx;
    LODWORD(r12) = 0x0;
	
	// loc_470e2:
    _SafeRelease();
    _SafeRelease(r15);
    _SafeRelease(r13);
    _SafeRelease(var_80);
    _SafeRelease(var_120);
    _SafeRelease(r12);
    _SafeRelease(var_32);
    _SafeRelease(var_104);
    _SafeRelease(r14);
    _SafeRelease(var_112);
    _SafeRelease(var_96);
    _SafeRelease(rbx);
    _SafeRelease(var_88);
    LODWORD(rax) = var_128;
    return rax;
}


#endif

#endif