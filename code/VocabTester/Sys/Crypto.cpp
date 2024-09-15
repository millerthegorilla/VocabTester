// (c) Reliable Software, 2003
#include <WinLibBase.h>
#include "Crypto.h"
#include "StringOp.h"
#include <Sys/SysVer.h>

namespace Crypt
{
	void CheckVersion ()
	{
		SystemVersion ver;
		if (!ver.IsOK ())
			throw Win::Exception ("Get system version failed");
		if (!ver.IsWinNT ())
			throw Win::Exception ("Password encryption not supported on this platform");
	}
}

void Crypt::String::Encrypt (std::string const & description)
{
	Assert (description.size () != 0);
	CheckVersion ();
	DATA_BLOB blobIn;
	blobIn.cbData = _plainText.size () + 1;
	blobIn.pbData = reinterpret_cast<unsigned char *> (&_plainText [0]);
	::CryptProtectData (&blobIn, 
					ToWString (description).c_str (), // description
					0, // entropy
					0, // reserved
					0, // prompt
					0, // flags (only currently logged user can decrypt)
					&_blob);
}

void Crypt::String::Decrypt ()
{
	CheckVersion ();
	DATA_BLOB blobOut;
	Assert (_blob.pbData != 0);
	::CryptUnprotectData (&_blob, 
					0, // description
					0, // entropy
					0, // reserved
					0, // prompt
					0, // flags (only currently logged user can decrypt)
					&blobOut);
	_plainText.resize (blobOut.cbData);
	std::copy (blobOut.pbData, blobOut.pbData + blobOut.cbData, &_plainText [0]);
	::LocalFree (blobOut.pbData);
}

//-------------
// Cryptography
//-------------

Crypt::Context::Context ()
	: _provider (0)
{
	if(!::CryptAcquireContext(
					&_provider, 
					NULL, 
					MS_ENHANCED_PROV, 
					PROV_RSA_FULL, 
					0))
	{
		if (GetLastError() == NTE_BAD_KEYSET)
		{
			if(!::CryptAcquireContext(
							&_provider, 
							NULL, 
							MS_ENHANCED_PROV, 
							PROV_RSA_FULL, 
							CRYPT_NEWKEYSET))
			{
				throw Win::Exception ("Cannot create cryptographic key set");
			}
		}
		else
			throw Win::Exception ("Cannot access cryptographic provider");
	}
}

Crypt::Context::~Context ()
{
	if(_provider != 0)
		::CryptReleaseContext(_provider, 0);
}

Crypt::Hash::Hash (Crypt::Context const & context, std::string const & str)
: _hash (0)
{
	if(!::CryptCreateHash(
		context.ToNative (), 
		CALG_MD5, 
		0, 
		0, 
		&_hash))
	{
		throw Win::Exception ("Cannot create cryptographic hash");
	}
	if(!::CryptHashData(
		_hash, 
		(BYTE *)str.c_str (), 
		str.size (), 
		0))
	{
		throw Win::Exception ("Cannot hash string");
	}
}

Crypt::Hash::~Hash ()
{
	if(_hash != 0) 
		::CryptDestroyHash(_hash);
}

Crypt::Key::Key (Crypt::Context const & context, ALG_ID algorithm, DWORD flags)
	:_key (0)
{
	if (!::CryptGenKey (context.ToNative (),
		algorithm,
		flags,
		&_key))
	{
		throw Win::Exception ("Can't generate cryptographic key");
	}

}

Crypt::Key::Key (Crypt::Context const & context, Crypt::Hash const & hash, ALG_ID algorithm, DWORD flags)
:_key (0)
{
	if(!::CryptDeriveKey(
		context.ToNative (), 
		algorithm, 
		hash.ToNative (), 
		flags, 
		&_key))
	{
		throw Win::Exception ("Cannot derive cryptographic key");
	}
}

Crypt::Key::~Key ()
{
	if(_key != 0)
		::CryptDestroyKey(_key);
}

Crypt::Streamer::Streamer (std::string const & password)
: _context (), 
  _hash (_context, password), 
  _key (_context, _hash),
  _inBlockLen (1000 - 1000 % ENCRYPT_BLOCK_SIZE) // multiple of block size
{
	// Determine the output size. If a block cipher is used, 
	// it must have room for an extra block. 

	unsigned bufLen = _inBlockLen;
	if (ENCRYPT_BLOCK_SIZE > 1) 
		bufLen += ENCRYPT_BLOCK_SIZE;
	_buf.resize (bufLen);
}

void Crypt::Streamer::Encrypt (std::ostream & out)
{
	bool isLast;
	do 
	{
		isLast = (_inSize <= _inBlockLen);
		unsigned len = isLast? _inSize: _inBlockLen;
		std::copy (_inBuf, _inBuf + len, _buf.begin ());
		_inBuf += len;
		_inSize -= len;
		DWORD outCount = len;
		if(!::CryptEncrypt(
			_key.ToNative (), 
			0, 
			isLast, 
			0, 
			&_buf [0], 
			&outCount, 
			_buf.size ()))
		{
			throw Win::Exception ("Encryption failed");
		}
		std::copy (&_buf [0], &_buf [outCount], std::ostream_iterator<unsigned char> (out));
	} while (!isLast);
}

void Crypt::Streamer::Decrypt (std::ostream & out)
{
	bool isLast;
	do 
	{
		isLast = (_inSize <= _inBlockLen);
		unsigned len = isLast? _inSize: _inBlockLen;
		std::copy (_inBuf, _inBuf + len, _buf.begin ());
		_inBuf += len;
		_inSize -= len;
		DWORD outCount = len;
		if(!::CryptDecrypt(
			_key.ToNative (), 
			0, 
			isLast, 
			0, 
			&_buf [0], 
			&outCount))
		{
			throw Win::Exception ("Encryption failed");
		}
		std::copy (&_buf [0], &_buf [outCount], std::ostream_iterator<unsigned char> (out));
	} while (!isLast);
}
