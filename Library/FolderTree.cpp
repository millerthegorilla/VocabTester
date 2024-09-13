#include "Templates/SmartPtr.h"
#include "FolderTree.h"
#include "FolderWatcher.h"
#include "FindFile.h"
#include "ImageList.h"
#include "Shell.h"
#include <tchar.h>
#include <vector>

using namespace Win;

unsigned const MSG_FOLDERCHANGED = NM_FIRST;

class Walker
{
  public:
    virtual ~Walker( void ) {}

    virtual bool HasChildren( void ) = 0;
    virtual bool AddChildren( TreeView& tree, HTREEITEM hParent ) = 0;
    virtual void Update( TreeView& tree, HTREEITEM hParent ) = 0;
    virtual TCHAR const* GetPath( void ) = 0;
    virtual bool ComparePath( TCHAR const path[] ) = 0;
    virtual bool CompareLastFolder( TCHAR const folder[] ) = 0;
    virtual void Watch( bool watch ) = 0;
};

class FolderWalker : public Walker
{
  public:
    FolderWalker( TCHAR const path[] );

    virtual bool HasChildren( void )
    { FindFileQuery query(_path, _T("*.*"));
      query.FoldersOnly();
      return !FolderContent(query).AtEnd();
    }

    virtual bool AddChildren( TreeView& tree, HTREEITEM hParent );
    virtual void Update( TreeView& tree, HTREEITEM hParent );
    
    virtual TCHAR const* GetPath( void )
    { return _path;
    }

    virtual bool ComparePath( TCHAR const path[] )
    { return ::_tcsicmp(_path, path) == 0;
    }

    virtual bool CompareLastFolder( TCHAR const folder[] )
    { return ::_tcsicmp(_lastFolder, folder) == 0;
    }

    virtual void Watch( bool watch ) {}
  
  protected: // Methods
    void UpdateChildren( TreeView& tree, HTREEITEM hParent );

  protected: // Data members
    TCHAR  _path[MAX_PATH];
    int    _pathLen;
    TCHAR* _lastFolder;
};

FolderWalker::FolderWalker( TCHAR const path[] )
: _pathLen(::_tcslen(path))
{ ::_tcscpy(_path, path);
  _lastFolder = ::_tcsrchr(_path, TEXT('\\'));
  _lastFolder = (_lastFolder == 0 ? _path : &_lastFolder[1]);
  if ( _lastFolder[0] == TEXT('\0') )
  { _lastFolder = _path;
  }
}

bool FolderWalker::AddChildren( TreeView& tree, HTREEITEM hParent )
{
  FindFileQuery query(_path, _T("*.*"));
  query.FoldersOnly();
  stdstring folder = _path;
  if ( folder[folder.length() - 1] != _T('\\') ) folder += _T('\\');
  for ( FolderContent folders(query); !folders.AtEnd(); folders.Advance() )
  { FindFileData match = folders.GetMatch();
    stdstring path = folder + match.GetFileName();
    ShellFileInfo info(path, match.GetAttr());
    TreeView::Item item(info.GetDisplayName(), info.GetIconIndex(), info.GetOpenIconIndex());
    FolderWalker* w = new FolderWalker(path.c_str());
    item.HasChildren(w->HasChildren());
    item.SetParam(reinterpret_cast<LPARAM>(w));
    tree.InsertItem(hParent, item);
  }
  return tree.GetChild(hParent) != 0;
}

class ToAddEntry
{
  public:
    ToAddEntry( void )
    : _imgIdx(0),
      _openImgIdx(0)
    {}

    ToAddEntry( TCHAR const path[], TCHAR const displayName[], int imgIdx, int openImgIdx )
    : _path(path),
      _displayName(displayName),
      _imgIdx(imgIdx),
      _openImgIdx(openImgIdx)
    {}

    TCHAR const* GetDisplayName( void ) const
    { return _displayName.c_str();
    }

    TCHAR const* GetPath( void ) const
    { return _path.c_str();
    }

    int GetImageIndex( void ) const
    { return _imgIdx;
    }

    int GetOpenImageIndex( void ) const
    { return _openImgIdx;
    }

  private:
    stdstring _path;
    stdstring _displayName;
    int       _imgIdx;
    int       _openImgIdx;
};

void FolderWalker::Update( TreeView& tree, HTREEITEM hParent )
{
  if ( !tree.IsExpanded(hParent) )
  { // Only update the children flag
    TreeView::Item item(hParent);
    FindFileQuery query(_path, _T("*.*"));
    query.FoldersOnly();
    item.HasChildren(!FolderContent(query).AtEnd());
    tree.SetItem(item);
    return;
  }
  UpdateChildren(tree, hParent);
}

