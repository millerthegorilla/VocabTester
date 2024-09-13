#ifndef WINTOOLS_H
#define WINTOOLS_H

#include <windows.h>

namespace Win
{

class StockObject
{
  public:
    StockObject( int type )
    : _obj(::GetStockObject(type))
    {}
		
    operator HGDIOBJ( void ) const
    { return _obj; 
    }

  protected:
    HGDIOBJ _obj;
};


class StockObjectHolder
{
  public:
    StockObjectHolder( HDC hdc, int type )
    : _hdc(hdc)
    { _hObjOld = ::SelectObject(_hdc, StockObject(type)); 
    }

    ~StockObjectHolder( void )
    { ::SelectObject(_hdc, _hObjOld); 
    }

  private:
    HGDIOBJ  _hObjOld;
    HDC      _hdc;
};

namespace Pen
{

class Stock : public Win::StockObject
{
  public:
    Stock( int type )
    : Win::StockObject(type)
    {}

    operator HPEN( void ) const
    { return reinterpret_cast<HPEN>(_obj); 
    }
};

class Color
{
  public:
    Color( void )
    : _hPen(0)
    {}

    Color( COLORREF color, int style = PS_SOLID )
    : _hPen(::CreatePen(style, 0, color))
    {}

    Color( Color& b )
    : _hPen(b.Release())
    {}

    ~Color( void )
    { Delete();
    }

    Color& operator=( Color& b )
    { if ( this != &b )
      { Delete();
        _hPen = b.Release();
      }
      return *this;
    }

    operator HPEN( void ) const
    { return _hPen; 
    }

    HPEN Release( void )
    { HPEN hTemp = _hPen;
      _hPen = 0;
      return hTemp;
    }

    void Delete( void )
    { if ( _hPen != 0 )
      { ::DeleteObject(_hPen);
      }
      _hPen = 0;
    }

  private:
    HPEN    _hPen;
};


class Holder
{
  public:
    Holder( HDC hdc, HPEN hPen )
    : _hdc(hdc)
    {	_hPenOld = reinterpret_cast<HPEN>(::SelectObject(_hdc, hPen)); 
    }

    ~Holder( void )
    { ::SelectObject(_hdc, _hPenOld); 
    }

  private:
    HDC     _hdc;
    HPEN    _hPenOld;

  public:
    // specialized classes
    class White : public Win::StockObjectHolder
    {
      public:
        White( HDC hdc )
        : Win::StockObjectHolder(hdc, WHITE_PEN)
        {}
    };

    class Black : public Win::StockObjectHolder
    {
      public:
        Black( HDC hdc )
        : Win::StockObjectHolder(hdc, BLACK_PEN)
        {}
    };

    class Instant
    {
      public:
        Instant( HDC hdc, COLORREF color )
        : _hdc(hdc)
        { _hPen = ::CreatePen(PS_SOLID, 0, color);
          _hPenOld = reinterpret_cast<HPEN>(::SelectObject(_hdc, _hPen));
        }

        ~Instant( void )
        { ::SelectObject(_hdc, _hPenOld);
          ::DeleteObject(_hPen);
        }

      private:
        HDC     _hdc;
        HPEN    _hPen;
        HPEN    _hPenOld;
    };
};

} // namespace Pen

namespace Brush
{
	
class Stock : public Win::StockObject
{
  public:
    Stock( int type ) 
    : Win::StockObject(type)
    {}

		operator HBRUSH( void ) const
    { return reinterpret_cast<HBRUSH>(_obj); 
    }
};


class Color
{
  public:
    Color( void )
    : _hBrush(0)
    {}

    Color( COLORREF color, int style = PS_SOLID )
    : _hBrush(::CreateSolidBrush(color))
    {}

    Color( Color& b )
    : _hBrush(b.Release())
    {}

    ~Color( void )
    { Delete();
    }

    Color& operator=( Color& b )
    { if ( this != &b )
      { Delete();
        _hBrush = b.Release();
      }
      return *this;
    }

    operator HBRUSH( void ) const
    { return _hBrush; 
    }

    HBRUSH Release( void )
    { HBRUSH hTemp = _hBrush;
      _hBrush = 0;
      return hTemp;
    }

