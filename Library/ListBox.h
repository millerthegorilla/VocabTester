#ifndef WINLISTBOX_H
#define WINLISTBOX_H

#include "Window.h"
#include "NotifyHandler.h"
#include "Maker.h"

namespace Win
{

class ListBox : public Win::Dow
{
  public:
    explicit ListBox( HWND hwnd = 0 )
    : Win::Dow(hwnd)
    {}

    ListBox( HWND dlg, int id )
    : Win::Dow(::GetDlgItem(dlg, id))
    {}

    int AddString( TCHAR const str[] )
    { return SendMessage(LB_ADDSTRING, 0, reinterpret_cast<LPARAM>(str));
    }

    void DeleteString( int itemIdx )
    { SendMessage(LB_DELETESTRING, itemIdx);
    }

    int FindString( TCHAR const str[], int startIdx = -1 ) const
    { return ::SendMessage(_hwnd, LB_FINDSTRING, startIdx, reinterpret_cast<LPARAM>(str));
    }

    int FindStringExact( TCHAR const str[], int startIdx = -1 ) const
    { return ::SendMessage(_hwnd, LB_FINDSTRINGEXACT, startIdx, reinterpret_cast<LPARAM>(str));
    }

    int GetCaretIndex( void ) const
    { return ::SendMessage(_hwnd, LB_GETCARETINDEX, 0, 0);
    }

    int GetItemCount( void ) const
    { return ::SendMessage(_hwnd, LB_GETCOUNT, 0, 0);
    }

    int GetHorizontalExtent( void ) const
    { return ::SendMessage(_hwnd, LB_GETHORIZONTALEXTENT, 0, 0);
    }

    LPARAM GetItemParam( int itemIdx ) const
    { return ::SendMessage(_hwnd, LB_GETITEMDATA, itemIdx, 0);
    }

    int GetItemHeight( int itemIdx ) const
    { return ::SendMessage(_hwnd, LB_GETITEMHEIGHT, itemIdx, 0);
    }

    void GetItemRect( int itemIdx, RECT& rect ) const
    { ::SendMessage(_hwnd, LB_GETITEMRECT, itemIdx, reinterpret_cast<LPARAM>(&rect));
    }

    bool IsSelected( int itemIdx ) const
    { return ::SendMessage(_hwnd, LB_GETSEL, itemIdx, 0) > 0;
    }

    stdstring GetItemText( int itemIdx ) const
    { int len = GetItemTextLength(itemIdx);
      stdstring str;
      str.reserve(len + 1);
      str.resize(len);
      ::SendMessage(_hwnd, LB_GETTEXT, itemIdx, reinterpret_cast<LPARAM>(&str[0]));
      return str;
    }

    int GetItemTextLength( int itemIdx ) const
    { return ::SendMessage(_hwnd, LB_GETTEXTLEN, itemIdx, 0);
    }

    int GetTopIndex( void ) const
    { return ::SendMessage(_hwnd, LB_GETTOPINDEX, 0, 0);
    }

    void SetItemCount( int itemCount, int maxTextLen )
    { SendMessage(LB_INITSTORAGE, itemCount, maxTextLen);
    }

    int InsertString( int insertAt, TCHAR const str[] )
    { return SendMessage(LB_INSERTSTRING, insertAt, reinterpret_cast<LPARAM>(str));
    }

    int ItemFromPoint( int x, int y )
    { DWORD res = SendMessage(LB_ITEMFROMPOINT, 0, MAKELPARAM(x,y));
      return LOWORD(res);
    }

    void Clear( void )
    { SendMessage(LB_RESETCONTENT);
    }

    int SelectString( TCHAR const str[], int startIdx = -1 )
    { return SendMessage(LB_SELECTSTRING, startIdx, reinterpret_cast<LPARAM>(str));
    }

    void SetCaretIndex( int itemIdx, bool partialVisibleOk = false )
    { SendMessage(LB_SETCARETINDEX, itemIdx, partialVisibleOk);
    }

    void SetHorizontalExtent( int extent )
    { SendMessage(LB_SETHORIZONTALEXTENT, extent);
    }

    void SetItemParam( int itemIdx, LPARAM param )
    { SendMessage(LB_SETITEMDATA, itemIdx, param);
    }

    void SetItemHeight( int itemIdx, int height )
    { SendMessage(LB_SETITEMHEIGHT, itemIdx, height);
    }

    void SetTopIndex( int itemIdx )
    { SendMessage(LB_SETTOPINDEX, itemIdx);
    }
};

class SingleSelectionListBox : public ListBox
{
  public:
    explicit SingleSelectionListBox( HWND hwnd = 0 )
    : ListBox(hwnd)
    {}

    SingleSelectionListBox( HWND dlg, int id )
    : ListBox(dlg, id)
    {}

    int GetSelectedItem( void ) const
    { return ::SendMessage(_hwnd, LB_GETCURSEL, 0, 0);
    }

