#ifndef WINDIRECTDRAW_H
#define WINDIRECTDRAW_H

#include <cassert>
#include <ddraw.h>
#include "Bitmap.h"
#include "Canvas.h"
#include "XWin.h"
#include "Logger.h"

namespace Win
{
namespace Direct
{

class XDraw : public Win::Exception
{
  public:
    XDraw( HRESULT hr, TCHAR const msg[] );
};

template <class I>
class IFace
{
  public:
    IFace( IFace<I>& face );
    ~IFace( void );

    IFace<I>& operator=( IFace<I>& face );

    I* operator->( void );
    operator I*( void );

  protected: // Methods
    IFace( void );

  protected: // Data members
    I* _i;
};

class DrawSurfaceDesc : public DDSURFACEDESC2
{
  public:
    DrawSurfaceDesc( void );

    void SetSize( long w, long h );
    void SetCaps( unsigned long caps );

    void* Buffer( void );
    long Width( void ) const;
    long Height( void ) const;
    long Pitch( void ) const;

    DDPIXELFORMAT PixelFormat( void ) const;
};

//
// The main DirectDraw object
// ~~~~~~~~~~~~~~~~~~~~~~~~~~
class Draw : public IFace<IDirectDraw7>
{
  public:
    Draw( void ) throw ();

    HRESULT Create( void ) throw ();
    void CreateX( void ) throw (XDraw);

    void SetCoopNormal( HWND hwnd ) throw (XDraw);
    void SetCoopFullScreen( HWND hwnd ) throw (XDraw);
    
    void GetDisplayMode( DrawSurfaceDesc& desc ) throw (XDraw);
    
    HRESULT SetDisplayMode( DWORD w, DWORD h, DWORD bpp, DWORD refreshRate = 0 ) throw ();
    void SetDisplayModeX( DWORD w, DWORD h, DWORD bpp, DWORD refreshRate = 0 ) throw (XDraw);
};

// Clipper for blitting
// ~~~~~~~~~~~~~~~~~~~~
class DrawClipper: public IFace<IDirectDrawClipper>
{
  public:
    DrawClipper( Draw& draw ) throw (XDraw);

    void SetWin( HWND hwnd ) throw (XDraw);
};

//
// Base class for all surfaces
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~
class DrawSurface : public IFace<IDirectDrawSurface7>
{
  public:
    DrawSurface( void ) throw ();

    void GetDesc( DrawSurfaceDesc& desc ) throw (XDraw);
    void SetClipper( DrawClipper& clipper ) throw (XDraw);
    void BltFrom( DrawSurface& src, RECT* dstRect = 0, RECT* srcRect = 0, DWORD flags = 0 ) throw (XDraw);
    void Fill( COLORREF c, RECT* dstRect = 0 ) throw (XDraw);

    void Lock( DrawSurfaceDesc& desc ) throw (XDraw);
    void Unlock( void ) throw ();
};

class DrawPixelFormat
{
  public:
    DrawPixelFormat( void ) throw ();
    DrawPixelFormat( DrawSurfaceDesc& desc ) throw (XDraw);

    void Init( DrawSurfaceDesc& desc ) throw (XDraw);
    
    int Bpp( void ) const  throw ();
    unsigned long Mask( void ) const  throw ();
    
    unsigned long ColorValue16( COLORREF color )  throw ();
    unsigned long ColorValue24( COLORREF color )  throw ();
    unsigned long ColorValue32 (COLORREF color)  throw ();
    unsigned long ColorValue( COLORREF color )  throw ();

  private: // Methods
    static unsigned HiBitShift( unsigned val );

  private: // Data members
    int              _bpp; // bits per pixel 4, 8, 16, 24, or 32
    unsigned long    _redMask;
    unsigned long    _greenMask;
    unsigned long    _blueMask;
    unsigned         _redShift;
    unsigned         _greenShift;
    unsigned         _blueShift;
    unsigned long    _mask;
};

class DrawCanvas : public Canvas
{
  public:
    DrawCanvas( DrawSurface& surf ) throw (XDraw);
    ~DrawCanvas( void );

