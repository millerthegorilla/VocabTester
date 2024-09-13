#ifndef WINTOOLBAR_H
#define WINTOOLBAR_H

#include "Window.h"
#include "NotifyHandler.h"
#include "Maker.h"
#include <commctrl.h>

namespace Win
{

class ToolBar : public Win::Dow
{
  public: // Class
    class Button : public TBBUTTON
    {
      public:
        Button( int cmdId, int bmpIdx )
        { iBitmap = bmpIdx;
          idCommand = cmdId;
          fsState = TBSTATE_ENABLED;
          fsStyle = 0;
          iString = 0;
        }

        Button( int cmdId, int bmpIdx, int stringIdx )
        { iBitmap = bmpIdx;
          idCommand = cmdId;
          fsState = TBSTATE_ENABLED;
          fsStyle = 0;
          iString = stringIdx;
        }
    };

  public: // Class
    class Separator : public Button
    {
      public:
        Separator( void )
        : Button(-1, -1)
        { fsStyle = TBSTYLE_SEP;
        }
    };

  public:
    explicit ToolBar( HWND hwnd = 0 )
    : Win::Dow(hwnd)
    {}

    int AddBitmap( HINSTANCE hInst, unsigned resId, int nButtons )
    { TBADDBITMAP bm = {hInst, resId};
      return SendMessage(TB_ADDBITMAP, nButtons, reinterpret_cast<LPARAM>(&bm));
    }

    int AddBitmap( HBITMAP hBitmap, int nButtons )
    { TBADDBITMAP bm = {0, reinterpret_cast<unsigned>(hBitmap)};
      return SendMessage(TB_ADDBITMAP, nButtons, reinterpret_cast<LPARAM>(&bm));
    }

    void AddButtons( TBBUTTON buttons[], unsigned nButtons )
    { SendMessage(TB_ADDBUTTONS, nButtons, reinterpret_cast<LPARAM>(buttons));
    }

    int AddString( HINSTANCE hInst, int resId )
    { return SendMessage(TB_ADDSTRING, reinterpret_cast<WPARAM>(hInst), MAKELONG(resId, 0));
    }

    // Strings must point to a character array with one or more
    // null-terminated strings. The last string in the array must be
    // terminated with two null characters. 
    int AddString( TCHAR const strings[] )
    { return SendMessage(TB_ADDSTRING, 0, reinterpret_cast<LPARAM>(strings));
    }

    void AutoSize( void )
    { SendMessage(TB_AUTOSIZE);
    }

    int GetButtonCount( void ) const
    { return SendMessage(TB_BUTTONCOUNT);
    }

    void SetButtonStructSize( void )
    { SendMessage(TB_BUTTONSTRUCTSIZE, sizeof(TBBUTTON));
    }

    void ChangeButtonBitmap( int cmdId, int bmpIdx )
    { SendMessage(TB_CHANGEBITMAP, cmdId, MAKELPARAM(bmpIdx, 0));
    }

    void CheckButton( int cmdId, bool check = true )
    { SendMessage(TB_CHECKBUTTON, cmdId, MAKELONG(check,0));
    }

    void UnCheckButton( int cmdId )
    { CheckButton(cmdId, false);
    }

    int CommandToButton( int cmdId ) const
    { return SendMessage(TB_COMMANDTOINDEX, cmdId);
    }

    void Customize( void )
    { SendMessage(TB_CUSTOMIZE);
    }

    void DeleteButton( int btnIdx )
    { SendMessage(TB_DELETEBUTTON, btnIdx);
    }

    void EnableButton( int cmdId, bool enable = true )
    { SendMessage(TB_ENABLEBUTTON, cmdId, MAKELONG(enable, 0));
    }

    void DisableButton( int cmdId )
    { EnableButton(cmdId, false);
    }

    bool IsAnchorHighlightEnabled( void ) const
    { return SendMessage(TB_GETANCHORHIGHLIGHT) != 0;
    }

    int GetButtonBitmap( int cmdId ) const
    { return SendMessage(TB_GETBITMAP, cmdId);
    }

