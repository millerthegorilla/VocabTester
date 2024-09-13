#ifndef WINFINDFILE_H
#define WINFINDFILE_H

#include "Icon.h"
#include <windows.h>
#include <tchar.h>
#include <cassert>

namespace Win
{

class FindFileData : public WIN32_FIND_DATA
{
  public:
    FindFileData( void )
    { ::ZeroMemory(this, sizeof(WIN32_FIND_DATA));
    }

    bool IsDot( void ) const { return cFileName[0] == _T('.'); }
    bool IsArchive( void ) const { return (dwFileAttributes&FILE_ATTRIBUTE_ARCHIVE) != 0; }
    bool IsCompressed( void ) const { return (dwFileAttributes&FILE_ATTRIBUTE_COMPRESSED) != 0; }
    bool IsFolder( void ) const { return (dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY) != 0; }
    bool IsHidden( void ) const { return (dwFileAttributes&FILE_ATTRIBUTE_HIDDEN) != 0; }
    bool IsNormal( void ) const { return (dwFileAttributes&FILE_ATTRIBUTE_NORMAL) != 0; }
    bool IsOffline( void ) const { return (dwFileAttributes&FILE_ATTRIBUTE_OFFLINE) != 0; }
    bool IsReadOnly( void ) const { return (dwFileAttributes&FILE_ATTRIBUTE_READONLY) != 0; }
    bool IsSystem( void ) const { return (dwFileAttributes&FILE_ATTRIBUTE_SYSTEM) != 0; }
    bool IsTemporary( void ) const { return (dwFileAttributes&FILE_ATTRIBUTE_TEMPORARY) != 0; }

    double GetSize( void ) const
    { double size = nFileSizeHigh;
      size *= double(MAXDWORD) + 1;
      size += nFileSizeLow;
      return size;
    }

    TCHAR const* GetFileName( void ) const { return cFileName; }
    TCHAR const* GetAlternateFileName( void ) const { return cAlternateFileName; }
    DWORD GetAttr( void ) const { return dwFileAttributes; }
};

class SimpleFindFile
{
  public:
    ~SimpleFindFile( void )
    { if ( _h != INVALID_HANDLE_VALUE )
      { ::FindClose( _h ); 
      }
    }

    bool AtEnd( void )
    { return _h == INVALID_HANDLE_VALUE || _done;  
    }

    void Advance( WIN32_FIND_DATA& data )
    { assert(_h != INVALID_HANDLE_VALUE);
      _done = (::FindNextFile(_h, &data) == 0); 
    }

  protected: // Methods
    SimpleFindFile( HANDLE h = INVALID_HANDLE_VALUE )
    : _h(h),
      _done(false)
    {}

  protected: // Data mebers
    HANDLE        _h;
    bool          _done;
};

class FindFile : public SimpleFindFile
{
  public:
    FindFile( TCHAR const path[], WIN32_FIND_DATA& data )
    : SimpleFindFile(::FindFirstFile(path, &data))
    {}
};

class FolderContent : public SimpleFindFile, public FindFileData
{
  public:
    FolderContent( TCHAR const folder[], TCHAR const wildcards[] = TEXT("*.*") )
    { ::_tcscpy(_path, folder);
      if ( _path[::_tcslen(_path) - 1] != TEXT('\\') )
      { ::_tcscat(_path, TEXT("\\"));
      }
      ::_tcscat(_path, wildcards);
      _fileName = &::_tcsrchr(_path, TEXT('\\'))[1];
      _h = ::FindFirstFile(_path, this);
      MakePath();
    }

    void Advance( void )
    { SimpleFindFile::Advance(*this);
      MakePath();
    }

    TCHAR const* GetPath( void ) const { return _path; }

  private:
    void MakePath( void )
    { if ( !AtEnd() )
      { ::_tcscpy(_fileName, GetFileName());
      }
    }

  protected:
    TCHAR     _path[MAX_PATH];
    TCHAR*    _fileName;
};

class EnumFolders : public FolderContent
{
  public:
    EnumFolders( TCHAR const folder[], TCHAR const wildcards[] = TEXT("*.*"), bool skipDots = true )
    : FolderContent(folder, wildcards)
    { while ( !AtEnd() && (!IsFolder() || (skipDots ? (IsFolder() && _fileName[0] == TEXT('.')) : false)) )
      { FolderContent::Advance();
      }
    }

    void Advance( void )
    { do
      { FolderContent::Advance();
      } while ( !AtEnd() && !IsFolder() );
    }
};

class EnumFiles : public FolderContent
{
  public:
    EnumFiles( TCHAR const folder[], TCHAR const wildcards[] = TEXT("*.*") )
    : FolderContent(folder, wildcards)
    { while ( !AtEnd() && IsFolder() )
      { FolderContent::Advance();
      }
    }

