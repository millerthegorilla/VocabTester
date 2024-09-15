//-----------------------------------
// (c) Reliable Software 2001 -- 2005
//-----------------------------------

#include <WinLibBase.h>
#include "StringOp.h"
#include <iomanip>

UpCaseTable const TheUpCaseTable;

int ToInt (std::string const & str)
{
	return ::atoi (str.c_str ());
}

unsigned int HexStrToUnsigned (std::string const & str)
{
	return ::strtoul (str.c_str (), 0, 16);
}

std::string ToHexString (unsigned value)
{
    std::ostringstream buffer;
	buffer << std::hex << value;
    return buffer.str ();
}

const char HexDigits [] = "0123456789abcdef";

std::string ToHexStr (char c)
{
	unsigned char highBitQuartet = static_cast<unsigned char> (c) >> 4;
	unsigned char lowBitQuartet  = static_cast<unsigned char> (c) & 0x0f;

	std::string hexStr;
	hexStr += HexDigits [highBitQuartet];
	hexStr += HexDigits [lowBitQuartet];

	return hexStr;
}

std::wstring ToWString (std::string const & src)
{
	std::wstring wStr;
	int translatedLength = ::MultiByteToWideChar (CP_ACP, 0, src.c_str (), -1, 0, 0);
	if (translatedLength == 0)
		throw Win::Exception ("Internal error: Cannot get Unicode string length", src.c_str ());

	wStr.resize (translatedLength);
	if (::MultiByteToWideChar (CP_ACP, 0, src.c_str (), -1, &wStr [0], translatedLength) == 0)
		throw Win::Exception ("Internal error: Cannot convert ANSI string to Unicode", src.c_str ());

	return wStr;
}

std::string ToMBString (wchar_t const * wstr)
{
	std::string str;
	int translatedLength = ::WideCharToMultiByte (CP_ACP, 0, wstr, -1, 0, 0, 0, 0);
	if (translatedLength == 0)
		throw Win::Exception ("Internal error: Cannot get Multibyte string length");

	str.resize (translatedLength);
	if (::WideCharToMultiByte (CP_ACP, 0, wstr, -1, &str [0], translatedLength, 0, 0) == 0)
		throw Win::Exception ("Internal error: Cannot convert Unicode string to ANSI");

	return str;
}

std::string FormatFileSize (__int64 size)
{
	if (size == 0)
		return "0 KB";

	__int64 sizeInKb;
	if (size > 1023)
	{
		sizeInKb = size >> 10;
		if (size % 1024 > 512)
			sizeInKb += 1;
	}
	else
	{
		return "1 KB";
	}

	std::vector<std::string> thousandGroups;
	while (sizeInKb != 0)
	{
		__int64 group = sizeInKb % 1000;
		sizeInKb /= 1000;
		if (sizeInKb != 0)
		{
			std::ostringstream groupStr;
			groupStr << std::setw (3) << std::setfill ('0') << group;
			thousandGroups.push_back (groupStr.str ());
		}
		else
		{
			// Last group
			thousandGroups.push_back (ToString (group));
		}
	}
	
	std::string sizeStr;
	while (!thousandGroups.empty ())
	{
		sizeStr += thousandGroups.back ();
		sizeStr += " ";
		thousandGroups.pop_back ();
	}
	sizeStr += "KB";
	return sizeStr;
}

bool HasGuidFormat (std::string const & str)
{
	// {XXXXXXXX-XXXX-XXXX-XXXX-XXXXXXXXXXXX}
	// X-hex digit: 0..9, A..F
	if (str.length () != 38)
		return false;

	if (str [0] != '{' || str [37] != '}')
		return false;

	for (int i = 1; i < 37; ++i)
	{
		if (i == 9 || i == 14 || i == 19 || i == 24)
		{
			if (str [i] != '-')
				return false;
		}
		else if (!std::isxdigit (str [i]))
			return false;
	}
	return true;
}

void TrimmedString::Init (std::string const & str)
{
	// Eat leading white spaces
	size_t nonSpace = str.find_first_not_of (' ');
	if (nonSpace != std::string::npos)
		assign (str.substr (nonSpace));
	// Eat trailing white spaces
	nonSpace = find_last_not_of (' ');
	if (nonSpace != std::string::npos)
		erase (nonSpace + 1);
}
