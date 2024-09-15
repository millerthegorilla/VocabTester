#include "precompiled.h"
#include "TvCtrl.h"
#include "OutSink.h"

TvCtrl::TvCtrl ( LeftWinCtrl &leftwinctrl )
{
	
	_templ.push_back (Menu::contextItem1 );


	_tvCtxCommander.reset (new TvCtxCommander (*this, GetWindow().ToNative(), leftwinctrl));
	_tvCtxCmdVector.reset (new TvCtxCmdVector (Cmd::TvCtxTable, _tvCtxCommander.get()));
	_treeContext.reset (new Menu::Context (_templ, *_tvCtxCmdVector));
}

bool TvCtrl::OnRButtonDown(int x, int y, Win::KeyState kState)
{
	HTREEITEM ht;
	POINT pt;
	TVHITTESTINFO  tvHitTest;
	pt.x = x;
	pt.y = y;
	tvHitTest.pt = pt;
	HWND hWnd = GetWindow().ToNative();
	ht = TreeView_HitTest(hWnd,&tvHitTest);
	if( ht != NULL)
	{
		TreeView_Select(hWnd,ht,TVGN_CARET);
		RECT windowrect;
		::GetWindowRect(hWnd,&windowrect);
		_treeContext->TrackMenu(hWnd, windowrect.left + x, windowrect.top + y);
	}
	return true;
}

bool TvCtrl::OnLButtonDown (int x, int y, Win::KeyState kState)
{
HTREEITEM ht;
	POINT pt;
	TVHITTESTINFO  tvHitTest;
	pt.x = x;
	pt.y = y;
	tvHitTest.pt = pt;
	HWND hWnd = GetWindow().ToNative();
	ht = TreeView_HitTest(hWnd,&tvHitTest);
	if( ht != NULL)
	{
		TreeView_Select(hWnd,ht,TVGN_CARET);
	}
	return false;
}

bool TvCtrl::OnInitPopup (Menu::Handle menu, int pos) throw ()
{
	try
	{
		_treeContext->RefreshPopup (menu, pos);
	}
	catch (...) 
	{
		Win::ClearError ();
		return false;
	}
	return true;
}
bool TvCtrl::OnCommand (int cmdId, bool isAccel)
{
	try
	{
		_tvCtxCmdVector->Execute(cmdId);
	}
	catch (Win::Exception e)
    {
        TheOutput.Display (e);
    }
	catch(...)
	{
		Win::ClearError ();
        TheOutput.Display ("Internal error: Unknown exception", Out::Error);
	}
	return true;
}

