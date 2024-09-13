#ifndef WINTREEVIEW_H
#define WINTREEVIEW_H

#include "Window.h"
#include "NotifyHandler.h"
#include "Maker.h"
#include "ImageList.h"

namespace Win
{

class TreeView : public Win::Dow
{
  public: // Class
    class Item : public TVITEM
    {
      public: // Class
        class State
        {
          public:
            State( UINT state )
            : _state(state)
            {}

            bool IsBold( void ) const { return (_state&TVIS_BOLD) != 0; }
            bool IsCut( void ) const { return (_state&TVIS_CUT) != 0; }
            bool IsDropHilited( void ) const { return (_state&TVIS_DROPHILITED) != 0; }
            bool IsExpanded( void ) const { return (_state&TVIS_EXPANDED) != 0; }
            bool IsExpandedOnce( void ) const { return (_state&TVIS_EXPANDEDONCE) != 0; }
            bool IsSelected( void ) const { return (_state&TVIS_SELECTED) != 0; }

          private:
            UINT _state;
        };

      public:
        Item( HTREEITEM item = 0 )
        { mask = 0;
          if ( item != 0 )
          { mask |= TVIF_HANDLE;
          }
          hItem = item;
          state = 0;
          stateMask = 0;
          pszText = 0;
          cchTextMax = 0;
          iImage = 0;
          iSelectedImage = 0;
          cChildren = 0;
          lParam = 0;
        }

        // Use for specifying item's text
        Item( TCHAR const text[], int imgIdx = -1, int selImgIdx = -1 )
        { mask = TVIF_TEXT;
          hItem = 0;
          state = 0;
          stateMask = 0;
          pszText = const_cast<TCHAR*>(text);
          cchTextMax = 0;
          if ( imgIdx >= 0 )
          { mask |= TVIF_IMAGE;
          }
          iImage = imgIdx;
          if ( selImgIdx >= 0 )
          { mask |= TVIF_SELECTEDIMAGE;
          }
          iSelectedImage = selImgIdx;
          cChildren = 0;
          lParam = 0;
        }

        void MakeBold( void )
        { mask |= TVIF_STATE;
          state |= TVIS_BOLD;
          stateMask |= TVIS_BOLD;
        }

        void MakeCut( void )
        { mask |= TVIF_STATE;
          state |= TVIS_CUT;
          stateMask |= TVIS_CUT;
        }

        void MakeDropHilited( void )
        { mask |= TVIF_STATE;
          state |= TVIS_DROPHILITED;
          stateMask |= TVIS_DROPHILITED;
        }

        void SetText( TCHAR const text[] )
        { mask |= TVIF_TEXT;
          pszText = const_cast<TCHAR*>(text);
        }

        void AskForText( void )
        { mask |= TVIF_TEXT;
          pszText = LPSTR_TEXTCALLBACK;
        }

        void SetImage( int imgIdx )
        { mask |= TVIF_IMAGE;
          iImage = imgIdx;
        }

        void AskForImage( void )
        { mask |= TVIF_IMAGE;
          iImage = I_IMAGECALLBACK;
        }

        void SetSelectedImage( int imgIdx )
        { mask |= TVIF_SELECTEDIMAGE;
          iSelectedImage = imgIdx;
        }

        void AskForSelectedImage( void )
        { mask |= TVIF_SELECTEDIMAGE;
          iSelectedImage = I_IMAGECALLBACK;
        }

        void HasChildren( bool has )
        { mask |= TVIF_CHILDREN;
          cChildren = has;
        }

        void AskForChildren( void )
        { mask |= TVIF_CHILDREN;
          cChildren = I_CHILDRENCALLBACK;
        }

        void SetParam( LPARAM param )
        { mask |= TVIF_PARAM;
          lParam = param;
        }
    };

  public: // Class
    class HitTestInfo : public TVHITTESTINFO
    {
      public:
        HitTestInfo( POINT const& point )
        { pt = point;
          flags = 0;
          hItem = 0;
        }

