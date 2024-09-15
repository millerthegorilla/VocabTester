#if !defined (WIN_DOW_H)
#define WIN_DOW_H
// (c) Reliable Software (c) 1998 -- 2004
#include <Win/Instance.h>

namespace Win
{
	class Message;
	class RegisteredMessage;
	class Placement;
	class Controller;
	class SubController;
	class Point;
	class Rect;

	// To set various styles, use operator << , for instance
	// style << AddClientEdge << AddVScrollBar << AddHScrollBar;
	class Style
	{
	public:
		enum Bits
		{
		    AddSysMenu = WS_SYSMENU,
		    AddVScrollBar = WS_VSCROLL,
		    AddHScrollBar = WS_HSCROLL,
		    AddDlgBorder = WS_DLGFRAME,
		    AddBorder = WS_BORDER,
		    AddTitleBar = WS_CAPTION,
		    ClipChildren= WS_CLIPCHILDREN,
		    ClipSiblings= WS_CLIPSIBLINGS,
		    Visible = WS_VISIBLE,
		    Child = WS_CHILD,
		    Popup = WS_POPUP,
		    OverlappedWindow = WS_OVERLAPPEDWINDOW
		};

		class Ex
		{
		public:
			enum Bits
			{
				AddClientEdge = WS_EX_CLIENTEDGE
			};
		};
	public:
		Style (unsigned long style = 0, unsigned long exStyle = 0)
			: _style (style), _exStyle (exStyle) 
		{}
		unsigned long GetStyleBits () { return _style; }
		unsigned long GetExStyleBits () { return _exStyle; }
		Style & Clear () { _style = 0; _exStyle = 0; return *this; }
		void OrIn (Style::Bits bits)
		{
			_style |= bits;
		}
		void OrIn (Style::Ex::Bits bits)
		{
			_exStyle |= bits;
		}
		void SetMaximized () { _style |= WS_MAXIMIZE; }
		// Turn off the "visible" bit for those makers that make visible windows by default
		void SetInvisible () { _style &= ~WS_VISIBLE; }
		void DontClipChildren () { _style &= ~WS_CLIPCHILDREN; }
	protected:
		unsigned long _style;
		unsigned long _exStyle;
	};
	inline Style & operator << (Style & style, Style::Bits bits)
	{
		style.OrIn (bits);
		return style;
	}
	inline Style & operator << (Style & style, Style::Ex::Bits bits)
	{
		style.OrIn (bits);
		return style;
	}

	//-----------------
	// Global functions
	//-----------------
	void Quit (int errorCode = 0);
	inline void Sleep (unsigned milliSec) { ::Sleep (milliSec); }

	// Don't use directly, Access as Win::Dow::Handle
	class window_handle: public Handle<HWND>
	{
	public:
		window_handle (HWND h = NullValue ())
			: Handle<HWND> (h) 
		{}
		void Init (window_handle h = window_handle ())
		{
			Reset (h.ToNative ());
		}
		// Get/Set Window Long
		template <class T>
		inline T GetLong (int which = GWL_USERDATA)
		{
			return reinterpret_cast<T> (::GetWindowLong (H (), which));
		}
		template<>	// specialization for long
		inline long GetLong (int which)
		{
			return ::GetWindowLong (H (), which);
		}

		template <class T>
		inline T SetLong (T value, int which = GWL_USERDATA)
		{
			return reinterpret_cast<T> (::SetWindowLong (H (), which, reinterpret_cast<long> (value)));
		}

