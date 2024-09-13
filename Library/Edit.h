#ifndef WINEDIT_H
#define WINEDIT_H

#include "Window.h"
#include "NotifyHandler.h"
#include "Maker.h"
#include "stdstring"

namespace Win
{

class SimpleEdit : public Win::Dow
{
  public:
    bool CanUndo( void )
    { return SendMessage(EM_CANUNDO) != 0;
    }

    void EmptyUndoBuffer( void )
    { SendMessage(EM_EMPTYUNDOBUFFER);
    }

    int GetLimitText( void )
    { SendMessage(EM_GETLIMITTEXT);
    }

    bool IsModified( void )
    { return SendMessage(EM_GETMODIFY) != 0;
    }

    void GetSelection( int& firstSelected, int& firstNonSelected )
    { SendMessage(EM_GETSEL, reinterpret_cast<WPARAM>(&firstSelected), reinterpret_cast<LPARAM>(&firstNonSelected));
    }

    void SetLimitText( int limit )
    { SendMessage(EM_LIMITTEXT, limit);
    }

    void ReplaceSelection( TCHAR const newText[], bool canBeUndone = true )
    { SendMessage(EM_REPLACESEL, canBeUndone,  reinterpret_cast<LPARAM>(newText));
    }

    void ScrollCaret( void )
    { SendMessage(EM_SCROLLCARET);
    }

    void SetModifiedFlag( void )
    { SendMessage(EM_SETMODIFY, TRUE);
    }

    void ResetModifiedFlag( void )
    { SendMessage(EM_SETMODIFY, FALSE);
    }

    void SetReadOnlyFlag( void )
    { SendMessage(EM_SETREADONLY, TRUE);
    }

    void ResetReadOnlyFlag( void )
    { SendMessage(EM_SETREADONLY, FALSE);
    }

    void SetSelection( int firstSelected, int firstNonSelected )
    { SendMessage(EM_SETSEL, firstSelected, firstNonSelected);
    }

    void Undo( void )
    { SendMessage(EM_UNDO);
    }

    void AppendText( TCHAR const text[] )
    { SetSelection(GetTextLength(), -1);
      ReplaceSelection(text,false);
    }

    void SelectAll( void )
    { SetSelection(0,-1);
    }

    void Clear( void )
    { SelectAll();
      SendMessage(WM_CLEAR);
    }


  protected:
    explicit SimpleEdit( HWND hwnd )
    : Win::Dow(hwnd)
    {}

    SimpleEdit( HWND dlg, int id )
    : Win::Dow(::GetDlgItem(dlg, id))
    {}
};

class Edit : public SimpleEdit
{
  public: // Methods
    void InsertSoftLineBreaks( void )
    { SendMessage(EM_FMTLINES, TRUE);
    }

    void RemoveSoftLineBreaks( void )
    { SendMessage(EM_FMTLINES, FALSE);
    }

    void GetMargins( int& cxLeft, int& cxRight )
    { LRESULT res = SendMessage(EM_GETMARGINS);
      cxLeft = LOWORD(res); cxRight = HIWORD(res);
    }

  protected: // Methods
    Edit( HWND hwnd )
    : SimpleEdit(hwnd)
    {}

    Edit( HWND dlg, int id )
    : SimpleEdit(::GetDlgItem(dlg, id))
    {}    
};

class EditNotifyHandler : public Win::NotifyHandler
{
  public:
    // Overriden method
    virtual bool OnNotify( NMHDR* hdr, LRESULT& result );

    // Edit specific methods
    /*
    // Only Windows 2000+
    virtual bool OnAlignLTR( void ) { return false; }
    virtual bool OnAlignRTL( void ) { return false; }
    */
    virtual bool OnChange( HWND hwnd ) { return false; }
    virtual bool OnErrSpace( HWND hwnd ) { return false; }
    virtual bool OnHScroll( HWND hwnd ) { return false; }
    virtual bool OnKillFocus( HWND hwnd ) { return false; }
    virtual bool OnMaxText( HWND hwnd ) { return false; }
    virtual bool OnSetFocus( HWND hwnd ) { return false; }
    virtual bool OnUpdate( HWND hwnd ) { return false; }
    virtual bool OnVScroll( HWND hwnd ) { return false; }
};

class SingleLineEdit : public Edit
{
  public:
    explicit SingleLineEdit( HWND hwnd = 0 )
    : Edit(hwnd)
    {}

    SingleLineEdit( HWND dlg, int id )
    : Edit(::GetDlgItem(dlg, id))
    {}

    int CharFromPos( POINT const& pt )
    { return LOWORD(SendMessage(EM_CHARFROMPOS, 0, MAKELPARAM(pt.x,pt.y)));
    }

    int GetFirstVisibleChar( void )
    { return SendMessage(EM_GETFIRSTVISIBLELINE);
    }

    int GetPasswordChar( void )
    { return SendMessage(EM_GETPASSWORDCHAR);
    }

    void SetPasswordChar( TCHAR ch )
    { SendMessage(EM_SETPASSWORDCHAR, ch);
    }
};

class MultiLineEdit : public Edit
{
  public:
    explicit MultiLineEdit( HWND hwnd = 0 )
    : Edit(hwnd)
    {}

    MultiLineEdit( HWND dlg, int id )
    : Edit(::GetDlgItem(dlg, id))
    {}

    int CharFromPos( POINT const& pt, int& lineIdx )
    { LRESULT res = SendMessage(EM_CHARFROMPOS, 0, MAKELPARAM(pt.x,pt.y));
      lineIdx = HIWORD(res);
      return LOWORD(res);
    }

