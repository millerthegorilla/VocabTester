#ifndef WINFINDFILE_H
#define WINFINDFILE_H

#include "Icon.h"
#include <windows.h>
#include <shlobj.h>
#include <tchar.h>
#include <cassert>
#include "stdstring"

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

    unsigned long GetSize( unsigned long* highPart ) const
    { if ( highPart != 0 ) *highPart = nFileSizeHigh;
      return nFileSizeLow;
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

    void Advance( FindFileData& data )
    { assert(_h != INVALID_HANDLE_VALUE);
      assert(!_done);
      _done = (::FindNextFile(_h, &data) == 0); 
    }

  protected: // Methods
    SimpleFindFile( HANDLE h = INVALID_HANDLE_VALUE )
    : _h(h),
      _done(false)
    {}

  protected: // Data mebers
    HANDLE _h;
    bool   _done;
};

class FindFileQuery
{
  public:
    FindFileQuery( TCHAR const wildcards[] )
    : _path(wildcards),
      _filesOnly(false),
      _foldersOnly(false),
      _skipDots(true)
    {}

    FindFileQuery( stdstring const& wildcards )
    : _path(wildcards),
      _filesOnly(false),
      _foldersOnly(false),
      _skipDots(true)
    {}

    FindFileQuery( TCHAR const folder[], TCHAR const wildcards[] )
    : _path(folder),
      _filesOnly(false),
      _foldersOnly(false),
      _skipDots(true)
    { 
      if ( _path[_path.length() - 1] != _T('\\') )
      { _path += _T('\\');
      }
      _path += wildcards;
    }

    FindFileQuery( stdstring const& folder, stdstring const& wildcards )
    : _path(folder),
      _filesOnly(false),
      _foldersOnly(false),
      _skipDots(true)
    { 
      if ( _path[_path.length() - 1] != _T('\\') )
      { _path += _T('\\');
      }
      _path += wildcards;
    }

    void FilesOnly( bool filesOnly = true ) { _filesOnly = filesOnly; }
    void FoldersOnly( bool foldersOnly = true ) { _foldersOnly = foldersOnly; }
    void SkipDots( bool skipDots = true ) { _skipDots = skipDots; }
    
    bool Filter( FindFileData const& data ) const
    { if ( _filesOnly && data.IsFolder() ) return false;
      if ( _foldersOnly && !data.IsFolder() ) return false;
      if ( _skipDots && data.IsDot() ) return false;
      return true;
    }
    
    stdstring GetPath( void ) const { return _path; }

  private: // Data members
    stdstring _path;
    bool      _filesOnly;
    bool      _foldersOnly;
    bool      _skipDots;
};

class FolderContent : public SimpleFindFile
{
  public:
    FolderContent( FindFileQuery const& query )
    : _query(query)
    { 
      _h = ::FindFirstFile(query.GetPath().c_str(), &_match);
      while ( !AtEnd() && !_query.Filter(_match) )
      { SimpleFindFile::Advance(_match);
      }
    }

    void Advance( void )
    { do
      { SimpleFindFile::Advance(_match);
      } while ( !AtEnd() && !_query.Filter(_match) );
    }

    FindFileData GetMatch( void ) const { return _match; }

  private: // Data members
    FindFileQuery const& _query;
    FindFileData         _match;
};

class EnumDrives
{
  public:
    EnumDrives( void )
    : _drive(_drives),
      _type(0)
    { 
      _drives[0] = _T('\0');
      ::GetLogicalDriveStrings(BUFSIZE, _drives);
      GetType();
    }

    bool AtEnd( void ) const { return _drive[0] == _T('\0'); }

    void Advance( void )
    { 
      _drive = &_drive[::_tcslen(_drive) + 1]; 
      GetType();
    }

    TCHAR const* GetDrive( void ) const { return _drive; }

    bool IsRemovable( void ) const { return _type == DRIVE_REMOVABLE; }
    bool IsFixed( void ) const { return _type == DRIVE_FIXED; }
    bool IsRemote( void ) const { return _type == DRIVE_REMOTE; }
    bool IsCdRom( void ) const { return _type == DRIVE_CDROM; }
    bool IsRamDisk( void ) const { return _type == DRIVE_RAMDISK; }

  private: // Enums
    enum { BUFSIZE = 26 * (3 + 1) + 1 };
    // 26 possible characters
    // times (3 characters (e.g. c:\) plus 1 terminating zero)
    // plus 1 final terminating zero

