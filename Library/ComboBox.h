#ifndef WINCOMBOBOX_H
#define WINCOMBOBOX_H

#include "Window.h"
#include "NotifyHandler.h"
#include "Maker.h"
#include "stdstring"

namespace Win
{

class ComboBox : public Win::Dow
{
  public:
    explicit ComboBox( HWND hwnd = 0 )
    : Win::Dow(hwnd)
    {}

    ComboBox( HWND dlg, int id )
    : Win::Dow(::GetDlgItem(dlg, id))
    {}

    int AddString( TCHAR const str[] )
    { return SendMessage(CB_ADDSTRING, 0, reinterpret_cast<LPARAM>(str));
    }

    int AddString( stdstring const& str )
    { return AddString(str.c_str());
    }

    void DeleteString( int itemIdx )
    { SendMessage(CB_DELETESTRING, itemIdx);
    }

    int FindString( TCHAR const str[], int startIdx = -1 ) const
    { return SendMessage(CB_FINDSTRING, startIdx, reinterpret_cast<LPARAM>(str));
    }

    int FindString( stdstring const& str, int startIdx = -1 ) const
    { return FindString(str.c_str(), startIdx);
    }

    int FindStringExact( TCHAR const str[], int startIdx = -1 ) const
    { return SendMessage(CB_FINDSTRINGEXACT, startIdx, reinterpret_cast<LPARAM>(str));
    }

    int FindStringExact( stdstring const& str, int startIdx = -1 ) const
    { return FindStringExact(str.c_str(), startIdx);
    }

    int GetItemCount( void ) const
    { return SendMessage(CB_GETCOUNT);
    }

    int GetSelectedItem( void ) const
    { return SendMessage(CB_GETCURSEL);
    }

    void GetDroppedControlRect( RECT& rect ) const
    { SendMessage(CB_GETDROPPEDCONTROLRECT, 0, reinterpret_cast<LPARAM>(&rect));
    }

    bool IsDropped( void ) const
    { return SendMessage(CB_GETDROPPEDSTATE) != 0;
    }

    int GetDroppedWidth( void ) const
    { return SendMessage(CB_GETDROPPEDWIDTH);
    }

    void GetEditSelection( DWORD& start, DWORD& end ) const
    { SendMessage(CB_GETEDITSEL, reinterpret_cast<WPARAM>(&start), reinterpret_cast<LPARAM>(&end));
    }

    bool HasExtendedUI( void ) const
    { return SendMessage(CB_GETEXTENDEDUI) != 0;
    }

    int GetHorizontalExtent( void ) const
    { return SendMessage(CB_GETHORIZONTALEXTENT);
    }

    LPARAM GetItemData( int itemIdx ) const
    { return SendMessage(CB_GETITEMDATA, itemIdx);
    }

    int GetItemHeight( int itemIdx ) const
    { return SendMessage(CB_GETITEMHEIGHT, itemIdx);
    }

	stdstring GetString( int itemIdx ) const
    { int len = GetStringLength(itemIdx);
	stdstring str;
      str.reserve(len + 1);
      str.resize(len);
      SendMessage(CB_GETLBTEXT, itemIdx, reinterpret_cast<LPARAM>(&str[0]));
      return str;
    }

    int GetStringLength( int itemIdx ) const
    { return SendMessage(CB_GETLBTEXTLEN, itemIdx);
    }

    int GetTopIndex( void ) const
    { return SendMessage(CB_GETTOPINDEX);
    }

    void SetItemCount( int itemCount, int maxTextLen )
    { SendMessage(CB_INITSTORAGE, itemCount, maxTextLen);
    }

    int InsertString( int desiredIdx, TCHAR const str[] )
    { return SendMessage(CB_INSERTSTRING, desiredIdx, reinterpret_cast<LPARAM>(str));
    }

    int InsertString( int desiredIdx, stdstring const& str )
    { return InsertString(desiredIdx, str.c_str());
    }

    void SetLimitText( int limit )
    { SendMessage(CB_LIMITTEXT, limit);
    }

    void Clear( void )
    { SendMessage(CB_RESETCONTENT);
    }

