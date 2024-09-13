#ifndef WINSHELLDFS_H
#define WINSHELLDFS_H

#include "stdstring"
#include "Templates/SmartPtr.h"
#include "Shell.h"

//
// DFS = Distributed file system
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

namespace Win
{

class Desktop : public SIFacePtr<IShellFolder>
{
  public:
    Desktop( void )
    { HRESULT hr = ::SHGetDesktopFolder(&_p);
      if ( FAILED(hr) )
      { throw XCom(hr, TEXT("::SHGetDesktopFolder failed"));
      }
    }
};

class ShPath : public SShellPtr<ITEMIDLIST>
{
  public:
    ShPath( SIFacePtr<IShellFolder>& folder, wchar_t* path )
    { ULONG lenParsed = 0;
      _hresult = folder->ParseDisplayName(0, 0, path, &lenParsed, &_p, 0);
    }

    bool IsOk( void ) const { return SUCCEEDED( _hresult ); }

  private:
    HRESULT _hresult;
};

class FolderBrowser : public SShellPtr<ITEMIDLIST>
{
  public:
    FolderBrowser( HWND hwndOwner, SShellPtr<ITEMIDLIST>& root,
                   UINT browseForWhat, TCHAR const title[] )
    {
      _displayName[0] = '\0';
      _fullPath[0] = '\0';
      _browseInfo.hwndOwner = hwndOwner;
      _browseInfo.pidlRoot = root;
      _browseInfo.pszDisplayName = _displayName;
      _browseInfo.lpszTitle = title;
      _browseInfo.ulFlags = browseForWhat;
      _browseInfo.lpfn = 0;
      _browseInfo.lParam = 0;
      _browseInfo.iImage = 0;

      // Let the user do the browsing
      _p = ::SHBrowseForFolder(&_browseInfo);
      if (_p != 0 )
      { ::SHGetPathFromIDList(_p, _fullPath);
      }
    }  

    TCHAR const* GetDisplayName( void ) const { return _displayName; }
    TCHAR const* GetPath( void ) const { return _fullPath; }
    bool IsOk( void ) const { return _p != 0; };

