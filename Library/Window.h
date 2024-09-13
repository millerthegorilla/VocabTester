#ifndef WINDOW_H
#define WINDOW_H

#include <windows.h>
#include "stdstring"

namespace Win
{

template <class T>
inline T GetLong( HWND hwnd, int which = GWL_USERDATA )
{ return reinterpret_cast<T>(::GetWindowLong(hwnd, which));
}

template <class T>
inline void SetLong( HWND hwnd, T value, int which = GWL_USERDATA )
{ ::SetWindowLong(hwnd, which, reinterpret_cast<long>(value) );
}

class Dow
{
  public:
    explicit Dow( HWND hwnd = 0 )
    : _hwnd(hwnd) 
    {}

	void Init( HWND hwnd )
    { _hwnd = hwnd; 
    }

    void InitDlg( HWND dlg, int id )
    { _hwnd = ::GetDlgItem(dlg, id);
    }

		operator HWND( void )
    { return _hwnd; 
    }

    void AdjustRect( RECT& rect, bool menu = false )
    { ::AdjustWindowRect(&rect, GetStyle(), menu);
    }

    Win::Dow Activate( void )
    { return Win::Dow(::SetActiveWindow(_hwnd));
    }

		Win::Dow GetParent( void ) const
    { return ::GetParent(_hwnd); 
    }

		HINSTANCE GetInstance( void ) const
		{ return reinterpret_cast<HINSTANCE>(::GetWindowLong(_hwnd, GWL_HINSTANCE));
    }

    DWORD GetStyle( void ) const
    { return ::GetWindowLong(_hwnd, GWL_STYLE); 
    }

    void SetStyle( DWORD style )
    { ::SetWindowLong(_hwnd, GWL_STYLE, style);
    }

    DWORD GetExStyle( void ) const
    { return ::GetWindowLong(_hwnd, GWL_EXSTYLE); 
    }

    void SetExStyle( DWORD exStyle )
    { ::SetWindowLong(_hwnd, GWL_EXSTYLE, exStyle);
    }

	LRESULT SendMessage( UINT msg, WPARAM wParam = 0, LPARAM lParam = 0 )
	{	return ::SendMessage(_hwnd, msg, wParam, lParam); 
    }

    LRESULT SendNotifyMessage( UINT msg, WPARAM wParam = 0, LPARAM lParam = 0 )
	{	return ::SendNotifyMessage(_hwnd, msg, wParam, lParam); 
    }

	void PostMessage( UINT msg, WPARAM wParam = 0, LPARAM lParam = 0 )
	{	::PostMessage(_hwnd, msg, wParam, lParam ); 
    }

	void Destroy( void )
	{	::DestroyWindow(_hwnd);
    }

		// Focus
		void SetFocus( void )
		{	::SetFocus(_hwnd);
    }

		bool HasFocus( void ) const
		{	return ::GetFocus() == _hwnd; 
    }

		// Capture
		void CaptureMouse( void )
		{	::SetCapture(_hwnd); 
    }

		void ReleaseMouse( void )
		{	if ( HasCapture() )
      { ::ReleaseCapture();	
      }
    }

		bool HasCapture( void ) const
		{	return ::GetCapture() == _hwnd; 
    }

		// Text/Caption
		void SetText( TCHAR const text[] ) 
		{ ::SetWindowText(_hwnd, text);
    }

    int GetTextLength( void ) const
    { return ::GetWindowTextLength(_hwnd);
    }

	stdstring GetText( void ) const
    { int len = GetTextLength();
	stdstring text;
      text.reserve(len + 1);
      text.resize(len);
      ::GetWindowText(_hwnd, &text[0], len + 1);
      return text;
    }

		// Font
		void SetFont( HFONT hFont )
		{ SendMessage(WM_SETFONT, reinterpret_cast<WPARAM>(hFont), MAKELPARAM(TRUE, 0));
		}

    HFONT GetFont( void )
    { return reinterpret_cast<HFONT>(SendMessage(WM_GETFONT));
    }

		// Window visibility
		void Show( int cmdShow = SW_SHOW ) 
		{ ::ShowWindow(_hwnd, cmdShow); 
    }

		void Hide( void ) 
		{ ::ShowWindow(_hwnd, SW_HIDE); 
    }

    bool IsVisible( void ) const
    { return ::IsWindowVisible(_hwnd) == TRUE; 
    }

    bool IsMinimized( void ) const
    { return ::IsIconic(_hwnd) == TRUE;
    }

    bool IsMaximized( void ) const
    { return ::IsZoomed(_hwnd) == TRUE; 
    }

    void InvalidateRect( RECT const& rect, bool erase = true )
    { ::InvalidateRect(_hwnd, &rect, erase ? TRUE : FALSE); 
    }

    void Invalidate( bool erase = true )
    { ::InvalidateRect(_hwnd, 0, erase ? TRUE : FALSE); 
    }

		void Update( void ) 
		{ ::UpdateWindow(_hwnd); 
    }
    
    void Display( int cmdShow = SW_SHOW )
	{ Show( cmdShow );
      Update();
		}

    void Enable( bool enable = true )
    { ::EnableWindow(_hwnd, enable);	
    }

		void Disable( void )
		{	Enable(false); 
    }

    bool IsEnabled( void ) const
    { return ::IsWindowEnabled(_hwnd) != FALSE; 
    }

		// Moving
		void Move( int x, int y, int width, int height, bool repaint = true )
		{ ::MoveWindow(_hwnd, x, y, width, height, repaint ? TRUE : FALSE); 
    }

    void GetWindowRect( RECT& rect ) const
    { ::GetWindowRect(_hwnd, &rect); 
    }

    void GetClientRect( RECT& rect ) const
    { ::GetClientRect(_hwnd, &rect); 
    }

    void ScreenToClient( POINT& pt ) const
    { ::ScreenToClient(_hwnd, &pt); 
    }

    void ScreenToClient( RECT& r ) const
    { { POINT pt = {r.left, r.top};
        ScreenToClient(pt);
        r.left = pt.x;
        r.top = pt.y;
      }
      { POINT pt = {r.right, r.bottom};
        ScreenToClient(pt);
        r.right = pt.x;
        r.bottom = pt.y;
      }
    }

    void ClientToScreen( POINT& pt ) const
    { ::ClientToScreen(_hwnd, &pt); 
    }

    void ClientToScreen( RECT& r ) const
    { { POINT pt = {r.left, r.top};
        ClientToScreen(pt);
        r.left = pt.x;
        r.top = pt.y;
      }
      { POINT pt = {r.right, r.bottom};
        ClientToScreen(pt);
        r.right = pt.x;
        r.bottom = pt.y;
      }
    }

    // Menu
    HMENU GetMenu( void ) const
    { return ::GetMenu(_hwnd); 
    }

    // Timer
    void SetTimer( UINT id, UINT delay )
    { ::SetTimer(_hwnd, id, delay, 0);
    }

    void KillTimer( UINT id )
    { ::KillTimer(_hwnd, id);
    }

  protected: // Methods
    LRESULT SendMessage( UINT msg, WPARAM wParam = 0, LPARAM lParam = 0 ) const
		{	return ::SendMessage(_hwnd, msg, wParam, lParam); 
    }    

	protected: // Data members
		HWND _hwnd;
};

} // namespace Win

#endif // WINDOW_H