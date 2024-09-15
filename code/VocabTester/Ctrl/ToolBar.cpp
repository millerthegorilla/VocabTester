//--------------------------------
// (c) Reliable Software 1998-2003
//--------------------------------
#include <WinLibBase.h>
#include "ToolBar.h"
#include <Win/Geom.h>

using namespace Tool;
using namespace Notify;

int Handle::Height () const
{
	Win::ClientRect rect (H ());
	return rect.Height () + 2; // Heuristics: add 2 pixels below buttons
}

void Handle::ClearButtons ()
{
	while (Delete (0))
		continue;
}

void Handle::AddWindow (Win::Dow::Handle hwndTool)
{
	Tool::ToolWindow  toolWnd (hwndTool, H ());
    HWND hwndTT = reinterpret_cast<HWND> (SendMsg (TB_GETTOOLTIPS, 0, 0));
	if (hwndTT == 0)
		Win::Exception ("Internal error: Cannot add window tool to the toolbar.");
    ::SendMessage (hwndTT, TTM_ADDTOOL, 0, reinterpret_cast<LPARAM>(&toolWnd));
}

void Handle::GetButtonRect (int buttonIdx, Win::Rect & rect)
{
	SendMsg (TB_GETITEMRECT, (WPARAM) buttonIdx, (LPARAM) (&rect));
}

int Handle::CmdIdToButtonIndex (int cmdId)
{
	return SendMsg (TB_COMMANDTOINDEX, (WPARAM) cmdId);
}

int Handle::GetToolTipDelay ()
{
    HWND hwndTT = reinterpret_cast<HWND> (SendMsg (TB_GETTOOLTIPS, 0, 0));
	if (hwndTT == 0)
		Win::Exception ("Internal error: Cannot get the toolbar tool tip delay.");
    return ::SendMessage (hwndTT, TTM_GETDELAYTIME, TTDT_AUTOPOP, 0);
}

void Handle::SetToolTipDelay (int milliSeconds)
{
    HWND hwndTT = reinterpret_cast<HWND> (SendMsg (TB_GETTOOLTIPS, 0, 0));
	if (hwndTT == 0)
		Win::Exception ("Internal error: Cannot set the toolbar tool tip delay.");
    ::SendMessage (hwndTT, TTM_SETDELAYTIME, TTDT_AUTOPOP, milliSeconds);
}

void Handle::InsertSeparator (int idx, int width)
{
	BarSeparator separator (width);
	SendMsg (TB_INSERTBUTTON, idx, (LPARAM) (LPTBBUTTON) & separator);
}

//----------
// Tool::Bar
//----------
Bar::Bar (Win::Dow::Handle winParent,
		  int toolbarId,
		  int bitmapId,
		  int buttonWidth,
		  Cmd::Vector const & cmdVector,
		  bool topAlign)
	: _imageList (winParent.GetInstance (),
						bitmapId,
						buttonWidth),
	  _cmdVector (cmdVector),
	  _buttonItems (0)
{
	Tool::Maker maker (winParent, toolbarId);
	maker.Style () << Tool::Style::Tips;
	if (!topAlign)
		maker.Style () << Tool::Style::NoMoveY;
	Reset (maker.Create ());
	int width, height;
	_imageList.GetImageSize (width, height);
	SetButtonSize (width, height);
	SetImageList (_imageList);
}

void Bar::SetButtons (Tool::Item const * buttonItems)
{
	ClearButtons ();
	_cmdIds.clear ();
	_buttonItems = buttonItems;
	std::vector<Tool::Button> buttons;
	for (unsigned i = 0; _buttonItems [i].buttonId != Item::idEnd; ++i)
	{
		int id = _buttonItems [i].buttonId;
		if (id == Item::idSeparator)
		{
			buttons.push_back (Tool::BarSeparator ());
			_cmdIds.push_back (-1);
		}
		else
		{
			int cmdId = _cmdVector.Cmd2Id (_buttonItems [i].cmdName);
			_cmdIds.push_back (cmdId);
			buttons.push_back (Tool::BarButton (id, cmdId));
		}
	}
	AddButtons (buttons);
}

int Bar::GetButtonsEnd ()
{
	int lastIndex = ButtonCount () - 1;
	Win::Rect rect;
	GetButtonRect (lastIndex, rect);
	return rect.right;
}

int Bar::GetButtonWidth (int idx)
{
	Win::Rect rect;
	GetButtonRect (idx, rect);
	return rect.Width ();
}