        HTREEITEM GetItem( void ) const { return hItem; }
        bool IsAbove( void ) const { return (flags&TVHT_ABOVE) != 0; }
        bool IsBelow( void ) const { return (flags&TVHT_BELOW) != 0; }
        bool IsNowhere( void ) const { return (flags&TVHT_NOWHERE) != 0; }
        bool IsOnItem( void ) const { return (flags&TVHT_ONITEM) != 0; }
        bool IsOnItemButton( void ) const { return (flags&TVHT_ONITEMBUTTON) != 0; }
        bool IsOnItemIcon( void ) const { return (flags&TVHT_ONITEMICON) != 0; }
        bool IsOnItemIndent( void ) const { return (flags&TVHT_ONITEMINDENT) != 0; }
        bool IsOnItemLabel( void ) const { return (flags&TVHT_ONITEMLABEL) != 0; }
        bool IsOnItemRight( void ) const { return (flags&TVHT_ONITEMRIGHT) != 0; }
        bool IsOnItemStateIcon( void ) const { return (flags&TVHT_ONITEMSTATEICON) != 0; }
        bool IsLeft( void ) const { return (flags&TVHT_TOLEFT) != 0; }
        bool IsRight( void ) const { return (flags&TVHT_TORIGHT) != 0; }
    };

  public: // Methods
    explicit TreeView( HWND hwnd = 0 )
    : Win::Dow(hwnd)
    {}

    TreeView( HWND dlg, int id )
    : Win::Dow(::GetDlgItem(dlg, id))
    {}

    Win::ImageList CreateDragImage( HTREEITEM hItem ) const
    { return Win::ImageList(TreeView_CreateDragImage(_hwnd, hItem));
    }

    void Clear( void )
    { TreeView_DeleteAllItems(_hwnd);
    }

    void DeleteItem( HTREEITEM hItem )
    { TreeView_DeleteItem(_hwnd, hItem);
    }

    HWND EditLabel( HTREEITEM hItem )
    { return TreeView_EditLabel(_hwnd, hItem);
    }

    void EndEditLabelNow( bool saveChanges = false )
    { TreeView_EndEditLabelNow(_hwnd, !saveChanges);
    }

    void EnsureVisible( HTREEITEM hItem )
    { TreeView_EnsureVisible(_hwnd, hItem);
    }

    void Expand( HTREEITEM hItem )
    { TreeView_Expand(_hwnd, hItem, TVE_EXPAND);
    }

    void Collapse( HTREEITEM hItem )
    { TreeView_Expand(_hwnd, hItem, TVE_COLLAPSE);
    }

    void CollapseReset( HTREEITEM hItem )
    { TreeView_Expand(_hwnd, hItem, TVE_COLLAPSE|TVE_COLLAPSERESET);
    }

    void Toggle( HTREEITEM hItem )
    { TreeView_Expand(_hwnd, hItem, TVE_TOGGLE);
    }

    COLORREF GetBkColor( void ) const
    { TreeView_GetBkColor(_hwnd);
    }
/*  Windows 2000+
    bool GetCheckState( HTREEITEM hItem )
    { return TreeView_GetCheckState(_hwnd, hItem) != 0;
    }
*/
    HTREEITEM GetChild( HTREEITEM hItem ) const
    { return TreeView_GetChild(_hwnd, hItem);
    }

    UINT GetItemCount( void ) const
    { return TreeView_GetCount(_hwnd);
    }

    HTREEITEM GetDropHilight( void ) const
    { return TreeView_GetDropHilight(_hwnd);
    }

    HWND GetEditControl( void ) const
    { return TreeView_GetEditControl(_hwnd);
    }

    HTREEITEM GetFirstVisible( void ) const
    { return TreeView_GetFirstVisible(_hwnd);
    }

    HIMAGELIST GetNormalImageList( void ) const
    { return TreeView_GetImageList(_hwnd, TVSIL_NORMAL);
    }

    HIMAGELIST GetStateImageList( void ) const
    { return TreeView_GetImageList(_hwnd, TVSIL_STATE);
    }

    UINT GetIndent( void ) const
    { return TreeView_GetIndent(_hwnd);
    }

    COLORREF GetInsertMarkColor( void ) const
    { return TreeView_GetInsertMarkColor(_hwnd);
    }