  private:
    DrawSurface& _surf;
};

class DrawSurfaceBuffer
{
  public:
    DrawSurfaceBuffer( DrawSurface& surf ) throw (XDraw);
    ~DrawSurfaceBuffer( void );
    
    void SetPixel( int x, int y, COLORREF color ) throw ();
    void Line( int x1, int y1, int x2, int y2, COLORREF c ) throw ();

  private: // Methods
    unsigned char* Addr( int x, int y ) throw ();

    void Line16( int x1, int y1, int x2, int y2, COLORREF c ) throw ();
    void Line24( int x1, int y1, int x2, int y2, COLORREF c ) throw ();
    void Line32( int x1, int y1, int x2, int y2, COLORREF c ) throw ();

  private: // Data members
    DrawSurface&    _surf;
    unsigned char*  _buf;
    int             _pitch;
    DrawPixelFormat _format;
};

class DrawPrimarySurface : public DrawSurface
{
  public:
    DrawPrimarySurface( void ) throw ();
    DrawPrimarySurface( Draw& draw ) throw (XDraw);

    void Create( Draw& draw ) throw (XDraw);

    int Width( void ) const throw ();
    int Height( void ) const throw ();
    int Pitch( void ) const throw ();

  private:
    int _w;
    int _h;
    int _pitch;
};

class DrawOffScreenSurface: public DrawSurface
{
  public:
    DrawOffScreenSurface( void ) throw ();
    DrawOffScreenSurface( Draw& draw, int width, int height ) throw (XDraw);
    DrawOffScreenSurface( Draw& draw, HINSTANCE hInst, int resId ) throw (XDraw);

    void Create( Draw& draw, int width, int height ) throw (XDraw);
    void LoadBitmap( Draw& draw, HINSTANCE hInst, int resId ) throw (XDraw);

    int Width( void ) const throw ();
    int Height( void ) const throw ();
    int Pitch( void ) const throw ();