  private: // Methods
    void GetType( void )
    { 
      _type = 0;
      if ( !AtEnd() )
      { _type = ::GetDriveType(_drive);
      }
    }

  protected: // Data members
    TCHAR        _drives[BUFSIZE];
    TCHAR const* _drive;
    UINT         _type;
};

class EnumFixedDrives : public EnumDrives
{
  public:
    EnumFixedDrives( void )
    {
      while ( !IsFixed() && !AtEnd() ) EnumDrives::Advance();
    }

    void Advance( void )
    {
      do
      {
        EnumDrives::Advance();
      } while ( !IsFixed() && !AtEnd() );
    }
};

/*

  COM must be initialized before creating this object

*/
class ShellFileInfo
{
  public:
    ShellFileInfo( TCHAR const path[], DWORD attr )
    : _path(path),
      _attr(attr),
      _flags(SHGFI_USEFILEATTRIBUTES)
    {
      GetInfo(_si, SHGFI_DISPLAYNAME|SHGFI_SYSICONINDEX);
    }

    ShellFileInfo( stdstring const& path, DWORD attr )
    : _path(path.c_str()),
      _attr(attr),
      _flags(SHGFI_USEFILEATTRIBUTES)
    { 
      GetInfo(_si, SHGFI_DISPLAYNAME|SHGFI_SYSICONINDEX);
    }

    ShellFileInfo( TCHAR const path[] )
    : _path(path),
      _attr(0),
      _flags(0)
    {
      GetInfo(_si, SHGFI_DISPLAYNAME|SHGFI_SYSICONINDEX);
    }

    ShellFileInfo( stdstring const& path )
    : _path(path.c_str()),
      _attr(0),
      _flags(0)
    { 
      GetInfo(_si, SHGFI_DISPLAYNAME|SHGFI_SYSICONINDEX);
    }

    ShellFileInfo( ITEMIDLIST const* idList )
    : _path(reinterpret_cast<TCHAR const*>(idList)),
      _attr(0),
      _flags(SHGFI_PIDL)
    {
      GetInfo(_si, SHGFI_DISPLAYNAME|SHGFI_SYSICONINDEX);
    }

    TCHAR const* GetDisplayName( void ) const { return _si.szDisplayName; }
    
    int GetIconIndex( void ) const { return _si.iIcon; }

    int GetOpenIconIndex( void ) const
    { SHFILEINFO si;
      GetInfo(si, SHGFI_SYSICONINDEX|SHGFI_OPENICON);
      return si.iIcon;
    }

    SmallIcon GetSmallIcon( void ) const
    { SHFILEINFO si;
      GetInfo(si, SHGFI_ICON|SHGFI_SMALLICON);
      return SmallIcon(si.hIcon);
    }

    LargeIcon GetLargeIcon( void ) const
    { SHFILEINFO si;
      GetInfo(si, SHGFI_ICON|SHGFI_LARGEICON);
      return LargeIcon(si.hIcon);
    }

    Icon GetShellIcon( void ) const
    { SHFILEINFO si;
      GetInfo(si, SHGFI_ICON|SHGFI_SHELLICONSIZE);
      return Icon(si.hIcon);
    }

    SmallIcon GetSmallOpenIcon( void ) const
    { SHFILEINFO si;
      GetInfo(si, SHGFI_ICON|SHGFI_SMALLICON|SHGFI_OPENICON);
      return SmallIcon(si.hIcon);
    }

    LargeIcon GetLargeOpenIcon( void ) const
    { SHFILEINFO si;
      GetInfo(si, SHGFI_ICON|SHGFI_LARGEICON|SHGFI_OPENICON);
      return LargeIcon(si.hIcon);
    }

    Icon GetShellOpenIcon( void ) const
    { SHFILEINFO si;
      GetInfo(si, SHGFI_ICON|SHGFI_SHELLICONSIZE|SHGFI_OPENICON);
      return Icon(si.hIcon);
    }

  private: // Methods
    void GetInfo( SHFILEINFO& si, UINT flags ) const
    { ::ZeroMemory(&si, sizeof(si));
      ::SHGetFileInfo(_path, _attr, &si, sizeof(si),
                      flags|_flags);
    }

  private: // Data members
    TCHAR const* _path;
    DWORD        _attr;
    UINT         _flags;
    SHFILEINFO   _si;
};

} // namespace Win

#endif // WINFINDFILE_H