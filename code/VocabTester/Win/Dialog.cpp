//------------------------------------
//  (c) Reliable Software, 1996 - 2005
//------------------------------------

#include <WinLibBase.h>
#include "Dialog.h"

#include <Ctrl/Output.h>
#include <Win/Help.h>

using namespace Dialog;

void Handle::MapRectangle (Win::Rect & rect)
{
	::MapDialogRect (H (), &rect);
}

bool Controller::OnHelp () throw ()
{
	if (_helpEngine != 0 && _dlgId != -1)
	{
		return _helpEngine->OnDialogHelp (_dlgId);
	}
	return false;
}

BOOL CALLBACK Dialog::Procedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	Dialog::Handle win (hwnd);
	Controller * ctrl = win.GetLong<Controller *> ();
	LRESULT result = 0;
	try
	{
		if (ctrl != 0)
		{
			if (ctrl->Dispatch (message, wParam, lParam, result))
				return TRUE;
		}
		else if (message == WM_INITDIALOG)
		{
			ctrl = reinterpret_cast<Dialog::Controller *> (lParam);
			if (ctrl != 0)
			{
				Controller::Attach (win, ctrl);
				if (ctrl->Dispatch (message, wParam, lParam, result))
					return TRUE;
			}
		}
	}
	catch (Win::Exception e)
	{
		// display error
		Out::Sink::DisplayException (e, win);
		// exit dialog
		if (ctrl != 0)
			ctrl->EndCancel ();
	}

	return FALSE;
}