  private:
    int _w;
    int _h;
    int _pitch;
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ~~~ IMPLEMENTATTION ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//
// Class IFaxe
// ~~~~~~~~~~~
template <class I>
IFace<I>::IFace( IFace<I>& face )
: _i(face._i)
{ 
  if ( _i != 0 ) _i->AddRef();
}

template <class I>
IFace<I>::~IFace( void )
{ 
  if ( _i != 0 ) _i->Release();
}

template <class I>
IFace<I>& IFace<I>::operator=( IFace<I>& face )
{
  if ( this != &face )
  {
    if ( _i != 0 ) _i->Release();
    _i = face._i;
    if ( _i != 0 ) _i->AddRef();
  }
  return *this;
}

template <class I>
I* IFace<I>::operator->( void ) { return _i; }

template <class I>
IFace<I>::operator I*( void ) { return _i; }

template <class I>  
IFace<I>::IFace( void )
: _i(0)
{}

//
// Class DrawSurfaceDesc
// ~~~~~~~~~~~~~~~~~~~~~
inline DrawSurfaceDesc::DrawSurfaceDesc( void )
{ 
  ::ZeroMemory(this, sizeof(DDSURFACEDESC2));
  dwSize = sizeof(DDSURFACEDESC2);
}

inline void DrawSurfaceDesc::SetSize( long w, long h )
{ 
  dwFlags |= DDSD_HEIGHT|DDSD_WIDTH;
  dwHeight = h;
  dwWidth = w;
}

inline void DrawSurfaceDesc::SetCaps( unsigned long caps )
{ 
  dwFlags |= DDSD_CAPS;
  ddsCaps.dwCaps |= caps;
}

inline void* DrawSurfaceDesc::Buffer( void ) { return lpSurface; }
inline long DrawSurfaceDesc::Width( void ) const { return dwWidth; }
inline long DrawSurfaceDesc::Height( void ) const { return dwHeight; }
inline long DrawSurfaceDesc::Pitch( void ) const { return lPitch; }
inline DDPIXELFORMAT DrawSurfaceDesc::PixelFormat( void ) const { return ddpfPixelFormat; }

//
// Class Draw
// ~~~~~~~~~~
inline Draw::Draw( void ) throw () {}

inline HRESULT Draw::Create( void ) throw ()
{ 
  assert(_i == 0);
  HRESULT hr = ::DirectDrawCreateEx(0, (void**)&_i, IID_IDirectDraw7, 0);

#ifdef LOGGING
/*
  if ( hr == DD_OK )
  { deblog(_T("Win::Direct::Draw::Create: succeeded"));
  }
  else
  { XDraw e(hr, "Win::Direct::Draw::Create: failed");
    deblog(e.Message());
  }
*/
#endif

  return hr;
}

inline void Draw::CreateX( void ) throw (XDraw)
{
  HRESULT hr = Create();
  if ( hr != DD_OK )
  { throw XDraw(hr, _T("Win::Direct::Draw::CreateX: cannot create DirectDraw7 object"));
  }
}

inline void Draw::SetCoopNormal( HWND hwnd ) throw (XDraw)
{
  assert(_i != 0);
  HRESULT hr = _i->SetCooperativeLevel(hwnd, DDSCL_NORMAL);

#ifdef LOGGING
/*
  if ( hr == DD_OK )
  { deblog(_T("Win::Direct::Draw::SetCoopNormal: succeeded"));
  }
  else
  { XDraw e(hr, "Win::Direct::Draw::SetCoopNormal: failed");
    deblog(e.Message());
  }
*/
#endif

  if ( hr != DD_OK )
  { throw XDraw(hr, _T("Win::Direct::Draw::SetCoopNormal: cannot set normal cooperative level"));
  }
}

inline void Draw::SetCoopFullScreen( HWND hwnd ) throw (XDraw)
{ 
  assert(_i != 0);
  HRESULT hr = _i->SetCooperativeLevel(hwnd, DDSCL_EXCLUSIVE|DDSCL_FULLSCREEN|DDSCL_ALLOWREBOOT);

#ifdef LOGGING
/*
  if ( hr == DD_OK )
  { deblog(_T("Win::Direct::Draw::SetCoopFullScreen: succeeded"));
  }
  else
  { XDraw e(hr, "Win::Direct::Draw::SetCoopFullScreen: failed");
    deblog(e.Message());
  }
*/
#endif

  if ( hr != DD_OK )
  { throw XDraw(hr, _T("Win::Direct::Draw::SetCoopFullScreen: cannot set exclusive full-screen cooperative level"));
  }
}

inline void Draw::GetDisplayMode( DrawSurfaceDesc& desc ) throw (XDraw)
{ 
  HRESULT hr = _i->GetDisplayMode(&desc);

#ifdef LOGGING
/*
  if ( hr == DD_OK )
  { deblog(_T("Win::Direct::Draw::GetDisplayMode: succeeded"));
  }
  else
  { XDraw e(hr, "Win::Direct::Draw::GetDisplayMode: failed");
    deblog(e.Message());
  }
*/
#endif

  if ( hr != DD_OK )
  { throw XDraw(hr, _T("Win::Direct::Draw::GetDisplayMode: cannot obtain display mode information"));
  }
}

inline HRESULT Draw::SetDisplayMode( DWORD w, DWORD h, DWORD bpp, DWORD refreshRate ) throw ()
{ 
  assert(_i != 0);
  HRESULT hr = _i->SetDisplayMode(w, h, bpp, refreshRate, 0);

#ifdef LOGGING
/*
  if ( hr == DD_OK )
  { deblog(_T("Win::Direct::Draw::SetDisplayMode: succeeded"));
  }
  else
  { XDraw e(hr, "Win::Direct::Draw::SetDisplayMode: failed");
    deblog(e.Message());
  }
*/
#endif
  
  return hr;
}

inline void Draw::SetDisplayModeX( DWORD w, DWORD h, DWORD bpp, DWORD refreshRate ) throw (XDraw)
{ 
  HRESULT hr = SetDisplayMode(w, h, bpp, refreshRate);
  if ( hr != DD_OK )
  { throw XDraw(hr, _T("Win::Direct::Draw::SetDisplayModeX: cannot set requested display mode"));
  }
}

//
// Class DrawClipper
// ~~~~~~~~~~~~~~~~~
inline DrawClipper::DrawClipper( Draw& draw ) throw (XDraw)
{ 
  assert(draw != 0);
  HRESULT hr = draw->CreateClipper(0, &_i, 0);
  if ( hr != DD_OK )
  { throw XDraw(hr, _T("Win::Direct::DrawClipper: cannot create clipper"));
  }
}

inline void DrawClipper::SetWin( HWND hwnd ) throw (XDraw)
{ 
  assert(_i != 0);
  HRESULT hr = _i->SetHWnd(0, hwnd);
  if ( hr != DD_OK )
  { throw XDraw(hr, _T("Win::Direct::DrawClipper: cannot set window handle to the clipper object"));
  }
}

//
// Class DrawSurface
// ~~~~~~~~~~~~~~~~~
inline DrawSurface::DrawSurface( void ) throw () {}

inline void DrawSurface::GetDesc( DrawSurfaceDesc& desc ) throw (XDraw)
{
  assert(_i != 0);
  HRESULT hr = _i->GetSurfaceDesc(&desc);
  if ( hr != DD_OK )
  { throw XDraw(hr, _T("Win::Direct::DrawSurface::GetDesc: cannot get surface description"));
  }
}

inline void DrawSurface::SetClipper( DrawClipper& clipper ) throw (XDraw)
{
  assert(_i != 0);
  HRESULT hr = _i->SetClipper(clipper);
  if ( hr != DD_OK )
  { throw XDraw(hr, _T("Win::Direct::DrawSurface::SetClipper: cannot set clipper"));
  }
}

inline void DrawSurface::BltFrom( DrawSurface& src, RECT* dstRect, RECT* srcRect, DWORD flags ) throw (XDraw)
{
  assert(_i != 0);
  HRESULT hr = _i->Blt(dstRect, src._i, srcRect, DDBLT_WAIT|flags, 0);
  if ( hr != DD_OK )
  { throw XDraw(hr, _T("Win::Direct::DrawSurface::BltFrom: cannot perform bit block tranfer operation"));
  }
}

inline void DrawSurface::Fill( COLORREF c, RECT* dstRect ) throw (XDraw)
{ 
  assert(_i != 0);
  DrawSurfaceDesc desc;
  GetDesc(desc);
  DrawPixelFormat format(desc);
  DDBLTFX fx;
  fx.dwSize = sizeof(fx);
  fx.dwFillColor = format.ColorValue(c);
  HRESULT hr = _i->Blt(dstRect, 0, 0, DDBLT_WAIT|DDBLT_COLORFILL, &fx);
  if ( hr != DD_OK )
  { throw XDraw(hr, _T("Win::Direct::DrawSurface::Fill: cannot perform bit block tranfer operation"));
  }
}

inline void DrawSurface::Lock( DrawSurfaceDesc& desc ) throw (XDraw)
{ 
  HRESULT hr = _i->Lock(0, &desc, DDLOCK_WAIT, 0);
  if ( hr != DD_OK )
  { throw XDraw(hr, _T("Win::Direct::DrawSurface::Lock: cannot lock surface"));
  }
}

inline void DrawSurface::Unlock( void ) throw () { _i->Unlock(0); }

//
// Class DrawPixelFormat
// ~~~~~~~~~~~~~~~~~~~~~
inline DrawPixelFormat::DrawPixelFormat( void ) throw () {}

inline DrawPixelFormat::DrawPixelFormat( DrawSurfaceDesc& desc ) throw (XDraw)
{ 
  Init(desc);
}

inline void DrawPixelFormat::Init( DrawSurfaceDesc& desc ) throw (XDraw)
{ 
  DDPIXELFORMAT format = desc.PixelFormat();
  if ( format.dwFlags != DDPF_RGB )
  { throw XDraw(DD_OK, _T("Win::Direct::DrawPixelFormat: non-RGB formats not supported"));
  }
  _bpp = format.dwRGBBitCount;
  _redMask = format.dwRBitMask;
  _greenMask = format.dwGBitMask;
  _blueMask = format.dwBBitMask;
  _mask = ~(_redMask|_greenMask|_blueMask);
  switch (_bpp)
  {
    case 16:
      _redShift   = HiBitShift(_redMask) + 8;
      _greenShift = HiBitShift(_greenMask) + 8;
      _blueShift  = HiBitShift(_blueMask) + 8;
      break;
    
    case 24:
    case 32:
      break;
    
    default:
      throw XDraw(DD_OK, _T("Win::Direct::DrawPixelFormat::Init: only 16, 24 and 32 bit graphics supported"));
  }
}

inline int DrawPixelFormat::Bpp( void ) const  throw () { return _bpp; }

inline unsigned long DrawPixelFormat::Mask( void ) const  throw () { return _mask; }

inline unsigned long DrawPixelFormat::ColorValue16( COLORREF color )  throw ()
{
  return ( (GetRValue(color) << _redShift) & (_redMask << 16)
         | (GetGValue(color) << _greenShift) & (_greenMask << 16)
         | (GetBValue(color) << _blueShift) & (_blueMask << 16)
         ) >> 16;
}

inline unsigned long DrawPixelFormat::ColorValue24( COLORREF color )  throw ()
{
  return (GetRValue(color) << 16) & _redMask
       | (GetGValue(color) << 8) & _greenMask
       | GetBValue(color) & _blueMask;
}

inline unsigned long DrawPixelFormat::ColorValue32 (COLORREF color)  throw ()
{
  return (GetRValue(color) << 16) & _redMask
       | (GetGValue(color) << 8) & _greenMask
       | GetBValue(color) & _blueMask;
}

inline unsigned long DrawPixelFormat::ColorValue( COLORREF color )  throw ()
{
  switch ( _bpp )
  {
    case 16: return ColorValue16(color);
    case 24: return ColorValue24(color);
    case 32: return ColorValue32(color);
    default: assert(0); return 0;
  }
}

inline unsigned DrawPixelFormat::HiBitShift( unsigned val )
{ 
  unsigned i = 0;
  while ( val != 0 )
  { val >>= 1;
    ++i;
  }
  return i;
}

//
// Class DrawCanvas
// ~~~~~~~~~~~~~~~~
inline DrawCanvas::DrawCanvas( DrawSurface& surf ) throw (XDraw)
: Canvas(0),
  _surf(surf)
{ 
  assert(_surf != 0);
  HRESULT hr;
  do
  { hr = _surf->GetDC(&_hdc);
  } while ( hr == DDERR_WASSTILLDRAWING );
  if ( hr != DD_OK )
  { throw XDraw(hr, _T("Win::Direct::DrawCanvas: cannot obtain device context for a surface"));
  }
}

inline DrawCanvas::~DrawCanvas( void )
{
  _surf->ReleaseDC(_hdc);
}

//
// Class DrawSurfaceBuffer
// ~~~~~~~~~~~~~~~~~~~~~~~
inline DrawSurfaceBuffer::DrawSurfaceBuffer( DrawSurface& surf ) throw (XDraw)
: _surf(surf)
{
  DrawSurfaceDesc desc;
  _surf.Lock(desc);
  _pitch = desc.Pitch();
  _buf = static_cast<unsigned char *> (desc.Buffer ());
  _format.Init(desc);
  int bpp = _format.Bpp();
  if ( bpp != 16 && bpp != 24 && bpp != 32 )
  { _surf.Unlock();
    throw XDraw(DD_OK, _T("Win::Direct::DrawSurfaceBuffer: only high color and true color supported"));
  }
}

inline DrawSurfaceBuffer::~DrawSurfaceBuffer( void )
{
  _surf.Unlock();
}

inline void DrawSurfaceBuffer::SetPixel( int x, int y, COLORREF color ) throw ()
{
  switch ( _format.Bpp() )
  {
    case 16:
      {  int offset = y * _pitch + x * 2;
         unsigned short* p = reinterpret_cast<unsigned short*>(_buf + offset);
         *p &= _format.Mask();
         *p |= static_cast<unsigned short>(_format.ColorValue16(color));
      }
      break;
    
    case 24:
      { int offset = y * _pitch + x * 3;
        unsigned long* p = reinterpret_cast<unsigned long*>(_buf + offset);
        *p &= _format.Mask();
        *p |= _format.ColorValue24(color);
      }
      break;
  
    case 32:
      { int offset = y * _pitch + x * 4;
        unsigned long* p = reinterpret_cast<unsigned long*>(_buf + offset);
        *p &= _format.Mask();
        *p |= _format.ColorValue32(color);
      }
      break;
  }
}

inline unsigned char* DrawSurfaceBuffer::Addr( int x, int y ) throw ()
{ 
  unsigned long offset = 0;
  switch ( _format.Bpp() )
  {
    case 16: offset = y * _pitch + x * 2;
    case 24: offset = y * _pitch + x * 3;
    case 32: offset = y * _pitch + x * 4;
  }
  return &_buf[offset];
}

//
// Class DrawPrimatySurface
// ~~~~~~~~~~~~~~~~~~~~~~~~
inline DrawPrimarySurface::DrawPrimarySurface( void ) throw ()
: _w(0),
  _h(0),
  _pitch(0)
{}

inline DrawPrimarySurface::DrawPrimarySurface( Draw& draw ) throw (XDraw)
{ 
  Create(draw); 
}

inline void DrawPrimarySurface::Create( Draw& draw ) throw (XDraw)
{
  assert(_i == 0);
  assert(draw != 0);
  DrawSurfaceDesc desc;
  desc.SetCaps(DDSCAPS_PRIMARYSURFACE);
  HRESULT hr = draw->CreateSurface(&desc, &_i, 0);
  if ( hr != DD_OK )
  { throw XDraw(hr, _T("Win::Direct::DrawPrimarySurface::Create: cannot create primary surface"));
  }
  GetDesc(desc);
  _w = desc.Width();
  _h = desc.Height();
  _pitch = desc.Pitch();
}

inline int DrawPrimarySurface::Width( void ) const throw () { return _w; }
inline int DrawPrimarySurface::Height( void ) const throw () { return _h; }
inline int DrawPrimarySurface::Pitch( void ) const throw () { return _pitch; }

//
// Class DrawOffScreenSurface
// ~~~~~~~~~~~~~~~~~~~~~~~~~~
inline DrawOffScreenSurface::DrawOffScreenSurface( void ) throw ()
: _w(0),
  _h(0),
  _pitch(0)
{}
    
inline DrawOffScreenSurface::DrawOffScreenSurface( Draw& draw, int width, int height ) throw (XDraw)
{ 
  Create(draw, width, height); 
}

inline DrawOffScreenSurface::DrawOffScreenSurface( Draw& draw, HINSTANCE hInst, int resId ) throw (XDraw)
{ 
  LoadBitmap(draw, hInst, resId);
}

inline void DrawOffScreenSurface::Create( Draw& draw, int width, int height ) throw (XDraw)
{
  assert(draw != 0);
  if ( _i != 0 ) _i->Release();

  DrawSurfaceDesc desc;
  desc.SetCaps(DDSCAPS_OFFSCREENPLAIN);
  desc.SetSize(width, height);
  HRESULT hr = draw->CreateSurface(&desc, &_i, 0);
  if ( hr != DD_OK )
  { throw XDraw(hr, _T("Win::Direct::DrawOffScreenSurface::Create: cannot create off-screen surface"));
  }
  GetDesc(desc);
  _w = desc.Width();
  _h = desc.Height();
  _pitch = desc.Pitch();
}

inline void DrawOffScreenSurface::LoadBitmap( Draw& draw, HINSTANCE hInst, int resId ) throw (XDraw)
{ 
  Win::DisplayBitmap bitmap;
  bitmap.Load(hInst, resId);
  Create(draw, bitmap.Width(), bitmap.Height());
  DrawCanvas canvas(*this);
  Blitter blitter(canvas, 0, 0, bitmap.Width(), bitmap.Height());
  blitter.BlitFrom(Win::BitmapCanvas(canvas, bitmap));
}

inline int DrawOffScreenSurface::Width( void ) const throw () { return _w; }
inline int DrawOffScreenSurface::Height( void ) const throw () { return _h; }
inline int DrawOffScreenSurface::Pitch( void ) const throw () { return _pitch; }


} // namespace Direct
} // namespace Win

#endif // WINDIRECTDRAW_H