    void SelectItem( int itemIdx )
    { SendMessage(LB_SETCURSEL, itemIdx);
    }
};

class MultiSelectionListBox : public ListBox
{
  public:
    explicit MultiSelectionListBox( HWND hwnd = 0 )
    : ListBox(hwnd)
    {}

    MultiSelectionListBox( HWND dlg, int id )
    : ListBox(dlg, id)
    {}

    int GetAnchorIndex( void ) const
    { return ::SendMessage(_hwnd, LB_GETANCHORINDEX, 0, 0);
    }

    int GetSelectedCount( void ) const
    { return ::SendMessage(_hwnd, LB_GETSELCOUNT, 0, 0);
    }

    int GetSelectedItems( int items[], int nItems ) const
    { return ::SendMessage(_hwnd, LB_GETSELITEMS, nItems, reinterpret_cast<LPARAM>(items));
    }

    void SelectRange( int firstIdx, int lastIdx )
    { SendMessage(LB_SELITEMRANGE, TRUE, MAKELPARAM(firstIdx, lastIdx));
    }

    void DeselectRange( int firstIdx, int lastIdx )
    { SendMessage(LB_SELITEMRANGE, FALSE, MAKELPARAM(firstIdx, lastIdx));
    }

    void SetAnchorIndex( int itemIdx )
    { SendMessage(LB_SETANCHORINDEX, itemIdx);
    }

    void SelectItem( int itemIdx )
    { SendMessage(LB_SETSEL, TRUE, itemIdx);
    }

    void DeselectItem( int itemIdx )
    { SendMessage(LB_SETSEL, FALSE, itemIdx);
    }

    void SelectAll( void )
    { SelectItem(-1);
    }

    void DeselectAll( void )
    { DeselectItem(-1);
    }
};

class ListBoxNotifyHandler : public Win::NotifyHandler
{
  public:
    // Overrident methods
    virtual bool OnNotify( NMHDR* hdr, LRESULT& result );

    // New methods
    virtual bool OnDoubleClick( HWND hwnd ) { return false; }
    virtual bool OnOutOfMemory( HWND hwnd ) { return false; }
    virtual bool OnKillFocus( HWND hwnd ) { return false; }
    virtual bool OnSelectionCancel( HWND hwnd ) { return false; }
    virtual bool OnSelectionChange( HWND hwnd ) { return false; }
    virtual bool OnSetFocus( HWND hwnd ) { return false; }
};

class SimpleListBoxMaker : public ControlMakerHandler<ListBoxNotifyHandler>
{
  public:
    void DisableNoScroll( void )
    { _style |= LBS_DISABLENOSCROLL;
    }

    void ExtendedSel( void )
    { _style |= LBS_EXTENDEDSEL;
    }

    void HasStrings( void )
    { _style |= LBS_HASSTRINGS;
    }

    void MultiSelection( void )
    { _style |= LBS_MULTIPLESEL;
    }

    void NoIntegralHeight( void )
    { _style |= LBS_NOINTEGRALHEIGHT;
    }

    void NoSelection( void )
    { _style |= LBS_NOSEL;
    }

    void Notify( void )
    { _style |= LBS_NOTIFY;
    }

    void OwnerDrawFixed( void )
    { _style |= LBS_OWNERDRAWFIXED;
    }

    void OwnerDrawVariable( void )
    { _style |= LBS_OWNERDRAWVARIABLE;
    }

    void Sort( void )
    { _style |= LBS_SORT;
    }

    void WantKeyboardInput( void )
    { _style |= LBS_WANTKEYBOARDINPUT;
    }

  protected:
    SimpleListBoxMaker( Win::Dow parent, int id, DWORD style, DWORD exStyle, int x, int y, int w, int h )
    : ControlMakerHandler<ListBoxNotifyHandler>(TEXT("LISTBOX"), parent, id, style, exStyle, x, y, w, h)
    {}
};

class SingleSelectionListBoxMaker : public SimpleListBoxMaker
{
  public:
    SingleSelectionListBoxMaker( Win::Dow parent, int id, DWORD style = WS_VISIBLE|LBS_STANDARD, DWORD exStyle = 0, int x = 0, int y = 0, int w = 0, int h = 0 )
    : SimpleListBoxMaker(parent, id, style, exStyle, x, y, w, h)
    {}
};

class MultiSelectionListBoxMaker : public SimpleListBoxMaker
{
  public:
    MultiSelectionListBoxMaker( Win::Dow parent, int id, DWORD style = WS_VISIBLE|LBS_STANDARD|LBS_MULTIPLESEL, DWORD exStyle = 0, int x = 0, int y = 0, int w = 0, int h = 0 )
    : SimpleListBoxMaker(parent, id, style, exStyle, x, y, w, h)
    {}
};

} // namespace Win

#endif // WINLISTBOX_H