#ifndef WINLISTVIEW_H
#define WINLISTVIEW_H

#include "Window.h"
#include "ImageList.h"
#include "NotifyHandler.h"
#include "Maker.h"

namespace Win
{

class ListView : public Win::Dow
{
  public: // Class
    class Item : public LVITEM
    {
      public:
        Item( int itemIdx, int subItemIdx = 0 )
        { 
          mask = 0;
          iItem = itemIdx;
          iSubItem = subItemIdx;
          state = 0;
          stateMask = 0;
          pszText = 0;
          cchTextMax = 0;
          iImage = 0;
          lParam = 0;
          iIndent = 0;
        }

        Item( int itemIdx, int subItemIdx, TCHAR const text[] )
        { 
          mask = LVIF_TEXT;
          iItem = itemIdx;
          iSubItem = subItemIdx;
          state = 0;
          stateMask = 0;
          pszText = const_cast<TCHAR*>(text);
          cchTextMax = 0;
          iImage = 0;
          lParam = 0;
          iIndent = 0;
        }

        Item( int itemIdx, TCHAR const text[], int imgIdx = -1 )
        {
          mask = LVIF_TEXT|(imgIdx >= 0 ? LVIF_IMAGE : 0);
          iItem = itemIdx;
          iSubItem = 0;
          state = 0;
          stateMask = 0;
          pszText = const_cast<TCHAR*>(text);
          cchTextMax = 0;
          iImage = imgIdx;
          lParam = 0;
          iIndent = 0;
        }

        Item( TCHAR const text[], int imgIdx = -1 )
        {
          mask = LVIF_TEXT|(imgIdx >= 0 ? LVIF_IMAGE : 0);
          iItem = -1;
          iSubItem = 0;
          state = 0;
          stateMask = 0;
          pszText = const_cast<TCHAR*>(text);
          cchTextMax = 0;
          iImage = imgIdx;
          lParam = 0;
          iIndent = 0;
        }

        void MakeCut( void )
        { mask |= LVIF_STATE;
          state |= LVIS_CUT;
          stateMask |= LVIS_CUT;
        }

        void MakeDropHilited( void )
        { mask |= LVIF_STATE;
          state |= LVIS_DROPHILITED;
          stateMask |= LVIS_DROPHILITED;
        }

        void MakeFocused( void )
        { mask |= LVIF_STATE;
          state |= LVIS_FOCUSED;
          stateMask |= LVIS_FOCUSED;
        }

        void MakeSelected( void )
        { mask |= LVIF_STATE;
          state |= LVIS_SELECTED;
          stateMask |= LVIS_SELECTED;
        }

        void SetOverayImageIndex( int imgIdx )
        { mask |= LVIF_STATE;
          state |= INDEXTOOVERLAYMASK(imgIdx);
          stateMask |= LVIS_OVERLAYMASK;
        }

        void SetStateImageIndex( int imgIdx )
        { mask |= LVIF_STATE;
          state |= INDEXTOSTATEIMAGEMASK(imgIdx);
          stateMask |= LVIS_STATEIMAGEMASK;
        }

        void SetText( TCHAR const text[] )
        { mask |= LVIF_TEXT;
          pszText = const_cast<TCHAR*>(text);
        }

        void AskForText( void )
        { mask |= LVIF_TEXT;
          pszText = LPSTR_TEXTCALLBACK;
        }

        void SetImage( int imgIdx )
        { mask |= LVIF_IMAGE;
          iImage = imgIdx;
        }

        void AskForImage( void )
        { mask |= LVIF_IMAGE;
          iImage = I_IMAGECALLBACK;
        }

        void SetParam( LPARAM param )
        { mask |= LVIF_PARAM;
          lParam = param;
        }

        void SetIndent( int times )
        { mask |= LVIF_INDENT;
          iIndent = times;
        }
    };

  public: // Class
    class Column : public LVCOLUMN
    {
      public:
        Column( int subItemIdx, TCHAR const text[], int width, int format = LVCFMT_LEFT )
        {
          mask = LVCF_SUBITEM|LVCF_TEXT|LVCF_WIDTH|LVCF_FMT;
          fmt = format;
          cx = width;
          pszText = const_cast<TCHAR*>(text);
          cchTextMax = 0;
          iSubItem = subItemIdx; 
        }
    };

