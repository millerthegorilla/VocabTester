// (c) Reliable Software 2003
#include <WinLibBase.h>
#include "Error.h"

LastSysErr::LastSysErr ()
	: _err (::GetLastError ()), _msg (0)
{
    ::FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
        NULL,
        _err,
        MAKELANGID (LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
        (LPTSTR) &_msg,
        0,
        NULL);
}

LastSysErr::~LastSysErr ()
{
    Win::ClearError ();
    if (_msg != 0)
        ::LocalFree (_msg);
}

SysMsg::SysMsg (DWORD errCode, HINSTANCE hModule)
    : _msg (0)
{
    if (errCode != 0)
    {
		unsigned flags = FORMAT_MESSAGE_ALLOCATE_BUFFER;
		flags |= (hModule != 0)? FORMAT_MESSAGE_FROM_HMODULE: FORMAT_MESSAGE_FROM_SYSTEM;
        ::FormatMessage(
            flags,
            hModule,
            errCode,
            MAKELANGID (LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
            (LPTSTR) &_msg,
            0,
            NULL);
		if (_msg == 0 && hModule != 0)
		{
			// try system error
			::FormatMessage(
				FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER,
				0,
				errCode,
				MAKELANGID (LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
				(LPTSTR) &_msg,
				0,
				NULL);
		}
		if (_msg == 0)
		{
			int len = 0;
			_msg = static_cast<char *> (::LocalAlloc (LPTR, 64));
			strcpy (_msg, "Error ");
			len = strlen (_msg);
			_ultoa (errCode, _msg + len, 10);
			strcat (_msg, " (0x");
			len = strlen (_msg);
			_ultoa (errCode, _msg + len, 16);
			strcat (_msg, ")");
		}
    }
}

SysMsg::~SysMsg ()
{
    if (_msg != 0)
        ::LocalFree (_msg);
}

