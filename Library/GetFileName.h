#ifndef WINGETFILENAME_H
#define WINGETFILENAME_H

#include <windows.h>
#include <tchar.h>

namespace Win
{
  
class SimpleOpenFileName : public OPENFILENAME
{
  public: // Class
    class EnumFiles
    {
      public:
        EnumFiles( TCHAR const files[] )
        : _files(files),
          _filePtr(0),
          _fileName(0),
          _singleFile(true)
        {
          _path[0] = TEXT('\0');

          DWORD attr = ::GetFileAttributes(_files);
          if ( attr == 0xFFFFFFFF ) return;

          ::_tcscpy(_path, _files);
          _singleFile = !(attr&FILE_ATTRIBUTE_DIRECTORY);
          if ( _singleFile )
          { _fileName = ::_tcsrchr(_path, TEXT('\\'));
            if ( _fileName != 0 )
            { _fileName = &_fileName[1];
            }
            else
            { _fileName = _path;
            }
          }
          else
          { if ( _path[::_tcslen(_path) - 1] != TEXT('\\') )
            { ::_tcscat(_path, TEXT("\\"));
            }
            _fileName = &_path[::_tcslen(_path)];

            _filePtr = &_files[::_tcslen(_files) + 1];
            ::_tcscpy(_fileName, _filePtr);
          }
        }

        bool AtEnd( void ) const
        { return _path[0] == TEXT('\0');
        }

        void Advance( void )
        { if ( _singleFile )
          { _path[0] = TEXT('\0');
          }
          else
          { _filePtr = &_filePtr[::_tcslen(_filePtr) + 1];
            if ( _filePtr[0] == TEXT('\0') )
            { _path[0] = TEXT('\0');
            }
            else
            { ::_tcscpy(_fileName, _filePtr);
            }
          }
        }

        TCHAR const* GetPath( void ) const { return _path; }
        TCHAR const* GetFileName( void ) const { return _fileName; }

      private:
        TCHAR const* _files;
        TCHAR const* _filePtr;
        TCHAR        _path[MAX_PATH];
        TCHAR*       _fileName;
        bool         _singleFile;
    };

  public: // Methods
    void InitialFileName( TCHAR const name[] ) { ::_tcscpy(_buf, name); }
    void InitialFolder( TCHAR const folder[] ) { lpstrInitialDir = folder; }
    void FileMustExist( void ) { Flags |= OFN_FILEMUSTEXIST; }
    void PathMustExist( void ) { Flags |= OFN_PATHMUSTEXIST; }
    void HideReadOnly( void ) { Flags |= OFN_HIDEREADONLY; }
    void SetCommonFlags( void ) { Flags |= OFN_FILEMUSTEXIST|OFN_PATHMUSTEXIST|OFN_HIDEREADONLY; }
    void AllowMultiselect( void ) { Flags |= OFN_ALLOWMULTISELECT; }
    void Explorer( void ) { Flags |= OFN_EXPLORER; }
    void SetShareAware( void ) { Flags |= OFN_SHAREAWARE; }
    void SetOverwritePrompt( void ) { Flags |= OFN_OVERWRITEPROMPT; }
    void SetTitle( TCHAR const title[] ) { lpstrTitle = title; }
    void SetDefaultExt( TCHAR const ext[] ) { lpstrDefExt = ext; }

    TCHAR const* GetFileName( void ) const { return _buf; }
    int GetFileOffset( void ) const { return nFileOffset; }

  protected:
    SimpleOpenFileName( TCHAR const filter[] = 0, int filterIdx = 0, HWND owner = 0, TCHAR buf[] = 0, int bufSize = 0 )
    { _buf = (buf == 0 ? _buffer : buf);
      _buf[0] = TEXT('\0');
      lStructSize = sizeof(OPENFILENAME);
      hwndOwner = owner;
      lpstrFilter = filter;
      nFilterIndex = filterIdx;
      lpstrFile = _buf;
      nMaxFile = (buf == 0 ? MAX_PATH : bufSize);
      
      Flags = 0;
      hInstance = 0; 
      lpstrCustomFilter = 0; 
      nMaxCustFilter = 0; 
      lpstrFileTitle = 0; 
      nMaxFileTitle = 0; 
      lpstrInitialDir = 0; 
      lpstrTitle = 0;
      nFileOffset = 0; 
      nFileExtension = 0; 
      lpstrDefExt = 0; 
      lCustData = 0; 
      lpfnHook = 0; 
      lpTemplateName = 0; 
    }

  private:
    TCHAR  _buffer[MAX_PATH];
    TCHAR* _buf;
};


class OpenFileName : public SimpleOpenFileName
{
  public: // Typedefs
    typedef SimpleOpenFileName::EnumFiles EnumFiles;

  public: // Methods
    OpenFileName( TCHAR const filter[] = 0, int filterIdx = 0, HWND owner = 0, TCHAR buf[] = 0, int bufSize = 0 )
    : SimpleOpenFileName(filter, filterIdx, owner, buf, bufSize)
    {}

    bool GetOpenFileName( void )
    { return ::GetOpenFileName(this) != 0;
    }
};

class SaveFileName : public SimpleOpenFileName
{
  public: // Typedefs
    typedef SimpleOpenFileName::EnumFiles EnumFiles;

  public: // Methods
    SaveFileName( TCHAR const filter[] = 0, int filterIdx = 0, HWND owner = 0, TCHAR buf[] = 0, int bufSize = 0 )
    : SimpleOpenFileName(filter, filterIdx, owner, buf, bufSize)
    {}

    bool GetSaveFileName( void )
    { return ::GetSaveFileName( this ) != 0;
    }
};

} // namespace Win

#endif // WINGETFILENAME_H