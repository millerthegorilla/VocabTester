// ------------------------------
// (c) Reliable Software, 2000-03
// ------------------------------
#include <WinLibBase.h>
#include "Drives.h"
#include <Sys/SysVer.h>

LogicalDrives::LogicalDrives ()
{
	size_t bufSize = ::GetLogicalDriveStrings (0, 0);
	std::string buffer;
	buffer.resize (bufSize);
	::GetLogicalDriveStrings (bufSize, &buffer [0]);
	size_t offset = 0;
	while (buffer [offset] != '\0')
	{
		size_t len = 0;
		while (buffer [offset + len] != '\0')
			++len;
		_drives.push_back (buffer.substr (offset, len));
		offset += len + 1;
	}
}

DriveInfo::DriveInfo (std::string const & rootPath)
: _rootPath (rootPath)
{
	_type = ::GetDriveType (rootPath.c_str ());
}

int DriveInfo::MbytesFree () const
{
	SystemVersion ver;
	if (!ver.IsOK ())
		throw Win::Exception ("Get system version failed");

	int mbFree = 0;
	if (ver.IsWin32Windows ())
	{
		DWORD secPerClu, bytePerSec, freeClu, totalClu;
		::GetDiskFreeSpace (_rootPath.c_str (), &secPerClu, &bytePerSec, &freeClu, &totalClu);
		mbFree = MulDiv (freeClu, bytePerSec * secPerClu, 1024 * 1024);
	}
	else if (ver.IsWinNT ())
	{
		//	GetDiskFreeSpaceEx is not available on Windows 95, so we can't call
		//	it directly.  Instead, now that we know we're running on NT, get
		//	the function address from kernel32.dll
		
		//	declaration from winbase.h
		typedef BOOL WINAPI PFNGetDiskFreeSpaceExA(LPCSTR,
												   PULARGE_INTEGER,
												   PULARGE_INTEGER,
												   PULARGE_INTEGER);
		PFNGetDiskFreeSpaceExA *pGetDiskFreeSpaceExA;
		pGetDiskFreeSpaceExA = (PFNGetDiskFreeSpaceExA *)
			::GetProcAddress(::GetModuleHandle("kernel32.dll"),
							 "GetDiskFreeSpaceExA");
		Assert(pGetDiskFreeSpaceExA != 0);	//	must always exist on NT platforms

		ULARGE_INTEGER bytesAvail, bytesTotal, bytesFree;
		pGetDiskFreeSpaceExA (_rootPath.c_str (), &bytesAvail, &bytesTotal, &bytesFree);
		mbFree = (bytesAvail.HighPart << (32 - 20)) + (bytesAvail.LowPart >> 20);
	}
	else
		throw Win::Exception ("Unknown operating system version.");
	return mbFree;
}