    void GetItem( TVITEM& item ) const
    { TreeView_GetItem(_hwnd, &item);
    }

    int GetItemHeight( void ) const
    { TreeView_GetItemHeight(_hwnd);
    }

    void GetItemRect( HTREEITEM hItem, RECT& rect, bool textOnly = true ) const
    { TreeView_GetItemRect(_hwnd, hItem, &rect, textOnly);
    }

    LPARAM GetItemParam( HTREEITEM hItem ) const
    { TVITEM item;
		  item.hItem = hItem;
		  item.mask = TVIF_PARAM;
		  GetItem(item);
      return item.lParam;
    }

    UINT GetItemState( HTREEITEM hItem ) const
    { TVITEM item;
		  item.hItem = hItem;
		  item.mask = TVIF_STATE|TVIF_HANDLE;
      item.state = 0xFFFFFFFF;
      item.stateMask = 0xFFFFFFFF;
		  GetItem(item);
      return item.state&item.stateMask;
    }

    bool IsBold( HTREEITEM hItem ) const
    { return (GetItemState(hItem)&TVIS_BOLD) != 0;
    }

    bool IsCut( HTREEITEM hItem ) const
    { return (GetItemState(hItem)&TVIS_CUT) != 0;
    }

    bool IsDropHilited( HTREEITEM hItem ) const
    { return (GetItemState(hItem)&TVIS_DROPHILITED) != 0;
    }

    bool IsExpanded( HTREEITEM hItem ) const
    { return (GetItemState(hItem)&TVIS_EXPANDED) != 0;
    }

    bool IsExpandedOnce( HTREEITEM hItem ) const
    { return (GetItemState(hItem)&TVIS_EXPANDEDONCE) != 0;
    }

    bool IsSelected( HTREEITEM hItem ) const
    { return (GetItemState(hItem)&TVIS_SELECTED) != 0;
    }

    HTREEITEM GetLastVisible( void ) const
    { return TreeView_GetLastVisible(_hwnd);
    }
/*  Windows 2000+
    COLORREF GetLineColor( void )
    { return TreeView_GetLineColor(_hwnd);
    }
*/
    HTREEITEM GetNextSibling( HTREEITEM hItem ) const
    { return TreeView_GetNextSibling(_hwnd, hItem);
    }

    HTREEITEM GetNextVisible( HTREEITEM hItem ) const
    { return TreeView_GetNextVisible(_hwnd, hItem);
    }

    HTREEITEM GetParent( HTREEITEM hItem ) const
    { return TreeView_GetParent(_hwnd, hItem);
    }

    HTREEITEM GetPrevSibling( HTREEITEM hItem ) const
    { return TreeView_GetPrevSibling(_hwnd, hItem);
    }

    HTREEITEM GetPrevVisible( HTREEITEM hItem ) const
    { return TreeView_GetPrevVisible(_hwnd, hItem);
    }

    HTREEITEM GetRoot( void ) const
    { return TreeView_GetRoot(_hwnd);
    }

    UINT GetScrollTime( void ) const
    { return TreeView_GetScrollTime(_hwnd);
    }

    HTREEITEM GetSelection( void ) const
    { return TreeView_GetSelection(_hwnd);
    }

    COLORREF GetTextColor( void ) const
    { return TreeView_GetTextColor(_hwnd);
    }

    HWND GetToolTipControl( void ) const
    { return TreeView_GetToolTips(_hwnd);
    }

    bool IsUnicode( void ) const
    { return TreeView_GetUnicodeFormat(_hwnd) != 0;
    }

    bool IsAnsi( void ) const
    { return TreeView_GetUnicodeFormat(_hwnd) == 0;
    }

    UINT GetVisibleCount( void ) const
    { return TreeView_GetVisibleCount(_hwnd);
    }

    HTREEITEM HitTest( TVHITTESTINFO& info ) const
    { return TreeView_HitTest(_hwnd, &info);
    }