void FolderWalker::UpdateChildren( TreeView& tree, HTREEITEM hParent )
{
  std::vector<HTREEITEM> found;
  std::vector<ToAddEntry> toAdd;
  FindFileQuery query(_path, _T("*.*"));
  query.FoldersOnly();
  stdstring folder(_path);
  if ( folder[folder.length() - 1] != _T('\\') ) folder += _T('\\');
  for ( FolderContent folders(query); !folders.AtEnd(); folders.Advance() )
  { FindFileData match = folders.GetMatch();
    HTREEITEM hFound = 0;
    for ( HTREEITEM hItem = tree.GetChild(hParent); hItem != 0 && hFound == 0; hItem = tree.GetNextSibling(hItem) )
    { Walker* w = reinterpret_cast<Walker*>(tree.GetItemParam(hItem));
      if ( w->CompareLastFolder(match.GetFileName()) )
      { hFound = hItem;
      }
    }
    
    if ( hFound != 0 )
    { found.push_back(hFound);
    }
    else
    { stdstring path = folder + match.GetFileName();
      ShellFileInfo info(path, match.GetAttr());
      toAdd.push_back(ToAddEntry(path.c_str(), info.GetDisplayName(), info.GetIconIndex(), info.GetOpenIconIndex()));
    }
  }

  if ( found.size() == 0 && toAdd.size() == 0 )
  { // There are no sub-folders in this folder
    bool reselect = false;
    for ( HTREEITEM hItem = tree.GetParent(tree.GetSelection()); hItem != 0 && !reselect; hItem = tree.GetParent(hItem) )
    { reselect = (hParent == hItem);
    }
    tree.CollapseReset(hParent);
    if ( reselect )
    { HTREEITEM hSel = tree.GetSelection();
      tree.SelectItem(0);   // Deselect any selected item
      tree.SelectItem(hSel); // Select item and send TVN_SELCHANGED
    }  
    return;
  }

  // Remove folders that were not found
  std::vector<HTREEITEM> toRemove;
  int i;
  HTREEITEM hItem;
  for ( hItem = tree.GetChild(hParent); hItem != 0; hItem = tree.GetNextSibling(hItem) )
  { for ( i = 0; i < found.size() && found[i] != hItem; ++i )
    { continue;
    }

    if ( i == found.size() )
    { toRemove.push_back(hItem);
    }
  }

  for ( i = 0; i < toRemove.size(); ++i )
  { tree.DeleteItem(toRemove[i]);
  }

  // Add new entries
  for ( i = 0; i < toAdd.size(); ++i )
  { TreeView::Item item(toAdd[i].GetDisplayName(), toAdd[i].GetImageIndex(), toAdd[i].GetOpenImageIndex());
		FolderWalker* w = new FolderWalker(toAdd[i].GetPath());
		item.HasChildren(w->HasChildren());
		item.SetParam(reinterpret_cast<LPARAM>(w));
		tree.InsertItem(hParent, item);
  }

  // Update children
  for ( hItem = tree.GetChild(hParent); hItem != 0; hItem = tree.GetNextSibling(hItem) )
  { Walker* w = reinterpret_cast<Walker*>(tree.GetItemParam(hItem));
    w->Update(tree, hItem);
  }
}

class DriveWalker : public FolderWalker
{
  public:
    DriveWalker( TCHAR const drive[], HWND parent, NMHDR const& hdr )
    : FolderWalker(drive),
      _watcher(parent, hdr)
    {}

    virtual void Update( TreeView& tree, HTREEITEM hParent );

    virtual void Watch( bool watch )
    { _watcher.WatchFolder(watch ? _path : 0, true, FILE_NOTIFY_CHANGE_DIR_NAME);
    }

  private:
    FolderWatcher _watcher;
};

void DriveWalker::Update( TreeView& tree, HTREEITEM hParent )
{ 
  if ( tree.IsExpanded(hParent) )
  { UpdateChildren(tree, hParent); 
  }
}

class DrivesWalker : public Walker
{
  public:
    DrivesWalker( HWND parent, NMHDR const& hdr )
    : _parent(parent)
    { _hdr = hdr;
    }

    virtual bool HasChildren( void )
    { return !EnumDrives().AtEnd();
    }

    virtual bool AddChildren( TreeView& tree, HTREEITEM hParent );

    virtual void Update( TreeView& tree, HTREEITEM hParent ) {}    
    virtual TCHAR const* GetPath( void ) { return TEXT(""); }
    virtual bool ComparePath( TCHAR const path[] ) { return false; }
    virtual bool CompareLastFolder( TCHAR const folder[] ) { return false; }
    virtual void Watch( bool watch ) {}

