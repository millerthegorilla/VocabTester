//-----------------------------------
// (c) Reliable Software 1998 -- 2005
//-----------------------------------

#include <WinLibBase.h>
#include "Output.h"

#include <Graph/Cursor.h>
#include <File/ErrorLog.h>
#include <Sys/Synchro.h>
#include <Ex/Error.h>

using namespace Out;

std::string Sink::FormatExceptionMsg (Win::Exception const & ex)
{
	if (ex.GetMessage () != 0)
	{
		try
		{
			std::string exceptionMsg (ex.GetMessage ());
			DWORD errCode = ex.GetError ();
			if (errCode != 0)
			{
				SysMsg sysMsg (errCode, ex.GetModuleHandle ());
				exceptionMsg += "\nSystem tells us: ";
				exceptionMsg += sysMsg.Text ();
			}
			char const * objectName = ex.GetObjectName ();
			if (objectName [0] != '\0')
			{
				exceptionMsg += "\n";
				exceptionMsg += objectName;
			}
			return exceptionMsg;
		}
		catch ( ... )
		{
			// Ignore all exceptions during message formating
			Win::ClearError ();
		}
	}
	return std::string ();
}

void Sink::DisplayException (Win::Exception const & ex, 
							 Win::Dow::Handle owner, 
							 char const * appName,
							 char const * title, 
							 int flags)
{
	if (ex.GetMessage () == 0)
		return;		// Quiet exception -- nothing to display

	try
	{
		std::string displayMsg (appName);
		displayMsg += " problem: ";
		displayMsg += FormatExceptionMsg (ex);
		std::string exceptionMsg = FormatExceptionMsg (ex);
		::MessageBox (owner.ToNative (), displayMsg.c_str (), title, flags);
		TheErrorLog << WriteError (displayMsg);
		dbg << displayMsg.c_str () << std::endl;
	}
	catch (...)
	{
		Win::ClearError ();
	}
}

void Sink::Display (char const * msg, Severity sev, Win::Dow::Handle owner) const throw ()
{
	if (_verbose || sev == Error)
	{
		try
		{
			Cursor::Arrow arrow;
			Cursor::Holder cursorSwitch (arrow);
			unsigned flag = MB_OK;
			if (_topWin.IsNull ())
				flag |= MB_TASKMODAL;
			if (_foreground)
				flag |= MB_SETFOREGROUND;
			flag |= sev;

			Win::Dow::Handle own = (owner.IsNull ()) ? _topWin : owner;
			Win::UnlockPtr unLock (_critSect);
			if (::MessageBox (own.ToNative (), msg, _appName.c_str (), flag) != 0)
				Win::ClearError ();	// If message box displayed successfuly clear any error code
									// set by system in the process of displaying message box.
		}
		catch (...)
		{
			Win::ClearError ();
		}
	}
}

void Sink::DisplayModal (char const * msg, Severity sev) const throw ()
{
	if (_verbose || sev == Error)
	{
		try
		{
			unsigned flag = MB_OK | MB_TASKMODAL;				
			if (_foreground)
				flag |= MB_SETFOREGROUND;
			flag |= sev;			
			Win::UnlockPtr unLock (_critSect);
			if (::MessageBox (0, msg, _appName.c_str (), flag) != 0)
				Win::ClearError ();	// If message box displayed successfuly clear any error code
									// set by system in the process of displaying message box.
		}
		catch (...)
		{
			Win::ClearError ();
		}
	}
}

void Sink::Display (Win::Exception const & ex) const throw ()
{
	unsigned flag = MB_ICONERROR | MB_OK;
	if (_topWin.IsNull ())
		flag |= MB_TASKMODAL;
	if (_foreground)
		flag |= MB_SETFOREGROUND;
	// don't release lock while displaying exception
	DisplayException (ex, _topWin, _appName.c_str (), "Exception", flag);
}

Answer Sink::Prompt (char const * question, PromptStyle style, Win::Dow::Handle owner) const throw ()
{
	Win::Dow::Handle own = (owner.IsNull ()) ? _topWin : owner;
	unsigned flag = 0;
	if (_foreground)
		flag |= MB_SETFOREGROUND;
	flag |= style;
	int answer = -1;
	try
	{
		Cursor::Arrow arrow;
		Cursor::Holder cursorSwitch (arrow);
		Win::UnlockPtr unLock (_critSect);
		answer = ::MessageBox (own.ToNative (), question, _appName.c_str (), flag);
		if (answer != 0)
			Win::ClearError ();	// If message box displayed successfully clear any error code
								// set by system in the process of displaying message box.
	}
	catch (...)
	{
		Win::ClearError ();
	}

	switch (answer)
	{
	case IDYES:
		return Yes;
	case IDNO:
		return No;
	case IDRETRY:
		return Retry;
	case IDIGNORE:
		return Ignore;
	case IDABORT:
		return Abort;
	case IDOK:
		return OK;
	default:
		return Cancel;
	};
}

Answer Sink::PromptModal (char const * question, PromptStyle style) const throw ()
{
	unsigned flag = 0;
	if (_foreground)
		flag |= MB_SETFOREGROUND;
	flag |= style;
	flag |= MB_TASKMODAL;
	int answer = -1;
	try
	{
		Win::UnlockPtr unLock (_critSect);
		answer = ::MessageBox (0, question, _appName.c_str (), flag);
		if (answer != 0)
			Win::ClearError ();	// If message box displayed successfuly clear any error code
								// set by system in the process of displaying message box.
	}
	catch (...)
	{
		Win::ClearError ();
	}

	switch (answer)
	{
	case IDYES:
		return Yes;
	case IDNO:
		return No;
	case IDRETRY:
		return Retry;
	case IDIGNORE:
		return Ignore;
	case IDABORT:
		return Abort;
	case IDOK:
		return OK;
	default:
		return Cancel;
	};
}


unsigned PromptStyle::CalcDefButton () const throw ()
{
	switch (_buttons)
	{
	case RetryCancel:
		return Retry == _defaultAnswer ? MB_DEFBUTTON1 : MB_DEFBUTTON2;
	case YesNo :
		return Yes == _defaultAnswer ? MB_DEFBUTTON1 : MB_DEFBUTTON2;
	case YesNoCancel:
		if (Yes == _defaultAnswer)
			return MB_DEFBUTTON1;
		else if (No == _defaultAnswer)
			return MB_DEFBUTTON2;
		else
			return MB_DEFBUTTON3;
	case AbortRetryIgnore:
		if (Abort == _defaultAnswer)
			return MB_DEFBUTTON1;
		else if (Retry == _defaultAnswer)
			return MB_DEFBUTTON2;
		else
			return MB_DEFBUTTON3;
	default:
		return MB_DEFBUTTON1;
	};
}
