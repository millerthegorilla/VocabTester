#ifndef WINRICHEDIT_H
#define WINRICHEDIT_H

#include "Edit.h"
#include "SmartLib.h"
#include <richedit.h>

namespace Win
{

class UseRichEdit10 : public SLib
{
  public:
    UseRichEdit10( void )
    : SLib(TEXT("riched32.dll")) 
    {}
};

class UseRichEdit20 : public SLib
{
  public:
    UseRichEdit20( void )
    : SLib(TEXT("riched20.dll")) 
    {}
};


class RichEdit : public SimpleEdit
{
  public: // Class
    class CharFormat : public CHARFORMAT
    {
      public:
        CharFormat( void )
        { cbSize = sizeof(*this);
          dwMask = 0;
          dwEffects = 0;
          yHeight = 0;
          yOffset = 0;
          crTextColor = 0;
          bCharSet = 0;
          bPitchAndFamily = 0;
          szFaceName[0] = '\0'; 
        }

        void BoldOn( void ) { dwMask |= CFM_BOLD; dwEffects |= CFE_BOLD; }
        void BoldOff( void ) { dwMask |= CFM_BOLD; dwEffects &= ~CFE_BOLD; }
        void ItalicOn( void ) { dwMask |= CFM_ITALIC; dwEffects |= CFE_ITALIC; }
        void ItalicOff( void ) { dwMask |= CFM_ITALIC; dwEffects &= ~CFE_ITALIC; }
        void StrikeOutOn( void ) { dwMask |= CFM_STRIKEOUT; dwEffects |= CFE_STRIKEOUT; }
        void StrikeOutOff( void ) { dwMask |= CFM_STRIKEOUT; dwEffects &= ~CFE_STRIKEOUT; }
        void UnderlineOn( void ) { dwMask |= CFM_UNDERLINE; dwEffects |= CFE_UNDERLINE; }
        void UnderlineOff( void ) { dwMask |= CFM_UNDERLINE; dwEffects &= ~CFE_UNDERLINE; }
        void ProtectedOn( void ) { dwMask |= CFM_PROTECTED; dwEffects |= CFE_PROTECTED; }
        void ProtectedOff( void ) { dwMask |= CFM_PROTECTED; dwEffects &= ~CFE_PROTECTED; }
        void SetHeight( long h ) { dwMask |= CFM_SIZE; yHeight = h; }
        void SetOffset( long offset ) { dwMask |= CFM_OFFSET; yOffset = offset; }
        void SetTextColor( COLORREF color ) { dwMask |= CFM_COLOR; crTextColor = color; }
        void SetFaceName( stdstring const& name ) { SetFaceName(name.c_str()); }
        void SetFaceName( TCHAR const name[] ) { dwMask |= CFM_FACE; ::_tcscpy(szFaceName, name); }
        void SetCharSet( int charSet ) { bCharSet = charSet; }
    };

  public: // Methods
    int CharFromPos( POINT const& pt )
    { return SendMessage(EM_CHARFROMPOS, 0, reinterpret_cast<LPARAM>(&pt));
    }

    TCHAR GetPasswordChar( void )
    { return SendMessage(EM_GETPASSWORDCHAR);
    }

    void SetPasswordChar( TCHAR ch )
    { SendMessage(EM_SETPASSWORDCHAR, ch);
    }

    void SelectAll( void )
    { CHARRANGE r = { 0, -1 };
      SendMessage(EM_EXSETSEL, 0, reinterpret_cast<LPARAM>(&r));	
    }

    void SetSelection( int firstSelected, int firstNonSelected )
    { CHARRANGE r = { firstSelected, firstNonSelected };
      SendMessage(EM_EXSETSEL, 0, reinterpret_cast<LPARAM>(&r)); 
    }

    void SetTextLimit( DWORD maxChar )
    { SendMessage(EM_EXLIMITTEXT, 0, maxChar);
    }

    void SetBkColor( COLORREF color )
    { SendMessage( EM_SETBKGNDCOLOR, 0, color); 
    }

    void SetDefaultCharFormat( CharFormat const& fmt )
    { SendMessage(EM_SETCHARFORMAT, 0, reinterpret_cast<LPARAM>(&fmt));
    }

    void Append( CharFormat const& fmt, TCHAR const buf[] )
    { SetSelection(GetTextLength(), -1);
      SendMessage(EM_SETCHARFORMAT, SCF_SELECTION, reinterpret_cast<LPARAM>(&fmt));
      SendMessage(EM_REPLACESEL, 0, reinterpret_cast<LPARAM>(buf));
    }

  protected:
    RichEdit( HWND hwnd )
    : SimpleEdit(hwnd)
    {}

    RichEdit( HWND dlg, int id )
    : SimpleEdit(::GetDlgItem(dlg, id))
    {}    
};

class RichEditNotifyHandler : public EditNotifyHandler
{
  public:
    // Overriden method
    //virtual bool OnNotify( NMHDR* hdr, LRESULT& result );
};

class SingleLineRichEdit : public RichEdit
{
  public:
    SingleLineRichEdit( HWND hwnd = 0 )
    : RichEdit(hwnd)
    {}

