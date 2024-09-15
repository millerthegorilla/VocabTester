#include "precompiled.h"
#include "RightWinCtrl.h"

void RightWinCtrl::OnInit ()
{
	//load icons and add to iconvector
	Icon::Maker iconmaker;
	iconmaker.SetSize(32,32);
	Icon::AutoHandle ah = iconmaker.Load(GetWindow().GetInstance(),IDI_MAIN);
	_imageindex = _imageList.AddIcon(ah);
	//_canvas.reset(new Win::Canvas ());
	BOOL bob = ImageList_Draw(_imageList.ToNative(),_imageindex,::GetDC(GetWindow().ToNative()),30,10, ILD_NORMAL );
	GetWindow().ForceRepaint();
}

bool RightWinCtrl::OnLButtonDown (int x, int y, Win::KeyState kState)
{
	return false;
}

bool RightWinCtrl::OnCreate (Win::CreateData const * create, bool & success) throw ()
{
	success = true;
	GetWindow().PostMsg (_initMsg);
	return true;
}

bool RightWinCtrl::OnRegisteredMessage (Win::Message & msg) throw ()
{
	if (msg == _initMsg)
	{
		OnInit ();
		return true;
	}
	return false;
}

bool RightWinCtrl::OnSize (int width, int height, int flag) throw ()
{
	BOOL bob = ImageList_Draw(_imageList.ToNative(),_imageindex,::GetDC(GetWindow().ToNative()),30,10, ILD_NORMAL );
	//GetWindow().Invalidate();
	return true;
}

bool RightWinCtrl::OnPaint () throw ()
{
	BOOL bob = ImageList_Draw(_imageList.ToNative(),0,::GetDC(GetWindow().ToNative()),30,10, ILD_NORMAL );
	return false;
}