  public: // Class
    class HitTestInfo : public LVHITTESTINFO
    {
      public:
        HitTestInfo( POINT const& point )
        { pt = point;
          flags = 0;
          iItem = 0;
          iSubItem = 0;
        }

        HitTestInfo( int x, int y )
        { pt.x = x; pt.y = y;
          flags = 0;
          iItem = 0;
          iSubItem = 0;
        }

        int GetItem( void ) const { return iItem; }
        int GetSubItem( void ) const { return iSubItem; }
        bool IsAbove( void ) const { return (flags&LVHT_ABOVE) != 0; }
        bool IsBelow( void ) const { return (flags&LVHT_BELOW) != 0; }
        bool IsNowhere( void ) const { return (flags&LVHT_NOWHERE) != 0; }
        bool IsOnItem( void ) const { return (flags&LVHT_ONITEM) != 0; }
        bool IsOnItemIcon( void ) const { return (flags&LVHT_ONITEMICON) != 0; }
        bool IsOnItemStateIcon( void ) const { return (flags&LVHT_ONITEMSTATEICON) != 0; }
        bool IsLeft( void ) const { return (flags&LVHT_TOLEFT) != 0; }
        bool IsRight( void ) const { return (flags&LVHT_TORIGHT) != 0; }
    };

  public: // Class
    class EnumSelectedItems
    {
      public:
        EnumSelectedItems( ListView& list )
        : _list(list),
          _cur(_list.GetNextSelected())
        {}

        bool AtEnd( void ) const
        { return _cur < 0;
        }

        void Advance( void )
        { _cur = _list.GetNextSelected(_cur);
        }

        int GetItemIndex( void )
        { return _cur;
        }

      private:
        ListView& _list;
        int       _cur;
    };

  public: // Methods
    explicit ListView( HWND hwnd = 0 )
    : Win::Dow(hwnd)
    {}

    ListView( HWND dlg, int id )
    : Win::Dow(::GetDlgItem(dlg, id))
    {}

    void ApproximateViewRect( int& cx, int& cy, int nItems = -1 ) const
    { DWORD rect = ListView_ApproximateViewRect(_hwnd, cx, cy, nItems);
      cx = LOWORD(rect);
      cy = HIWORD(rect);
    }

    void Arrange( UINT code )
    { ListView_Arrange(_hwnd, code);
    }

    void ArrangeAlignLeft( void )
    { Arrange(LVA_ALIGNLEFT);
    }

    void ArrangeAlignTop( void )
    { Arrange(LVA_ALIGNTOP);
    }

    void ArrangeDefault( void )
    { Arrange(LVA_DEFAULT);
    }

    void ArrangeSnapToGrid( void )
    { Arrange(LVA_SNAPTOGRID);
    }

    Win::ImageList CreateDragImage( int itemIdx, POINT& upLeft ) const
    { return Win::ImageList(ListView_CreateDragImage(_hwnd, itemIdx, &upLeft));
    }

    void Clear( void )
    { ListView_DeleteAllItems(_hwnd);
    }

    void DeleteColumn( int colIdx )
    { ListView_DeleteColumn(_hwnd, colIdx);
    }

    void DeleteItem( int itemIdx )
    { ListView_DeleteItem(_hwnd, itemIdx);
    }

    HWND EditLabel( int itemIdx )
    { return ListView_EditLabel(_hwnd, itemIdx);
    }

    void EnsureVisible( int itemIdx, bool partialOk = false ) const
    { ListView_EnsureVisible(_hwnd, itemIdx, partialOk);
    }

    int FindItem( LVFINDINFO const& info, int startIdx = -1 ) const
    { return ListView_FindItem(_hwnd, startIdx, &info);
    }

    COLORREF GetBkColor( void ) const
    { return ListView_GetBkColor(_hwnd);
    }

    void GetBkImage( LVBKIMAGE& data ) const
    { ListView_GetBkImage(_hwnd, &data);
    }

    UINT GetCallbackMask( void ) const
    { return ListView_GetCallbackMask(_hwnd);
    }

    bool GetCheckState( int itemIdx ) const
    { return ListView_GetCheckState(_hwnd, itemIdx) != 0;
    }

    void GetColumn( int colIdx, LVCOLUMN& col ) const
    { ListView_GetColumn(_hwnd, colIdx, &col);
    }

    void GetColumnOrderArray( int array[], int nCols ) const
    { ListView_GetColumnOrderArray(_hwnd, nCols, array);
    }

