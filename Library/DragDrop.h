#ifndef WINDRAGDROP_H
#define WINDRAGDROP_H

#include <oleidl.h>
#include <shlobj.h>
#include "MessageData.h"
#include "DoubleZeroString.h"
#include "XWin.h"

namespace Win
{

class EnumDropFiles
{
  public:
    EnumDropFiles( HDROP hDrop )
    : _hDrop(hDrop),
      _cur(0),
      _count(::DragQueryFile(_hDrop, 0xFFFFFFFF, 0, 0))
    { if ( !AtEnd() )
      { ::DragQueryFile(_hDrop, _cur, _path, MAX_PATH);
      }
    }

    bool AtEnd( void ) const
    { return _cur >= _count;
    }

    void Advance( void )
    { ++_cur;
      if ( !AtEnd() )
      { ::DragQueryFile(_hDrop, _cur, _path, MAX_PATH);
      }
    }

    TCHAR const* GetPath( void ) const
    { return _path;
    }

    int GetFileCount( void ) const
    { return _count;
    }

  private:
    HDROP  _hDrop;
    int    _cur;
    int    _count;
    TCHAR  _path[MAX_PATH];
};

class DroppedFiles
{
  public:
    DroppedFiles( HDROP hDrop )
    : _hDrop(hDrop),
      _files(reinterpret_cast<DROPFILES*>(::GlobalLock(_hDrop)))
    {}

    ~DroppedFiles( void )
    { ::GlobalUnlock(_hDrop);
    }

    TCHAR const* GetDroppedFiles( void ) const
    { return &reinterpret_cast<TCHAR const*>(_files)[_files->pFiles];
    }

  private:
    HDROP      _hDrop;
    DROPFILES* _files;
};

class FileDropTarget : public IDropTarget
{
  public: // Class
    class NotifySink
    {
      public:
        virtual void OnDragEnter( int x, int y, Win::KeyState const& keyState, DWORD& effect, HDROP hDrop ) = 0;
        virtual void OnDragOver( int x, int y, Win::KeyState const& keyState, DWORD& effect ) = 0;
        virtual void OnDragLeave( void ) = 0;
        virtual void OnDrop( int x, int y, Win::KeyState const& keyState, DWORD& effect, HDROP hDrop ) = 0;
    };
    
  public: // Methods
    FileDropTarget( NotifySink& sink )
    : _ref(1),
      _hwnd(0),
      _sink(sink),
      _formatOk(false)
    {}

    ~FileDropTarget( void )
    { Revoke();
    }

    void Revoke( void )
    { if ( _hwnd != 0 )
      { ::RevokeDragDrop(_hwnd);
      }
      _hwnd = 0;
    }
    
    void Register( HWND hwnd )
    { Revoke();
      _hwnd = hwnd;
      HRESULT hr = ::RegisterDragDrop(_hwnd, this);
      if ( FAILED(hr) )
      { throw XCom(hr, TEXT("::RegisterDragDrop failed"));
      }
    }
    
    // IUnknown methods 
    STDMETHOD(QueryInterface)( REFIID iid,	void** ppvObject	);
    STDMETHOD_(ULONG, AddRef)( void );
    STDMETHOD_(ULONG, Release)( void );
    
    // IDropTarget methods
    STDMETHOD(DragEnter)( IDataObject* pDataObject, DWORD grfKeyState, POINTL pt, DWORD* pdwEffect );
    STDMETHOD(DragOver)( DWORD grfKeyState, POINTL pt, DWORD* pdwEffect );
    STDMETHOD(DragLeave)( void );
    STDMETHOD(Drop)( IDataObject* pDataObject, DWORD grfKeyState, POINTL pt, DWORD* pdwEffect );
    
  private:
    int         _ref;
    HWND        _hwnd;
		NotifySink& _sink;
		bool        _formatOk;
};

class FileDropObject : public IDataObject
{
  public:
    FileDropObject( void )
    : _ref(1)
    {}

    void AddFile( TCHAR const path[] )
    { _files.AddString(path);
    }

    int GetFileCount( void )
    { return _files.GetStringCount();
    }

    // IUknown methods
    STDMETHOD(QueryInterface)( REFIID riid, void** ppvObject );
    STDMETHOD_(ULONG, AddRef)( void );
    STDMETHOD_(ULONG, Release)( void );

    // IDataObject methods
    STDMETHOD(GetData)( FORMATETC* pFormatEtcIn, STGMEDIUM* pMedium );
    STDMETHOD(GetDataHere)( FORMATETC* pFormatEtc, STGMEDIUM* pMedium );
    STDMETHOD(QueryGetData)( FORMATETC* pFormatEtc );
    STDMETHOD(GetCanonicalFormatEtc)( FORMATETC* pFormatEtcIn, FORMATETC* pFormatEtcOut );
    STDMETHOD(SetData)( FORMATETC* pFormatEtc, STGMEDIUM* pMedium, BOOL release );
    STDMETHOD(EnumFormatEtc)( DWORD direction, IEnumFORMATETC** ppEnumFormatEtc );
    STDMETHOD(DAdvise)( FORMATETC* pFormatEtc, DWORD advf, IAdviseSink* pAdvSink, DWORD* pConnection );
    STDMETHOD(DUnadvise)( DWORD connection );
    STDMETHOD(EnumDAdvise)( IEnumSTATDATA** ppEnumAdvise );

  private:
    ULONG            _ref;
    DoubleZeroString _files;
};

class FileDropSource : public IDropSource
{
  public:
    FileDropSource( void )
    : _ref(1)
    {}
    
    // IUnknown methods
    STDMETHOD(QueryInterface)( REFIID riid, void** ppvObject );
    STDMETHOD_(ULONG, AddRef)( void );
    STDMETHOD_(ULONG, Release)( void );
    
    // IDropSource methods
    STDMETHOD(QueryContinueDrag)( BOOL escapePressed, DWORD keyState );
    STDMETHOD(GiveFeedback)( DWORD effect );

  private:
    ULONG _ref;
};

class StgMedium
{
  public:
    StgMedium( IDataObject* pDataObject, FORMATETC& fmt )
    : _hr(pDataObject->GetData(&fmt, &_medium))
    {}

    ~StgMedium( void )
    { ::ReleaseStgMedium(&_medium);
    }

    bool IsOk( void ) const
    { return _hr == S_OK;
    }

    STGMEDIUM* operator->( void )
    { return &_medium;
    }

  private:
    HRESULT   _hr;
    STGMEDIUM _medium;
};

} // namespace Win

#endif // WINDRAGDROP_H