		template<>	// specialization for long
		inline long SetLong<long> (long value, int which)
		{
			return ::SetWindowLong (H (), which, value);
		}
		template<>	// specialization for unsigned long
		inline unsigned long SetLong<unsigned long> (unsigned long value, int which)
		{
			return static_cast<unsigned long> (::SetWindowLong (H (), which, static_cast<long> (value)));
		}
		Win::Style GetStyle ()
		{
			return Win::Style ( ::GetWindowLong (H (), GWL_STYLE),
								::GetWindowLong (H (), GWL_EXSTYLE));
		}
		Win::Style SetStyle (Win::Style style)
		{
			return Win::Style (SetLong (style.GetStyleBits (), GWL_STYLE),
							   SetLong (style.GetExStyleBits (), GWL_EXSTYLE));
		}
		// Used to work around a bug in tracking context menu
		void ForceTaskSwitch ()
		{
			::PostMessage (H (), WM_NULL, 0, 0);
		}
		// Messages
		LRESULT SendMsg (UINT msg, WPARAM wparam = 0, LPARAM lparam = 0) const
		{
			Assert (H () != 0);
			return ::SendMessage (H (), msg, wparam, lparam);
		}
		void SendMsg (Message & msg) const;
		bool PostMsg (UINT msg, WPARAM wparam = 0, LPARAM lparam = 0) const
		{
			Assert (H () != 0);
			return ::PostMessage (H (), msg, wparam, lparam) != FALSE;
		}
		bool PostMsg (Message const & msg) const;
		void SendInterprocessPackage (RegisteredMessage & msg) const;
		Win::Instance GetInstance () const
		{ 
			return reinterpret_cast<HINSTANCE> (::GetWindowLongPtr (H (), GWL_HINSTANCE));		
		}
		//	return the window that defines the drawing origin
		window_handle GetParent () const
		{
			//	Windows ::GetParent returns the owner window for pop-up
			//	windows, so we only use it if this is a child window
			if (::GetWindowLong (H (), GWL_STYLE) & WS_CHILD)
			{
				return ::GetParent(H ());
			}

			//	for top level windows the drawing origin is defined by the desktop
			return ::GetDesktopWindow();
		}
		window_handle GetOwner () const
		{
			//	child windows are owned by their parents
			if (::GetWindowLong (H (), GWL_STYLE) & WS_CHILD)
			{
				return ::GetParent(H ());
			}

			//	::GetWindow will only return the owner for non-child windows
			return ::GetWindow(H (), GW_OWNER);
		}
		Win::Controller * GetController ()
		{
			return reinterpret_cast<Win::Controller *>(::GetWindowLongPtr (H (), GWL_USERDATA));
		}
		void SetParent (Handle<HWND> hwndParent)
		{
			::SetParent (H (), hwndParent.ToNative ());
		}
		int GetId () const // child windows and controls have ids
		{
			Assert (!IsNull ());
			return ::GetWindowLong (H (), GWL_ID);
		}
		// Destroying
		void Destroy ()
		{
			::DestroyWindow (H ());
		}
		Brush::Handle GetBgBrush() const;
		void SetClassBgBrush (Brush::Handle brush);
		// Change window class properties
		void SetClassHRedraw (bool yes)
		{
			// redraw whole window on horizontal resize
			DWORD style = ::GetClassLong (H (), GCL_STYLE);
			if (yes)
				style |= CS_HREDRAW;
			else
				style &= ~CS_HREDRAW;
			::SetClassLong (H (), GCL_STYLE, style);
		}
		 
		// Coordinates
		void ClientToScreen (Win::Point & pt) const;
		// Messages
		long SendMessageTo (int idChild, UINT msg, WPARAM wparam = 0, LPARAM lparam = 0) const
		{
			return ::SendDlgItemMessage (H (), idChild, msg, wparam, lparam);
		}
		// Register hwnddow for drag/drop messages
		void DragAcceptFiles ()
		{
			::DragAcceptFiles (H (), TRUE);
		}
		// Focus
		void SetFocus ()
		{ 
			::SetFocus (H ()); 
		}
		bool HasFocus () const
		{
			return ::GetFocus () == H ();
		}
		// Timer
		void SetTimer (int idTimer, int milliSec)
		{
			if (::SetTimer (H (), idTimer, milliSec, 0) == 0)
				throw Win::Exception ("Internal error: Cannot set timer.");
		}
		void KillTimer (int idTimer)
		{
			::KillTimer (H (), idTimer);
		}
		// Mouse capture
		void CaptureMouse ()
		{
			::SetCapture (H ());
		}
		static void ReleaseMouse ()
		{
			::ReleaseCapture ();
		}
		bool HasCapture () const
		{
			return ::GetCapture () == H ();
		}
		// Text/Caption/Class name
		void SetText (std::string const & text)
		{
			SetText (text.c_str ());
		}
		void SetText (char const * text) 
		{ 
			::SetWindowText (H (), text); 
		}
		int GetText (char * buf, int len) const// len includes null
		{ 
			return ::GetWindowText (H (), buf, len); 
		}
		int GetTextLength () const
		{
			return ::GetWindowTextLength(H ());
		}
		int GetClassName (char * buf, int len) const
		{
			// Revisit: there is better API RealGetWindowClass, but it is supported
			// only in Win98, NT 4.0 with SP3 and Win2000
			return ::GetClassName (H (), buf, len);
		}
		// Font
		Font::Handle GetFont() const;
		void SetFont (Font::Handle font);
		// Icon
		void SetIcon (Icon::Handle icon);
		// Visibility
		bool Exists () const
		{
			// An application should not use Exists for a window that it did not create
			// because the window could be destroyed after this function was called.
			// Further, because window handles are recycled the handle could even point
			// to a different window.
			return ::IsWindow (H ()) != 0;
		}
		void Enable ()
		{
			::EnableWindow (H (), TRUE);
		}

		void Disable ()
		{
			::EnableWindow (H (), FALSE);
		}

		bool IsEnabled ()
		{
			return ::IsWindowEnabled (H ()) != 0;
		}

		void Show (int cmdShow = SW_SHOW) 
		{ 
			::ShowWindow (H (), cmdShow); 
		}