    int GetColumnWidth( int colIdx ) const
    { return ListView_GetColumnWidth(_hwnd, colIdx);
    }

    int GetCountPerPage( void ) const
    { return ListView_GetCountPerPage(_hwnd);
    }

    HWND GetEditControl( void )
    { return ListView_GetEditControl(_hwnd);
    }

    DWORD GetExtendedListViewStyle( void ) const
    { return ListView_GetExtendedListViewStyle(_hwnd);
    }

    HWND GetHeaderControl( void )
    { return ListView_GetHeader(_hwnd);
    }

    HCURSOR GetHotCursor( void ) const
    { return ListView_GetHotCursor(_hwnd);
    }

    int GetHotItem( void ) const
    { return ListView_GetHotItem(_hwnd);
    }

    DWORD GetHoverTime( void ) const
    { return ::SendMessage(_hwnd, LVM_GETHOVERTIME, 0, 0);
    }

    HIMAGELIST GetLargeImageList( void )
    { return ListView_GetImageList(_hwnd, LVSIL_NORMAL);
    }

    HIMAGELIST GetSmallImageList( void )
    { return ListView_GetImageList(_hwnd, LVSIL_SMALL);
    }

    HIMAGELIST GetStateImageList( void )
    { return ListView_GetImageList(_hwnd, LVSIL_STATE);
    }

    void GetItem( LVITEM& item ) const
    { ListView_GetItem(_hwnd, &item);
    }

    LPARAM GetItemParam( int itemIdx, int subItemIdx = 0 ) const
    { LVITEM item;
      item.mask = LVIF_PARAM;
      item.iItem = itemIdx;
      item.iSubItem = subItemIdx;
      item.lParam = 0;
      GetItem(item);
      return item.lParam;
    }

    int GetItemCount( void ) const
    { return ListView_GetItemCount(_hwnd);
    }

    void GetItemPosition( int itemIdx, POINT& pt ) const
    { ListView_GetItemPosition(_hwnd, itemIdx, &pt);
    }

    void GetItemBounds( int itemIdx, RECT& rect ) const
    { ListView_GetItemRect(_hwnd, itemIdx, &rect, LVIR_BOUNDS);
    }

    void GetItemIconRect( int itemIdx, RECT& rect ) const
    { ListView_GetItemRect(_hwnd, itemIdx, &rect, LVIR_ICON);
    }

    void GetItemLabelRect( int itemIdx, RECT& rect ) const
    { ListView_GetItemRect(_hwnd, itemIdx, &rect, LVIR_LABEL);
    }

    void GetItemSelectBounds( int itemIdx, RECT& rect ) const
    { ListView_GetItemRect(_hwnd, itemIdx, &rect, LVIR_SELECTBOUNDS);
    }

    DWORD GetItemSpacing( void ) const
    { DWORD style = ::GetWindowLong(_hwnd, GWL_STYLE);
      if ( style&LVS_SMALLICON )
      { return ListView_GetItemSpacing(_hwnd, TRUE);
      }
      else if ( style&LVS_ICON )
      { return ListView_GetItemSpacing(_hwnd, FALSE);
      }
      return 0;
    }

    UINT GetItemState( int itemIdx, UINT mask ) const
    { return ListView_GetItemState(_hwnd, itemIdx, mask);
    }

    bool IsCut( int itemIdx ) const
    { return (GetItemState(itemIdx, LVIS_CUT)&LVIS_CUT) != 0;
    }

    bool IsDropHilited( int itemIdx ) const
    { return (GetItemState(itemIdx, LVIS_DROPHILITED)&LVIS_DROPHILITED) != 0;
    }

    bool IsFocused( int itemIdx ) const
    { return (GetItemState(itemIdx, LVIS_FOCUSED)&LVIS_FOCUSED) != 0;
    }

    bool IsSelected( int itemIdx ) const
    { return (GetItemState(itemIdx, LVIS_SELECTED)&LVIS_SELECTED) != 0;
    }

    int GetOverlayMaskIndex( int itemIdx ) const
    { UINT state = GetItemState(itemIdx, LVIS_OVERLAYMASK);
      return (state&LVIS_OVERLAYMASK) >> 8;
    }

    int GetStateMaskIndex( int itemIdx ) const
    { UINT state = GetItemState(itemIdx, LVIS_STATEIMAGEMASK);
      return (state&LVIS_STATEIMAGEMASK) >> 12;
    }

/*
    std::string GetItemText( int itemIdx, int subItem = 0 )
    {
    }
*/
    