    int SelectString( TCHAR const str[], int startIdx = -1 )
    { return SendMessage(CB_SELECTSTRING, startIdx, reinterpret_cast<LPARAM>(str));
    }

    int SelectString( stdstring const& str, int startIdx = -1 )
    { return SelectString(str.c_str(), startIdx);
    }

    void SelectItem( int itemIdx )
    { SendMessage(CB_SETCURSEL, itemIdx);
    }

    void SetDroppedWidth( int width )
    { SendMessage(CB_SETDROPPEDWIDTH, width);
    }

    void SetEditSelection( int start, int end )
    { SendMessage(CB_SETEDITSEL, 0, MAKELPARAM(start, end));
    }

    void SetExtendedUI( bool set = true )
    { SendMessage(CB_SETEXTENDEDUI, set);
    }

    void SetHorizontalExtent( int extent )
    { SendMessage(CB_SETHORIZONTALEXTENT, extent);
    }

    void SetItemData( int itemIdx, LPARAM data )
    { SendMessage(CB_SETITEMDATA, itemIdx, data);
    }

    void SetItemHeight( int itemIdx, int height )
    { SendMessage(CB_SETITEMHEIGHT, itemIdx, height);
    }

    void SetTopIndex( int itemIdx )
    { SendMessage(CB_SETTOPINDEX, itemIdx);
    }

    void ShowDropDown( bool show = true )
    { SendMessage(CB_SHOWDROPDOWN, show);
    }

    void HideDropDown( void )
    { ShowDropDown(false);
    }
};

class ComboBoxNotifyHandler : public Win::NotifyHandler
{
  public:
    // Overriden methods
    virtual bool OnNotify( NMHDR* hdr, LRESULT& result );

    // New methods
    virtual bool OnDoubleClick( HWND hwnd ) { return false; }
    virtual bool OnOutOfMemory( HWND hwnd ) { return false; }
    virtual bool OnKillFocus( HWND hwnd ) { return false; }
    virtual bool OnSelectionEndCancel( HWND hwnd ) { return false; }
    virtual bool OnSelectionEndOk( HWND hwnd ) { return false; }
    virtual bool OnSelectionChanged( HWND hwnd ) { return false; }
    virtual bool OnSetFocus( HWND hwnd ) { return false; }
    virtual bool OnCloseUp( HWND hwnd ) { return false; }
    virtual bool OnDropDown( HWND hwnd ) { return false; }
    virtual bool OnEditChanged( HWND hwnd ) { return false; }
    virtual bool OnEditUpdate( HWND hwnd ) { return false; }
};

class ComboBoxMaker : public ControlMakerHandler<ComboBoxNotifyHandler>
{
  public:
    ComboBoxMaker( Win::Dow parent, int id, DWORD style = WS_VISIBLE|CBS_DROPDOWNLIST, DWORD exStyle = 0, int x = 0, int y = 0, int w = 0, int h = 0 )
    : ControlMakerHandler<ComboBoxNotifyHandler>(TEXT("COMBOBOX"), parent, id, style, exStyle, x, y, w, h)
    {}

    void AutoHScroll( void )
    { _style |= CBS_AUTOHSCROLL;
    }

    void DisableNoScroll( void )
    { _style |= CBS_DISABLENOSCROLL;
    }

    void MakeDropDown( void )
    { _style |= CBS_DROPDOWN;
    }

    void MakeDropDownList( void )
    { _style |= CBS_DROPDOWNLIST;
    }

    void MakeSimple( void )
    { _style |= CBS_SIMPLE;
    }

    void HasStrings( void )
    { _style |= CBS_HASSTRINGS;
    }

    void LowerCase( void )
    { _style |= CBS_LOWERCASE;
    }

    void UpperCase( void )
    { _style |= CBS_UPPERCASE;
    }

    void NoIntergralHeight( void )
    { _style |= CBS_NOINTEGRALHEIGHT;
    }

    void OwnerDrawFixed( void )
    { _style |= CBS_OWNERDRAWFIXED;
    }

    void OwnerDrawVariable( void )
    { _style |= CBS_OWNERDRAWVARIABLE;
    }

    void Sort( void )
    { _style |= CBS_SORT;
    }
};

} // namespace Win

#endif // WINCOMBOBOX_H