		void ShowMaximized () { Show (SW_SHOWMAXIMIZED); }
		void ShowMinimized () { Show (SW_SHOWMINIMIZED); }

		void Restore ()
		{ 
			::ShowWindow (H (), SW_RESTORE); 
		}

		void Hide () 
		{ 
			::ShowWindow (H (), SW_HIDE); 
		}
		bool IsVisible()
		{
			return ::IsWindowVisible(H ()) != FALSE;
		}
		void Update () 
		{ 
			::UpdateWindow (H ()); 
		}
		void SetForeground ()
		{
			::SetForegroundWindow (H ());
		}
		void BringToTop ()
		{
			::BringWindowToTop (H ());
		}
		void Display (int cmdShow = SW_SHOW)
		{
			Show (cmdShow);
			Update ();
		}
		bool SetPlacement (Win::Placement const & placement) const;
		// Clipping
		bool SetClippingTo (Region::AutoHandle region);
		// Moving
		void Move (int x, int y, int width, int height)
		{
			::MoveWindow (H (), x, y, width, height, TRUE);
		}
		void Move (Win::Rect const & rect);
		void MoveDelayPaint (int x, int y, int width, int height)
		{
			::MoveWindow (H (), x, y, width, height, FALSE);
		}
		// Positioning
		void ShiftTo (int x, int y);
		void Size (int width, int height);
		void Position (Win::Rect const & rect);
		void CenterOverParent ();
		void CenterOverScreen ();
		void CenteredOverScreen (bool topMost);
		// Repainting
		void SetRedraw (bool value)
		{
			::SendMessage (H (), WM_SETREDRAW, value? TRUE: FALSE, 0);
		}
		void Invalidate ()
		{
			::InvalidateRect (H (), 0, TRUE);
		}
		void Invalidate (Win::Rect const & rect);
		void ForceRepaint ()
		{
			Invalidate ();
			Update ();
		}
		// Scrolling
		void Scroll (int xAmount, int yAmount)
		{
			::ScrollWindow (H (), xAmount, yAmount, 0, 0);
		}
		void Scroll (Win::Rect & rect, int xAmount, int yAmount);
		// Rectangles
		void GetWindowRect (Win::Rect & rect) const;
		void GetClientRect (Win::Rect & rect) const;

		//	Subclassing
		void SubClass (SubController * ctrl);
		void UnSubClass ();
	};

	class Dow
	{
	public:
		typedef window_handle Handle;
		typedef AutoHandle<Win::Dow::Handle> Owner;
	};

	class Placement
	{
	public:
		Placement ();
		Placement (Dow::Handle win);
		void Init (Dow::Handle win);

		int  GetFlags () const;
		bool IsMaximized () const;
		bool IsMinimized () const;
		bool IsHidden () const;
        void GetRect (Win::Rect & rect) const;
		void GetMaxCorner (Win::Point & pt) const;
		void GetMinCorner (Win::Point & pt) const;

		void CombineShowCmd (int cmdShow);
		void SetFlags (int flags);
		void SetMaximized ();
		void SetMinimized ();
		void SetHidden ();
		void SetNormal ();
		void SetRect (Win::Rect & rect);
		void SetMaxCorner (Win::Point & point);
		void SetMinCorner (Win::Point & point);

		WINDOWPLACEMENT const * Get () const { return &_pl; }

	private:
		WINDOWPLACEMENT _pl;
	};

	// For temporarily switching window style
	// Modify this style as needed and then call Switch
	class StyleHolder: public Win::Style
	{
	public:
		StyleHolder (Win::Dow::Handle win)
			: Win::Style (win.GetStyle ()), _win (win)  
		{
			// remember the old style for the destructor
			_oldStyle = Win::Style (GetStyleBits (), GetExStyleBits ());
		}
		~StyleHolder ()
		{
			_win.SetStyle (_oldStyle);
		}
		void Switch () { _win.SetStyle (*this); }
	private:
		Win::Dow::Handle _win;
		Win::Style _oldStyle;
	};

	// Temporarily delay repainting of window (and its children)
	// Note: it causes annoying desktop blinking
	class PaintLock
	{
	public:
		PaintLock (Win::Dow::Handle h)
		{
			if (!h.IsNull ())
			{
				BOOL result = ::LockWindowUpdate (h.ToNative ());
				// Assert that we are not nesting update locks
				Assert (result != 0);
			}
		}
		~PaintLock ()
		{
			::LockWindowUpdate (0);
		}
	};

	class RedrawLock
	{
	public:
		RedrawLock (Win::Dow::Handle h)
			: _h (h)
		{
			_h.SetRedraw (false);
		}
		~RedrawLock ()
		{
			_h.SetRedraw (true);
			_h.Invalidate ();
		}
	private:
		Win::Dow::Handle _h;
	};
}

#endif