    int GetNextItem( UINT flags, int startIdx = -1 ) const
    { return ListView_GetNextItem(_hwnd, startIdx, flags);
    }

    int GetNextCut( int startIdx = -1 ) const
    { return GetNextItem(LVNI_CUT, startIdx);
    }

    int GetNextDropHilited( int startIdx = -1 ) const
    { return GetNextItem(LVNI_DROPHILITED, startIdx);
    }

    int GetNextSelected( int startIdx = -1 ) const
    { return GetNextItem(LVNI_SELECTED, startIdx);
    }

    int GetNextAbove( int startIdx = -1 ) const
    { return GetNextItem(LVNI_ABOVE, startIdx);
    }

    int GetNextBelow( int startIdx = -1 ) const
    { return GetNextItem(LVNI_BELOW, startIdx);
    }

    int GetNextToLeft( int startIdx = -1 ) const
    { return GetNextItem(LVNI_TOLEFT, startIdx);
    }

    int GetNextToRight( int startIdx = -1 ) const
    { return GetNextItem(LVNI_TORIGHT, startIdx);
    }

    int GetFocusedItem( void ) const
    { return GetNextItem(LVNI_FOCUSED);
    }

    UINT GetNumberOfWorkAreas( void ) const
    { UINT number = 0;
      ListView_GetNumberOfWorkAreas(_hwnd, &number);
      return number;
    }

    void GetOrigin( POINT& pt ) const
    { ListView_GetOrigin(_hwnd, &pt);
    }

    UINT GetSelectedCount( void ) const
    { return ListView_GetSelectedCount(_hwnd);
    }

    int GetSelectionMark( void ) const
    { return ListView_GetSelectionMark(_hwnd);
    }

    int GetStringWidth( TCHAR const str[] ) const
    { return ListView_GetStringWidth(_hwnd, str);
    }

    void GetSubItemBounds( int itemIdx, int subItemIdx, RECT& rect ) const
    { ListView_GetSubItemRect(_hwnd, itemIdx, subItemIdx, LVIR_BOUNDS, &rect);
    }

    void GetSubItemIconRect( int itemIdx, int subItemIdx, RECT& rect ) const
    { ListView_GetSubItemRect(_hwnd, itemIdx, subItemIdx, LVIR_ICON, &rect);
    }

    void GetSubItemLabelRect( int itemIdx, int subItemIdx, RECT& rect ) const
    { ListView_GetSubItemRect(_hwnd, itemIdx, subItemIdx, LVIR_LABEL, &rect);
    }

    COLORREF GetTextBkColor( void ) const
    { return ListView_GetTextBkColor(_hwnd);
    }

    COLORREF GetTextColor( void ) const
    { return ListView_GetTextColor(_hwnd);
    }

    HWND GetToolTipControl( void )
    { return reinterpret_cast<HWND>(SendMessage(LVM_GETTOOLTIPS));
    }

    int GetTopIndex( void ) const
    { return ListView_GetTopIndex(_hwnd);
    }

    bool IsUnicode( void ) const
    { return ListView_GetUnicodeFormat(_hwnd) != 0;
    }

    bool IsAnsi( void ) const
    { return ListView_GetUnicodeFormat(_hwnd) == 0;
    }

    void GetViewRect( RECT& rect ) const
    { ListView_GetViewRect(_hwnd, &rect);
    }

    void GetWorkAreas( RECT areas[], int nAreas ) const
    { ListView_GetWorkAreas(_hwnd, nAreas, areas);
    }

    int HitTest( LVHITTESTINFO& info ) const
    { return ListView_HitTest(_hwnd, &info);
    }

    int InsertColumn( int colIdx, LVCOLUMN const& col )
    { return ListView_InsertColumn(_hwnd, colIdx, &col);
    }

    int InsertItem( LVITEM const& item )
    { return ListView_InsertItem(_hwnd, &item);
    }

    void RedrawItems( int firstItemIdx, int lastItemIdx ) const
    { ListView_RedrawItems(_hwnd, firstItemIdx, lastItemIdx);
    }

    void Scroll( int dx, int dy )
    { ListView_Scroll(_hwnd, dx, dy);
    }

    void SetBkColor( COLORREF color )
    { ListView_SetBkColor(_hwnd, color);
    }

    void SetBkImage( LVBKIMAGE& data )
    { ListView_SetBkImage(_hwnd, &data);
    }

