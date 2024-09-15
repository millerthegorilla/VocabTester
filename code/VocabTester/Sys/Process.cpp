//----------------------------------------------------
// Process.cpp
// (c) Reliable Software 2000 -- 02
//
//----------------------------------------------------

#include <WinLibBase.h>
#include "Process.h"

using namespace Win;

ProcessProxy::ProcessProxy (char const * className, char const * windowName)
	: _win (0)
{
	Init (className, windowName);
}

ProcessProxy::ProcessProxy (unsigned int threadId)
	: _win (0)
{
	::EnumThreadWindows (threadId, &EnumCallback, reinterpret_cast<LPARAM>(this));
}

void ProcessProxy::Init (char const * className, char const * windowName)
{
	_win.Reset (::FindWindow (className, windowName));
}

bool ProcessProxy::PostMsg (int msg)
{
	if (!_win.IsNull ())
	{
		return _win.PostMsg (msg);
	}
	return false;
}

bool ProcessProxy::PostMsg (Win::Message & msg)
{
	if (!_win.IsNull ())
	{
		return _win.PostMsg (msg);
	}
	return false;
}

void ProcessProxy::Kill (int timeout)
{
	if (!_win.IsNull ())
	{
		DWORD processId = 0;
		DWORD threadId = ::GetWindowThreadProcessId (_win.ToNative (), &processId);
		HANDLE processHandle = 0;
		if (processId != 0)
		{
			processHandle = ::OpenProcess (STANDARD_RIGHTS_REQUIRED | 
                                           SYNCHRONIZE,
                                           FALSE, processId);
		}
		PostMsg (WM_DESTROY);
		_win.Reset ();
		if (processHandle != 0)
		{
			// Wait for process death 
			::WaitForSingleObject (processHandle, timeout);
		}
	}
}

BOOL CALLBACK ProcessProxy::EnumCallback (HWND hwnd, LPARAM lParam)
{
	// Revisit: what if thread has multiple nonchild windows?
	Assert (lParam != 0);
	ProcessProxy * thisProcessProxy = reinterpret_cast<ProcessProxy *>(lParam);
	thisProcessProxy->Init (hwnd);
	return FALSE;
}

ChildProcess::ChildProcess (std::string const & cmdLine, bool inheritParentHandles)
	: _cmdLine (cmdLine),
	  _inheritParentHandles (inheritParentHandles),
	  _createSuspended(false)
{
	memset (&_processInfo, 0, sizeof (PROCESS_INFORMATION));
	memset (&_startupInfo, 0, sizeof (STARTUPINFO));
	_startupInfo.cb = sizeof (STARTUPINFO);
}

ChildProcess::~ChildProcess()
{
	if (_processInfo.hProcess != 0)
	{
		::CloseHandle(_processInfo.hProcess);
	}

	if (_processInfo.hThread != 0)
	{
		::CloseHandle(_processInfo.hThread);
	}
}

void ChildProcess::ShowMinimizedNotActive ()
{
	_startupInfo.dwFlags |= STARTF_USESHOWWINDOW;
	_startupInfo.wShowWindow = SW_SHOWMINNOACTIVE;
}

void ChildProcess::ShowNormal ()
{
	_startupInfo.dwFlags |= STARTF_USESHOWWINDOW;
	_startupInfo.wShowWindow = SW_SHOWNORMAL;
}

bool ChildProcess::Create (unsigned timeoutMilliSec)
{
	char const * appName = _appName.empty () ? 0 : _appName.c_str ();
	char const * curFolder = _curFolder.empty () ? 0 : _curFolder.c_str ();
	char const * cmdLine = _cmdLine.empty () ? 0 : _cmdLine.c_str ();
	DWORD dwCreationFlags = NORMAL_PRIORITY_CLASS;
	if (_createSuspended)
	{
		dwCreationFlags |= CREATE_SUSPENDED;
	}
	BOOL result = ::CreateProcess (appName,	// pointer to name of executable module
								   const_cast<char *>(cmdLine),	// pointer to command line string
								   0,		// process security attributes
								   0,		// thread security attributes  
								   _inheritParentHandles ? TRUE : FALSE,// handle inheritance flag
								   dwCreationFlags,	// creation flags
								   0,		// pointer to New environment block
								   curFolder,// pointer to current directory name
								   &_startupInfo,
								   &_processInfo);
	if (result != FALSE)
	{
		// Wait till child process finishes its initialization or times out
		if (timeoutMilliSec == 0)
			return true;
		else
			return ::WaitForInputIdle (_processInfo.hProcess, timeoutMilliSec) == 0;
	}
	else
		return false;
}

void ChildProcess::Resume ()
{
	::ResumeThread(_processInfo.hThread);
}

bool ChildProcess::WaitForDeath (unsigned int timeout) const
{
	DWORD result = ::WaitForSingleObject (_processInfo.hProcess, timeout);
	return result == WAIT_OBJECT_0;
}

bool ChildProcess::IsAlive () const
{
	unsigned long exitCode = GetExitCode ();
	return exitCode == STILL_ACTIVE;
}

void ChildProcess::Terminate ()
{
	::TerminateProcess (_processInfo.hProcess, -1);
	_processInfo.hProcess = 0;
	_processInfo.hThread = 0;
	_processInfo.dwProcessId = 0;
	_processInfo.dwThreadId = 0;
}

unsigned long ChildProcess::GetExitCode () const
{
	unsigned long exitCode;
	::GetExitCodeProcess (_processInfo.hProcess, &exitCode);
	return exitCode;
}
