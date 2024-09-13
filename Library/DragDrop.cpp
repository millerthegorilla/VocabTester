#include "DragDrop.h"
#include "GlobalMem.h"

using namespace Win;

// IUnknown methods 
STDMETHODIMP FileDropTarget::QueryInterface( REFIID iid, void** ppv )
{ *ppv = 0;
	if ( iid == IID_IUnknown || iid == IID_IDropTarget )
	{ *ppv = this;
    AddRef();
    return S_OK; 
	}
  else 
	{ return E_NOINTERFACE; 
	}
}

STDMETHODIMP_(ULONG) FileDropTarget::AddRef( void )
{ return ++_ref; 
}

STDMETHODIMP_(ULONG) FileDropTarget::Release( void )
{ --_ref;
  return _ref;
}

// IDropTarget methods
STDMETHODIMP FileDropTarget::DragEnter( IDataObject* pDataObject, DWORD grfKeyState, POINTL pt, DWORD* pdwEffect )
{
  *pdwEffect = DROPEFFECT_NONE;
  
  FORMATETC fmt;
  fmt.cfFormat = CF_HDROP;
  fmt.ptd = 0;
  fmt.dwAspect = DVASPECT_CONTENT;
  fmt.lindex = -1;
  fmt.tymed = TYMED_HGLOBAL;
  HRESULT hr = pDataObject->QueryGetData(&fmt);
  _formatOk = (hr == S_OK);
  if ( _formatOk )
  { StgMedium med(pDataObject, fmt);
    _sink.OnDragEnter(pt.x, pt.y, KeyState(grfKeyState), *pdwEffect, reinterpret_cast<HDROP>(med->hGlobal));
	}
  return S_OK; 
}

STDMETHODIMP FileDropTarget::DragOver( DWORD grfKeyState, POINTL pt, DWORD* pdwEffect )
{
  *pdwEffect = DROPEFFECT_NONE;
  if ( _formatOk )
  { _sink.OnDragOver(pt.x, pt.y, KeyState(grfKeyState), *pdwEffect);
  }
  return S_OK; 
}

STDMETHODIMP FileDropTarget::DragLeave( void )
{
  if ( _formatOk )
  { _sink.OnDragLeave();
  }
  return S_OK; 
}

STDMETHODIMP FileDropTarget::Drop( IDataObject* pDataObject, DWORD grfKeyState, POINTL pt, DWORD* pdwEffect )
{
  *pdwEffect = DROPEFFECT_NONE;
  if ( _formatOk )
  { FORMATETC fmt;
    fmt.cfFormat = CF_HDROP;
    fmt.ptd = 0;
    fmt.dwAspect = DVASPECT_CONTENT;
    fmt.lindex = -1;
    fmt.tymed = TYMED_HGLOBAL;
    StgMedium med(pDataObject, fmt);
    _sink.OnDrop(pt.x, pt.y, KeyState(grfKeyState), *pdwEffect, reinterpret_cast<HDROP>(med->hGlobal));
  }
  return S_OK; 
}

//
// FileDropObject
// ~~~~~~~~~~~~~~
// IUknown methods
STDMETHODIMP FileDropObject::QueryInterface( REFIID riid, void** ppvObject )
{ *ppvObject = 0; 
  if ( riid == IID_IUnknown || riid == IID_IDataObject )
  { *ppvObject = this;
    AddRef();
    return S_OK; 
  }
  else 
  { return E_NOINTERFACE; 
  }
}

STDMETHODIMP_(ULONG) FileDropObject::AddRef( void )
{ return ++_ref; 
}

STDMETHODIMP_(ULONG) FileDropObject::Release( void )
{ --_ref;
  return _ref;
}

