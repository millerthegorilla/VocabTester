#ifndef WINFOLDERTREE_H
#define WINFOLDERTREE_H

#include "TreeView.h"
#include <dbt.h>
#include "stdstring"
#include "ImageList.h"

namespace Win
{

class FolderTree : public TreeView
{
  public: // Methods
    explicit FolderTree( HWND hwnd = 0 )
    : TreeView(hwnd)
    { InitTree();
    }

    FolderTree( HWND dlg, int id )
    : TreeView(dlg, id)
    { InitTree();
    }

    void Init( HWND hwnd )
    { TreeView::Init(hwnd);
      InitTree();
    }

    COLORREF SetBkColor( COLORREF color );

    stdstring GetFolder( HTREEITEM hItem ) const;
    stdstring GetSelectedFolder( void ) const;
    bool SelectSubFolder( TCHAR const subFolder[] );
    void ExpandFolder( TCHAR const path[] );

  protected: // Methods
    virtual void InitTree( void );

  private: // Methods
    bool ExpandFolder( HTREEITEM hParent, TCHAR folder[] );

  private: // Data members
    Win::SystemSmallImageList _icons;
};

class FolderTreeNotifyHandler : public TreeViewNotifyHandler
{
  public:
    FolderTreeNotifyHandler( FolderTree& tree )
    : _tree(tree)
    {}

    // Overriden methods - must be called from overriden methods !!
    virtual bool OnNotify( NMHDR* hdr, LRESULT& result );
    virtual bool OnDeleteItem( HWND hwnd, TVITEM& item );
    virtual bool OnItemCollapsing( HWND hwnd, TVITEM& item, UINT action, LRESULT& result );
    virtual bool OnItemExpanding( HWND hwnd, TVITEM& item, UINT action, LRESULT& result );

    // New methods
    // The owner of FolderTree is responsible for calling this method !!
    // If he fails call it he'll miss some changes in the file system.
    // For example when the user inserts/removes the disk into/from CD-ROM.
    // Note: The hwnd is a handle of the particular FolderTree window.
    virtual bool OnDeviceChange( HWND hwnd, UINT event, DEV_BROADCAST_HDR* data, LRESULT& result );

  private: // Methods
    bool OnFolderChanged( HWND hwnd );

  protected: // Data members
    FolderTree& _tree;
};

} // namespace Win

#endif // WINFOLDERTREE_H