void Bar::Enable () throw ()
{
	for (unsigned i = 0; i < _cmdIds.size (); ++i)
	{
		int cmdId = _cmdIds [i];
		if (cmdId == -1)
			continue;

		Cmd::Status state = _cmdVector.Test (_buttonItems [i].cmdName);
		Release (cmdId);

		if (state == Cmd::Enabled)
			Handle::Enable (cmdId);
		else if (state == Cmd::Checked)
			Handle::Press (cmdId);
		else
			Handle::Disable (cmdId);
	}
}

void Bar::Disable () throw ()
{
	for (unsigned i = 0; i < _cmdIds.size (); ++i)
	{
		int cmdId = _cmdIds [i];
		if (cmdId == -1)
			continue;

		Handle::Disable (cmdId);
	}
}

void Bar::FillToolTip (Tool::TipForCtrl * tip) const
{
	int buttonId = tip->IdFrom ();
	for (unsigned i = 0; i < _cmdIds.size (); ++i)
	{
		if (_cmdIds [i] == buttonId)
		{
			tip->SetText (_buttonItems [i].tip);
			return;
		}
	}
}

bool Bar::IsCmdButton (int cmdId) const
{
	return std::find (_cmdIds.begin (), _cmdIds.end (), cmdId) != _cmdIds.end ();
}

void MultiBar::SetButtonDescriptions (Tool::Item const * buttonItems)
{
	_cmdIds.clear ();
	_buttonItems = buttonItems;
	for (unsigned i = 0; _buttonItems [i].buttonId != Item::idEnd; ++i)
	{
		int id = _buttonItems [i].buttonId;
		if (id == Item::idSeparator)
		{
			_cmdIds.push_back (-1);
		}
		else
		{
			int cmdId = _cmdVector.Cmd2Id (_buttonItems [i].cmdName);
			_cmdIds.push_back (cmdId);
		}
	}
}

void MultiBar::SetLayout (int const * layout)
{
	ClearButtons ();
	std::vector<Tool::Button> buttons;
	for (int i = 0; layout [i] != Item::idEnd; ++i)
	{
		int buttonId = layout [i];
		if (buttonId == Item::idSeparator)
		{
			buttons.push_back (Tool::BarSeparator ());
		}
		else
		{
			// translate buttonId to cmdId
			int idx;
			for (idx = 0; _buttonItems [idx].buttonId != Item::idEnd; ++idx)
				if (_buttonItems [idx].buttonId == buttonId)
					break;
			Assert (_buttonItems [idx].buttonId != Item::idEnd);
			int cmdId = _cmdIds [idx];
			buttons.push_back (Tool::BarButton (buttonId, cmdId));
		}
	}
	AddButtons (buttons);
}

//
// Toolbar data
//

Button::Button ()
{
	memset (this, 0, sizeof (TBBUTTON));
	fsState = TBSTATE_ENABLED;
}

BarSeparator::BarSeparator (int width)
{
	fsStyle = TBSTYLE_SEP;
	iBitmap = width;
}

BarButton::BarButton (int buttonId, int cmdId)
{
	fsStyle = TBSTYLE_BUTTON;
    iBitmap = buttonId;
    idCommand = cmdId;
}

ToolWindow::ToolWindow (Win::Dow::Handle hwndTool, Win::Dow::Handle hwndToolBar)
{
	memset (this, 0, sizeof (TOOLINFO));
	cbSize = sizeof (TOOLINFO);
	uFlags = TTF_IDISHWND | TTF_SUBCLASS;	// Tool id is a window handle and subclass tooltip control, so we
											// don't have to handle mouse messages send to the tool window.
											// Tooltip control does this for us.
	uId = reinterpret_cast<UINT>(hwndTool.ToNative ());	// Window added to the toolbar
    hwnd = hwndToolBar.ToNative ();						// Toolbar window that contains tool window
	lpszText = LPSTR_TEXTCALLBACK;			// Send TTN_NEEDTEXT message to the toolbar window
}

bool ToolTipHandler::OnNotify (NMHDR * hdr, long & result)
{
	// hdr->code
	// hdr->idFrom;
	// hdr->hwndFrom;
	switch (hdr->code)
	{
	case TTN_NEEDTEXT:
		{
			Tool::Tip * tip = reinterpret_cast<Tool::Tip *>(hdr); 
			if (tip->IsHwndFrom ())
			{
				return OnNeedText (reinterpret_cast<Tool::TipForWindow *>(tip));
			}
			else
			{
				Assert (tip->IsIdFrom ());
				return OnNeedText (reinterpret_cast<Tool::TipForCtrl *>(tip));
			}
		}
	}
	return false;
}
