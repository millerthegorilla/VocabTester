//------------------------------------
//  control.cpp
//  (c) Reliable Software, 1996, 97
//------------------------------------

#include "control.h"
#include "..\Resources\resource.h"
#include "message.h"
#include "..\Library\Maker.h"
#include "..\Library\Splitter.h"
#include "main.h"
#include "..\Library\FolderTree.h"


LRESULT CALLBACK PaneClass::WndProc (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	
	switch (message)
    {
    case WM_CREATE:
		return 0;
		break;
	}
	// Do-nothing window procedure
    return ::DefWindowProc (hwnd, message, wParam, lParam);
}

LRESULT CALLBACK MainClass::WndProc (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    WinController * pCtrl = Win::GetLong<WinController *> (hwnd,0);

    switch (message)
    {
    case WM_CREATE:
        try
        {
            pCtrl = new WinController (hwnd);//, reinterpret_cast<CREATESTRUCT *>(lParam));
			Win::SetLong<WinController *> (hwnd, pCtrl,0);
        }
        catch (char const * msg)
        {
            MessageBox (hwnd, msg, "Initialization", 
                MB_ICONEXCLAMATION | MB_OK);
            return -1;
        }
        catch (...)
        {
            MessageBox (hwnd, "Unknown Error", "Initialization", 
                MB_ICONEXCLAMATION | MB_OK);
            return -1;
        }
        return 0;
	case WM_NOTIFY:
		if(wParam == pCtrl->GetSplitterId ())
		{
			Win::VSplitterBar::Notify * notify = reinterpret_cast<Win::VSplitterBar::Notify*> (lParam);
			switch(notify->GetCode ())
			{
			case Win::VSplitterBar::BEGINDRAG:
				return 0;
				break;
			case Win::VSplitterBar::MOVESPLITTER:
				pCtrl->MoveSplitter (notify->GetX());
				return 0;
				break;
			}
			//size panes and move splitter
			return 0;
		}
		return 0;
    case WM_SIZE:
        pCtrl->Size (LOWORD(lParam), HIWORD(lParam));
        return 0;
	case MSG_MOVESPLITTER:
		pCtrl->MoveSplitter (wParam);
		return 0;
    case WM_DESTROY:
		Win::SetLong<WinController *> (hwnd, 0, 0);
		delete pCtrl;
		return 0;
    }

    return ::DefWindowProc (hwnd, message, wParam, lParam);
}

WinController::WinController (HWND hwnd)//, CREATESTRUCT * pCreat)
   :
Win::Controller (hwnd),
	_hwnd(hwnd),
	_leftWin (0),
	_rightWin (0),
	_splitRatio (50),
	_vBarMaker(_hwnd,ID_SPLITTER, WS_VISIBLE, 0, 200, 0, splitWidth, 400)
{
    // Create child windows
	{
		Win::ControlMaker leftWinMaker ("PaneClass", _hwnd, ID_LEFT_WINDOW);
		_leftWin.Init (leftWinMaker.Create());

		_leftWin.Display ();
	}

	{
		Win::ControlMaker rightWinMaker ("PaneClass", _hwnd, ID_RIGHT_WINDOW);
		_rightWin.Init (rightWinMaker.Create());

		_rightWin.Display ();
	}
	
	_vSplitter = _vBarMaker.Create(TEXT(""),&_splitNotifier);	
	int bob = _vSplitter->GetId();
	//_splitter.Display();
	//Splitter::MakeWindow (_splitter, _hwnd, ID_SPLITTER);
}

WinController::~WinController ()
{
    ::PostQuitMessage (0);
}

void WinController::Size (int cx, int cy) 
{
	_cx = cx;
	_cy = cy;
	int xSplit = (_cx * _splitRatio) / 100;
	if (xSplit < 0)
		xSplit = 0;
	_vSplitter->Move(xSplit, 0, splitWidth, cy, false);
	_leftWin.Move (0, 0, xSplit, cy);
    _rightWin.Move (xSplit + splitWidth, 0, cx - xSplit - splitWidth, cy);

	_vSplitter->Display ();
}


void WinController::MoveSplitter (int x)
{
	_splitRatio = x * 100 / _cx;
	if (_splitRatio < 0)
		_splitRatio = 0;
	else if (_splitRatio > 100)
		_splitRatio = 100;
	Size (_cx, _cy);
}