    DWORD GetBitmapFlags( void ) const
    { return SendMessage(TB_GETBITMAPFLAGS);
    }

    void GetButton( int btnIdx, TBBUTTON& btn ) const
    { SendMessage(TB_GETBUTTON, btnIdx, reinterpret_cast<LPARAM>(&btn));
    }

    int GetButtonInfo( int cmdId, TBBUTTONINFO& info ) const
    { return SendMessage(TB_GETBUTTON, cmdId, reinterpret_cast<LPARAM>(&info));
    }

    void GetButtonSize( SIZE& size ) const
    { DWORD ret = SendMessage(TB_GETBUTTONSIZE);
      size.cx = LOWORD(ret);
      size.cy = HIWORD(ret);
    }

    HIMAGELIST GetDisabledImageList( void ) const
    { return reinterpret_cast<HIMAGELIST>(SendMessage(TB_GETDISABLEDIMAGELIST));
    }

    DWORD GetExtendedStyle( void ) const
    { return SendMessage(TB_GETEXTENDEDSTYLE);
    }

    HIMAGELIST GetHotImageList( void ) const
    { return reinterpret_cast<HIMAGELIST>(SendMessage(TB_GETHOTIMAGELIST));
    }

    int GetHotButton( void ) const
    { return SendMessage(TB_GETHOTITEM);
    }

    HIMAGELIST GetImageList( void ) const
    { return reinterpret_cast<HIMAGELIST>(SendMessage(TB_GETIMAGELIST));
    }

    void GetButtonRect( int btnIdx, RECT& r ) const
    { SendMessage(TB_GETITEMRECT, btnIdx, reinterpret_cast<LPARAM>(&r));
    }

    void GetMaxSize( SIZE& size ) const
    { SendMessage(TB_GETMAXSIZE, 0, reinterpret_cast<LPARAM>(&size));
    }

    int GetNoRows( void ) const
    { return SendMessage(TB_GETROWS);
    }

    DWORD GetButtonState( int cmdId ) const
    { return SendMessage(TB_GETSTATE, cmdId);
    }

    bool IsButtonChecked( int cmdId ) const
    { return (GetButtonState(cmdId) & TBSTATE_CHECKED) != 0;
    }

    bool IsButtonEnabled( int cmdId ) const
    { return (GetButtonState(cmdId) & TBSTATE_ENABLED) != 0;
    }

    bool IsButtonHidden( int cmdId ) const
    { return (GetButtonState(cmdId) & TBSTATE_HIDDEN) != 0;
    }

    bool IsButtonGrayed( int cmdId ) const
    { return (GetButtonState(cmdId) & TBSTATE_INDETERMINATE) != 0;
    }

    bool IsButtonPressed( int cmdId ) const
    { return (GetButtonState(cmdId) & TBSTATE_PRESSED) != 0;
    }

    DWORD GetStyle( void ) const
    { return SendMessage(TB_GETSTYLE);
    }

    int GetNoTextRows( void ) const
    { return SendMessage(TB_GETTEXTROWS);
    }

    HWND GetToolTipsControl( void ) const
    { return reinterpret_cast<HWND>(SendMessage(TB_GETTOOLTIPS));
    }

    bool IsUnicode( void ) const
    { return SendMessage(TB_GETUNICODEFORMAT) != 0;
    }

    bool IsAnsi( void ) const
    { return !IsUnicode();
    }

    void ShowButton( int cmdId, bool show = true )
    { SendMessage(TB_HIDEBUTTON, cmdId, MAKELONG(!show, 0));
    }

    void HideButton( int cmdId )
    { ShowButton(cmdId, false);
    }

    int HitTest( POINT& pt ) const
    { return SendMessage(TB_HITTEST, 0, reinterpret_cast<LPARAM>(&pt));
    }

    void GrayButton( int cmdId, bool gray = true )
    { SendMessage(TB_INDETERMINATE, cmdId, MAKELONG(gray,0));
    }

    void UnGrayButton( int cmdId )
    { GrayButton(cmdId, false);
    }

