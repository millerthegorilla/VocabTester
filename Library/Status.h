#ifndef WINSTATUSBAR_H
#define WINSTATUSBAR_H

#include "Window.h"
#include "NotifyHandler.h"
#include "Maker.h"

namespace Win
{

class StatusBar : public Win::Dow
{
  public: // Methods
    explicit StatusBar( HWND hwnd = 0 )
    : Win::Dow(hwnd)
    {}

    StatusBar( HWND dlg, int id )
    : Win::Dow(::GetDlgItem(dlg, id))
    {}

    void GetBorders( int borders[] /* array of three (3) integers */ )
    { SendMessage(SB_GETBORDERS, 0, reinterpret_cast<LPARAM>(borders));
    }

    HICON GetPartIcon( int partIdx )
    { return reinterpret_cast<HICON>(SendMessage(SB_GETICON, partIdx));
    }

    int GetPartCount( void )
    { return SendMessage(SB_GETPARTS);
    }

    void GetParts( int nParts, int rightCoords[] )
    { SendMessage(SB_GETPARTS, nParts, reinterpret_cast<LPARAM>(rightCoords));
    }

    void GetPartRect( RECT& rect, int partIdx = 0 )
    { SendMessage(SB_GETRECT, partIdx, reinterpret_cast<LPARAM>(&rect));
    }

    stdstring GetPartText( int partIdx = 0 )
    { int len = GetPartTextLength(partIdx);
      stdstring text;
      text.reserve(len + 1);
      text.resize(len);
      SendMessage(SB_GETTEXT, partIdx, reinterpret_cast<LPARAM>(&text[0]));
      return text;
    }

    int GetPartTextLength( int partIdx = 0 )
    { return LOWORD(SendMessage(SB_GETTEXTLENGTH, partIdx));
    }

    bool IsUnicode( void )
    { return SendMessage(SB_GETUNICODEFORMAT) != 0;
    }

    bool IsSimple( void )
    { return SendMessage(SB_ISSIMPLE) != 0;
    }

    COLORREF SetBkColor( COLORREF color = CLR_DEFAULT )
    { return SendMessage(SB_SETBKCOLOR, 0, color);
    }

    void SetPartIcon( HICON hIcon, int partIdx = 0 )
    { SendMessage(SB_SETICON, partIdx, reinterpret_cast<LPARAM>(hIcon));
    }

    void RemovePartIcon( int partIdx = 0 )
    { SetPartIcon(0, partIdx);
    }

    void SetMinHeight( int minHeight )
    { SendMessage(SB_SETMINHEIGHT, minHeight);
    }

    void SetParts( int nParts, int rightCoords[] )
    { SendMessage(SB_SETPARTS, nParts, reinterpret_cast<LPARAM>(rightCoords));
    }

    void SetPartText( TCHAR const text[], int partIdx = 0, int type = 0 )
    { SendMessage(SB_SETTEXT, partIdx|type, reinterpret_cast<LPARAM>(text));
    }

    void SetPartTipText( TCHAR const tipText[], int partIdx = 0 )
    { SendMessage(SB_SETTIPTEXT, partIdx, reinterpret_cast<LPARAM>(tipText));
    }

    void SetUnicode( void )
    { SendMessage(SB_SETUNICODEFORMAT, TRUE);
    }

    void SetAnsi( void )
    { SendMessage(SB_SETUNICODEFORMAT, FALSE);
    }

    void SetSimple( void )
    { SendMessage(SB_SIMPLE, TRUE);
    }

    void SetNormal( void )
    { SendMessage(SB_SIMPLE, FALSE);
    }
};

class StatusBarNotifyHandler : public Win::CommonNotifyHandler
{
  public:
    // Overriden methods
    virtual bool OnNotify( NMHDR* hdr, LRESULT& result );

    // New methods
    virtual bool OnClick( HWND hwnd, NMMOUSE* data, LRESULT& result ) { return false; }
    virtual bool OnDoubleClick( HWND hwnd, NMMOUSE* data, LRESULT& result ) { return false; }
    virtual bool OnRightClick( HWND hwnd, NMMOUSE* data, LRESULT& result ) { return false; }
    virtual bool OnRightDoubleClick( HWND hwnd, NMMOUSE* data, LRESULT& result ) { return false; }
    virtual bool OnSimpleModeChange( HWND hwnd ) { return false; }
};

/*
**

  StatusBar styles
  ~~~~~~~~~~~~~~~~
  
  CCS_BOTTOM
    Causes the control to position itself at the bottom of the parent 
    window's client area and sets the width to be the same as the 
    parent window's width. Status windows have this style by default. 

  CCS_NOPARENTALIGN
    Prevents the control from automatically moving to the top or bottom
    of the parent window. Instead, the control keeps its position within
    the parent window despite changes to the size of the parent.
    If CCS_TOP or CCS_BOTTOM is also used, the height is adjusted
    to the default, but the position and width remain unchanged.

  CCS_TOP
    Causes the control to position itself at the top of the parent
    window's client area and sets the width to be the same as
    the parent window's width. Toolbars have this style by default.

  SBARS_SIZEGRIP 
    The status bar control will include a sizing grip at the right 
    end of the status bar. A sizing grip is similar to a sizing border; 
    it is a rectangular area that the user can click and drag to resize 
    the parent window.  

  SBT_TOOLTIPS 
    Version 4.71. Use this style to enable tooltips. 

  SBARS_TOOLTIPS 
    Version 5.80. Identical to SBT_TOOLTIPS. Use this flag
    for versions 5.00 and later.  

**
*/

class StatusBarMaker : public ControlMakerHandler<StatusBarNotifyHandler>
{
  public:
    StatusBarMaker( Win::Dow parent, int id, DWORD style = WS_VISIBLE|CCS_BOTTOM|SBARS_SIZEGRIP, DWORD exStyle = 0, int x = 0, int y = 0, int w = 0, int h = 0 )
    : ControlMakerHandler<StatusBarNotifyHandler>(STATUSCLASSNAME, parent, id, style, exStyle, x, y, w, h)
    {}
};

} // namespace Win

#endif // WINSTATUSBAR_H