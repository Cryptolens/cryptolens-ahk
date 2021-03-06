// AHKIntegration.cpp : Defines the exported functions for the DLL application.

#include "cryptolens\core.hpp"
#include "cryptolens\Error.hpp"
#include "cryptolens\RequestHandler_WinHTTP.hpp"
#include "cryptolens\SignatureVerifier_CryptoAPI.hpp"

namespace cryptolens = ::cryptolens_io::v20180502;
using Cryptolens = cryptolens::basic_SKM<cryptolens::RequestHandler_WinHTTP, cryptolens::SignatureVerifier_CryptoAPI>;

struct AHKIntegrationResult {
	__int64 subsystem;
	__int64 reason;
	unsigned __int64 extra;
	unsigned __int64 features;
};

__int64 uts()
{
	FILETIME ft;
	GetSystemTimeAsFileTime(&ft);

	constexpr __int64 EPOCH = 0x019DB1DED53E8000;
	constexpr __int64 TICKS_PER_SECOND = 10000000;

	__int64 tlow = ft.dwLowDateTime;
	__int64 thigh = ft.dwHighDateTime;
	__int64 t = thigh << 32 | tlow;
	return (t - EPOCH) / TICKS_PER_SECOND;
}

extern "C" __declspec(dllexport) unsigned int CheckKey(char const* key_string, struct AHKIntegrationResult * result)
{
	Cryptolens cryptolens_handle;
	cryptolens::Error e;
	// Setting up the signature verifier with credentials from "Security Settings"
	// on serialkeymanager.com
	cryptolens_handle.signature_verifier.set_modulus_base64(e, "khbyu3/vAEBHi339fTuo2nUaQgSTBj0jvpt5xnLTTF35FLkGI+5Z3wiKfnvQiCLf+5s4r8JB/Uic/i6/iNjPMILlFeE0N6XZ+2pkgwRkfMOcx6eoewypTPUoPpzuAINJxJRpHym3V6ZJZ1UfYvzRcQBD/lBeAYrvhpCwukQMkGushKsOS6U+d+2C9ZNeP+U+uwuv/xu8YBCBAgGb8YdNojcGzM4SbCtwvJ0fuOfmCWZvUoiumfE4x7rAhp1pa9OEbUe0a5HL+1v7+JLBgkNZ7Z2biiHaM6za7GjHCXU8rojatEQER+MpgDuQV3ZPx8RKRdiJgPnz9ApBHFYDHLDzDw==");

	cryptolens::optional<cryptolens::LicenseKey> license_key =
		cryptolens_handle.activate(
			e,
			// SKM Access Token
			"WyI0NjUiLCJBWTBGTlQwZm9WV0FyVnZzMEV1Mm9LOHJmRDZ1SjF0Vk52WTU0VzB2Il0=",
			// Product id
			"3646",
			// License Key
			key_string,
			// Machine Code
			"289jf2afs3"
		);

	result->subsystem = e.get_subsystem();
	result->reason = e.get_reason();
	result->extra = e.get_extra();
	result->features = 0;

	if (e) {
		return 0;
	}

	__int64 t = uts();
	if (t > license_key->get_expires()) { 
		result->subsystem = 1;
		result->reason = 9;
		result->extra = 0;
		return 0;
	}

	result->features = 1
		| license_key->get_f1() << 1
		| license_key->get_f2() << 2
		| license_key->get_f3() << 3
		| license_key->get_f4() << 4
		| license_key->get_f5() << 5
		| license_key->get_f6() << 6
		| license_key->get_f7() << 7
		| license_key->get_f8() << 8
		;
	return 1;
}
