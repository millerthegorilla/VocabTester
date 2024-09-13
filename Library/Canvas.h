#ifndef WINCANVAS_H
#define WINCANVAS_H

#include <windows.h>
#include <tchar.h>
#include <cassert>
#include "stdstring"

namespace Win
{

class Canvas
{
  public:
    operator HDC( void ) 
    { return _hdc; 
    }

    void DrawIcon( int x, int y, HICON hIcon )
    { ::DrawIcon(_hdc, x, y, hIcon);
    }

    // Point
    void SetPixel( int x, int y, COLORREF color = 0x00000000 )
    { ::SetPixel(_hdc, x, y, color);	
    }

    COLORREF GetPixel( int x, int y )
    { return ::GetPixel(_hdc, x, y);
    }

    // Line
    void LineTo( int x, int y )
    { ::LineTo(_hdc, x, y); 
    }

    void MoveTo( int x, int y )
    { ::MoveToEx(_hdc, x, y, 0); 
    }

    void Line( int x1, int y1, int x2, int y2 )
    { MoveTo(x1, y1);
      LineTo(x2, y2);
    }

    // Text
    void Text( int x, int y, TCHAR const buf[], int count )
    { ::TextOut(_hdc, x, y, buf, count); 
    }

    void Text( int x, int y, TCHAR const buf[] )
    { Text(x, y, buf, ::_tcslen(buf));
    }

    void Text( int x, int y, stdstring const& text )
    { Text(x, y, text.c_str(), text.length());
    }

    void TabbedText( int x, int y, TCHAR const buf[], int count )
    { ::TabbedTextOut(_hdc, x, y, buf, count, 0, 0, 0); 
    }

    void TabbedText( int x, int y, TCHAR const buf[], int count, int tabStopPositions[], int nTabPositions )
    { ::TabbedTextOut(_hdc, x, y, buf, count, nTabPositions, tabStopPositions, 0);     }

    void TabbedText( int x, int y, TCHAR const buf[] )
    { TabbedText(x, y, buf, ::_tcslen(buf));
    }

    void TabbedText( int x, int y, TCHAR const buf[], int tabStopPositions[], int nTabPositions )
    { TabbedText(x, y, buf, ::_tcslen(buf), tabStopPositions, nTabPositions);
    }

    void GetCharSize( SIZE& size )
    { TEXTMETRIC tm;
      ::GetTextMetrics(_hdc, &tm);
      size.cx = tm.tmAveCharWidth;
      size.cy = tm.tmHeight + tm.tmExternalLeading;
    }

    void GetTextSize( TCHAR const text[], int textLen, SIZE& size )
    { ::GetTextExtentPoint32(_hdc, text, textLen, &size);
    }

    void GetTextSize( TCHAR const text[], SIZE& size )
    { GetTextSize(text, ::_tcslen(text), size);
    }

    void GetTextSize( stdstring const& text, SIZE& size )
    { GetTextSize(text.c_str(), text.length(), size);
    }

    // Rectangle
    void WhiteWash( RECT const& rect )
    { ::PatBlt(_hdc, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, WHITENESS); 
    }

    void BlackWash( RECT const& rect )
    { ::PatBlt(_hdc, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, BLACKNESS); 
    }

    void FilledRect( RECT const& rect )
	  { ::PatBlt(_hdc, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, PATCOPY); 
    }

    void BoundedFilledRect( RECT const& rect )
    { ::Rectangle(_hdc, rect.left, rect.top, rect.right, rect.bottom);
    }

    void Rect( RECT const& rect )
    { POINT pts[5] = 
      {
        { rect.left,      rect.top        },
        { rect.right - 1, rect.top        },
        { rect.right - 1, rect.bottom - 1 },
        { rect.left,      rect.bottom - 1 },
        { rect.left,      rect.top        }
      };
      ::Polyline(_hdc, pts, 5);
    }

    // Circle
    void Circle( int x, int y, int r )
    { ::Arc(_hdc, x - r, y - r, x + r, y + r, x - r, y - r, x - r, y - r);
    }

    void FilledCircle( int x, int y, int r )
    { ::Ellipse(_hdc, x - r, y - r, x + r, y + r);
    }

    // Map Modes
    void SetMapModeText( void )
    { ::SetMapMode(_hdc, MM_TEXT); 
    }

    // Filling
    void FloodFillBorder( int xStart, int yStart, COLORREF color )
    { ::ExtFloodFill(_hdc, xStart, yStart, color, FLOODFILLBORDER); 
    }

    void FloodFillSurface( int xStart, int yStart, COLORREF color )
    { ::ExtFloodFill(_hdc, xStart, yStart, color, FLOODFILLSURFACE);
    }

    // Paths
    void FillPath( void )
    { ::FillPath(_hdc); 
    }

    void StrokePath( void )
    { ::StrokePath(_hdc); 
    }

    void StrokeAndFillPath( void )
    { ::StrokeAndFillPath(_hdc); 
    }
    
    // Edge
    void DrawEdge( RECT& r, UINT edge, UINT grfFlags )
    { ::DrawEdge(_hdc, &r, edge, grfFlags); 
    }

  protected: // Methods
    // only derived classes are usable
    Canvas( HDC hdc )
    : _hdc(hdc) 
    {}

  protected: // Data members
    HDC  _hdc;
};

class Blitter
{
  public:
    Blitter( HDC dest, int x, int y, int w, int h )
    : _dest(dest),
      _x(x),
      _y(y),
      _w(w),
      _h(h)
    {}

