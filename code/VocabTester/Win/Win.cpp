//
// (c) Reliable Software 1997-2002
//
#include <WinLibBase.h>
#include <Ctrl/Messages.h>
#include <Sys/SharedMem.h>
#include <Ctrl/Accelerator.h>
#include <Win/Region.h>
#include <Win/Message.h>
#include <Win/Atom.h>
#include <Win/Procedure.h>
#include <Win/Controller.h>

using namespace Win;

template<>
void Win::Disposal<Win::window_handle>::Dispose (Win::window_handle h) throw ()
{
	::DestroyWindow (h.ToNative ());
}

void Win::Quit (int errorCode)
{
	::PostQuitMessage (errorCode);
}

void Dow::Handle::SendMsg (Message & msg) const
{
	Assert (!IsNull ());
	LRESULT result = ::SendMessage (H (), msg.GetMsg (), msg.GetWParam (), msg.GetLParam ());
	msg.SetResult (result);
}

bool Dow::Handle::PostMsg (Message const & msg) const
{
	Assert (!IsNull ());
	return ::PostMessage (H (), msg.GetMsg (), msg.GetWParam (), msg.GetLParam ()) != FALSE;
}

void Dow::Handle::ClientToScreen (Win::Point & pt) const
{
	::ClientToScreen (H (), &pt);
}

void Dow::Handle::Invalidate (Win::Rect const & rect)
{
	::InvalidateRect (H (), &rect, TRUE);
}

void Dow::Handle::GetClientRect (Win::Rect & rect) const
{
	::GetClientRect (H (), &rect);
}

void Dow::Handle::GetWindowRect (Win::Rect & rect) const
{
	::GetWindowRect (H (), &rect);
}

void Dow::Handle::Move (Win::Rect const & rect)
{
	::MoveWindow (H (), rect.Left (), rect.Right (), rect.Width (), rect.Height (), TRUE);
}

void Dow::Handle::Scroll (Win::Rect & rect, int xAmount, int yAmount)
{
	::ScrollWindow (H (), xAmount, yAmount, &rect, 0);
}

void Dow::Handle::SendInterprocessPackage (RegisteredMessage & msg) const
{
	unsigned int packageLen = msg.GetWParam ();
	void const * package = reinterpret_cast<void const *>(msg.GetLParam ());
	Assert (package != 0);
	SharedMem buf (packageLen);
	GlobalAtom memName (buf.GetName ().c_str ());
	if (!memName.IsOK ())
		throw Exception ("Internal error: Cannot prepare interprocess package");
	memcpy (&buf [0], package, packageLen);
	UserMessage ipcMsg (UM_INTERPROCESS_PACKAGE, msg.GetMsg (), memName.GetAtom ());
	SendMsg (ipcMsg);
	msg.SetResult (ipcMsg.GetResult ());
}

Brush::Handle Dow::Handle::GetBgBrush() const
{
	return reinterpret_cast<HBRUSH> (::GetClassLong(H (), GCL_HBRBACKGROUND));
}

void Dow::Handle::SetClassBgBrush (Brush::Handle brush)
{
	::SetClassLongPtr (H (), GCLP_HBRBACKGROUND, reinterpret_cast <long> (brush.ToNative ()));
}

Font::Handle Dow::Handle::GetFont() const
{
	return reinterpret_cast<HFONT> (SendMsg(WM_GETFONT));
}

void Dow::Handle::SetFont (Font::Handle font)
{
	BOOL fRedraw = TRUE;
	SendMsg (WM_SETFONT, (WPARAM) font.ToNative (), MAKELPARAM(fRedraw, 0));
}

void Dow::Handle::SetIcon (Icon::Handle icon)
{
	Win::Message msg (WM_SETICON, true, LPARAM (icon.ToNative ()));
	SendMsg (msg);
}

void Dow::Handle::ShiftTo (int x, int y)
{
	if (!::SetWindowPos (H (),
					NULL, // insert after (in z-order)
					x,
					y,
					0,
					0,
					SWP_NOSIZE | SWP_NOZORDER))
	{
		throw Win::Exception ("Cannot position window");
	}
}

void Dow::Handle::Size (int width, int height)
{
	if (!::SetWindowPos (H (),
					NULL, // insert after (in z-order)
					0,
					0,
					width,
					height,
					SWP_NOMOVE | SWP_NOZORDER))
	{
		throw Win::Exception ("Cannot position window");
	}
}

void Dow::Handle::Position (Win::Rect const & rect)
{
	if (!::SetWindowPos (H (),
					NULL, // insert after (in z-order)
					rect.Left (),
					rect.Top (),
					rect.Width (),
					rect.Height (),
					SWP_NOZORDER))
	{
		throw Win::Exception ("Cannot position window");
	}
}

bool Dow::Handle::SetPlacement (Placement const & placement) const
{
	Assert (!IsNull ());
	return ::SetWindowPlacement (H (), placement.Get ()) != FALSE;
}

bool Dow::Handle::SetClippingTo (Region::AutoHandle region)
{
	return ::SetWindowRgn (H (), region.release (), true) != 0;
}