    void InsertButton( int pos, TBBUTTON const& btn )
    { SendMessage(TB_INSERTBUTTON, pos, reinterpret_cast<LPARAM>(&btn));
    }

    void MoveButton( int oldPos, int newPos )
    { SendMessage(TB_MOVEBUTTON, oldPos, newPos);
    }

    void PressButton( int cmdId, bool press = true )
    { SendMessage(TB_PRESSBUTTON, cmdId, MAKELONG(press,0));
    }

    void ReleaseButton( int cmdId )
    { PressButton(cmdId, false);
    }

    void ReplaceBitmap( HINSTANCE hInstOld, unsigned resIdOld, HINSTANCE hInstNew, unsigned resIdNew, int nButtons )
    { TBREPLACEBITMAP info;
      info.hInstOld = hInstOld;
      info.nIDOld = resIdOld;
      info.hInstNew = hInstNew;
      info.nIDNew = resIdNew;
      info.nButtons = nButtons;
      SendMessage(TB_REPLACEBITMAP, 0, reinterpret_cast<LPARAM>(&info));
    }

    void ReplaceBitmap( HBITMAP hBmpOld, HBITMAP hBmpNew, int nButtons )
    { TBREPLACEBITMAP info;
      info.hInstOld = 0;
      info.nIDOld = reinterpret_cast<unsigned>(hBmpOld);
      info.hInstNew = 0;
      info.nIDNew = reinterpret_cast<unsigned>(hBmpNew);
      info.nButtons = nButtons;
      SendMessage(TB_REPLACEBITMAP, 0, reinterpret_cast<LPARAM>(&info));
    }

    void EnableAnchorHighlight( bool enable = true )
    { SendMessage(TB_SETANCHORHIGHLIGHT, enable);
    }

    void DisableAnchorHighlight( void )
    { EnableAnchorHighlight(false);
    }

    void SetBitmapSize( int cx, int cy )
    { SendMessage(TB_SETBITMAPSIZE, 0, MAKELONG(cx, cy));
    }

    void SetButtonInfo( int btnIdx, TBBUTTONINFO const& info )
    { SendMessage(TB_SETBUTTONINFO, btnIdx, reinterpret_cast<LPARAM>(&info));
    }

    void SetButtonSize( int cx, int cy )
    { SendMessage(TB_SETBUTTONSIZE, 0, MAKELONG(cx, cy));
    }

    void SetButtonWidth( int cxMin, int cxMax )
    { SendMessage(TB_SETBUTTONWIDTH, 0, MAKELONG(cxMin, cxMax));
    }

    void SetButtonCommand( int btnIdx, int cmdId )
    { SendMessage(TB_SETCMDID, btnIdx, cmdId);
    }

    void SetDisableImageList( HIMAGELIST himl )
    { SendMessage(TB_SETDISABLEDIMAGELIST, 0, reinterpret_cast<LPARAM>(himl));
    }

    void SetExtendedStyle( DWORD style )
    { SendMessage(TB_SETEXTENDEDSTYLE, 0, style);
    }

    void SetHotImageList( HIMAGELIST himl )
    { SendMessage(TB_SETHOTIMAGELIST, 0, reinterpret_cast<LPARAM>(himl));
    }

    void SetHotButton( int btnIdx )
    { SendMessage(TB_SETHOTITEM, btnIdx);
    }

    void SetImageList( HIMAGELIST himl )
    { SendMessage(TB_SETIMAGELIST, 0, reinterpret_cast<LPARAM>(himl));
    }

    void SetIndent( int indent )
    { SendMessage(TB_SETINDENT, indent);
    }

    void SetMaxTextRows( int maxRows )
    { SendMessage(TB_SETMAXTEXTROWS, maxRows);
    }

    void SetPadding( int cx, int cy )
    { SendMessage(TB_SETPADDING, 0, MAKELPARAM(cx, cy));
    }

    void SetParent( HWND parent )
    { SendMessage(TB_SETPARENT, reinterpret_cast<WPARAM>(parent));
    }

