#ifndef WINFILE_H
#define WINFILE_H

#include <cassert>
#include "stdstring"
#include "SmartHandle.h"
#include "Shell.h"
#include "XWin.h"
#include <tchar.h>


namespace Win
{

class File : public SHandle
{
  public:
    File( void )
    {}

    File( TCHAR const fileName[], DWORD desiredAccess = GENERIC_READ, DWORD creationDistribution = OPEN_EXISTING, DWORD shareMode = FILE_SHARE_READ, DWORD attr = FILE_ATTRIBUTE_NORMAL )
    { Open(fileName, desiredAccess, creationDistribution, shareMode, attr);
    }

    File( stdstring const& fileName, DWORD desiredAccess = GENERIC_READ, DWORD creationDistribution = OPEN_EXISTING, DWORD shareMode = FILE_SHARE_READ, DWORD attr = FILE_ATTRIBUTE_NORMAL )
    { Open(fileName, desiredAccess, creationDistribution, shareMode, attr);
    }

    static DWORD GetAttr( TCHAR const fileName[] )
    { return ::GetFileAttributes(fileName);
    }

    static DWORD GetAttr( stdstring const& fileName )
    { return GetAttr(fileName.c_str());
    }

    static bool Exists( TCHAR const fileName[] )
    { DWORD attr = GetAttr(fileName);
      return  attr != 0xFFFFFFFF;
    }

    static bool Exists( stdstring const& fileName )
    { return Exists(fileName.c_str());
    }

    static bool IsFolder( TCHAR const fileName[] )
    { DWORD attr = GetAttr(fileName);
      return (attr&FILE_ATTRIBUTE_DIRECTORY) != 0;
    }

    static bool IsFolder( stdstring const& fileName )
    { return IsFolder(fileName.c_str());
    }

    static bool Delete( TCHAR const fileName[] )
    { return ::DeleteFile(fileName) != 0;
    }

    static bool Delete( stdstring const& fileName )
    { return Delete(fileName.c_str());
    }

    static bool Move( TCHAR const existingName[], TCHAR const newName[] )
    { return ::MoveFile(existingName, newName) != 0;
    }

    static bool Move( stdstring const& existingName, stdstring const& newName )
    { return Move(existingName.c_str(), newName.c_str());
    }

    static bool Copy( TCHAR const existingName[], TCHAR const newName[], bool overwriteExisting = false )
    { return ::CopyFile(existingName, newName, !overwriteExisting) != 0;
    }

    static bool Copy( stdstring const& existingName, stdstring const& newName, bool overwriteExisting = false )
    { return Copy(existingName.c_str(), newName.c_str(), overwriteExisting);
    }

    void Open( TCHAR const fileName[], DWORD desiredAccess = GENERIC_READ, DWORD creationDistribution = OPEN_EXISTING, DWORD shareMode = FILE_SHARE_READ, DWORD attr = FILE_ATTRIBUTE_NORMAL )
    { Close();
      _h = ::CreateFile(fileName, desiredAccess, shareMode, 0, creationDistribution, attr, 0);
      if ( _h == INVALID_HANDLE_VALUE )
      { throw Win::Exception(TEXT("Cannot open file"));
      }
    }

    void Open( stdstring const& fileName, DWORD desiredAccess = GENERIC_READ, DWORD creationDistribution = OPEN_EXISTING, DWORD shareMode = FILE_SHARE_READ, DWORD attr = FILE_ATTRIBUTE_NORMAL )
    { Open(fileName.c_str(), desiredAccess, creationDistribution, shareMode, attr);
    }

    bool TryOpen( TCHAR const fileName[], DWORD desiredAccess = GENERIC_READ, DWORD creationDistribution = OPEN_EXISTING, DWORD shareMode = FILE_SHARE_READ, DWORD attr = FILE_ATTRIBUTE_NORMAL )
    { Close();
      _h = ::CreateFile(fileName, desiredAccess, shareMode, 0, creationDistribution, attr, 0);
      return IsOk();
    }

    bool TryOpen( stdstring const& fileName, DWORD desiredAccess = GENERIC_READ, DWORD creationDistribution = OPEN_EXISTING, DWORD shareMode = FILE_SHARE_READ, DWORD attr = FILE_ATTRIBUTE_NORMAL )
    { return TryOpen(fileName.c_str(), desiredAccess, creationDistribution, shareMode, attr);
    }

    double Size( void ) const
    { assert(_h != INVALID_HANDLE_VALUE);
      DWORD high;
      DWORD low = ::GetFileSize(_h, &high);
      if ( low == 0xFFFFFFFF && ::GetLastError() != NO_ERROR )
      { return 0.0; // Error
      }
      double size = high;
      size *= double(MAXDWORD) + 1;
      size += low;
      return size;
    }

    unsigned long Read( void* buf, unsigned long bufSize )
    { assert(_h != INVALID_HANDLE_VALUE);
      unsigned long bytesRead = 0;
      if ( !::ReadFile(_h, buf, bufSize, &bytesRead, 0) )
      { throw Win::Exception(TEXT("Read file failed"));
      }
      return bytesRead;
    }

    long GetLong( void )
    { long buf;
      Read(&buf, sizeof(buf));
      return buf;
    }