    SingleLineRichEdit( HWND dlg, int id )
    : RichEdit(::GetDlgItem(dlg, id))
    {}    
};

class MultiLineRichEdit : public RichEdit
{
  public:
    MultiLineRichEdit( HWND hwnd = 0 )
    : RichEdit(hwnd)
    {}

    MultiLineRichEdit( HWND dlg, int id )
    : RichEdit(::GetDlgItem(dlg, id))
    {}

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
    { return SendMessage(EM_EXLINEFROMCHAR, 0, charIdx);
    }

    int GetFirstCharIndex( int lineIdx )
    { return SendMessage(EM_LINEINDEX, lineIdx);
    }

    int GetLineLength( int lineIdx )
    { return SendMessage(EM_LINELENGTH, GetFirstCharIndex(lineIdx));
    }

    void Scroll( int nLines )
    { SendMessage(EM_LINESCROLL, 0, nLines);
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

class SimpleRichEdit10Maker : public ControlMakerHandler<RichEditNotifyHandler>
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

    void NoHideSelection( void ) { _style |= ES_NOHIDESEL; }
    void Number( void ) { _style |= ES_NUMBER; }
    void Password( void ) { _style |= ES_PASSWORD; }
    void ReadOnly( void ) { _style |= ES_READONLY; }
    void WantReturn( void ) { _style |= ES_WANTRETURN; }

  protected:
    SimpleRichEdit10Maker( Win::Dow parent, int id, DWORD style = WS_VISIBLE|ES_AUTOHSCROLL|ES_LEFT, DWORD exStyle = WS_EX_CLIENTEDGE, int x = 0, int y = 0, int w = 0, int h = 0 )
    : ControlMakerHandler<RichEditNotifyHandler>(TEXT("RICHEDIT"), parent, id, style, exStyle, x, y, w, h)
    {}
};

class SingleLineRichEdit10Maker : public SimpleRichEdit10Maker
{
  public:
    SingleLineRichEdit10Maker( Win::Dow parent, int id, DWORD style = WS_VISIBLE|ES_AUTOHSCROLL|ES_LEFT, DWORD exStyle = WS_EX_CLIENTEDGE, int x = 0, int y = 0, int w = 0, int h = 0 )
    : SimpleRichEdit10Maker(parent, id, style, exStyle, x, y, w, h)
    {}
};

class MultiLineRichEdit10Maker : public SimpleRichEdit10Maker
{
  public:
    MultiLineRichEdit10Maker( Win::Dow parent, int id, DWORD style = WS_VISIBLE|WS_VSCROLL|WS_HSCROLL|ES_AUTOHSCROLL|ES_AUTOVSCROLL|ES_LEFT|ES_MULTILINE, DWORD exStyle = WS_EX_CLIENTEDGE, int x = 0, int y = 0, int w = 0, int h = 0 )
    : SimpleRichEdit10Maker(parent, id, style, exStyle, x, y, w, h)
    {}
};

class SimpleRichEdit20Maker : public ControlMakerHandler<RichEditNotifyHandler>
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

    void NoHideSelection( void ) { _style |= ES_NOHIDESEL; }
    void Number( void ) { _style |= ES_NUMBER; }
    void Password( void ) { _style |= ES_PASSWORD; }
    void ReadOnly( void ) { _style |= ES_READONLY; }
    void WantReturn( void ) { _style |= ES_WANTRETURN; }

  protected:
    SimpleRichEdit20Maker( Win::Dow parent, int id, DWORD style = WS_VISIBLE|ES_AUTOHSCROLL|ES_LEFT, DWORD exStyle = WS_EX_CLIENTEDGE, int x = 0, int y = 0, int w = 0, int h = 0 )
    : ControlMakerHandler<RichEditNotifyHandler>(RICHEDIT_CLASS, parent, id, style, exStyle, x, y, w, h)
    {}
};

class SingleLineRichEdit20Maker : public SimpleRichEdit20Maker
{
  public:
    SingleLineRichEdit20Maker( Win::Dow parent, int id, DWORD style = WS_VISIBLE|ES_AUTOHSCROLL|ES_LEFT, DWORD exStyle = WS_EX_CLIENTEDGE, int x = 0, int y = 0, int w = 0, int h = 0 )
    : SimpleRichEdit20Maker(parent, id, style, exStyle, x, y, w, h)
    {}
};

class MultiLineRichEdit20Maker : public SimpleRichEdit20Maker
{
  public:
    MultiLineRichEdit20Maker( Win::Dow parent, int id, DWORD style = WS_VISIBLE|WS_VSCROLL|WS_HSCROLL|ES_AUTOHSCROLL|ES_AUTOVSCROLL|ES_LEFT|ES_MULTILINE, DWORD exStyle = WS_EX_CLIENTEDGE, int x = 0, int y = 0, int w = 0, int h = 0 )
    : SimpleRichEdit20Maker(parent, id, style, exStyle, x, y, w, h)
    {}
};

} // namespace Win

#endif // WINRICHEDIT_H