// IDataObject methods
STDMETHODIMP FileDropObject::GetData( FORMATETC* pFormatEtcIn, STGMEDIUM* pMedium )
{
  pMedium->tymed = TYMED_NULL;
  pMedium->hGlobal = 0;
  pMedium->pUnkForRelease = 0;
  if ( pFormatEtcIn->cfFormat != CF_HDROP )
  { return DV_E_FORMATETC;
  }
  if ( !(pFormatEtcIn->tymed&TYMED_HGLOBAL) )
  { return DV_E_TYMED;
  }
  
  DROPFILES dropFiles;
  dropFiles.pFiles = sizeof(dropFiles);
  dropFiles.pt.x = 0;
  dropFiles.pt.y = 0;
  dropFiles.fNC = FALSE;
  dropFiles.fWide = FALSE;
  pMedium->hGlobal = ::GlobalAlloc(0, sizeof(dropFiles) + _files.GetStringSize());
  if ( pMedium->hGlobal == 0 )
  { return STG_E_MEDIUMFULL;
  }
  pMedium->tymed = TYMED_HGLOBAL;
  Win::GlobalMemLock<TCHAR> locked(pMedium->hGlobal);
  ::memcpy(locked, &dropFiles, sizeof(dropFiles));
  ::memcpy(&locked[sizeof(dropFiles)], _files, _files.GetStringSize());
  return S_OK;
}

STDMETHODIMP FileDropObject::GetDataHere( FORMATETC* pFormatEtc, STGMEDIUM* pMedium )
{
  if ( pFormatEtc->cfFormat != CF_HDROP )
  { return DV_E_FORMATETC;
  }
  if ( !(pFormatEtc->tymed&TYMED_HGLOBAL) || pMedium->tymed != TYMED_HGLOBAL )
  { return DV_E_TYMED;
  }
  DROPFILES dropFiles;
  dropFiles.pFiles = sizeof(dropFiles);
  dropFiles.pt.x = 0;
  dropFiles.pt.y = 0;
  dropFiles.fNC = FALSE;
  dropFiles.fWide = FALSE;
  Win::GlobalMemLock<TCHAR> locked(pMedium->hGlobal);
  ::memcpy(locked, &dropFiles, sizeof(dropFiles));
  ::memcpy(&locked[sizeof(dropFiles)], _files, _files.GetStringSize());
  return S_OK;
}

STDMETHODIMP FileDropObject::QueryGetData( FORMATETC* pFormatEtc )
{
  if ( pFormatEtc->cfFormat != CF_HDROP )
  { return DV_E_FORMATETC;
  }
  if ( !(pFormatEtc->tymed&TYMED_HGLOBAL) )
  { return DV_E_TYMED;
  }
  return S_OK;
}

STDMETHODIMP FileDropObject::GetCanonicalFormatEtc( FORMATETC* pFormatEtcIn, FORMATETC* pFormatEtcOut )
{
  ::memcpy(pFormatEtcOut, pFormatEtcIn, sizeof(FORMATETC));
  pFormatEtcOut->ptd = 0;
  return DATA_S_SAMEFORMATETC;
}

STDMETHODIMP FileDropObject::SetData( FORMATETC* pFormatEtc, STGMEDIUM* pMedium, BOOL release )
{ return E_NOTIMPL; 
}

class EnumFormatEtcObject : public IEnumFORMATETC
{
  public:
    EnumFormatEtcObject( void )
    : _ref(1),
      _cur(0)
    { FORMATETC fmt;
      fmt.cfFormat = CF_HDROP;
      fmt.ptd = 0;
      fmt.dwAspect = DVASPECT_CONTENT;
      fmt.lindex = -1;
      fmt.tymed = TYMED_HGLOBAL;
      _formats[0] = fmt;
    }

    STDMETHOD(QueryInterface)( REFIID iid, void** ppvObject );
    STDMETHOD_(ULONG, AddRef)( void );
    STDMETHOD_(ULONG, Release)( void );

    STDMETHOD(Next)( ULONG celt, FORMATETC* rgelt, ULONG* pceltFetched );
    STDMETHOD(Skip)( ULONG celt );
    STDMETHOD(Reset)( void );
    STDMETHOD(Clone)( IEnumFORMATETC** ppEnumFormatEtc );