    void SetCallbackMask( UINT mask )
    { ListView_SetCallbackMask(_hwnd, mask);
    }

    void SetCheckState( int itemIdx, bool state )
    { SetStateImageIndex(itemIdx, state ? 2 : 1);
    }

    void Check( int itemIdx )
    { SetCheckState(itemIdx, true);
    }

    void UnCheck( int itemIdx )
    { SetCheckState(itemIdx, false);
    }

    void SetColumn( int colIdx, LVCOLUMN& col )
    { ListView_SetColumn(_hwnd, colIdx, &col);
    }

    void SetColumnOrderArray( int array[], int nCols )
    { ListView_SetColumnOrderArray(_hwnd, nCols, array);
    }

    void SetColumnWidth( int colIdx, int cx )
    { ListView_SetColumnWidth(_hwnd, colIdx, cx);
    }

    void SetExtendedListViewStyle( DWORD style )
    { ListView_SetExtendedListViewStyleEx(_hwnd, style, style);
    }

    void SetExStyleCheckBoxes( void )
    { SetExtendedListViewStyle(LVS_EX_CHECKBOXES);
    }

    void SetExStyleFlatScrollBars( void )
    { SetExtendedListViewStyle(LVS_EX_FLATSB);
    }

    void SetExStyleFullRowSelect( void )
    { SetExtendedListViewStyle(LVS_EX_FULLROWSELECT);
    }

    void SetExStyleGridLines( void )
    { SetExtendedListViewStyle(LVS_EX_GRIDLINES);
    }

    void SetExStyleHeaderDragDrop( void )
    { SetExtendedListViewStyle(LVS_EX_HEADERDRAGDROP);
    }

    void SetExStyleInfoTip( void )
    { SetExtendedListViewStyle(LVS_EX_INFOTIP);
    }
/*
    void SetExStyleLabelTip( void )
    { SetExtendedListViewStyle(LVS_EX_LABELTIP);
    }
*/
    void SetExStyleMultiWorkAreas( void )
    { SetExtendedListViewStyle(LVS_EX_MULTIWORKAREAS);
    }

    void SetExStyleOneClickActivate( void )
    { SetExtendedListViewStyle(LVS_EX_ONECLICKACTIVATE);
    }

    void SetExStyleRegional( void )
    { SetExtendedListViewStyle(LVS_EX_REGIONAL);
    }

    void SetExStyleSubItemImages( void )
    { SetExtendedListViewStyle(LVS_EX_SUBITEMIMAGES);
    }

    void SetExStyleTrackSelect( void )
    { SetExtendedListViewStyle(LVS_EX_TRACKSELECT);
    }

    void SetExStyleTwoClickActivate( void )
    { SetExtendedListViewStyle(LVS_EX_TWOCLICKACTIVATE);
    }

    void SetExStyleUnderlineCold( void )
    { SetExtendedListViewStyle(LVS_EX_UNDERLINECOLD);
    }

    void SetExStyleUnderlineHot( void )
    { SetExtendedListViewStyle(LVS_EX_UNDERLINEHOT);
    }

    HCURSOR SetHotCursor( HCURSOR hCursor )
    { return ListView_SetHotCursor(_hwnd, hCursor);
    }

    int SetHotItem( int itemIdx )
    { return ListView_SetHotItem(_hwnd, itemIdx);
    }

    void SetHoverTime( DWORD time )
    { SendMessage(LVM_SETHOVERTIME, 0, time);
    }

    void SetIconSpacing( int cx, int cy )
    { ListView_SetIconSpacing(_hwnd, cx, cy);
    }

    void SetLargeImageList( HIMAGELIST himl )
    { ListView_SetImageList(_hwnd, himl, LVSIL_NORMAL);
    }

    void SetSmallImageList( HIMAGELIST himl )
    { ListView_SetImageList(_hwnd, himl, LVSIL_SMALL);
    }

    void SetStateImageList( HIMAGELIST himl )
    { ListView_SetImageList(_hwnd, himl, LVSIL_STATE);
    }

    void SetItem( LVITEM const& item )
    { ListView_SetItem(_hwnd, &item);
    }

    void SetItemCount( int count )
    { ListView_SetItemCount(_hwnd, count);
    }

    void SetItemCountNoInvalidateAll( int count )
    { ListView_SetItemCountEx(_hwnd, count, LVSICF_NOINVALIDATEALL);
    }