void Dow::Handle::CenterOverParent ()
{
	Dow::Handle hParent (GetParent ());
	Rect parentRect;
	hParent.GetWindowRect (parentRect);
	Rect thisWndRect;
	GetWindowRect (thisWndRect);
	int x = parentRect.left + (parentRect.Width() - thisWndRect.Width()) / 2;
	int y = parentRect.top + (parentRect.Height() - thisWndRect.Height()) / 2;
	MoveDelayPaint (x, y, thisWndRect.Width(), thisWndRect.Height());
}

void Dow::Handle::CenterOverScreen ()
{
	Dow::Handle hScreen (GetDesktopWindow ());
	Rect screenRect;
	hScreen.GetWindowRect (screenRect);
	Rect thisWndRect;
	GetWindowRect (thisWndRect);
	int x = (screenRect.Width () - thisWndRect.Width ()) / 2;
	int y = (screenRect.Height () - thisWndRect.Height ()) / 2;
	MoveDelayPaint (x, y, thisWndRect.Width (), thisWndRect.Height ());
}

void Dow::Handle::CenteredOverScreen (bool topMost)
{
	Dow::Handle hScreen (GetDesktopWindow ());
	Rect screenRect;
	hScreen.GetWindowRect (screenRect);
	Rect thisWndRect;
	GetWindowRect (thisWndRect);
	int x = (screenRect.Width () - thisWndRect.Width ()) / 2;
	int y = (screenRect.Height () - thisWndRect.Height ()) / 2;
	::SetWindowPos (H (),
					topMost ? HWND_TOPMOST :	// Places the window above all non-topmost windows. The window maintains its topmost position even when it is deactivated.
							  HWND_NOTOPMOST,	// Places the window above all non-topmost windows (that is, behind all topmost windows).
					x, y,
					0, 0,
					SWP_NOSIZE);	// Retain the current window size (ignores cx and cy parameters)
}

void Dow::Handle::SubClass (SubController * subCtrl)
{
	// get previous window procedure and controller (if any)
	ProcPtr prevProc = GetLong<ProcPtr>(GWL_WNDPROC);

	Controller * prevCtrl = GetLong<Controller *> ();
	// remember them in the new controller
	subCtrl->Init (H (), prevProc, prevCtrl);
	// attach new controller to window
	SetLong<SubController *>(subCtrl);
	// attach new window procedure to window
	SetLong(reinterpret_cast<long> (SubProcedure), GWL_WNDPROC);
}

void Dow::Handle::UnSubClass ()
{
	// Get the current subclass controller
	SubController * pCtrl = GetLong<SubController *>();
	// restore previous window procedure and controller (if any)
	SetLong<Win::ProcPtr> (pCtrl->GetPrevProc(), GWL_WNDPROC);
	SetLong<Win::Controller *> (pCtrl->GetPrevController ());
}

//-----------------------
// Placement methods
//-----------------------

Placement::Placement ()
{
	memset (this, 0, sizeof (WINDOWPLACEMENT));
	_pl.length = sizeof (WINDOWPLACEMENT);
}

Placement::Placement (Dow::Handle win)
{
	Init (win);
}

void Placement::Init (Dow::Handle win)
{
	Assert (!win.IsNull ());
	_pl.length = sizeof (WINDOWPLACEMENT);
	::GetWindowPlacement (win.ToNative (), &_pl);
}

int  Placement::GetFlags () const
{
	return _pl.flags;
}

bool Placement::IsMaximized () const
{
	return _pl.showCmd == SW_SHOWMAXIMIZED;
}

bool Placement::IsMinimized () const
{
	return _pl.showCmd == SW_SHOWMINIMIZED;
}

bool Placement::IsHidden () const
{
	return _pl.showCmd == SW_HIDE;
}

void  Placement::GetMaxCorner (Win::Point & pt) const
{
	pt = _pl.ptMaxPosition;
}

void  Placement::GetMinCorner (Win::Point & pt) const
{
	pt = _pl.ptMinPosition;
}

void  Placement::GetRect (Win::Rect & rect) const
{
	rect = _pl.rcNormalPosition;
}

void Placement::SetFlags (int flags)
{
	_pl.flags = flags;
}

void Placement::SetNormal ()
{
	_pl.showCmd = SW_SHOWNORMAL;
}

void Placement::SetMaximized ()
{
	_pl.showCmd = SW_SHOWMAXIMIZED;
}

void Placement::SetMinimized ()
{
	_pl.showCmd = SW_SHOWMINIMIZED;
}

void Placement::SetHidden ()
{
	_pl.showCmd = SW_HIDE;
}

void Placement::SetRect (Win::Rect & rect)
{
	_pl.rcNormalPosition = rect;
}

void Placement::SetMinCorner(Win::Point & pt)
{
	_pl.ptMinPosition = pt;
}

void Placement::SetMaxCorner(Win::Point & pt)
{
	_pl.ptMaxPosition = pt;
}

void Placement::CombineShowCmd (int cmdShow)
{
	if (cmdShow == SW_SHOWDEFAULT)
	{
		//	SW_SHOWDEFAULT instructs Windows to use the show value specified in
		//	the STARTUPINFO passed to CreateProcess.  We want to test against
		//	that final value
		STARTUPINFO startUpInfo;
		::GetStartupInfo (&startUpInfo);
		cmdShow = startUpInfo.wShowWindow;
	}

	if (cmdShow != SW_SHOW && cmdShow != SW_SHOWNORMAL)
		_pl.showCmd = cmdShow;
}