  private:
    TCHAR       _displayName [MAX_PATH];
    TCHAR       _fullPath [MAX_PATH];
    BROWSEINFO  _browseInfo;
};
/*
class ShFolder : public SIFacePtr<IShellFolder>
{
  public:
    ShFolder( void )
    {}

    ShFolder( ShFolder& parent, ItemIdList const& idList )
    { HRESULT hr = parent->BindToObject(idList, 0, IID_IShellFolder, reinterpret_cast<void**>(&_p));
      if ( FAILED(hr) )
      { throw XCom(hr, TEXT("IShellFolder::BindToObject failed"));
      }
    }

    ShFolder( TCHAR const path[] )
    { InitFromPath(path);
    }

    ShFolder( stdstring const& path )
    { InitFromPath(path.c_str());
    }

    ShFolder( ShFolder const& b )
    { _p = b._p;
      if ( _p != 0 )
      { _p->AddRef();
      }
    }

    ShFolder& operator=( ShFolder const& b )
    { if ( this != &b )
      { Free();
        _p = b._p;
        if ( _p != 0 )
        { _p->AddRef();
        }
      }
      return *this;
    }

  private: // Class
    class ParseDispName : public SShellPtr<ITEMIDLIST>
    {
      public:
        ParseDispName( SIFacePtr<IShellFolder>& parent, wTCHAR_t path[] )
        { HRESULT hr = parent->ParseDisplayName(0, 0, path, 0, &_p, 0);
          if ( FAILED(hr) )
          { throw XCom(hr, TEXT("IShellFolder::ParseDisplayName failed"));
          }
        }
    };

  private: // Class
    class Desktop : public SIFacePtr<IShellFolder>
    {
      public:
        Desktop( void )
        { HRESULT hr = ::SHGetDesktopFolder(&_p);
          if ( FAILED(hr) )
          { throw XCom(hr, TEXT("::SHGetDesktopFolder failed"));
          }
        }
    };

  private: // Methods
    void InitFromPath( TCHAR const path[] )
    { Desktop desktop;
      unsigned pathLen = ::MultiByteToWideTCHAR(CP_ACP, 0, path, -1, 0, 0);
      SArr<wTCHAR_t> wPath(new wTCHAR_t[pathLen]);
      ::MultiByteToWideTCHAR(CP_ACP, 0, path, -1, wPath, pathLen);
      ParseDispName idList(desktop, wPath);
      HRESULT hr = desktop->BindToObject(idList, 0, IID_IShellFolder, reinterpret_cast<void**>(&_p));
      if ( FAILED(hr) )
      { throw XCom(hr, TEXT("IShellFolder::BindToObject failed"));
      }
    }
};

class ShDesktop : public ShFolder
{
  public:
    ShDesktop( void )
    { HRESULT hr = ::SHGetDesktopFolder(&_p);
      if ( FAILED(hr) )
      { throw XCom(hr, TEXT("::SHGetDesktopFolder failed"));
      }
    }
};

class ShItemIdList : public SShellPtr<ITEMIDLIST>
{
  public:
    ItemIdList( void )
    : _attr(0)
    {}

    ItemIdList( ItemIdList const& b )
    { Duplicate(b);
    }

    ItemIdList& operator=( ItemIdList const& b )
    { if ( this != &b )
      { Duplicate(b);
      }
      return *this;
    }

    void Duplicate( ItemIdList const& b )
    { Free();
      if ( b._p != 0 )
      { unsigned cb = GetSize(b._p);
        _p = reinterpret_cast<ITEMIDLIST*>(_malloc->Alloc(cb));
        if ( _p == 0 )
        { throw Win::Exception(TEXT("Out of memory"));
        }
        ::CopyMemory(_p, b._p, cb);
        _parent = b._parent;
        _attr = b._attr;
      }
    }

    stdstring GetDisplayName( void ) const
    { StrRet strRet;
      _parent->GetDisplayNameOf(_p, SHGDN_INFOLDER, &strRet);
      return InterpretStrRet(strRet);
    }

    stdstring GetName( void ) const
    { StrRet strRet;
      _parent->GetDisplayNameOf(_p, SHGDN_INFOLDER|SHGDN_FORPARSING, &strRet);
      return InterpretStrRet(strRet);
    }

    stdstring GetPath( void ) const
    { StrRet strRet;
      _parent->GetDisplayNameOf(_p, SHGDN_NORMAL|SHGDN_FORPARSING, &strRet);
      return InterpretStrRet(strRet);
    }

    ULONG GetAttr( void ) const { return _attr; }
    bool IsFolder( void ) const { return (_attr&SFGAO_FOLDER) != 0; }
    bool IsFileSystem( void ) const { return (_attr&SFGAO_FILESYSTEM) != 0; }
    bool IsNewContent( void ) const { return (_attr&SFGAO_NEWCONTENT) != 0; }
    bool IsRemovable( void ) const { return (_attr&SFGAO_REMOVABLE) != 0; }

  private: // Class
    class StrRet : public STRRET
    {
      public:
        StrRet( void )
        { HRESULT hr = ::SHGetMalloc(&_malloc);
          if ( FAILED(hr) )
          { throw XCom(hr, TEXT("::SHGetMalloc failed"));
          }
          uType = STRRET_CSTR;
          pOleStr = 0;
          cStr[0] = '\0';
        }

        ~StrRet( void )
        { if ( uType == STRRET_WSTR && pOleStr != 0 )
          { _malloc->Free(pOleStr);
          }
          _malloc->Release();
        }

      private:
        IMalloc*  _malloc;
    };

  private: // Methods
    stdstring InterpretStrRet( StrRet& ret ) const
    { if ( ret.uType == STRRET_CSTR )
      { return stdstring(ret.cStr);
      }
      else if ( ret.uType == STRRET_WSTR )
      { unsigned strSize = ::WideTCHARToMultiByte(CP_ACP, 0, ret.pOleStr, -1, 0, 0, 0, 0);//cStr, MAX_PATH, 0, 0);
        SArr<TCHAR> str(new TCHAR[strSize]);
        ::WideTCHARToMultiByte(CP_ACP, 0, ret.pOleStr, -1, str, strSize, 0, 0);
        return stdstring(str);
      }
      else if ( ret.uType == STRRET_OFFSET )
      { return stdstring(reinterpret_cast<TCHAR*>(_p) + ret.uOffset);
      }
      else
      { return stdstring("(this is an error string)");
      }
    }

    static ITEMIDLIST* GetNextItemId( ITEMIDLIST const* idList )
    { // Check for valid pidl.
      if ( idList == 0 ) return 0;
      
      // Get the size of the specified item identifier. 
      int cb = idList->mkid.cb; 
      
      // If the size is zero, it is the end of the list. 
      if ( cb == 0 ) return 0; 
      
      // Add cb to pidl (casting to increment by bytes). 
      idList = (ITEMIDLIST const*)(((LPBYTE)idList) + cb); 
      
      // Return NULL if it is null-terminating, or a pidl otherwise. 
      return (idList->mkid.cb == 0) ? 0 : (ITEMIDLIST*)idList; 
    }

    static unsigned GetSize( ITEMIDLIST const* idList )
    { unsigned cbTotal = 0;
      if ( idList != 0 )
      { cbTotal += sizeof(idList->mkid.cb); // Null terminator
        while ( idList != 0 )
        { cbTotal += idList->mkid.cb;
          idList = GetNextItemId(idList);
        }
      }
      return cbTotal;
    }

  protected: // Methods
    ItemIdList( ShFolder const& parent )
    : _parent(parent),
      _attr(0)
    {}

  protected: // Class
    ShFolder _parent;
    ULONG    _attr;
};

class ShFolderContent : public ShItemIdList
{
  public:
    ShFolderContent( ShFolder const& parent, HWND owner = 0, DWORD flags = SHCONTF_FOLDERS|SHCONTF_NONFOLDERS )
    : ShItemIdList(parent),
      _enum(_parent, owner, flags),
      _hr(S_FALSE)
    { if ( _enum.IsOk() )
      { Advance();
      }
    }

    bool AtEnd( void ) const
    { return (_enum.IsOk() ? _hr == S_FALSE || FAILED(_hr) : true);
    }

    void Advance( void )
    { Free();
      _hr = _enum->Next(1, &_p, 0);
      if ( !AtEnd() )
      { _attr = 0xFFFFFFFF;
        _parent->GetAttributesOf(1, const_cast<const ITEMIDLIST**>(&_p), &_attr);
      }
    }
    
  private: // Class
    class EnumIDList : public SIFacePtr<IEnumIDList>
    {
      public:
        EnumIDList( ShFolder& parent, HWND owner, DWORD flags )
        : _hr(S_FALSE)
        { _hr = parent->EnumObjects(owner, flags, &_p);
        }

        bool IsOk( void ) const
        { return !FAILED(_hr);
        }

      private:
        HRESULT _hr;
    };

  private: // Data members
    EnumIDList  _enum;
    HRESULT     _hr;
};

class ShEnumFolders : public ShFolderContent
{
  public:
    ShEnumFolders( ShFolder& parent, HWND owner = 0 )
    : ShFolderContent(parent, owner, SHCONTF_FOLDERS)
    {}
};

class ShEnumFiles : public ShFolderContent
{
  public:
    ShEnumFiles( ShFolder& parent, HWND owner = 0 )
    : ShFolderContent(parent, owner, SHCONTF_NONFOLDERS)
    {}
};

*/
} // namespace Win

#endif // WINSHELLDFS_H