    void SetItemCountNoScroll( int count )
    { ListView_SetItemCountEx(_hwnd, count, LVSICF_NOSCROLL);
    }

    void SetItemPosition( int itemIdx, POINT const& pt )
    { ListView_SetItemPosition32(_hwnd, itemIdx, pt.x, pt.y);
    }

    void SetItemState( int itemIdx, UINT state, UINT mask )
    { ListView_SetItemState(_hwnd, itemIdx, state, mask);
    }

    void MakeCut( int itemIdx, bool make = true )
    { SetItemState(itemIdx, make ? LVIS_CUT : 0, LVIS_CUT);
    }

    void MakeDropHilited( int itemIdx, bool make = true )
    { SetItemState(itemIdx, make ? LVIS_DROPHILITED : 0, LVIS_DROPHILITED);
    }

    void MakeFocused( int itemIdx, bool make = true )
    { SetItemState(itemIdx, make ? LVIS_FOCUSED : 0, LVIS_FOCUSED);
    }

    void MakeSelected( int itemIdx, bool make = true )
    { SetItemState(itemIdx, make ? LVIS_SELECTED : 0, LVIS_SELECTED);
    }

    void SetOverlayImageIndex( int itemIdx, int imgIdx )
    { SetItemState(itemIdx, INDEXTOOVERLAYMASK(imgIdx), LVIS_OVERLAYMASK);
    }

    void SetStateImageIndex( int itemIdx, int imgIdx )
    { SetItemState(itemIdx, INDEXTOSTATEIMAGEMASK(imgIdx), LVIS_STATEIMAGEMASK);
    }

    void SetItemText( int itemIdx, TCHAR const text[], int subItemIdx = 0 )
    { ListView_SetItemText(_hwnd, itemIdx, subItemIdx, const_cast<TCHAR*>(text));
    }

    int SetSelectionMark( int newMarkIdx )
    { return ListView_SetSelectionMark(_hwnd, newMarkIdx);
    }

    void SetTextBkColor( COLORREF color )
    { ListView_SetTextBkColor(_hwnd, color);
    }

    void SetTextColor( COLORREF color )
    { ListView_SetTextColor(_hwnd, color);
    }

    HWND SetToolTipControl( HWND hwndToolTip )
    { return reinterpret_cast<HWND>(SendMessage(LVM_SETTOOLTIPS, 0, reinterpret_cast<WPARAM>(hwndToolTip)));
    }

    void SetUnicode( void )
    { ListView_SetUnicodeFormat(_hwnd, TRUE);
    }

    void SetAnsi( void )
    { ListView_SetUnicodeFormat(_hwnd, FALSE);
    }

    void SetWorkAreas( RECT areas[], int nAreas )
    { ListView_SetWorkAreas(_hwnd, nAreas, areas);
    }

    void SortItems( PFNLVCOMPARE compare, LPARAM lParam )
    { ListView_SortItems(_hwnd, compare, lParam);
    }
/*
    void SortItemsEx( PFNLVCOMPARE compare, LPARAM lParam )
    { ListView_SortItemsEx(_hwnd, compare, lParam);
    }
*/
    int SubItemHitTest( LVHITTESTINFO& info ) const
    { return ListView_SubItemHitTest(_hwnd, &info);
    }

    void UpdateItem( int itemIdx ) const
    { ListView_Update(_hwnd, itemIdx);
    }

    int AppendItem( LVITEM& item )
    { item.iItem = GetItemCount();
      return InsertItem(item);
    }
};

class ListViewNotifyHandler : public Win::CommonNotifyHandler
{
  public:
    // Overriden methods
    virtual bool OnNotify( NMHDR* hdr, LRESULT& result );

    // New methods
    // Common messages
    virtual bool OnClick( HWND hwnd, int itemIdx, int subItemIdx, POINT& ptAction ) { return false; }
    virtual bool OnCustomDraw( HWND hwnd, NMLVCUSTOMDRAW& cd, LRESULT& result ) { return false; }
    virtual bool OnDoubleClick( HWND hwnd, int itemIdx, int subItemIdx, POINT& ptAction ) { return false; }
    virtual bool OnRightClick( HWND hwnd, int itemIdx, int subItemIdx, POINT& ptAction ) { return false; }
    virtual bool OnRightDoubleClick( HWND hwnd, int itemIdx, int subItemIdx, POINT& ptAction ) { return false; }
    