    void SetButtonState( int cmdId, DWORD state )
    { SendMessage(TB_SETSTATE, cmdId, MAKELONG(state,0));
    }

    void SetStyle( DWORD style )
    { SendMessage(TB_SETSTYLE, 0, style);
    }

    void SetToolTipsControl( HWND hwnd ) const
    { SendMessage(TB_SETTOOLTIPS, reinterpret_cast<WPARAM>(hwnd));
    }

    void SetUnicode( void )
    { SendMessage(TB_SETUNICODEFORMAT, true);
    }

    void SetAnsi( void )
    { SendMessage(TB_SETUNICODEFORMAT, false);
    }
};

class ToolBarNotifyHandler : public Win::CommonNotifyHandler
{
  public:
    // Overriden methods
    virtual bool OnNotify( NMHDR* hdr, LRESULT& result );

    // New methods
    virtual bool OnCustomDraw( HWND hwnd, NMTBCUSTOMDRAW& info, LRESULT& result ) { return false; }
    virtual bool OnBeginAdjust( HWND hwnd ) { return false; }
    virtual bool OnBeginDrag( HWND hwnd, int cmdId ) { return false; }
    virtual bool OnCustomHelp( HWND hwnd ) { return false; }
    virtual bool OnDeletingButton( HWND hwnd, int cmdId ) { return false; }
    virtual bool OnDragOut( HWND hwnd, int cmdId ) { return false; }
    virtual bool OnDropDown( HWND hwnd, int cmdId, LRESULT& result ) { return false; }
    virtual bool OnEndAdjust( HWND hwnd ) { return false; }
    virtual bool OnEndDrag( HWND hwnd, int cmdId ) { return false; }
    virtual bool OnGetButtonInfo( HWND hwnd, NMTOOLBAR& info, LRESULT& result ) { return false; }
    virtual bool OnGetDispInfo( HWND hwnd, NMTBDISPINFO& info ) { return false; }
    virtual bool OnGetInfoTip( HWND hwnd, NMTBGETINFOTIP& info ) { return false; }
    virtual bool OnHotButtonChange( HWND hwnd, NMTBHOTITEM const& info, LRESULT& result ) { return false; }
    virtual bool OnInitCustomize( HWND hwnd, LRESULT& result ) { return false; }
    virtual bool OnQueryDelete( HWND hwnd, int btnIdx, LRESULT& result ) { return false; }
    virtual bool OnQueryInsert( HWND hwnd, int btnIdx, LRESULT& result ) { return false; }
    virtual bool OnReset( HWND hwnd ) { return false; }
/*
    virtual bool OnRestore( HWND hwnd, NMTBRESTORE const& info, LRESULT& result ) { return false; }
    virtual bool OnSave( HWND hwnd, NMTBSAVE const& info ) { return false; }
*/
    virtual bool OnToolBarChange( HWND hwnd ) { return false; }
};

class ToolBarMaker : public Win::ControlMakerHandler<ToolBarNotifyHandler>
{
  public:
    ToolBarMaker( Win::Dow parent, int id, DWORD style = WS_VISIBLE, DWORD exStyle = 0, int x = 0, int y = 0, int w = 0, int h = 0 )
    : ControlMakerHandler<ToolBarNotifyHandler>(TOOLBARCLASSNAME, parent, id, style, exStyle, x, y, w, h)
    {}

    void AltDrag( void ) { _style |= TBSTYLE_ALTDRAG; }
    void CustomErase( void ) { _style |= TBSTYLE_CUSTOMERASE; }
    void Flat( void ) { _style |= TBSTYLE_FLAT; }
    void List( void ) { _style |= TBSTYLE_LIST; }
    void RegisterDrop( void ) { _style |= TBSTYLE_REGISTERDROP; }
    void ToolTips( void ) { _style |= TBSTYLE_TOOLTIPS; }
    void Transparent( void ) { _style |= TBSTYLE_TRANSPARENT; }
    void Wrapable( void ) { _style |= TBSTYLE_WRAPABLE; }
};

} // namespace Win

#endif // WINTOOLBAR_H