  private: // Data members
    HWND  _parent;
    NMHDR _hdr;
};

bool DrivesWalker::AddChildren( TreeView& tree, HTREEITEM hParent )
{
  for ( EnumDrives drives; !drives.AtEnd(); drives.Advance() )
  { ShellFileInfo info(drives.GetDrive());
    TreeView::Item item(info.GetDisplayName(), info.GetIconIndex(), info.GetOpenIconIndex());
    DriveWalker* w = new DriveWalker(drives.GetDrive(), _parent, _hdr);
    if ( drives.IsRemovable() || drives.IsCdRom() )
    { item.HasChildren(true);
    }
    else
    { item.HasChildren(w->HasChildren());
    }
    item.SetParam(reinterpret_cast<LPARAM>(w));
    tree.InsertItem(hParent, item, TVI_LAST);
  }
  return tree.GetChild(hParent) != 0;
}

void FolderTree::InitTree( void )
{
  if ( _hwnd != 0 && GetItemCount() == 0 )
  { // Set the image list to the system image list
    SetNormalImageList(_icons);

    // Insert root item - The computer
    DrivesFolder drives;
    TreeView::Item item(drives.GetDisplayName(), drives.GetIconIndex(), drives.GetOpenIconIndex());
    NMHDR hdr = { _hwnd, ::GetDlgCtrlID(_hwnd), MSG_FOLDERCHANGED };
    DrivesWalker* w = new DrivesWalker(Win::Dow(_hwnd).GetParent(), hdr);
    item.HasChildren(w->HasChildren());
    item.SetParam(reinterpret_cast<LPARAM>(w));
    InsertItem(TVI_ROOT, item);
  }
}

COLORREF FolderTree::SetBkColor( COLORREF color )
{ _icons.SetBkColor(color);
  return TreeView::SetBkColor(color);
}

stdstring FolderTree::GetFolder( HTREEITEM hItem ) const
{
  if ( hItem != 0 && hItem != GetRoot() )
  { Walker* w = reinterpret_cast<Walker*>(GetItemParam(hItem));
    return stdstring(w->GetPath());
  }
  return stdstring();
}

stdstring FolderTree::GetSelectedFolder( void ) const
{
  HTREEITEM sel = GetSelection();
  if ( sel != 0 && sel != GetRoot() )
  { Walker* w = reinterpret_cast<Walker*>(GetItemParam(sel));
    return stdstring(w->GetPath());
  }
  return stdstring();
}

bool FolderTree::SelectSubFolder( TCHAR const subFolder[] )
{
  HTREEITEM hSel = GetSelection();
  if ( hSel == 0 )
  { return false;
  }
  if ( ::_tcsicmp(subFolder, TEXT("..")) == 0 )
  { HTREEITEM hParent = GetParent(hSel);
    if ( hSel != GetRoot() && hParent != GetRoot() )
    { SelectItem(hParent);
      return true;
    }
  }
  else
  { if ( !IsExpanded(hSel) )
    { Expand(hSel);
    }
    for ( HTREEITEM hItem = GetChild(hSel); hItem != 0; hItem = GetNextSibling(hItem) )
    { Walker* w = reinterpret_cast<Walker*>(GetItemParam(hItem));
      if ( w->CompareLastFolder(subFolder) )
      { SelectItem(hItem);
        return true;
      }
    }
  }
  return false;
}

void FolderTree::ExpandFolder( TCHAR const path[] )
{
  TCHAR folder[MAX_PATH];
  ::_tcscpy(folder, path);
  TCHAR* slash = ::_tcschr(folder, TEXT('\\'));
  if ( slash == 0 )
  { return;
  }

  TCHAR c = slash[1];
  bool lastFolder = (slash[1] == TEXT('\0'));
  if ( !lastFolder )
  { slash[1] = TEXT('\0');
  }

  if ( !IsExpanded(GetRoot()) )
  { Expand(GetRoot());
  }

  for ( HTREEITEM hItem = GetChild(GetRoot()); hItem != 0; hItem = GetNextSibling(hItem) )
  { Walker* w = reinterpret_cast<Walker*>(GetItemParam(hItem));
    if ( w->CompareLastFolder(folder) )
    { slash[1] = c;
      if ( lastFolder || !ExpandFolder(hItem, &slash[1]) )
      { EnsureVisible(hItem);
        SelectItem(hItem);
      }
      return;
    }
  }
}