    HTREEITEM InsertItem( HTREEITEM hParent, TVITEM& item, HTREEITEM hInsertAfter = TVI_SORT )
    { TVINSERTSTRUCT is;
      is.hParent = hParent;
      is.hInsertAfter = hInsertAfter;
      is.item = item;
      return TreeView_InsertItem(_hwnd, &is);
    }

    void SelectDropTarged( HTREEITEM hItem )
    { TreeView_SelectDropTarget(_hwnd, hItem);
    }

    void SelectItem( HTREEITEM hItem )
    { TreeView_SelectItem(_hwnd, hItem);
    }

    void SelectSetFirstVisible( HTREEITEM hItem )
    { TreeView_SelectSetFirstVisible(_hwnd, hItem);
    }

    COLORREF SetBkColor( COLORREF color )
    { return TreeView_SetBkColor(_hwnd, color);
    }

/*  Windows 2000+
    void Check( HTREEITEM hItem )
    { TreeView_SetCheckState(_hwnd, hItem, TRUE);
    }

    void UnCheck( HTREEITEM hItem )
    { TreeView_SetCheckState(_hwnd, hItem, FALSE);
    }

    void SetCheckState( HTREEITEM hItem, bool checked )
    { TreeView_SetCheckState(_hwnd, hItem, checked);
    }
*/
    HIMAGELIST SetNormalImageList( HIMAGELIST himl )
    { return TreeView_SetImageList(_hwnd, himl, TVSIL_NORMAL);
    }

    HIMAGELIST SetStateImageList( HIMAGELIST himl )
    { return TreeView_SetImageList(_hwnd, himl, TVSIL_STATE);
    }

    void SetIndent( int indent )
    { TreeView_SetIndent(_hwnd, indent);
    }

    void SetInsertMarkAfter( HTREEITEM hItem )
    { TreeView_SetInsertMark(_hwnd, hItem, TRUE);
    }

    void SetInsertMarkBefore( HTREEITEM hItem )
    { TreeView_SetInsertMark(_hwnd, hItem, FALSE);
    }

    void RemoveInsertMark( void )
    { TreeView_SetInsertMark(_hwnd, 0, 0);
    }

    COLORREF SetInsertMarkColor( COLORREF color )
    { return TreeView_SetInsertMarkColor(_hwnd, color);
    }

    void SetItem( TVITEM& item )
    { TreeView_SetItem(_hwnd, &item);
    }

    void SetItemParam( HTREEITEM hItem, LPARAM lParam )
    { TVITEM item;
		  item.hItem = hItem;
      item.lParam = lParam;
		  item.mask = TVIF_PARAM;
		  SetItem(item);
    }

    int SetItemHeight( int cy )
    { return TreeView_SetItemHeight(_hwnd, cy);
    }

    void SetItemState( HTREEITEM hItem, UINT state, UINT mask )
    { TVITEM item;
      item.hItem = hItem;
      item.mask = TVIF_STATE;
      item.state = state;
      item.stateMask = mask;
      SetItem(item);
    }

    void MakeBold( HTREEITEM hItem, bool make = true )
    { SetItemState(hItem, make ? TVIS_BOLD : 0, TVIS_BOLD);
    }

    void MakeCut( HTREEITEM hItem, bool make = true )
    { SetItemState(hItem, make ? TVIS_CUT : 0, TVIS_CUT);
    }

    void MakeDropHilited( HTREEITEM hItem, bool make = true )
    { SetItemState(hItem, make ? TVIS_DROPHILITED : 0, TVIS_DROPHILITED);
    }

/*  Windows 2000+
    COLORREF SetLineColor( COLORREF color )
    { return TreeView_SetLineColor(_hwnd, color);
    }
*/
    UINT SetScrollTime( UINT time )
    { return TreeView_SetScrollTime(_hwnd, time);
    }

    COLORREF SetTextColor( COLORREF color )
    { return TreeView_SetTextColor(_hwnd, color);
    }

    HWND SetToolTipControl( HWND hwndToolTip )
    { return TreeView_SetToolTips(_hwnd, hwndToolTip);
    }

    void SetUnicode( void )
    { TreeView_SetUnicodeFormat(_hwnd, TRUE);
    }

    void SetAnsi( void )
    { TreeView_SetUnicodeFormat(_hwnd, FALSE);
    }