    // ListView specific messages
    virtual bool OnBeginDrag( HWND hwnd, int itemIdx ) { return false; }
    virtual bool OnBeginLabelEdit( HWND hwnd, int itemIdx, LRESULT& result ) { return false; }
    virtual bool OnBeginRightDrag( HWND hwnd, int itemIdx ) { return false; }
    virtual bool OnColumnClick( HWND hwnd, int colIdx ) { return false; }
    virtual bool OnDeleteAllItems( HWND hwnd, LRESULT& result ) { return false; }
    virtual bool OnDeleteItem( HWND hwnd, int itemIdx ) { return false; }
    virtual bool OnEndLabelEdit( HWND hwnd, int itemIdx, TCHAR const newLabel[], LRESULT& result ) { return false; }
    virtual bool OnGetDispInfo( HWND hwnd, LVITEM& item ) { return false; }
    virtual bool OnGetInfoTip( HWND hwnd, NMLVGETINFOTIP& data ) { return false; }
    virtual bool OnHotTrack( HWND hwnd, int& itemIdx, int subItemIdx, POINT const& ptAction, LRESULT& result ) { return false; }
    virtual bool OnInsertItem( HWND hwnd, int itemIdx ) { return false; }
    virtual bool OnItemActivate( HWND hwnd, NMITEMACTIVATE const& data ) { return false; }
    virtual bool OnItemChanged( HWND hwnd, NMLISTVIEW const& data ) { return false; }
    virtual bool OnItemChanging( HWND hwnd, NMLISTVIEW const& data, LRESULT& result ) { return false; }
    virtual bool OnMarqueeBegin( HWND hwnd, LRESULT& result ) { return false; }
    virtual bool OnOdCacheHint( HWND hwnd, int fromIdx, int toIdx ) { return false; }
    virtual bool OnOdFindItem( HWND hwnd, int startIdx, LVFINDINFO const& info, LRESULT& result ) { return false; }
    virtual bool OnOdStateChanged( HWND hwnd, int fromIdx, int toIdx, UINT oldState, UINT newState ) { return false; }
    virtual bool OnSetDispInfo( HWND hwnd, LVITEM const& item ) { return false; }
};

class ListViewMaker : public Win::ControlMakerHandler<ListViewNotifyHandler>
{
  public:
    ListViewMaker( Win::Dow parent, int id, DWORD style = WS_VISIBLE|LVS_LIST|LVS_SHAREIMAGELISTS, DWORD exStyle = WS_EX_CLIENTEDGE, int x = 0, int y = 0, int w = 0, int h = 0 )
    : ControlMakerHandler<ListViewNotifyHandler>(WC_LISTVIEW, parent, id, style, exStyle, x, y, w, h)
    {}

    void AlignLeft( void )
    { _style |= LVS_ALIGNLEFT;
    }

    void AlignTop( void )
    { _style |= LVS_ALIGNTOP;
    }

    void AutoArrange( void )
    { _style |= LVS_AUTOARRANGE;
    }

    void EditLabels( void )
    { _style |= LVS_EDITLABELS;
    }

    void LargeIcon( void )
    { _style |= LVS_ICON;
    }

    void List( void )
    { _style |= LVS_LIST;
    }

    void NoColumnHeader( void )
    { _style |= LVS_NOCOLUMNHEADER;
    }

    void NoLabelWrap( void )
    { _style |= LVS_NOLABELWRAP;
    }

    void NoScroll( void )
    { _style |= LVS_NOSCROLL;
    }

    void NoSortHeader( void )
    { _style |= LVS_NOSORTHEADER;
    }

    void OwnerData( void )
    { _style |= LVS_OWNERDATA;
    }

    void OwnerDrawFixed( void )
    { _style |= LVS_OWNERDRAWFIXED;
    }

    void Report( void )
    { _style |= LVS_REPORT;
    }

    void ShareImageLists( void )
    { _style |= LVS_SHAREIMAGELISTS;
    }

    void ShowSelAlways( void )
    { _style |= LVS_SHOWSELALWAYS;
    }

    void SingleSel( void )
    { _style |= LVS_SINGLESEL;
    }

    void SmallIcon( void )
    { _style |= LVS_SMALLICON;
    }

    void SortAscending( void )
    { _style |= LVS_SORTASCENDING;
    }

    void SortDescending( void )
    { _style |= LVS_SORTDESCENDING;
    }
};

} // namespace Win

#endif // WINLISTVIEW_H