#ifndef WINMESSAGEDATA_H
#define WINMESSAGEDATA_H

#include <windows.h>
#include <tchar.h>

namespace Win
{

class ActivateData
{
  public:
    ActivateData( WPARAM wParam, LPARAM lParam )
    : _wParam(wParam),
      _prev(reinterpret_cast<HWND>(lParam))
    {}

    HWND GetPrevWindow( void ) const { return _prev; }
    bool IsActivated( void ) const { return (LOWORD(_wParam)&(WA_ACTIVE|WA_CLICKACTIVE)) != 0; };
    bool ByClick( void ) const { return (LOWORD(_wParam)&WA_CLICKACTIVE) != 0; };
    bool IsMimimized( void ) const { return HIWORD(_wParam) != 0; }

  private:
    WPARAM _wParam;
    HWND   _prev;
};

class ActivateAppData
{
  public:
    ActivateAppData( WPARAM wParam, LPARAM lParam )
    : _activated(wParam != 0),
      _threadId(lParam)
    {}

    bool IsActivated( void ) const { return _activated; }
    DWORD GetThreadId( void ) const { return _threadId; }

  private:
    bool  _activated;
    DWORD _threadId;
};

class AskCBFormatNameData
{
  public:
    AskCBFormatNameData( WPARAM wParam, LPARAM lParam )
    : _buf(reinterpret_cast<TCHAR*>(lParam)),
      _bufSize(wParam)
    {}

    void SetFormatName( TCHAR const name[] )
    { ::_tcsncpy(_buf, name, _bufSize);
    }

  private:
    TCHAR* _buf;
    DWORD  _bufSize;
};

class KeyData
{
  public:
    KeyData( LPARAM lParam )
    : _data(lParam)
    {}

    int GetRepeatCount( void ) const { return _data&0x0000FFFF; }
    int GetScanCode( void ) const { return (_data&0x00FF0000) >> 16; }
    bool IsExtendedKey( void ) const { return (_data&0x01000000) != 0; }
    bool IsAltPressed( void ) const { return (_data&0x20000000) != 0; }
    bool WasPressed( void ) const { return (_data&0x40000000) != 0; }
    bool BeingPressed( void ) const { return (_data&0x80000000) == 0; }

  private:
    LPARAM _data;
};

class HotKeyData
{
  public:
    HotKeyData( WPARAM wParam, LPARAM lParam )
    : _id(wParam),
      _modifier(LOWORD(lParam)),
      _virtKey(HIWORD(lParam))
    {}

    int GetId( void ) const { return _id; }
    UINT GetVirtualKey( void ) const { return _virtKey; }
    bool IsAltPressed( void ) const { return (_modifier&MOD_ALT) != 0; }
    bool IsControlPressed( void ) const { return (_modifier&MOD_CONTROL) != 0; }
    bool IsShiftPressed( void ) const { return (_modifier&MOD_SHIFT) != 0; }
    bool IsWindowPressed( void ) const { return (_modifier&MOD_WIN) != 0; }

  private:
    int  _id;
    UINT _modifier;
    UINT _virtKey;
};

class KeyState
{
  public:
    KeyState( WPARAM wParam )
    : _state(wParam)
    {}

    bool IsControlPressed( void ) const { return (_state&MK_CONTROL) != 0; }
    bool IsLButtonPressed( void ) const { return (_state&MK_LBUTTON) != 0; }
    bool IsMButtonPressed( void ) const { return (_state&MK_MBUTTON) != 0; }
    bool IsRButtonPressed( void ) const { return (_state&MK_RBUTTON) != 0; }
    bool IsShiftPressed( void ) const { return (_state&MK_SHIFT) != 0; }

  private:
    WPARAM _state;
};

class MenuFlags
{
  public:
    MenuFlags( UINT flags )
    : _flags(flags)
    {}

