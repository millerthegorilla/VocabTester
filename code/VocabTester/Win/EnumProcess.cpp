//------------------------------
// (c) Reliable Software 2000-03
//------------------------------

#include <WinLibBase.h>
#include "EnumProcess.h"

using namespace Win;

EnumProcess::EnumProcess (Win::Dow::Handle callerWindow, Messenger & messenger)
	: _callerWindow (callerWindow),
	  _messenger (messenger)
{
	// Enumerate processes with the same class name as caller Window
	std::string buf;
	buf.resize (256);
	// Revisit: there is better API RealGetWindowClass, but it is supported
	// only in Win98, NT 4.0 with SP3 and Win2000
	int classNameLen = _callerWindow.GetClassName (&buf [0], buf.length ());
	if (classNameLen != 0)
	{
		buf.resize (classNameLen);
		_className = buf;
	}
	::EnumWindows (&EnumCallback, reinterpret_cast<LPARAM>(this));
}

EnumProcess::EnumProcess (std::string const & className, Messenger & messenger)
	: _callerWindow (0),
	  _className (className),
	  _messenger (messenger)
{
	// Enumerate processes with given class name
	::EnumWindows (&EnumCallback, reinterpret_cast<LPARAM>(this));
}

void EnumProcess::ProcessWindow (Win::Dow::Handle win)
{
	if (win != _callerWindow)
	{
		std::string buf;
		buf.resize (256);
		// Revisit: there is better API RealGetWindowClass, but it is supported
		// only in Win98, NT 4.0 with SP3 and Win2000
		int classNameLen = win.GetClassName (&buf [0], buf.length ());
		if (classNameLen != 0)
		{
			buf.resize (classNameLen);
			if (_className == buf)
			{
				// Found another window of the same class -- execute command
				_messenger.DeliverMessage (win);
			}
		}
	}
}

BOOL CALLBACK EnumProcess::EnumCallback (HWND hwnd, LPARAM lParam)
{
	Assert (lParam != 0);
	EnumProcess * thisEnum = reinterpret_cast<EnumProcess *>(lParam);
	thisEnum->ProcessWindow (hwnd);
	return TRUE;
}