    void Advance( void )
    { do
      { FolderContent::Advance();
      } while ( !AtEnd() && IsFolder() );
    }
};

/*

  COM must be initialized before creating this object

*/
class ShellFolderContent : public FolderContent
{
  public:
    ShellFolderContent( TCHAR const folder[], TCHAR const wildcards[] = TEXT("*.*") )
    : FolderContent(folder, wildcards)
    { GetInfo();
    }

    void Advance( void )
    { FolderContent::Advance();
      GetInfo();
    }

    TCHAR const* GetDisplayName( void ) const
    { return _si.szDisplayName;
    }

    int GetIconIndex( void ) const
    { return _si.iIcon;
    }

    int GetOpenIconIndex( void ) const
    { SHFILEINFO si;
      ::ZeroMemory(&si, sizeof(si));
      ::SHGetFileInfo(_path, GetAttr(), &si, sizeof(si), SHGFI_SYSICONINDEX|SHGFI_OPENICON|SHGFI_USEFILEATTRIBUTES);
      if ( si.hIcon != 0 )
      { ::DestroyIcon(si.hIcon);
      }
      return si.iIcon;
    }

    SmallIcon GetSmallIcon( void ) const
    { SHFILEINFO si;
      ::ZeroMemory(&si, sizeof(si));
      ::SHGetFileInfo(_path, GetAttr(), &si, sizeof(si), SHGFI_ICON|SHGFI_SMALLICON|SHGFI_USEFILEATTRIBUTES);
      return SmallIcon(si.hIcon);
    }

    LargeIcon GetLargeIcon( void ) const
    { SHFILEINFO si;
      ::ZeroMemory(&si, sizeof(si));
      ::SHGetFileInfo(_path, GetAttr(), &si, sizeof(si), SHGFI_ICON|SHGFI_LARGEICON|SHGFI_USEFILEATTRIBUTES);
      return LargeIcon(si.hIcon);
    }

    Icon GetShellIcon( void ) const
    { SHFILEINFO si;
      ::ZeroMemory(&si, sizeof(si));
      ::SHGetFileInfo(_path, GetAttr(), &si, sizeof(si), SHGFI_ICON|SHGFI_SHELLICONSIZE|SHGFI_USEFILEATTRIBUTES);
      return Icon(si.hIcon);
    }

    SmallIcon GetSmallOpenIcon( void ) const
    { SHFILEINFO si;
      ::ZeroMemory(&si, sizeof(si));
      ::SHGetFileInfo(_path, GetAttr(), &si, sizeof(si), SHGFI_ICON|SHGFI_SMALLICON|SHGFI_OPENICON|SHGFI_USEFILEATTRIBUTES);
      return SmallIcon(si.hIcon);
    }

    LargeIcon GetLargeOpenIcon( void ) const
    { SHFILEINFO si;
      ::ZeroMemory(&si, sizeof(si));
      ::SHGetFileInfo(_path, GetAttr(), &si, sizeof(si), SHGFI_ICON|SHGFI_LARGEICON|SHGFI_OPENICON|SHGFI_USEFILEATTRIBUTES);
      return LargeIcon(si.hIcon);
    }

    Icon GetShellOpenIcon( void ) const
    { SHFILEINFO si;
      ::ZeroMemory(&si, sizeof(si));
      ::SHGetFileInfo(_path, GetAttr(), &si, sizeof(si), SHGFI_ICON|SHGFI_SHELLICONSIZE|SHGFI_OPENICON|SHGFI_USEFILEATTRIBUTES);
      return Icon(si.hIcon);
    }

  private:
    void GetInfo( void )
    { ::ZeroMemory(&_si, sizeof(_si));
      if ( !AtEnd() )
      { ::SHGetFileInfo(_path, GetAttr(), &_si, sizeof(_si), SHGFI_DISPLAYNAME|SHGFI_SYSICONINDEX|SHGFI_USEFILEATTRIBUTES);
        if ( _si.hIcon != 0 )
        { ::DestroyIcon(_si.hIcon);
        }
      }
    }

  protected:
    SHFILEINFO _si;
};

/*

  COM must be initialized before creating this object

*/
class ShellEnumFolders : public ShellFolderContent
{
  public:
    ShellEnumFolders( TCHAR const folder[], TCHAR const wildcards[] = TEXT("*.*"), bool skipDots = true )
    : ShellFolderContent(folder, wildcards)
    { while ( !AtEnd() && (!IsFolder() || (skipDots ? (IsFolder() && _fileName[0] == TEXT('.')) : false)) )
      { ShellFolderContent::Advance();
      }
    }

    void Advance( void )
    { do
      { ShellFolderContent::Advance();
      } while ( !AtEnd() && !IsFolder() );
    }
};

/*

  COM must be initialized before creating this object

*/
class ShellEnumFiles : public ShellFolderContent
{
  public:
    ShellEnumFiles( TCHAR const folder[], TCHAR const wildcards[] = TEXT("*.*") )
    : ShellFolderContent(folder, wildcards)
    { while ( !AtEnd() && IsFolder() )
      { ShellFolderContent::Advance();
      }
    }

