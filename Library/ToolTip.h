#ifndef WINTOOLTIP_H
#define WINTOOLTIP_H

#include "Window.h"
#include "NotifyHandler.h"
#include "Maker.h"
#include <commctrl.h>

namespace Win
{

class ToolTip : public Win::Dow
{
  public: // Class
    class ToolInfo : public TOOLINFO
    {
      public:
        // Use for specifying tool tip information
        ToolInfo( HWND toolParent, UINT toolId, RECT& toolRect, TCHAR const toolText[], UINT flags = 0 )
        { cbSize = sizeof(TOOLINFO);
          uFlags = flags;
          hwnd = toolParent;
          uId = toolId;
          rect = toolRect;
          hinst = 0;
          lpszText = const_cast<TCHAR*>(toolText);
        }

        ToolInfo( HWND toolParent, UINT toolId, RECT& toolRect, int toolTextResId, HINSTANCE hInst, UINT flags = 0 )
        { cbSize = sizeof(TOOLINFO);
          uFlags = flags;
          hwnd = toolParent;
          uId = toolId;
          rect = toolRect;
          hinst = hInst;
          lpszText = MAKEINTRESOURCE(toolTextResId);
        }

        ToolInfo( HWND toolParent, HWND toolHwnd, TCHAR const toolText[], UINT flags = 0 )
        { cbSize = sizeof(TOOLINFO);
          uFlags = TTF_IDISHWND|flags;
          hwnd = toolParent;
          uId = reinterpret_cast<WPARAM>(toolHwnd);
          hinst = 0;
          lpszText = const_cast<TCHAR*>(toolText);
        }

        ToolInfo( HWND toolParent, HWND toolHwnd, int toolTextResId, HINSTANCE hInst, UINT flags = 0 )
        { cbSize = sizeof(TOOLINFO);
          uFlags = TTF_IDISHWND|flags;
          hwnd = toolParent;
          uId = reinterpret_cast<WPARAM>(toolHwnd);
          hinst = hInst;
          lpszText = MAKEINTRESOURCE(toolTextResId);
        }

        // Use for retrieving tool tip information
        ToolInfo( HWND toolParent, UINT toolId, TCHAR buf[] = 0 )
        { cbSize = sizeof(TOOLINFO);
          uFlags = 0;
          hwnd = toolParent;
          uId = toolId;
          hinst = 0;
          lpszText = buf;
        }

        ToolInfo( HWND toolParent, HWND toolHwnd, TCHAR buf[] = 0 )
        { cbSize = sizeof(TOOLINFO);
          uFlags = TTF_IDISHWND;
          hwnd = toolParent;
          uId = reinterpret_cast<WPARAM>(toolHwnd);
          hinst = 0;
          lpszText = buf;
        }
    };

  public:
    explicit ToolTip( HWND hwnd = 0 )
    : Win::Dow(hwnd)
    {}

    void Activate( void )
    { SendMessage(TTM_ACTIVATE, TRUE);
    }

    void Deactivate( void )
    { SendMessage(TTM_ACTIVATE, FALSE);
    }

    void AddTool( TOOLINFO& info )
    { SendMessage(TTM_ADDTOOL, 0, reinterpret_cast<LPARAM>(&info));
    }
/*  IE 5+
    void Window2DisplayTextRect( RECT& windowRect )
    { SendMessage(TTM_ADJUSTRECT, FALSE, reinterpret_cast<LPARAM>(&windowRect));
    }

    void DisplayText2WindowRect( RECT& displayTextRect )
    { SendMessage(TTM_ADJUSTRECT, TRUE, reinterpret_cast<LPARAM>(&displayTextRect));
    }
*/
    void DeleteTool( TOOLINFO& info )
    { SendMessage(TTM_DELTOOL, 0, reinterpret_cast<LPARAM>(&info));
    }

    void DeleteTool( HWND hwndTool, UINT toolId )
    { TOOLINFO info;
      info.cbSize = sizeof(info);
      info.hwnd = hwndTool;
      info.uId = toolId;
      DeleteTool(info);
    }

/*  IE 5+
    void GetBubbleSize( TOOLINFO& info, int& cx, int& cy )
    { DWORD size = SendMessage(TTM_GETBUBBLESIZE, 0, reinterpret_cast<LPARAM>(&info));
      cx = LOWORD(size);
      cy = HIWORD(size);
    }
*/
    void GetCurrentTool( TOOLINFO& info )
    { SendMessage(TTM_GETCURRENTTOOL, 0, reinterpret_cast<LPARAM>(&info));
    }

    bool CurrentToolExists( void )
    { return SendMessage(TTM_GETCURRENTTOOL) != 0;
    }

    int GetDelayTime( DWORD which )
    { return SendMessage(TTM_GETDELAYTIME, which);
    }

    int GetAutoPopDelay( void )
    { return GetDelayTime(TTDT_AUTOPOP);
    }

    int GetInitialDelay( void )
    { return GetDelayTime(TTDT_INITIAL);
    }

    int GetReshowDelay( void )
    { return GetDelayTime(TTDT_RESHOW);
    }

    void GetMargin( RECT& margin )
    { SendMessage(TTM_GETMARGIN, 0, reinterpret_cast<LPARAM>(&margin));
    }

    int GetMaxTipWidth( void )
    { return SendMessage(TTM_GETMAXTIPWIDTH);
    }

    COLORREF GetTipBkColor( void )
    { return SendMessage(TTM_GETTIPBKCOLOR);
    }

    COLORREF GetTipTextColor( void )
    { return SendMessage(TTM_GETTIPTEXTCOLOR);
    }

