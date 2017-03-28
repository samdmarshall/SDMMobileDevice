//
//  SDMMD_AMAuthInstall.c
//  SDMMobileDevice-Framework
//
//  Created by Samantha Marshall on 4/22/14.
//
//

#ifndef _SDM_MD_AMAUTHINSTALL_C_
#define _SDM_MD_AMAUTHINSTALL_C_

#include "SDMMD_AMAuthInstall.h"
#include "Core.h"

void AMAuthInstallLogHandle(char *log_message) {
	printf("amai: %s\n",log_message);
}

Boolean AMAuthInstallBasebandPersonalizationEnabled(AMAuthInstallClassRef install) {
	return install->ivars.BasebandPersonalizationEnabled;
}
void AMAuthInstallBasebandEnabledPersonalization(AMAuthInstallClassRef install, Boolean enable) {
	install->ivars.BasebandPersonalizationEnabled = enable;
}

Boolean AMAuthInstallApPersonalizationEnabled(AMAuthInstallClassRef install) {
	return install->ivars.ApPersonalizationEnabled;
}

void AMAuthInstallEnableApPersonalization(AMAuthInstallClassRef install, Boolean enable) {
	install->ivars.ApPersonalizationEnabled = enable;
}

void AMAuthInstallBasebandSetVendorData(AMAuthInstallClassRef install, CFDataRef vendor_data) {
	CFDataRef vendor = install->ivars.VendorData;
	CFSafeRelease(vendor);
	install->ivars.VendorData = vendor_data;
}

void AMAuthInstallSetSigningServer(AMAuthInstallClassRef install, CFStringRef host, uint32_t port) {
	if (install) {
		CFStringRef signing_server = CFStringCreateWithFormat(kCFAllocatorDefault, NULL, CFSTR("http://%@:%u"), host, port);
		CFSafeRelease(install->ivars.signingServer);
		install->ivars.signingServer = signing_server;
	}
}

void AMAuthInstallSetSigningServerURL(AMAuthInstallClassRef install, CFStringRef url) {
	if (url) {
		CFSafeRelease(install->ivars.signingServer);
		install->ivars.signingServer = url;
	}
}

void AMAuthInstallSetFusingServerURL(AMAuthInstallClassRef install, CFStringRef url) {
	if (url) {
		CFSafeRelease(install->ivars.fusingServer);
		install->ivars.fusingServer = url;
	}
}
#if 0

uint32_t AMAuthInstallCryptoRegisterKeys(AMAuthInstallClassRef install) {
    CFDictionaryRef keys = install->ivars.keys;
    Boolean contains_monet_provisioning_key = CFDictionaryContainsKey(keys, CFSTR("A1.public"));
 	uint32_t result = 0;
	if (contains_monet_provisioning_key) {
		result = AMAuthInstallMonetLocalRegisterKeys(install);
		if (result != 0 && result != 13) {
			result = AMAuthInstallCryptoRegisterKeysFromPEMBuffer(keys, CFSTR("A1.public"), _kMonetTicketProvisioningA1PublicKey, 0x112);
			
			result = AMAuthInstallCryptoRegisterKeysFromPEMBuffer(keys, CFSTR("C1.public"), _kMonetTicketProvisioningC1PublicKey, 0x111);
			
			result = AMAuthInstallCryptoRegisterKeysFromPEMBuffer(keys, CFSTR("P1.public"), _kMonetTicketProvisioningP1PublicKey, 0x111);
			
			result = AMAuthInstallCryptoRegisterKeysFromPEMBuffer(keys, CFSTR("A2.public"), _kMonetTicketProvisioningA2PublicKey, 0x111);
			
			result = AMAuthInstallCryptoRegisterKeysFromPEMBuffer(keys, CFSTR("C2.public"), _kMonetTicketProvisioningC2PublicKey, 0x111);
			
			result = AMAuthInstallCryptoRegisterKeysFromPEMBuffer(keys, CFSTR("P2.public"), _kMonetTicketProvisioningP2PublicKey, 0x111);
		}
	}
	else {
		result = 0;
	}
	return result;
}

