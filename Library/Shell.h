#ifndef WINSHELL_H
#define WINSHELL_H

#include "Icon.h"
#include "XWin.h"
#include <shellapi.h>
#include <shlobj.h>

WINOLEAPI  CoInitializeEx(LPVOID pvReserved, DWORD dwCoInit);

namespace Win
{

class UseOle
{
  public:
    UseOle( void )
    { HRESULT hr = ::OleInitialize(0);
      if ( FAILED(hr) )
      { throw XCom(hr, TEXT("OLE initialization failed"));
      }
    }

    ~UseOle( void )
    { ::OleUninitialize();
    }
};

class UseCom
{
  public:
    UseCom( void )
    { HRESULT hr = ::CoInitializeEx(0, 0);
      if ( FAILED(hr) )
      { throw XCom(hr, TEXT("COM initialization failed"));
      }
    }

    ~UseCom( void )
    { ::CoUninitialize();
    }
};

template <class T>
class SIFacePtr
{
  public:
    ~SIFacePtr( void )
    { Free();
    }
    
    T* operator->( void ) { return _p; }
    T const* operator->( void ) const { return _p; }
    operator T const*( void ) const { return _p; }

  protected: // Methods
    SIFacePtr( void )
    : _p(0)
    {}
    
    void Free( void )
    { if ( _p != 0 )
      { _p->Release();
      }
      _p = 0;
    }
  
  protected: // Data members  
    T * _p;
    
  private: // Methods
    SIFacePtr ( SIFacePtr<T> const& p ) {}
    void operator=( SIFacePtr<T> const& p ) {}
};

template <class T>
class SShellPtr
{
  public:
    ~SShellPtr( void )
    { Free();
      _malloc->Release();
    }

    T* operator->( void ) { return _p; }
    T const* operator->( void ) const { return _p; }
    operator T const*( void ) const { return _p; }

  protected: // Methods
    SShellPtr( void )
    : _p(0)
	  {
      // Obtain malloc here, rather than
      // in the destructor.
      // Destructor must be fail-proof.
      // Revisit: Would static IMalloc * _shellMalloc work?
      HRESULT hr = ::SHGetMalloc(&_malloc);
      if ( FAILED(hr) )
      { throw XCom(hr, TEXT("::SHGetMalloc failed"));
      }
    }

    void Free( void )
    { if ( _p != 0 )
      { _malloc->Free(_p);
      }
      _p = 0;
    }

  protected: // Data members
    T*        _p;
    IMalloc*  _malloc;

  private:
    SShellPtr( SShellPtr<T> const& p ) {}
    void operator=( SShellPtr<T> const& p ) {}
};

class SpecialFolder : public SShellPtr<ITEMIDLIST>
{
  public:
    TCHAR const* GetDisplayName( void ) const
    { return _si.szDisplayName;
    }

    int GetIconIndex( void ) const
    { return _si.iIcon;
    }

    int GetOpenIconIndex( void ) const
    { SHFILEINFO si;
      ::SHGetFileInfo(reinterpret_cast<TCHAR const*>(_p), 0, &si, sizeof(si), SHGFI_PIDL|SHGFI_SYSICONINDEX|SHGFI_OPENICON);
      if ( si.hIcon != 0 )
      { ::DestroyIcon(si.hIcon);
      }
      return si.iIcon;
    }

    SmallIcon GetSmallIcon( void ) const
    { SHFILEINFO si;
      ::SHGetFileInfo(reinterpret_cast<TCHAR const*>(_p), 0, &si, sizeof(si), SHGFI_PIDL|SHGFI_ICON|SHGFI_SMALLICON);
      return SmallIcon(si.hIcon);
    }

    LargeIcon GetLargeIcon( void ) const
    { SHFILEINFO si;
      ::SHGetFileInfo(reinterpret_cast<TCHAR const*>(_p), 0, &si, sizeof(si), SHGFI_PIDL|SHGFI_ICON|SHGFI_LARGEICON);
      return LargeIcon(si.hIcon);
    }

    Icon GetShellIcon( void ) const
    { SHFILEINFO si;
      ::SHGetFileInfo(reinterpret_cast<TCHAR const*>(_p), 0, &si, sizeof(si), SHGFI_PIDL|SHGFI_ICON|SHGFI_SHELLICONSIZE);
      return Icon(si.hIcon);
    }

    SmallIcon GetSmallOpenIcon( void ) const
    { SHFILEINFO si;
      ::SHGetFileInfo(reinterpret_cast<TCHAR const*>(_p), 0, &si, sizeof(si), SHGFI_PIDL|SHGFI_ICON|SHGFI_SMALLICON|SHGFI_OPENICON);
      return SmallIcon(si.hIcon);
    }

    LargeIcon GetLargeOpenIcon( void ) const
    { SHFILEINFO si;
      ::SHGetFileInfo(reinterpret_cast<TCHAR const*>(_p), 0, &si, sizeof(si), SHGFI_PIDL|SHGFI_ICON|SHGFI_LARGEICON|SHGFI_OPENICON);
      return LargeIcon(si.hIcon);
    }

    Icon GetShellOpenIcon( void ) const
    { SHFILEINFO si;
      ::SHGetFileInfo(reinterpret_cast<TCHAR const*>(_p), 0, &si, sizeof(si), SHGFI_PIDL|SHGFI_ICON|SHGFI_SHELLICONSIZE|SHGFI_OPENICON);
      return Icon(si.hIcon);
    }

