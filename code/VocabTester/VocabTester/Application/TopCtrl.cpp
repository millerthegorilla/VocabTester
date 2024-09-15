//----------------------------
// (c) Reliable Software, 2004
//----------------------------
#include "precompiled.h"
#include "TopCtrl.h"
#include "OutSink.h"
#include "WinTestRegistry.h"
#include "../Resource/resource.h"
#include "Commander.h"
#include "AccelTable.h"
#include "Menu.h"
#include "View.h"
#include <Sys/WinString.h>
#include <Win/MsgLoop.h>
#include <Ctrl/Splitter.h>
#include <Ctrl/Messages.h>
#include <Win/WinClass.h>

TopCtrl::TopCtrl (char const * cmdLine, Win::MessagePrepro & msgPrepro)
: _initMsg ("InitMessage"),
  _msgPrepro (msgPrepro),
  _ready (false),
  _cmdLine (cmdLine),
  _splitRatio(50)
{
}

TopCtrl::~TopCtrl ()
{}

bool TopCtrl::OnCreate (Win::CreateData const * create, bool & success) throw ()
{
	Win::Dow::Handle win = GetWindow();
	ResString caption (win.GetInstance (), ID_CAPTION);

	Win::Rect rc;
	win.GetWindowRect (rc);

	
	try
	{
	
		{//right child window
			Win::Maker rightmaker(ID_RIGHTWIN, win.GetInstance ());
			rightmaker.SetOwner (win);
			rightmaker.Style() << Win::Style::Child;
			rightmaker.SetPosition(rc.Width()/2 + splitWidth, 0, rc.Width()/2 - splitWidth, rc.Height());
			_rightWinHandle = rightmaker.Create (&_rightWinCtrl);
			_rightWinHandle.Show ();
		}

		{//left child window
			Win::Maker leftmaker(ID_LEFTWIN,win.GetInstance());
			leftmaker.SetOwner (win);
			leftmaker.Style() << Win::Style::Child;
			leftmaker.SetPosition(0,0,rc.Width()/2,rc.Height());
			_leftWinHandle = leftmaker.Create (&_leftWinCtrl);
			_leftWinHandle.Show ();
		}
		//splitter bar
		Splitter::MakeWindow (_splitterHandle,win,ID_SPLITTER);
		
		//error msgboxs
		TheOutput.Init (0, caption);

		win.SetText (caption);
		{//menu
			_commander.reset (new Commander (*this));
			_cmdVector.reset (new CmdVector (Cmd::Table, _commander.get ()));

			Accel::Maker accelMaker (Accel::Keys, *_cmdVector);
			_kbdAccel = accelMaker.Create ();
			_msgPrepro.SetKbdAccelerator (_h, _kbdAccel);
			_menu.reset (new Menu::DropDown (Menu::barItems, *_cmdVector));
			_menu->AttachToWindow (_h);
		}
		_view.reset (new View (_h));

		win.PostMsg (_initMsg);
		success = true;
	}
	catch (Win::Exception e)
	{
		TheOutput.Display (e);
		success = false;
	}
	catch (...)
	{
		Win::ClearError ();
		TheOutput.Display ("Initialization -- Unknown Error", Out::Error);
		success = false;
	}
	TheOutput.Init (win, caption);
	_ready = true;
	return true;
}

void TopCtrl::OnInit ()
{
	//GetWindow ().SetText (_cmdLine.c_str ());
	
#if defined(DIAGNOSTIC)
	FilePath userDesktopPath;
	ShellMan::VirtualDesktopFolder userDesktop;
	userDesktop.GetPath (userDesktopPath);
	if (!Dbg::TheLog.IsOn ()) 
		Dbg::TheLog.Open ("TestWin.txt", userDesktopPath.GetDir ());
#endif
}

bool TopCtrl::OnDestroy () throw ()
{
	try
	{
		Win::Placement placement (GetWindow ());
		TheRegistry.SavePlacement (placement);
	}
	catch (...)
	{}
	Win::Quit ();
	return true;
}

bool TopCtrl::OnRegisteredMessage (Win::Message & msg) throw ()
{
	if (msg == _initMsg)
	{
		OnInit ();
		return true;
	}
	return false;
}

bool TopCtrl::OnSize (int width, int height, int flag) throw ()
{
	if (_ready)
		_view->Size (width, height);
	_cx = width;
	_cy = height;
	int xSplit = (_cx * _splitRatio) / 100;
	if (xSplit < 0)
		xSplit = 0;
	_splitterHandle.MoveDelayPaint (xSplit, 0, splitWidth, _cy);
	_leftWinHandle.Move (0, 0, xSplit, _cy);
    _rightWinHandle.Move (xSplit + splitWidth, 0, _cx - xSplit - splitWidth, _cy);


	return true;
}

bool TopCtrl::OnUserMessage (Win::UserMessage & msg) throw ()
{
	switch(msg.GetMsg ())
	{
	case UM_MOVESPLITTER:
		{
			int x = msg.GetWParam();
			_splitRatio = x * 100 / _cx;
			if (_splitRatio < 0)
				_splitRatio = 0;
			else if (_splitRatio > 100)
				_splitRatio = 100;
			OnSize (_cx, _cy, 0);
		}
	}
	//switch usermessage - check for splitter
	return false;
}

bool TopCtrl::OnInitPopup (Menu::Handle menu, int pos) throw ()
{
	try
	{
		_menu->RefreshPopup (menu, pos);
	}
	catch (...) 
	{
		Win::ClearError ();
		return false;
	}
	return true;
}

bool TopCtrl::OnMenuSelect (int id, Menu::State state, Menu::Handle menu) throw ()
{
	if (state.IsDismissed ())
	{
		// Menu dismissed
	}
	else if (!state.IsSeparator () && !state.IsSysMenu () && !state.IsPopup ())
	{
		// Menu item selected
		char const * cmdHelp = _cmdVector->GetHelp (id);
	}
	else
	{
		return false;
	}
	return true;
}

Notify::Handler * TopCtrl::GetNotifyHandler (Win::Dow::Handle winFrom, unsigned idFrom) throw ()
{
	return 0;
}

bool TopCtrl::OnCommand (int cmdId, bool isAccel) throw ()
{
	if (isAccel)
	{
		// Revisit: maybe we should have Test method that takes cmdId?
		// Keyboard accelerators to invisible or disabled menu items are not executed
		char const * name = _cmdVector->GetName (cmdId);
		if (_cmdVector->Test (name) != Cmd::Enabled)
			return true;
	}
	MenuCommand (cmdId);
	return true;
}

void TopCtrl::MenuCommand (int cmdId)
{
	Win::ClearError ();
	try
	{
		_cmdVector->Execute (cmdId);
	}
	catch (Win::ExitException e)
	{
		TheOutput.Display (e);
		_h.Destroy ();
	}
	catch (Win::Exception e)
	{
		TheOutput.Display (e);
	}
	catch (std::bad_alloc bad)
	{
		Win::ClearError ();
		TheOutput.Display ("Operation aborted: not enough memory.");
	}
	catch (...)
	{
		Win::ClearError ();
		TheOutput.Display ("Internal Error: Command execution failure", Out::Error);
	}
}

bool TopCtrl::OnLButtonDown (int x, int y, Win::KeyState kState) throw ()
{
	return false;
}