    void Delete( void )
    { if ( _hBrush != 0 )
      { ::DeleteObject(_hBrush);
      }
      _hBrush = 0;
    }

  private:
    HBRUSH  _hBrush;
};


class Holder
{
  public:
    Holder( HDC hdc, HBRUSH hBrush )
    : _hdc (hdc)
    { _hBrushOld = reinterpret_cast<HBRUSH>(::SelectObject(_hdc, hBrush)); 
    }
		
    ~Holder( void )
    { ::SelectObject(_hdc, _hBrushOld); 
    }

  private:
    HDC     _hdc;
    HBRUSH  _hBrushOld;
	
  public:
    // specialized classes
    class White : public Win::StockObjectHolder
    {
      public:
        White( HDC hdc )
        : Win::StockObjectHolder(hdc, WHITE_BRUSH)
        {}
    };

    class Black : public Win::StockObjectHolder
    {
      public:
        Black( HDC hdc )
        : Win::StockObjectHolder(hdc, BLACK_BRUSH)
        {}
    };

    class Instant
    {
      public:
        Instant( HDC hdc, COLORREF color )
        : _hdc (hdc)
        { _hBrush = ::CreateSolidBrush(color);
          _hBrushOld = reinterpret_cast<HBRUSH>(::SelectObject(_hdc, _hBrush));
        }
			
        ~Instant( void )
        { ::SelectObject(_hdc, _hBrushOld);
          ::DeleteObject(_hBrush);
        }
		
      private:
        HDC     _hdc;
        HBRUSH  _hBrush;
        HBRUSH  _hBrushOld;
    };
};

} // namespace Brush

namespace Font
{

class Stock : public Win::StockObject
{
  public:
    Stock( int type )
    : Win::StockObject(type) 
    {}

    operator HFONT( void ) const
    { return reinterpret_cast<HFONT>(_obj); 
    }

    void GetTextSize( int &width, int &height );
};

class SysFixed : public Stock
{
  public:
    SysFixed( void )
    : Stock(SYSTEM_FIXED_FONT)
    {}
};

class OemFixed : public Stock
{
  public:
    OemFixed( void ) 
    : Stock(OEM_FIXED_FONT) 
    {}
};

class Descr
{
  public:
    Descr( TCHAR const name[], int height )
    : _name(name),
      _height(height),
      _width(0),
      _escapement(0),
      _orientation(0),
      _weight(FW_NORMAL),
      _italic(FALSE),
      _underline(FALSE),
      _strikeOut(FALSE),
      _charSet(DEFAULT_CHARSET),
      _outputPrecision(OUT_DEFAULT_PRECIS),
      _clipPrecision(CLIP_DEFAULT_PRECIS),
      _quality(DEFAULT_QUALITY),
      _pitchAndFamily(DEFAULT_PITCH|FF_DONTCARE)
    {}

    void MakeMedium( void ) { _weight = FW_MEDIUM; }
    void MakeSemiBold( void ) { _weight = FW_SEMIBOLD; }
    void MakeBold( void ) { _weight = FW_BOLD; }

  private:
    TCHAR const* _name;
    int          _height;
    int          _width;
    int          _escapement;
    int          _orientation;
    int          _weight;
    DWORD        _italic;
    DWORD        _underline;
    DWORD        _strikeOut;
    DWORD        _charSet;
    DWORD        _outputPrecision;
    DWORD        _clipPrecision;
    DWORD        _quality;
    DWORD        _pitchAndFamily;

  friend class Logical;
};

class Logical
{
  public:
    Logical( void )
    : _hFont(0)
    {}

    Logical( Descr const& descr )
    : _hFont(::CreateFont( descr._height,
                           descr._width,
                           descr._escapement,
                           descr._orientation,
                           descr._weight,
                           descr._italic,
                           descr._underline,
                           descr._strikeOut,
                           descr._charSet,
                           descr._outputPrecision,
                           descr._clipPrecision,
                           descr._quality,
                           descr._pitchAndFamily,
                           descr._name ))
    {}

    void Create( Descr const& descr )
    { _hFont = ::CreateFont( descr._height, 
                             descr._width,
                             descr._escapement,
                             descr._orientation,
                             descr._weight,
                             descr._italic,
                             descr._underline,
                             descr._strikeOut,
                             descr._charSet,
                             descr._outputPrecision,
                             descr._clipPrecision,
                             descr._quality,
                             descr._pitchAndFamily,
                             descr._name );
    }