    bool IsOk( void ) const 
    { return SUCCEEDED(_hr); 
    }

  protected:
    SpecialFolder( int folder )
    : _hr(::SHGetSpecialFolderLocation(0, folder, &_p))
    { ::ZeroMemory(&_si, sizeof(_si));
      if ( IsOk() )
      { ::SHGetFileInfo(reinterpret_cast<TCHAR const*>(_p), 0, &_si, sizeof(_si), SHGFI_PIDL|SHGFI_DISPLAYNAME|SHGFI_SYSICONINDEX);
        if ( _si.hIcon != 0 )
        { ::DestroyIcon(_si.hIcon);
        }
      }
    }

  private:
    HRESULT    _hr;
    SHFILEINFO _si;
};

class DrivesFolder : public SpecialFolder
{
  public:
    DrivesFolder( void )
    : SpecialFolder(CSIDL_DRIVES)
    {}
};

//
// Class ShFileOperation
// ~~~~~~~~~~~~~~~~~~~~~
class ShFileOperation
{
  public:
    bool IsOk( void )
    { return _result == 0;
    }
    
    bool Aborted( void )
    { return _aborted; 
    }
    
  protected:
    ShFileOperation( void )
    : _result(1),
      _aborted(true)
    {}
    
    void Perform( SHFILEOPSTRUCT& fileOp )
    { _result = ::SHFileOperation(&fileOp); 
      _aborted = (fileOp.fAnyOperationsAborted == TRUE);
    }
    
  private:
    int  _result;
    bool _aborted;
};

//
// Class ShMoveFile
// ~~~~~~~~~~~~~~~~
class ShMoveFile : public ShFileOperation
{
  public:
    ShMoveFile( HWND hwnd, TCHAR const from[], TCHAR const to[],
                TCHAR const* title = TEXT("Moving..."), 
                FILEOP_FLAGS flags = FOF_ALLOWUNDO|FOF_NOCONFIRMMKDIR|FOF_RENAMEONCOLLISION )
    { SHFILEOPSTRUCT fileOp;
      fileOp.hwnd = hwnd; 
      fileOp.wFunc = FO_MOVE; 
      fileOp.pFrom = from; 
      fileOp.pTo = to; 
      fileOp.fFlags = flags; 
      fileOp.fAnyOperationsAborted = FALSE; 
      fileOp.hNameMappings = 0; 
      fileOp.lpszProgressTitle = title; 
      Perform(fileOp);
		}
};

//
// Class ShCopyFile
// ~~~~~~~~~~~~~~~~
class ShCopyFile : public ShFileOperation
{
  public:
    ShCopyFile( HWND hwnd, TCHAR const from[], TCHAR const to[],
                TCHAR const* title = TEXT("Copying..."), 
                FILEOP_FLAGS flags = FOF_ALLOWUNDO|FOF_NOCONFIRMMKDIR|FOF_RENAMEONCOLLISION )
    { SHFILEOPSTRUCT fileOp;
      fileOp.hwnd = hwnd; 
      fileOp.wFunc = FO_COPY; 
      fileOp.pFrom = from; 
      fileOp.pTo = to;
      fileOp.fFlags = flags; 
      fileOp.fAnyOperationsAborted = FALSE; 
      fileOp.hNameMappings = 0; 
      fileOp.lpszProgressTitle = title; 
      Perform(fileOp);
		}
};

//
// Class ShDeleteFile
// ~~~~~~~~~~~~~~~~~~
class ShDeleteFile : public ShFileOperation
{
  public:
    ShDeleteFile( HWND hwnd, TCHAR const file[],
                  TCHAR const* title = TEXT("Deleting..."), 
                  FILEOP_FLAGS flags = FOF_ALLOWUNDO|FOF_NOCONFIRMMKDIR|FOF_RENAMEONCOLLISION )
    { SHFILEOPSTRUCT fileOp;
      fileOp.hwnd = hwnd; 
      fileOp.wFunc = FO_DELETE; 
      fileOp.pFrom = file; 
      fileOp.pTo = 0; 
      fileOp.fFlags = flags; 
      fileOp.fAnyOperationsAborted = FALSE; 
      fileOp.hNameMappings = 0; 
      fileOp.lpszProgressTitle = title; 
      Perform(fileOp);
		}
};

//
// Class ShRenameFile
// ~~~~~~~~~~~~~~~~~~
class ShRenameFile : public ShFileOperation
{
  public:
    // oldname AND newname MUST BE DOUBLE ZERO TERMINATED
    ShRenameFile( HWND hwnd, TCHAR const oldName[], TCHAR const newName[],
                  TCHAR const* title = TEXT("Renamimg..."), 
                  FILEOP_FLAGS flags = FOF_ALLOWUNDO|FOF_NOCONFIRMMKDIR|FOF_RENAMEONCOLLISION )
    { SHFILEOPSTRUCT fileOp;
      fileOp.hwnd = hwnd; 
      fileOp.wFunc = FO_RENAME; 
      fileOp.pFrom = oldName; 
      fileOp.pTo = newName; 
      fileOp.fFlags = flags; 
      fileOp.fAnyOperationsAborted = FALSE; 
      fileOp.hNameMappings = 0; 
      fileOp.lpszProgressTitle = title;
      Perform(fileOp);
		}
};

} // namespace Win

#endif // WINSHELL_H