    int GetToolCount( void )
    { return SendMessage(TTM_GETTOOLCOUNT);
    }

    void GetToolInfo( TOOLINFO& info )
    { SendMessage(TTM_GETTOOLINFO, 0, reinterpret_cast<LPARAM>(&info));
    }

    void GetToolInfo( int idx, TOOLINFO& info )
    {
      SendMessage(TTM_ENUMTOOLS, idx, reinterpret_cast<LPARAM>(&info));
    }

    bool HitTest( TTHITTESTINFO& info )
    { return SendMessage(TTM_HITTEST, 0, reinterpret_cast<LPARAM>(&info)) != 0;
    }

    void NewToolRect( TOOLINFO& info )
    { SendMessage(TTM_NEWTOOLRECT, 0, reinterpret_cast<LPARAM>(&info));
    }

    void Pop( void )
    { SendMessage(TTM_POP);
    }

    void RelayEvent( MSG& mouseMsg )
    { SendMessage(TTM_RELAYEVENT, 0, reinterpret_cast<LPARAM>(&mouseMsg));
    }

    void SetDelayTime( DWORD which, int ms )
    { SendMessage(TTM_SETDELAYTIME, which, MAKELPARAM(ms, 0));
    }

    void SetAutoPopDelay( int ms )
    { SetDelayTime(TTDT_AUTOPOP, ms);
    }

    void SetDefaultAutoPopDelay( void )
    { SetAutoPopDelay(-1);
    }

    void SetInitialDelay( int ms )
    { SetDelayTime(TTDT_INITIAL, ms);
    }

    void SetDefaultInitialDelay( void )
    { SetInitialDelay(-1);
    }

    void SetReshowDelay( int ms )
    { SetDelayTime(TTDT_RESHOW, ms);
    }

    void SetDefaultReshowDelay( void )
    { SetReshowDelay(-1);
    }

    void SetDefaultDelays( void )
    { SetDelayTime(TTDT_AUTOMATIC, -1);
    }

    void SetMargin( RECT& margin )
    { SendMessage(TTM_SETMARGIN, 0, reinterpret_cast<LPARAM>(&margin));
    }

    void SetMaxTipWidth( int maxWidth )
    { SendMessage(TTM_SETMAXTIPWIDTH, 0, maxWidth);
    }

    void SetTipBkColor( COLORREF color )
    { SendMessage(TTM_SETTIPBKCOLOR, color);
    }

    void SetTipTextColor( COLORREF color )
    { SendMessage(TTM_SETTIPTEXTCOLOR, color);
    }
/*  IE 5+
    void SetTitle( char const title[], int icon = 0 )
    { SendMessage(TTM_SETTITLE, icon, reinterpret_cast<LPARAM>(title));
    }

    void SetInfoTitle( char const title[] )
    { SetTitle(title, 1);
    }

    void SetWarningTitle( char const title[] )
    { SetTitle(title, 2);
    }

    void SetErrorTitle( char const title[] )
    { SetTitle(title, 3);
    }
*/
    void SetToolInfo( TOOLINFO& info )
    { SendMessage(TTM_SETTOOLINFO, 0, reinterpret_cast<LPARAM>(&info));
    }

    void TrackActivate( TOOLINFO& info )
    { SendMessage(TTM_TRACKACTIVATE, TRUE, reinterpret_cast<LPARAM>(&info));
    }

    void TrackDeactivate( TOOLINFO& info )
    { SendMessage(TTM_TRACKACTIVATE, FALSE, reinterpret_cast<LPARAM>(&info));
    }

    void TrackPosition( int x, int y )
    { SendMessage(TTM_TRACKPOSITION, 0, MAKELONG(x,y));
    }

    void UpdateCurrentTool( void )
    { SendMessage(TTM_UPDATE);
    }

    void UpdateTipText( TOOLINFO& info )
    { SendMessage(TTM_UPDATETIPTEXT, 0, reinterpret_cast<LPARAM>(&info));
    } 
};

class ToolTipNotifyHandler : public Win::CommonNotifyHandler
{
  public:
    // Overriden methods
    virtual bool OnNotify( NMHDR* hdr, LRESULT& result );

    // New methods
    virtual bool OnCustomDraw( HWND hwnd, NMTTCUSTOMDRAW& cd, LRESULT& result ) { return false; }
    virtual bool OnGetDispInfo( HWND hwnd, NMTTDISPINFO& info ) { return false; }
    virtual bool OnPop( HWND hwnd ) { return false; }
    virtual bool OnShow( HWND hwnd, LRESULT& result ) { return false; }
};

class ToolTipMaker : public ControlMakerHandler<ToolTipNotifyHandler>
{
  public:
    ToolTipMaker( Win::Dow parent, int id, DWORD style = WS_POPUP|TTS_NOPREFIX|TTS_ALWAYSTIP, DWORD exStyle = 0, int x = CW_USEDEFAULT, int y = CW_USEDEFAULT, int w = CW_USEDEFAULT, int h = CW_USEDEFAULT )
    : ControlMakerHandler<ToolTipNotifyHandler>(TOOLTIPS_CLASS, parent, id, style, exStyle, x, y, w, h)
    {}

    HWND Create( TCHAR const title[] = TEXT(""), ToolTipNotifyHandler* handler = 0 )
    { HWND hwnd = ControlMakerHandler<ToolTipNotifyHandler>::Create(title, handler);
      ::SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE|SWP_NOACTIVATE);
      return hwnd;
    }
};

} // namespace Win

#endif // WINTOOLTIP_H