    ~Logical( void )
    { if ( _hFont != 0 )
      { ::DeleteObject(_hFont); 
      }
    }

    operator HFONT( void ) const
    { return _hFont; 
    } 

  private:
    HFONT _hFont;
};

class Holder
{
  public:
    Holder( HDC hdc, HFONT font )
    : _hdc (hdc)
    { _fontOld = reinterpret_cast<HFONT>(::SelectObject(_hdc, font)); 
    }
		
    ~Holder( void )
    { ::SelectObject(_hdc, _fontOld); 
    }

  private:
    HDC     _hdc;
    HFONT   _fontOld;
	
  public:
    class Color
    {
      public:
        Color( HDC hdc, COLORREF color )
        : _hdc(hdc), 
          _oldColor(::SetTextColor(_hdc, color))
        {}

        ~Color( void )
        { ::SetTextColor(_hdc, _oldColor); 
        }
      
      private:
        HDC      _hdc;
        COLORREF _oldColor;
    };

    class Background
    {
      public:
        Background( HDC hdc, COLORREF color )
        : _hdc(hdc),
          _oldColor(::SetBkColor(_hdc, color))
        {}
        
        ~Background( void )
        { ::SetBkColor(_hdc, _oldColor);	
        }
      
      private:
        HDC      _hdc;
        COLORREF _oldColor;
    };

    class BackgroundMode
    {    
      public:
        ~BackgroundMode( void )
        { ::SetBkMode(_hdc, _oldMode);	
        }

      protected:
        BackgroundMode( HDC hdc, int mode )
        : _hdc(hdc),
          _oldMode(::SetBkMode(_hdc, mode))
        {}
      
      private:
        HDC _hdc;
        int _oldMode;
    };

    class BackgroundOpaque : public BackgroundMode
    {
      public:
        BackgroundOpaque( HDC hdc )
        : BackgroundMode(hdc, OPAQUE)
        {}
    };

    class BackgroundTransparent : public BackgroundMode
    {
      public:
        BackgroundTransparent( HDC hdc )
        : BackgroundMode(hdc, TRANSPARENT)
        {}
    };


    class SysFixed : public Win::StockObjectHolder
    {
      public:
        SysFixed( HDC hdc )
        : Win::StockObjectHolder(hdc, SYSTEM_FIXED_FONT)
        {}
    };
};

} // namespace Font

class Path
{
  public:
    Path( HDC hdc )
    : _hdc(hdc)
    { ::BeginPath(_hdc); 
    }

    ~Path( void )
    { ::EndPath(_hdc); 
    }

  private:
    HDC _hdc;
};

class Region
{
  public:
    Region( int left, int top, int right, int bottom )
    : _hRgn(::CreateRectRgn(left, top, right, bottom))
    {}

    Region( RECT const& rect )
    : _hRgn(::CreateRectRgnIndirect(&rect))
    {}

    ~Region( void )
    { if ( _hRgn != 0 )
      { ::DeleteObject(_hRgn);
      }
    }

    operator HRGN( void ) const { return _hRgn; }

  protected: // Methods
    Region( HRGN hRgn = 0 )
    : _hRgn(hRgn)
    {}

  protected: // Data members
    HRGN _hRgn;
};

class RegionFromPath : public Region
{
  public:
    RegionFromPath( HDC hdc )
    : Region(::PathToRegion(hdc))
    {}
};

class Clipper
{
  public:
    Clipper( HDC hdc, RECT const& r )
    : _hdc(hdc),
      _old(0)
    { 
      ::GetClipRgn(_hdc, _old);
      Region rgn(r);
      ::SelectClipRgn(_hdc, rgn);
    }

    void ExcludeRect( RECT const& r )
    { ::ExcludeClipRect(_hdc, r.left, r.top, r.right, r.bottom);
    }

    void IntersectRect( RECT const& r )
    { ::IntersectClipRect(_hdc, r.left, r.top, r.right, r.bottom);
    }

    ~Clipper( void )
    { ::SelectClipRgn(_hdc, _old);
    }

  private:
    HDC  _hdc;
    HRGN _old;
};

} // namespace Win

#endif // WINTOOLS_H