    bool IsBitmap( void ) const { return (_flags&MF_BITMAP) != 0; }
    bool IsChecked( void ) const { return (_flags&MF_CHECKED) != 0; }
    bool IsDisabled( void ) const { return (_flags&MF_DISABLED) != 0; }
    bool IsGrayed( void ) const { return (_flags&MF_GRAYED) != 0; }
    bool IsHilited( void ) const { return (_flags&MF_HILITE) != 0; }
    bool IsSelectedByMouse( void ) const { return (_flags&MF_MOUSESELECT) != 0; }
    bool IsOwnerDrawn( void ) const { return (_flags&MF_OWNERDRAW) != 0; }
    bool IsPopup( void ) const { return (_flags&MF_POPUP) != 0; }
    bool IsSysMenu( void ) const { return (_flags&MF_SYSMENU) != 0; }

  private:
    UINT _flags;
};

class HitTestData
{
  public:
    HitTestData( int data )
    : _data(data)
    {}

    bool InBorder( void ) const { return _data == HTBORDER; }
    bool InBottom( void ) const { return _data == HTBOTTOM; }
    bool InBottomLeft( void ) const { return _data == HTBOTTOMLEFT; }
    bool InBottomRight( void ) const { return _data == HTBOTTOMRIGHT; }
    bool InCaption( void ) const { return _data == HTCAPTION; }
    bool InClient( void ) const { return _data == HTCLIENT; }
    bool InHScrollBar( void ) const { return _data == HTHSCROLL; }
    bool InLeft( void ) const { return _data == HTLEFT; }
    bool InMenu( void ) const { return _data == HTMENU; }
    bool InMinimizeBox( void ) const { return _data == HTREDUCE; }
    bool InRight( void ) const { return _data == HTRIGHT; }
    bool InSizeBox( void ) const { return _data == HTSIZE; }
    bool InSysMenu( void ) const { return _data == HTSYSMENU; }
    bool InTop( void ) const { return _data == HTTOP; }
    bool InTopLeft( void ) const { return _data == HTTOPLEFT; }
    bool InTopRight( void ) const { return _data == HTTOPRIGHT; }
    bool InVScrollBar( void ) const { return _data == HTVSCROLL; }
    bool InMaximizeBox( void ) const { return _data == HTZOOM; }

  private:
    int _data;
};

class EdgeData
{
  public:
    EdgeData( WPARAM wParam )
    : _data(wParam)
    {}

    bool IsBottom( void ) const { return (_data&WMSZ_BOTTOM) != 0; }
    bool IsBottomLeft( void ) const { return (_data&WMSZ_BOTTOMLEFT) != 0; }
    bool IsBottomRight( void ) const { return (_data&WMSZ_BOTTOMRIGHT) != 0; }
    bool IsLeft( void ) const { return (_data&WMSZ_LEFT) != 0; }
    bool IsRight( void ) const { return (_data&WMSZ_RIGHT) != 0; }
    bool IsTop( void ) const { return (_data&WMSZ_TOP) != 0; }
    bool IsTopLeft( void ) const { return (_data&WMSZ_TOPLEFT) != 0; }
    bool IsTopRight( void ) const { return (_data&WMSZ_TOPRIGHT) != 0; }

  private:
    WPARAM _data;
};

class PrintFlags
{
  public:
    PrintFlags( LPARAM lParam )
    : _flags(lParam)
    {}

    bool CheckVisible( void ) const { return (_flags&PRF_CHECKVISIBLE) != 0; }
    bool Children( void ) const { return (_flags&PRF_CHILDREN) != 0; }
    bool Client( void ) const { return (_flags&PRF_CLIENT) != 0; }
    bool EraseBkgnd( void ) const { return (_flags&PRF_ERASEBKGND) != 0; }
    bool NonClient( void ) const { return (_flags&PRF_NONCLIENT) != 0; }
    bool Owned( void ) const { return (_flags&PRF_OWNED) != 0; }

  private:
    LPARAM _flags;
};

} // namespace Win

#endif // WINMESSAGEDATA_H