    int GetFirstVisibleLine( void )
    { return SendMessage(EM_GETFIRSTVISIBLELINE);
    }

    stdstring GetLine( int lineIdx )
    { int len = GetLineLength(lineIdx);
      if ( len < 4 ) len = 4;
      stdstring str;
      str.reserve(len + 1);
      str.resize(len);
      (*reinterpret_cast<WORD*>(&str[0])) = len + 1;
      SendMessage(EM_GETLINE, lineIdx, reinterpret_cast<LPARAM>(&str[0]));
      return str;
    }

    int GetLineIndex( int line )
    { return SendMessage(EM_LINEINDEX, line);
    }

    int GetLineCount( void )
    { return SendMessage(EM_GETLINECOUNT);
    }

    void DeleteLine( int line )
    { SetSelection(GetLineIndex(line), GetLineIndex(line + 1));
      SendMessage(WM_CLEAR);
    }

    int GetThumbPos( void )
    { return SendMessage(EM_GETTHUMB);
    }

    EDITWORDBREAKPROC GetWordBreakProc( void )
    { return reinterpret_cast<EDITWORDBREAKPROC>(SendMessage(EM_GETWORDBREAKPROC));
    }

    int GetLineFromChar( int charIdx )
    { return SendMessage(EM_LINEFROMCHAR, charIdx);
    }

    int GetFirstCharIndex( int lineIdx )
    { return SendMessage(EM_LINEINDEX, lineIdx);
    }

    int GetLineLength( int lineIdx )
    { return SendMessage(EM_LINELENGTH, GetFirstCharIndex(lineIdx));
    }

    void Scroll( int nLines, int nChars = 0 /* Scrolls horizontaly */ )
    { SendMessage(EM_LINESCROLL, nChars, nLines);
    }

    void GetFormatingRect( RECT& rect )
    { SendMessage(EM_GETRECT, 0, reinterpret_cast<LPARAM>(&rect));
    }

    void SetFormatingRect( RECT const& rect, bool redraw = true )
    { SendMessage(redraw ? EM_SETRECT : EM_SETRECTNP, 0, reinterpret_cast<LPARAM>(&rect));
    }

    void SetTabStops( unsigned int tabStop )
    { SendMessage(EM_SETTABSTOPS, 1, reinterpret_cast<LPARAM>(&tabStop));
    }

    void SetTabStops( unsigned int tabStops[], int nTabStops )
    { SendMessage(EM_SETTABSTOPS, nTabStops, reinterpret_cast<LPARAM>(tabStops));
    }

    void SetDefaultTabStops( void )
    { SendMessage(EM_SETTABSTOPS);
    }

    void SetWordBreakProc( EDITWORDBREAKPROC proc )
    { SendMessage(EM_SETWORDBREAKPROC, 0, reinterpret_cast<LPARAM>(proc));
    }
};


class SimpleEditMaker : public ControlMakerHandler<EditNotifyHandler>
{
  public:
    void CenterAlign( void )
    { ResetStyle(ES_LEFT|ES_RIGHT|ES_CENTER);
      _style |= ES_CENTER;
    }

    void LeftAlign( void )
    { ResetStyle(ES_LEFT|ES_RIGHT|ES_CENTER);
      _style |= ES_LEFT;
    }

    void RightAlign( void )
    { ResetStyle(ES_LEFT|ES_RIGHT|ES_CENTER);
      _style |= ES_RIGHT;
    }

    void LowerCase( void )
    { ResetStyle(ES_LOWERCASE|ES_UPPERCASE);
      _style |= ES_LOWERCASE;
    }

    void UpperCase( void )
    { ResetStyle(ES_LOWERCASE|ES_UPPERCASE);
      _style |= ES_UPPERCASE;
    }

    void NoHideSelection( void ) { _style |= ES_NOHIDESEL; }
    void Number( void ) { _style |= ES_NUMBER; }
    void Password( void ) { _style |= ES_PASSWORD; }
    void ReadOnly( void ) { _style |= ES_READONLY; }
    void WantReturn( void ) { _style |= ES_WANTRETURN; }

  protected:
    SimpleEditMaker( Win::Dow parent, int id, DWORD style = WS_VISIBLE|ES_AUTOHSCROLL|ES_LEFT, DWORD exStyle = WS_EX_CLIENTEDGE, int x = 0, int y = 0, int w = 0, int h = 0 )
    : ControlMakerHandler<EditNotifyHandler>(TEXT("EDIT"), parent, id, style, exStyle, x, y, w, h)
    {}
};

class SingleLineEditMaker : public SimpleEditMaker
{
  public:
    SingleLineEditMaker( Win::Dow parent, int id, DWORD style = WS_VISIBLE|ES_AUTOHSCROLL|ES_LEFT, DWORD exStyle = WS_EX_CLIENTEDGE, int x = 0, int y = 0, int w = 0, int h = 0 )
    : SimpleEditMaker(parent, id, style, exStyle, x, y, w, h)
    {}
};

class MultiLineEditMaker : public SimpleEditMaker
{
  public:
    MultiLineEditMaker( Win::Dow parent, int id, DWORD style = WS_VISIBLE|WS_VSCROLL|WS_HSCROLL|ES_AUTOHSCROLL|ES_AUTOVSCROLL|ES_LEFT|ES_MULTILINE, DWORD exStyle = WS_EX_CLIENTEDGE, int x = 0, int y = 0, int w = 0, int h = 0 )
    : SimpleEditMaker(parent, id, style, exStyle, x, y, w, h)
    {}
};

} // namespace Win

#endif // WINEDIT_H