  private: // Enums
    enum { NFORMATS = 1 };

  private: // Data members
    int       _ref;
    int       _cur;
    FORMATETC _formats[NFORMATS];
};

// IUnknown methods 
STDMETHODIMP EnumFormatEtcObject::QueryInterface( REFIID iid, void** ppv )
{ *ppv = 0;
	if ( iid == IID_IUnknown || iid == IID_IEnumFORMATETC )
	{ *ppv = this;
    AddRef();
    return S_OK; 
	}
  else 
	{ return E_NOINTERFACE; 
	}
}

STDMETHODIMP_(ULONG) EnumFormatEtcObject::AddRef( void )
{ return ++_ref; 
}

STDMETHODIMP_(ULONG) EnumFormatEtcObject::Release( void )
{ --_ref;
  if ( _ref == 0 )
  { delete this;
    return 0;
  }
  return _ref;
}

STDMETHODIMP EnumFormatEtcObject::Next( ULONG celt, FORMATETC* rgelt, ULONG* nFetched )
{
  ULONG fetched = 0;
  for ( ULONG i = 0; i < celt && _cur < NFORMATS; ++i, ++_cur, ++fetched )
  { rgelt[i] = _formats[_cur];
  }
  if ( nFetched != 0 )
  { *nFetched = fetched;
  }
  if ( fetched == celt )
  { return S_OK;
  }
  else
  { return S_FALSE;
  }
}

STDMETHODIMP EnumFormatEtcObject::Skip( ULONG celt )
{ _cur += celt;
  return S_OK;
}

STDMETHODIMP EnumFormatEtcObject::Reset( void )
{ _cur = 0;
  return S_OK;
}

STDMETHODIMP EnumFormatEtcObject::Clone( IEnumFORMATETC** ppEnumFormatEtc )
{ *ppEnumFormatEtc = new EnumFormatEtcObject;
  return S_OK;
}


STDMETHODIMP FileDropObject::EnumFormatEtc( DWORD direction, IEnumFORMATETC** ppEnumFormatEtc )
{ 
  if ( direction != DATADIR_GET )
  { return E_NOTIMPL;
  }
  *ppEnumFormatEtc = new EnumFormatEtcObject;
  return S_OK;
}

STDMETHODIMP FileDropObject::DAdvise( FORMATETC* pFormatEtc, DWORD advf, IAdviseSink* pAdvSink, DWORD* pConnection )
{ return E_NOTIMPL;
}

STDMETHODIMP FileDropObject::DUnadvise( DWORD connection )
{ return E_NOTIMPL;
}

STDMETHODIMP FileDropObject::EnumDAdvise( IEnumSTATDATA** ppEnumAdvise )
{ return E_NOTIMPL;
}

//
// Class FileDropSource
// ~~~~~~~~~~~~~~~~~~~~
STDMETHODIMP FileDropSource::QueryInterface( REFIID riid, void** ppvObject )
{ *ppvObject = 0;
	if ( riid == IID_IUnknown || riid == IID_IDropSource )
  { *ppvObject = this;
    AddRef();
    return S_OK; 
  }
  else 
  { return E_NOINTERFACE; 
  }
}

STDMETHODIMP_(ULONG) FileDropSource::AddRef( void )
{ return ++_ref;
}

STDMETHODIMP_(ULONG) FileDropSource::Release( void )
{ --_ref;
  return _ref;
}

// IDropSource methods
STDMETHODIMP FileDropSource::QueryContinueDrag( BOOL escapePressed, DWORD keyState )
{
  if ( escapePressed )
  { return DRAGDROP_S_CANCEL;
  }
  Win::KeyState state(keyState);
  if ( state.IsLButtonPressed() )
  { return S_OK;
  }
  return DRAGDROP_S_DROP;
}

STDMETHODIMP FileDropSource::GiveFeedback( DWORD effect )
{ return DRAGDROP_S_USEDEFAULTCURSORS;
}