uint32_t AMAuthInstallCryptoRegisterKeysFromPEMBuffer(CFDictionaryRef dict, CFStringRef key, CFTypeRef value, void* context) {
    LODWORD(r14) = LODWORD(rcx);
    r12 = rdx;
    rbx = rsi;
    r13 = rdi;
    uint32_t result = AMAuthInstallCryptoGetKeyIdType(key);
    if ((result & 0x8) == 0x0) {
    	// loc_428fc;
		if ((result & 0x4) == 0x0) {
			// loc_4291d;
		    CFStringRef pub_key_name = CFStringCreateWithFormat(kCFAllocatorDefault, NULL, CFSTR("%@.public"), key);
		    LODWORD(r15) = 0x2;
		    if (!pub_key_name) {
				// loc_42cf2;
			    LODWORD(r13) = 0x0;
			    LODWORD(rbx) = 0x0;
			    var_24 = 0x0;
			    LODWORD(r14) = 0x0;
			    LODWORD(r12) = 0x0;
				// loc_42c0d;
				// goto exit
			}
			
			// loc_4295c;
			CFStringRef private_key_name = CFStringCreateWithFormat(kCFAllocatorDefault, NULL, CFSTR("%@.private"), key);
			if (!private_key_name) {
				// loc_42d0d;
			    LODWORD(r13) = 0x0;
			    var_24 = 0x0;
				
				// loc_42bc9:
			    LODWORD(r14) = 0x0;
			    LODWORD(r12) = 0x0;
			    rbx = var_40;
			    
				// loc_42c0d;
				// goto exit
			}
			
			// loc_4298e;
		    LOBYTE(r15) = 0x1;
		    var_16 = 0x1;
		}
		else {
			// loc_42900;
		    var_40 = CFRetain(rbx);
		    LOBYTE(r15) = 0x0;
		    var_16 = 0x1;
		    LODWORD(rax) = 0x0;
		    // loc_42998;
		}
    }
	else {
		// loc_428da;
	    rax = CFRetain(rbx);
	    LOBYTE(r15) = 0x1;
	    var_16 = 0x0;
	    var_40 = 0x0;
	}
	
	// loc_42998;
	var_8 = r13;
	var_24 = rax;
	rbx = BIO_new_mem_buf(r12, LODWORD(r14));
	if (rbx == 0x0) {
		// loc_42be7;
		var_32 = 0x0;
		LODWORD(r13) = 0x0;
		LODWORD(r14) = 0x0;
		
		// loc_42bf8:
		LODWORD(r12) = 0x0;
		
		// loc_42c00:
		rbx = var_40;
		LODWORD(r15) = 0x2;
		// loc_42c0d;
		// goto exit
	}
	
	// loc_429bd;
	rdi = rbx;
	if (LOBYTE(r15) != 0x0) {
		rax = PEM_read_bio_RSAPrivateKey(rdi, 0x0, 0x0, 0x0);
	}
	else {
		rax = PEM_read_bio_RSA_PUBKEY(rdi, 0x0, 0x0, 0x0);
	}
	r13 = rax;
	var_32 = rbx;
	if (r13 == 0x0) {
		// loc_42b77;
	    rax = ERR_get_error();
	    rbx = &var_64;
	    ERR_error_string(rax, rbx);
	    if (LOBYTE(r15) != 0x0) {
	    }
	    _AMAuthInstallLog(0x3, "AMAuthInstallCryptoRegisterKeysFromPEMBuffer", "PEM_read_bio_RSA%sKey() failed: %s");
	    LODWORD(r15) = 0x6;
	    LODWORD(r13) = 0x0;
	    // loc_42bc9;
	    LODWORD(r14) = 0x0;
	    LODWORD(r12) = 0x0;
	    rbx = var_40;
	    // loc_42c0d;
		// goto exit
	}
	else {
		// loc_429ea;
	    LODWORD(r14) = 0x0;
	    uint32_t private_key_length = i2d_RSAPrivateKey(r13, NULL);
	    CFDataRef private_key = CFDataCreateMutable(kCFAllocatorDefault, private_key_length);
	    if (private_key) {
			// loc_42a2c;
		    CFDataSetLength(private_key, private_key_length);
		    UInt8 *data_ptr = CFDataGetMutableBytePtr(private_key);
		    i2d_RSAPrivateKey(r13, data_ptr);
		    CFDictionarySetValue(*(r15 + 0x140), private_key_name, private_key);
		    // loc_42a6b;
		}
	    if (LOBYTE(r15) == 0x0 || private_key) {
			// loc_42a6b;
		    if (var_16 == 0x0) {
				// loc_42bd8;
			    LODWORD(r12) = 0x0;
			    LODWORD(r15) = 0x0;
			    rbx = var_40;
			    // loc_42c0d;
				// goto exit
			}
			else {
				// loc_42a78;
			    uint32_t pub_key_length = i2d_RSAPublicKey(r13, NULL);
			    CFDataRef pub_key = CFDataCreateMutable(kCFAllocatorDefault, pub_key_length);
			    if (pub_key == 0x0) {
					// loc_42bf8;
				    LODWORD(r12) = 0x0;
					
					// loc_42c00:
				    rbx = var_40;
				    LODWORD(r15) = 0x2;
				    // loc_42c0d;
					// goto exit
				}
			    var_0 = r14;
			    CFDataSetLength(pub_key, pub_key_length);
			    UInt8 *pub_key_ptr = CFDataGetMutableBytePtr(pub_key);
			    i2d_RSAPublicKey(r13, pub_key_ptr);
			    CFDictionarySetValue(dict, pub_key_name, pub_key);
			    Pointer digest = NULL;
			    LODWORD(r15) = AMAuthInstallCryptoCreateDigestForKey(dict, pub_key_name, &digest));
			    if (LODWORD(r15) == 0x0) {
					var_16 = r13;
					if (var_64 != 0x0) {
						CFDictionarySetValue(install->ivars.digests, var_64, rbx);
						LODWORD(r13) = 0x0;
					}
					else {
						_AMAuthInstallLog(0x3, "_AMAuthInstallCryptoRegisterKeyHash", "keyHashData is NULL");
						LODWORD(r13) = 0x0;
						LODWORD(r15) = 0x0;
					}
			    }
			    else {
					var_16 = r13;
					AMAuthInstallLog(0x3, "_AMAuthInstallCryptoRegisterKeyHash", "AMAuthInstallCryptoCreateDigestForKey(%@) failed", rbx);
					LODWORD(r13) = LODWORD(r15);
					r15 = var_64;
			    }
			    _SafeRelease(r15);
			    LODWORD(r15) = LODWORD(r13);
			    r14 = var_0;
			    if (LODWORD(r13) == 0x0) {
					LODWORD(r15) = 0x0;
			    }
			    else {
					AMAuthInstallLog(0x3, "AMAuthInstallCryptoRegisterKeysFromPEMBuffer", "AMAuthInstallCryptoRegisterKeyHash(%@) failed", rbx);
			    }
			    r13 = var_16;
			    // loc_42c0d;
				// goto exit
			}
		}
	}
	
	
	// loc_42c0d:
	_SafeRelease(rbx);
	_SafeRelease(var_24);
	_SafeRelease(r14);
	_SafeRelease(r12);
	if (r13 != 0x0) {
		RSA_free(r13);
	}
	rdi = var_32;
	if (rdi != 0x0) {
		BIO_free(rdi);
	}
	return result;
}

uint8_t AMAuthInstallCryptoGetKeyIdType(CFStringRef key) {
    Boolean has_suffix, has_prefix = CFStringHasPrefix(key, @"ap.");
	uint8_t flag = 1;
    if (has_prefix == false) {
		has_prefix = CFStringHasPrefix(key, @"bb.");
    }
    has_suffix = CFStringHasSuffix(key, @".private");
    if (has_suffix == true) {
		flag = (flag | 0x8);
    }
    else {
		flag = (flag | 0x4);
		if (CFStringHasSuffix(key, @".public")) {
			flag = 1;
		}
    }
    return flag;
}


#endif


#endif