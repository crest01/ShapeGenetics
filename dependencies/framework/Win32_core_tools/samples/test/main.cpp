


#include <cassert>
#include <cstring>

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>

#include <win32/error.h>

#include <win32/unicode.h>

#include <win32/crypt.h>

#include <win32/unique_handle.h>
#include <win32/WindowClass.h>


int WINAPI wWinMain(HINSTANCE, HINSTANCE, PWSTR lpCmdLine, int nCmdShow)
{
	{
		auto test = u8"Iñtërnâtiônàlizætiøn";
		auto w = widen(test);
		auto n = narrow(w);
		assert(std::memcmp(n.c_str(), test, n.length()) == 0);
	}

	{
		auto test = std::string { u8"Iñtërnâtiônàlizætiøn" };
		auto w = widen(test);
		auto n = narrow(w);
		assert(n == test);
	}

	{
		auto prov = Win32::Crypt::acquireContext(nullptr, MS_ENH_RSA_AES_PROV_W, PROV_RSA_AES, CRYPT_VERIFYCONTEXT | CRYPT_SILENT);
		auto hash = Win32::Crypt::createHash(prov, CALG_SHA_256);

		unsigned char data[] = "bla";
		if (!CryptHashData(hash, data, sizeof(data), 0U))
			Win32::throw_last_error();

		unsigned char hash_data[32];
		DWORD hash_size = sizeof(hash_data);
		if (!CryptGetHashParam(hash, HP_HASHVAL, hash_data, &hash_size, 0U))
			Win32::throw_last_error();
	}

	return 0;
}
