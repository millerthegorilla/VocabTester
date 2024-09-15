// ---------------------------
// (c) Reliable Software, 2004
// ---------------------------
#include <WinLibBase.h>
#include "TaskBarIcon.h"
#include <Sys/Dll.h>

TaskbarIcon::TaskbarIcon (	Win::Dow::Handle winRecipient,
							int iconId,
							Icon::Handle image,
							Win::Message & msgNotify)
: _iconData (winRecipient),
  _image (image),
  _notifyMsg (msgNotify)
{
	_iconData.SetMessage (_notifyMsg.GetMsg ());
    _iconData.SetImage (_image);
    ::Shell_NotifyIcon (NIM_ADD, &_iconData);
    ::Shell_NotifyIcon (NIM_SETVERSION, &_iconData);
	_iconData.ResetFlags ();
}

void TaskbarIcon::ReShow ()
{
	::Shell_NotifyIcon (NIM_DELETE, &_iconData);
	_iconData.SetMessage (_notifyMsg.GetMsg ());
    _iconData.SetImage (_image);
    ::Shell_NotifyIcon (NIM_ADD, &_iconData);
    ::Shell_NotifyIcon (NIM_SETVERSION, &_iconData);
	_iconData.ResetFlags ();
}

TaskbarIcon::NotifyIconData::NotifyIconData (Win::Dow::Handle win)
: _isPreVersion5 (true)
{
	Dll shell32Dll ("shell32.dll");
	DllVersion ver (shell32Dll);
	if (ver.IsOk ())
	{
		_isPreVersion5 = ver.GetMajorVer () < 5;
	}

	if (_isPreVersion5)
	{
		ZeroMemory (this, NOTIFYICONDATA_V1_SIZE);
		cbSize = NOTIFYICONDATA_V1_SIZE;
	}
	else
	{
		ZeroMemory (this, sizeof (NOTIFYICONDATA));
		cbSize = sizeof (NOTIFYICONDATA);
		uVersion = NOTIFYICON_VERSION;
		dwInfoFlags = NIIF_INFO; // balloon icon set to Information
	}
	hWnd = win.ToNative ();
}