    void Advance( void )
    { do
      { ShellFolderContent::Advance();
      } while ( !AtEnd() && IsFolder() );
    }
};

class EnumDrives
{
  public:
    EnumDrives( void )
    : _drive(_drives),
      _type(0)
    { ::GetLogicalDriveStrings(MAX_PATH, _drives);
      GetType();
    }

    bool AtEnd( void ) const
    { return _drive[0] == TEXT('\0');
    }

    void Advance( void )
    { _drive = &_drive[::lstrlen(_drive) + 1]; 
      GetType();
    }

    TCHAR const* GetDrive( void ) const
    { return _drive; 
    }

    bool IsRemovable( void ) const { return _type == DRIVE_REMOVABLE; }
    bool IsFixed( void ) const { return _type == DRIVE_FIXED; }
    bool IsRemote( void ) const { return _type == DRIVE_REMOTE; }
    bool IsCdRom( void ) const { return _type == DRIVE_CDROM; }
    bool IsRamDisk( void ) const { return _type == DRIVE_RAMDISK; }

  private: // Methods
    void GetType( void )
    { _type = 0;
      if ( !AtEnd() )
      { _type = ::GetDriveType(_drive);
      }
    }

  protected: // Data members
    TCHAR        _drives[MAX_PATH];
    TCHAR const* _drive;
    UINT         _type;
};

class ShellEnumDrives : public EnumDrives
{
  public:
    ShellEnumDrives( void )
    { GetInfo();
    }

    void Advance( void )
    { EnumDrives::Advance();
      GetInfo();
    }

    TCHAR const* GetDisplayName( void ) const
    { return _si.szDisplayName;
    }

    int GetIconIndex( void ) const
    { return _si.iIcon;
    }

    int GetOpenIconIndex( void ) const
    { SHFILEINFO si;
      ::ZeroMemory(&si, sizeof(si));
      ::SHGetFileInfo(_drive, 0, &si, sizeof(si), SHGFI_SYSICONINDEX|SHGFI_OPENICON);
      if ( si.hIcon != 0 )
      { ::DestroyIcon(si.hIcon);
      }
      return si.iIcon;
    }

    SmallIcon GetSmallIcon( void ) const
    { SHFILEINFO si;
      ::ZeroMemory(&si, sizeof(si));
      ::SHGetFileInfo(_drive, 0, &si, sizeof(si), SHGFI_ICON|SHGFI_SMALLICON);
      return SmallIcon(si.hIcon);
    }

    LargeIcon GetLargeIcon( void ) const
    { SHFILEINFO si;
      ::ZeroMemory(&si, sizeof(si));
      ::SHGetFileInfo(_drive, 0, &si, sizeof(si), SHGFI_ICON|SHGFI_LARGEICON);
      return LargeIcon(si.hIcon);
    }

    Icon GetShellIcon( void ) const
    { SHFILEINFO si;
      ::ZeroMemory(&si, sizeof(si));
      ::SHGetFileInfo(_drive, 0, &si, sizeof(si), SHGFI_ICON|SHGFI_SHELLICONSIZE);
      return Icon(si.hIcon);
    }

    SmallIcon GetSmallOpenIcon( void ) const
    { SHFILEINFO si;
      ::ZeroMemory(&si, sizeof(si));
      ::SHGetFileInfo(_drive, 0, &si, sizeof(si), SHGFI_ICON|SHGFI_SMALLICON|SHGFI_OPENICON);
      return SmallIcon(si.hIcon);
    }

    LargeIcon GetLargeOpenIcon( void ) const
    { SHFILEINFO si;
      ::ZeroMemory(&si, sizeof(si));
      ::SHGetFileInfo(_drive, 0, &si, sizeof(si), SHGFI_ICON|SHGFI_LARGEICON|SHGFI_OPENICON);
      return LargeIcon(si.hIcon);
    }

    Icon GetShellOpenIcon( void ) const
    { SHFILEINFO si;
      ::ZeroMemory(&si, sizeof(si));
      ::SHGetFileInfo(_drive, 0, &si, sizeof(si), SHGFI_ICON|SHGFI_SHELLICONSIZE|SHGFI_OPENICON);
      return Icon(si.hIcon);
    }

  private:
    void GetInfo( void )
    { ::ZeroMemory(&_si, sizeof(_si));
      if ( !AtEnd() )
      { ::SHGetFileInfo(_drive, 0, &_si, sizeof(_si), SHGFI_DISPLAYNAME|SHGFI_SYSICONINDEX);
        if ( _si.hIcon != 0 )
        { ::DestroyIcon(_si.hIcon);
        }
      }
    }

  protected:
    SHFILEINFO _si;
};

} // namespace Win

#endif // WINFINDFILE_H