    void BlitFrom( HDC src, int mode = SRCCOPY, int x = 0 , int y = 0 )
    { ::BitBlt(_dest, _x, _y, _w, _h, src, x, y, mode);
    }

  private:
    HDC _dest;
    int _x;
    int _y;
    int _w;
    int _h;
};

class StretchBlitter
{
  public:
    StretchBlitter( HDC dest, int x, int y, int w, int h )
    : _dest(dest),
      _x(x),
      _y(y),
      _w(w),
      _h(h)
    {}

    void BlitFrom( HDC src, int x, int y, int w, int h, int mode = SRCCOPY )
    { ::StretchBlt(_dest, _x, _y, _w, _h, src, x, y, w, h, mode);
    }

  private:
    HDC _dest;
    int _x;
    int _y;
    int _w;
    int _h;
};


// Use for painting after WM_PAINT
class PaintCanvas : public Canvas
{
  public:
    PaintCanvas( HWND hwnd )
    : Canvas(::BeginPaint(hwnd, &_paint)),
		  _hwnd(hwnd)
    {}

    ~PaintCanvas( void )
    { ::EndPaint(_hwnd, &_paint); 
    }

    int Top( void ) const { return _paint.rcPaint.top; }
    int Bottom( void ) const { return _paint.rcPaint.bottom; }
    int Left( void ) const { return _paint.rcPaint.left; }
    int Right( void ) const { return _paint.rcPaint.right; }

	protected:             
    PAINTSTRUCT _paint;
    HWND        _hwnd;
};


// Device Context
// Use for painting other than WM_PAINT
class UpdateCanvas : public Canvas
{
  public:
    UpdateCanvas( HWND hwnd )
    : Canvas(::GetDC(hwnd)),
      _hwnd(hwnd)
    {}
    
    ~UpdateCanvas( void )
    { ::ReleaseDC(_hwnd, _hdc); 
    }

  protected:
    HWND _hwnd;
};


// Use when no window available, or to draw on the display
class DisplayCanvas : public Canvas
{
  public:
    DisplayCanvas( void )
    : Canvas(::CreateDC(TEXT("DISPLAY"), 0, 0, 0))
    {}

    ~DisplayCanvas( void )
    { ::DeleteDC(_hdc); 
    }
};

// Use this to paint to the non-client area
class DowCanvas : public Canvas
{
  public:
    DowCanvas( HWND hwnd )
	  : Canvas(::GetWindowDC(hwnd)),
 		  _hwnd(hwnd)
	  {}

	 ~DowCanvas( void )
	 { ::ReleaseDC(_hwnd, _hdc); 
   }

  protected:
    HWND _hwnd;
};


class MemCanvas : public Canvas
{
  public:
    MemCanvas( HDC hdc )
    : Canvas(::CreateCompatibleDC(hdc))
    {}

    ~MemCanvas( void )
    { ::DeleteDC(_hdc);
    }
};


class BitmapCanvas : public MemCanvas
{
  public:
    BitmapCanvas( HDC hdc, HBITMAP hBitmap )
    : MemCanvas(hdc)
    {
		  // Convert bitmap to format compatible with canvas
      _hOldBitmap = reinterpret_cast<HBITMAP>(::SelectObject(_hdc, hBitmap));
    }

   ~BitmapCanvas( void )
   { ::SelectObject(_hdc, _hOldBitmap); 
   }

  private:
    HBITMAP _hOldBitmap;
};

class StrangerCanvas : public Canvas
{
  public:
    StrangerCanvas( HDC hdc )
    : Canvas(hdc)
    {}
};

class Emf
{
  public:
    Emf( void )
    : _hEmf(0)
    {}

    Emf( RECT const& r )
    : _r(r),
      _hEmf(0)
    {}

    Emf( Emf const& b )
    : _r(b._r),
      _hEmf(0)
    { if ( b._hEmf != 0 )
      { _hEmf = ::CopyEnhMetaFile(b._hEmf, 0);
      }
    }

    ~Emf( void )
    { Destroy();
    }

    Emf& operator=( Emf const& b )
    { if ( this != &b )
      { Destroy();
        _r = b._r;
        if ( b._hEmf != 0 )
        { _hEmf = ::CopyEnhMetaFile(b._hEmf, 0);
        }
      }
    }

    void SetRect( RECT const& r ) { _r = r; }
    RECT const& GetRect( void ) const { return _r; }
    
    void Destroy( void )
    { if ( _hEmf != 0 )
      { ::DeleteEnhMetaFile(_hEmf);
        _hEmf = 0;
      }
    }

    void Play( HDC hDc, RECT const& r ) const
    { assert(_hEmf != 0);
      ::PlayEnhMetaFile(hDc, _hEmf, &r);
    }

  private: // Methods
    void SetEmf( HENHMETAFILE hEmf )
    { Destroy();
      _hEmf = hEmf;
    }

  private: // Data members
    RECT         _r;
    HENHMETAFILE _hEmf;

  // Friends
  friend class EmfCanvas;
};

class EmfCanvas : public Canvas
{
  public:
    EmfCanvas( Emf& emf )
    : Canvas(::CreateEnhMetaFile(0, 0, &emf.GetRect(), 0)),
      _emf(emf)
    {}

    ~EmfCanvas( void )
    { _emf.SetEmf(::CloseEnhMetaFile(_hdc));
    }

  private:
    Emf& _emf;
};

} // namespace Win

#endif