    short GetShort( void )
    { short buf;
      Read(&buf, sizeof(buf));
      return buf;
    }

    int GetChar( void )
    { char buf;
      return (Read(&buf, sizeof(buf)) == sizeof(buf) ? buf : EOF);
    }

    stdstring GetString( void )
    { long size = GetLong(); // Includes terminating null
      stdstring str;
      str.reserve(size);
      str.resize(size - 1);
      Read(&str[0], size * sizeof(TCHAR));
      return str;
    }

    bool GetBool( void )
    { long pattern = GetLong();
      if ( pattern == TRUE_PATTERN ) return true;
      if ( pattern == FALSE_PATTERN ) return false;
      throw Win::Exception(TEXT("Data corrupted"));
      return false;
    }

    double GetDouble( void )
    { double buf;
      Read(&buf, sizeof(buf));
      return buf;
    }

    float GetFloat( void )
    { float buf;
      Read(&buf, sizeof(buf));
      return buf;
    }

    unsigned long Write( void const* buf, unsigned long bufSize )
    { assert(_h != INVALID_HANDLE_VALUE);
      unsigned long bytesWritten = 0;
      if ( !::WriteFile(_h, buf, bufSize, &bytesWritten, 0) )
      { throw Win::Exception(TEXT("Write file failed"));
      }
      return bytesWritten;
    }

    void PutLong( long buf )
    { Write(&buf, sizeof(buf));
    }

    void PutShort( short buf )
    { Write(&buf, sizeof(buf));
    }

    void PutChar( char buf )
    { Write(&buf, sizeof(buf));
    }

    void PutString( TCHAR const str[] )
    { long size = ::_tcslen(str) + 1; // Includes terminating null
      PutLong(size);
      Write(str, size * sizeof(TCHAR));
    }

    void PutString( stdstring const& str )
    { PutString(str.c_str());
    }

    void PutBool( bool b )
    { PutLong(b ? TRUE_PATTERN : FALSE_PATTERN); 
    }

    void PutDouble( double buf )
    { Write(&buf, sizeof(buf));
    }

    void PutFloat( float buf )
    { Write(&buf, sizeof(buf));
    }

    void Flush( void )
    { assert(_h != INVALID_HANDLE_VALUE);
      if ( !::FlushFileBuffers(_h) )
      { throw Win::Exception(TEXT("::FlushFileBuffers failed"));
      }
    }

    void SetEndOfFile( void )
    { assert(_h != INVALID_HANDLE_VALUE);
      if ( !::SetEndOfFile(_h) )
      { throw Win::Exception(TEXT("::SetEndOfFile failed"));
      }
    }

    void Empty( void )
    { SetPosBegin(0);
      SetEndOfFile();
    }

    unsigned long GetPos( void )
    { assert(_h != INVALID_HANDLE_VALUE);
      unsigned long result = ::SetFilePointer(_h, 0, 0, FILE_CURRENT); 
      if ( result == 0xFFFFFFFF )
      { throw Win::Exception(TEXT("::SetFilePointer failed"));
      }
      return result;
    }

    long SetPosBegin( long offset )
    { assert(_h != INVALID_HANDLE_VALUE);
      offset = ::SetFilePointer(_h, offset, 0, FILE_BEGIN);
      if (  offset == 0xFFFFFFFF )
      { throw Win::Exception(TEXT("::SetFilePointer failed"));
      }
      return offset;
    }

    long SetPosCurrent( long offset )
    { assert(_h != INVALID_HANDLE_VALUE);
      offset = ::SetFilePointer(_h, offset, 0, FILE_CURRENT);
      if (  offset == 0xFFFFFFFF )
      { throw Win::Exception(TEXT("::SetFilePointer failed"));
      }
      return offset;
    }

    long SetPosEnd( long offset )
    { assert(_h != INVALID_HANDLE_VALUE);
      offset = ::SetFilePointer(_h, offset, 0, FILE_END);
      if (  offset == 0xFFFFFFFF )
      { throw Win::Exception(TEXT("::SetFilePointer failed"));
      }
      return offset;
    }

    static stdstring GetDisplayName( TCHAR const path[] )
    { SHFILEINFO si;
      ::ZeroMemory(&si, sizeof(si));
      ::SHGetFileInfo(path, 0, &si, sizeof(si), SHGFI_DISPLAYNAME);
      return stdstring(si.szDisplayName);
    }

    static stdstring GetDisplayName( stdstring const& path )
    { return GetDisplayName(path.c_str());
    }

    static int GetIconIndex( TCHAR const path[] )
    { SHFILEINFO si;
      ::ZeroMemory(&si, sizeof(si));
      ::SHGetFileInfo(path, 0, &si, sizeof(si), SHGFI_SYSICONINDEX);
      return si.iIcon;
    }

    static int GetIconIndex( stdstring const& path )
    { return GetIconIndex(path.c_str());
    }

  private: // Enums
    enum
    {
      TRUE_PATTERN = 0x5FE14CD8,
      FALSE_PATTERN = 0x4BADBAD5,
    };

  private: // Methods
    File( File const& b );
    File& operator=( File const& b );
};

} // namespace Win

#endif // WINFILE_H