bool FolderTree::ExpandFolder( HTREEITEM hParent, TCHAR folder[] )
{
  TCHAR* slash = ::_tcschr(folder, TEXT('\\'));
  bool lastFolder = (slash == 0);
  if ( !lastFolder )
  { slash[0] = TEXT('\0');
  }

  if ( !IsExpanded(hParent) )
  { Expand(hParent);
  }

  for ( HTREEITEM hItem = GetChild(hParent); hItem != 0; hItem = GetNextSibling(hItem) )
  { Walker* w = reinterpret_cast<Walker*>(GetItemParam(hItem));
    if ( w->CompareLastFolder(folder) )
    { if ( lastFolder || !ExpandFolder(hItem, &slash[1]) )
      { EnsureVisible(hItem);
        SelectItem(hItem);
      }
      return true;
    }
  }
  return false;
}

bool FolderTreeNotifyHandler::OnNotify( NMHDR* hdr, LRESULT& result )
{
  switch ( hdr->code )
  {
    case MSG_FOLDERCHANGED:
      if ( OnFolderChanged(hdr->hwndFrom) ) return true;
      break;
  }
  return TreeViewNotifyHandler::OnNotify(hdr, result);
}

bool FolderTreeNotifyHandler::OnDeleteItem( HWND hwnd, TVITEM& item )
{ Walker* w = reinterpret_cast<Walker*>(item.lParam);
  delete w;
  return true;
}

bool FolderTreeNotifyHandler::OnItemCollapsing( HWND hwnd, TVITEM& item, UINT action, LRESULT& result )
{
  bool reselect = false;
  for ( HTREEITEM hItem = _tree.GetParent(_tree.GetSelection()); hItem != 0 && !reselect; hItem = _tree.GetParent(hItem) )
  { reselect = (item.hItem == hItem);
  }

  _tree.CollapseReset(item.hItem);
  Walker* w = reinterpret_cast<Walker*>(item.lParam);
  w->Watch(false);

  // In the case that the selected item has been destroyed (child)
  // the TreeView doesn't send TVN_SELCHANGED
  // altough it gives selection to parent
  if ( reselect )
  { HTREEITEM hSel = _tree.GetSelection();
    _tree.SelectItem(0);   // Deselect any selected item
    _tree.SelectItem(hSel); // Select item and send TVN_SELCHANGED
  }
  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  result = TRUE; // Prevent default processing
  return true;
}

bool FolderTreeNotifyHandler::OnItemExpanding( HWND hwnd, TVITEM& item, UINT action, LRESULT& result )
{
  Walker* w = reinterpret_cast<Walker*>(item.lParam);
  if ( w->AddChildren(Win::TreeView(hwnd), item.hItem) )
  { w->Watch(true);
  }
  return true;
}

bool FolderTreeNotifyHandler::OnDeviceChange( HWND hwnd, UINT event, DEV_BROADCAST_HDR* data, LRESULT& result )
{
  if ( event == DBT_DEVICEARRIVAL || event == DBT_DEVICEREMOVECOMPLETE )
  { if ( data->dbch_devicetype == DBT_DEVTYP_VOLUME )
    { DEV_BROADCAST_VOLUME* vol = reinterpret_cast<DEV_BROADCAST_VOLUME*>(data);
      for ( HTREEITEM hDrive = _tree.GetChild(_tree.GetRoot()); hDrive != 0; hDrive = _tree.GetNextSibling(hDrive) )
      { Walker* w = reinterpret_cast<Walker*>(_tree.GetItemParam(hDrive));
        unsigned int unitmask = 0x00000001 << (::_totlower(w->GetPath()[0]) - TEXT('a'));
        if ( vol->dbcv_unitmask&unitmask )
        { if ( event == DBT_DEVICEREMOVECOMPLETE && _tree.IsExpanded(hDrive) )
          { bool reselect = false;
            for ( HTREEITEM hItem = _tree.GetParent(_tree.GetSelection()); hItem != 0 && !reselect; hItem = _tree.GetParent(hItem) )
            { reselect = (hDrive == hItem);
            }
            _tree.CollapseReset(hDrive);
            if ( reselect )
            { HTREEITEM hSel = _tree.GetSelection();
              _tree.SelectItem(0);   // Deselect any selected item
              _tree.SelectItem(hSel); // Select item and send TVN_SELCHANGED
            }  
          }
          else if ( _tree.IsSelected(hDrive) )
          { // Reselect to emit TVN_SELCHANGED notification
            _tree.SelectItem(0);
            _tree.SelectItem(hDrive);
          }
        }
      }
    }
  }
  return true;
}

bool FolderTreeNotifyHandler::OnFolderChanged( HWND hwnd )
{
  for ( HTREEITEM hItem = _tree.GetChild(_tree.GetRoot()); hItem != 0; hItem = _tree.GetNextSibling(hItem) )
  { Walker* w = reinterpret_cast<Walker*>(_tree.GetItemParam(hItem));
    w->Update(_tree, hItem);
  }
  return true;
}