    void SortChildren( HTREEITEM hParent )
    { TreeView_SortChildren(_hwnd, hParent, 0);
    }

    void SortChildren( HTREEITEM hParent, PFNTVCOMPARE compare, LPARAM lParam )
    { TVSORTCB sort = {hParent, compare, lParam};
      TreeView_SortChildrenCB(_hwnd, &sort, 0);
    }
};

class TreeViewNotifyHandler : public Win::CommonNotifyHandler
{
  public:
    // Overriden methods
    virtual bool OnNotify( NMHDR* hdr, LRESULT& result );

    // New methods
    virtual bool OnCustomDraw( HWND hwnd, NMTVCUSTOMDRAW& cd, LRESULT& result ) { return false; }
    virtual bool OnBeginDrag( HWND hwnd, TVITEM& item, POINT const& ptDrag ) { return false; }
    virtual bool OnBeginLabelEdit( HWND hwnd, TVITEM& item, LRESULT& result ) { return false; }
    virtual bool OnBeginRightDrag( HWND hwnd, TVITEM& item, POINT const& ptDrag ) { return false; }
    virtual bool OnDeleteItem( HWND hwnd, TVITEM& item ) { return false; }
    virtual bool OnEndLabelEdit( HWND hwnd, TVITEM& item, LRESULT& result ) { return false; }
    virtual bool OnGetDispInfo( HWND hwnd, TVITEM& item ) { return false; }
    virtual bool OnGetInfoTip( HWND hwnd, NMTVGETINFOTIP& data ) { return false; }
    virtual bool OnItemCollapsed( HWND hwnd, TVITEM& item, UINT action ) { return false; }
    virtual bool OnItemExpanded( HWND hwnd, TVITEM& item, UINT action ) { return false; }
    virtual bool OnItemCollapsing( HWND hwnd, TVITEM& item, UINT action, LRESULT& result ) { return false; }
    virtual bool OnItemExpanding( HWND hwnd, TVITEM& item, UINT action, LRESULT& result ) { return false; }
    virtual bool OnSelChanged( HWND hwnd, TVITEM& oldItem, TVITEM const& newItem, UINT action ) { return false; }
    virtual bool OnSelChanging( HWND hwnd, TVITEM& oldItem, TVITEM const& newItem, UINT action, LRESULT& result ) { return false; }
    virtual bool OnSetDispInfo( HWND hwnd, TVITEM& item ) { return false; }
/*
    virtual bool OnSingleExpand( HWND hwnd, NMTREEVIEW& data, LRESULT& result ) { return false; }
*/
};


class TreeViewMaker : public ControlMakerHandler<TreeViewNotifyHandler>
{
  public:
    TreeViewMaker( Win::Dow parent, int id, DWORD style = WS_VISIBLE|TVS_HASBUTTONS|TVS_HASLINES|TVS_LINESATROOT, DWORD exStyle = WS_EX_CLIENTEDGE, int x = 0, int y = 0, int w = 0, int h = 0 )
    : ControlMakerHandler<TreeViewNotifyHandler>(WC_TREEVIEW, parent, id, style, exStyle, x, y, w, h)
    {}

    void DisableDragDrop( void )
    { _style |= TVS_DISABLEDRAGDROP;
    }

    void EditLabels( void )
    { _style |= TVS_EDITLABELS;
    }

    void HasButtons( void )
    { _style |= TVS_HASBUTTONS;
    }

    void HasLines( void )
    { _style |= TVS_HASLINES;
    }

    void InfoTip( void )
    { _style |= TVS_INFOTIP;
    }

    void LinesAtRoot( void )
    { _style |= TVS_LINESATROOT;
    }

    void NoScroll( void )
    { _style |= TVS_NOSCROLL;
    }

    void NoToolTips( void )
    { _style |= TVS_NOTOOLTIPS;
    }

    void ShowSelAlways( void )
    { _style |= TVS_SHOWSELALWAYS;
    }

    void TrackSelect( void )
    { _style |= TVS_TRACKSELECT;
    }
};

} // namespace Win

#endif // WINTREEVIEW_H