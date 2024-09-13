#ifndef WINBITMAP_H
#define WINBITMAP_H

#include "Templates/RefPtr.h"
#include "Templates/SmartPtr.h"
#include "Canvas.h"
#include "XWin.h"
#include "File.h"
#include <cassert>
#include <cstring>
#include <cmath>

namespace Win
{

class SimpleBitmap
{
  public:
    bool IsOk( void ) const 
    { return _hBitmap != 0; 
    }

    operator HBITMAP( void ) const
    { return _hBitmap;
    }

  protected: // Methods
    SimpleBitmap( HBITMAP hBitmap = 0 )
    : _hBitmap(hBitmap)
    {}

  protected: // Data members
    HBITMAP _hBitmap;
};

/*

  Representation of 24bpp Device Independent Bitmap

*/
class DibSection24Rep : public SimpleBitmap, public RefCounted
{
  public:
    DibSection24Rep( void )
    : _baseaddr(0),
      _pitch(0)
    { ::ZeroMemory(&_bi, sizeof(_bi));
      _bi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
      _bi.bmiHeader.biPlanes = 1;
      _bi.bmiHeader.biBitCount = 24;
      _bi.bmiHeader.biCompression = BI_RGB;
    }

    DibSection24Rep( int w, int h )
    : _baseaddr(0),
      _pitch(0)
    { ::ZeroMemory(&_bi, sizeof(_bi));
      _bi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
      _bi.bmiHeader.biPlanes = 1;
      _bi.bmiHeader.biBitCount = 24;
      _bi.bmiHeader.biCompression = BI_RGB;
      Create(w,h);
    }

    DibSection24Rep( DibSection24Rep const& b )
    : _baseaddr(0),
      _pitch(0)
    { ::ZeroMemory(&_bi, sizeof(_bi));
      _bi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
      _bi.bmiHeader.biPlanes = 1;
      _bi.bmiHeader.biBitCount = 24;
      _bi.bmiHeader.biCompression = BI_RGB;
      Duplicate(b);
    }

    ~DibSection24Rep( void )
    { Destroy(); 
    }

    void Create( int w, int h, HDC hdc = 0 )
    { Destroy();
      _bi.bmiHeader.biWidth = w;
      _bi.bmiHeader.biHeight = h;
      _bi.bmiHeader.biSizeImage = 0;
      _hBitmap = ::CreateDIBSection(hdc, &_bi, DIB_RGB_COLORS, reinterpret_cast<void**>(&_baseaddr), 0, 0);
      if ( _hBitmap == 0 )
      { throw Win::Exception(_T("::CreateDibSection failed"));
      }
      _pitch = 3 * w;
      if ( _pitch&0x03 ) // Align to DWORD boundary
      { _pitch |= 0x03;
        ++_pitch;
      }
    }

    void Destroy( void )
    { if ( _hBitmap != 0 )
      { ::DeleteObject(reinterpret_cast<HGDIOBJ>(_hBitmap));
      }
      _hBitmap = 0;
      _baseaddr = 0;
      _pitch = 0;
      _bi.bmiHeader.biWidth = 0;
      _bi.bmiHeader.biHeight = 0;
      _bi.bmiHeader.biSizeImage = 0;
    }

    HBITMAP ReleaseBitmap( void )
    { HBITMAP temp = _hBitmap;
      _hBitmap = 0;
      _baseaddr = 0;
      _pitch = 0;
      _bi.bmiHeader.biWidth = 0;
      _bi.bmiHeader.biHeight = 0;
      _bi.bmiHeader.biSizeImage = 0;
      return temp;
    }

    void Duplicate( DibSection24Rep const& src )
    { if ( src.IsOk() )
      { Create(src._bi.bmiHeader.biWidth, src._bi.bmiHeader.biHeight);
        long size = src._pitch * src._bi.bmiHeader.biHeight;
        ::memcpy(_baseaddr, src._baseaddr, size);
      }
      else
      { Destroy();
      }
    }

    void Copy( HBITMAP hBmp, HDC hdc = DisplayCanvas() )
    {
      if ( hBmp != 0 )
      {
        BITMAP bmp;
        ::GetObject(hBmp, sizeof(bmp), &bmp);
        Create(bmp.bmWidth, bmp.bmHeight, hdc);
        ::GetDIBits(hdc, hBmp, 0, _bi.bmiHeader.biHeight,
                    _baseaddr, &_bi, DIB_RGB_COLORS);
      }
      else
      {
        Destroy();
      }
    }

    unsigned char* GetLine( int y ) 
    { assert(_baseaddr != 0);
      assert(y >= 0);
      assert(y < _bi.bmiHeader.biHeight);
      return &_baseaddr[_pitch * (_bi.bmiHeader.biHeight - y - 1)]; 
    }

    unsigned char const* GetLine( int y ) const
    { assert(_baseaddr != 0);
      assert(y >= 0);
      assert(y < _bi.bmiHeader.biHeight);
      return &_baseaddr[_pitch * (_bi.bmiHeader.biHeight - y - 1)]; 
    }

    unsigned char* GetPixel( int x, int y )
    { assert(_baseaddr != 0);
      assert(y >= 0);
      assert(y < _bi.bmiHeader.biHeight);
      assert(x >= 0);
      assert(x < _bi.bmiHeader.biWidth);
      return &_baseaddr[3 * x + _pitch * (_bi.bmiHeader.biHeight - y - 1)]; 
    }

    unsigned char const* GetPixel( int x, int y ) const
    { assert(_baseaddr != 0);
      assert(y >= 0);
      assert(y < _bi.bmiHeader.biHeight);
      assert(x >= 0);
      assert(x < _bi.bmiHeader.biWidth);
      return &_baseaddr[3 * x + _pitch * (_bi.bmiHeader.biHeight - y - 1)]; 
    }

    void SetPixel( int x, int y, unsigned char r, unsigned char g, unsigned char b )
    { assert(_baseaddr != 0);
      assert(y >= 0);
      assert(y < _bi.bmiHeader.biHeight);
      assert(x >= 0);
      assert(x < _bi.bmiHeader.biWidth);
      int baseIdx = 3 * x + _pitch * (_bi.bmiHeader.biHeight - y - 1);
      _baseaddr[baseIdx] = b; 
      _baseaddr[baseIdx + 1] = g; 
      _baseaddr[baseIdx + 2] = r; 
    }

    unsigned char& Red( int x, int y )
    { assert(_baseaddr != 0);
      assert(y >= 0);
      assert(y < _bi.bmiHeader.biHeight);
      assert(x >= 0);
      assert(x < _bi.bmiHeader.biWidth);
      return _baseaddr[3 * x + _pitch * (_bi.bmiHeader.biHeight - y - 1) + 2];
    }

    unsigned char const& Red( int x, int y ) const
    { assert(_baseaddr != 0);
      assert(y >= 0);
      assert(y < _bi.bmiHeader.biHeight);
      assert(x >= 0);
      assert(x < _bi.bmiHeader.biWidth);
      return _baseaddr[3 * x + _pitch * (_bi.bmiHeader.biHeight - y - 1) + 2];
    }

    unsigned char& Green( int x, int y )
    { assert(_baseaddr != 0);
      assert(y >= 0);
      assert(y < _bi.bmiHeader.biHeight);
      assert(x >= 0);
      assert(x < _bi.bmiHeader.biWidth);
      return _baseaddr[3 * x + _pitch * (_bi.bmiHeader.biHeight - y - 1) + 1];
    }

    unsigned char const& Green( int x, int y ) const
    { assert(_baseaddr != 0);
      assert(y >= 0);
      assert(y < _bi.bmiHeader.biHeight);
      assert(x >= 0);
      assert(x < _bi.bmiHeader.biWidth);
      return _baseaddr[3 * x + _pitch * (_bi.bmiHeader.biHeight - y - 1) + 1];
    }

    unsigned char& Blue( int x, int y )
    { assert(_baseaddr != 0);
      assert(y >= 0);
      assert(y < _bi.bmiHeader.biHeight);
      assert(x >= 0);
      assert(x < _bi.bmiHeader.biWidth);
      return _baseaddr[3 * x + _pitch * (_bi.bmiHeader.biHeight - y - 1)];
    }

    unsigned char const& Blue( int x, int y ) const
    { assert(_baseaddr != 0);
      assert(y >= 0);
      assert(y < _bi.bmiHeader.biHeight);
      assert(x >= 0);
      assert(x < _bi.bmiHeader.biWidth);
      return _baseaddr[3 * x + _pitch * (_bi.bmiHeader.biHeight - y - 1)];
    }

    int Width( void ) const { return _bi.bmiHeader.biWidth; }
    int Height( void ) const { return _bi.bmiHeader.biHeight; }
    int Pitch( void ) const { return _pitch; }

    // Use in SetDiBisToDevice
    void const* GetBits( void ) const { return _baseaddr; }
    BITMAPINFO const* GetBitmapInfo( void ) const { return &_bi; }

    // Utilities
    void DeinterleaveRow( unsigned char* row[3], int y ) const
    { unsigned char const* line = GetLine(y);
      for ( int i = 0, j = 0; j < Width(); i += 3, ++j )
      { row[0][j] = line[i];
        row[1][j] = line[i + 1];
        row[2][j] = line[i + 2];
      }
    }

    void DeinterleaveCol( unsigned char* col[3], int x ) const
    { for ( int y = Height() - 1, idx = 3 * x; y >= 0; --y, idx += _pitch )
      { col[0][y] = _baseaddr[idx];
        col[1][y] = _baseaddr[idx + 1];
        col[2][y] = _baseaddr[idx + 2];
      }
    }

  private: // Data members
    BITMAPINFO     _bi;
    unsigned char* _baseaddr;
    int            _pitch;
};

/*

  Ref counted 24bpp Dib section

*/
class DibSection24 : public RefPtr<DibSection24Rep>
{
  public: // Methods
    DibSection24( void )
    : RefPtr<DibSection24Rep>(new DibSection24Rep)
    {}

    DibSection24( int w, int h )
    : RefPtr<DibSection24Rep>(new DibSection24Rep(w,h))
    {}

    DibSection24( DibSection24 const& b )
    : RefPtr<DibSection24Rep>(b)
    {}

    void Create( int w, int h, HDC hdc = 0  )
    { Destroy();
      _p->Create(w, h, hdc);
    }

    void Destroy( void )
    { Release();
      _p = new DibSection24Rep;
    }

    HBITMAP ReleaseBitmap( void )
    { Clone();
      return _p->ReleaseBitmap();
    }

    void Duplicate( DibSection24 const& src )
    { Destroy();
      _p->Duplicate(*src._p);
    }

    void Copy( HBITMAP hBmp, HDC hdc = DisplayCanvas() )
    {
      Destroy();
      _p->Copy(hBmp, hdc);
    }

    unsigned char* GetLine( int y ) 
    { Clone();
      return _p->GetLine(y);
    }

    unsigned char const* GetLine( int y ) const
    { return _p->GetLine(y);
    }

    unsigned char* GetPixel( int x, int y ) 
    { Clone();
      return _p->GetPixel(x,y);
    }

    unsigned char const* GetPixel( int x, int y ) const
    { return _p->GetPixel(x,y);
    }

    void SetPixel( int x, int y, unsigned char r, unsigned char g, unsigned char b )
    { Clone();
      _p->SetPixel(x,y,r,g,b);
    }

    unsigned char& Red( int x, int y )
    { Clone();
      return _p->Red(x,y);
    }

    unsigned char const& Red( int x, int y ) const
    { return _p->Red(x,y);
    }

    unsigned char& Green( int x, int y )
    { Clone();
      return _p->Green(x,y);
    }

    unsigned char const& Green( int x, int y ) const
    { return _p->Green(x,y);
    }

    unsigned char& Blue( int x, int y )
    { Clone();
      return _p->Blue(x,y);
    }

    unsigned char const& Blue( int x, int y ) const
    { return _p->Blue(x,y);
    }

    operator HBITMAP( void ) const { return static_cast<HBITMAP>(*_p); }

    int Width( void ) const { return _p->Width(); }
    int Height( void ) const { return _p->Height(); }
    int Pitch( void ) const { return _p->Pitch(); }

    bool IsOk( void ) const { return _p->IsOk(); }

    // Use in SetDiBisToDevice
    void const* GetBits( void ) const { return _p->GetBits(); }
    BITMAPINFO const* GetBitmapInfo( void ) const { return _p->GetBitmapInfo(); }

    // Utilities
    void DeinterleaveRow( unsigned char* row[3], int y ) const
    { _p->DeinterleaveRow(row, y);
    }

    void DeinterleaveCol( unsigned char* col[3], int x ) const
    { _p->DeinterleaveCol(col, x);
    }

    template <class _Filter>
    DibSection24 Resample( int newWidth, int newHeight, _Filter& filter ) const
    {
      // Create destination image
      DibSection24 dst(newWidth, newHeight);
      DibSection24Rep const& src = *_p;

      // Create intermediate image to hold horizontal zoom
      DibSection24 temp(dst.Width(), src.Height());
      double xScale = double(dst.Width()) / src.Width();
      double yScale = double(dst.Height()) / src.Height();

      {
        // Pre-calculate filter contributions for a row
        SArr<ContribList> contrib(new ContribList[dst.Width()]);
	      if ( xScale < 1.0 ) 
        { double width = filter.Width() / xScale;
          double fScale = 1.0 / xScale;
          for ( int i = 0; i < dst.Width(); ++i )
          { double center = double(i) / xScale;
            int left = int(::ceil(center - width));
            int right = int(::floor(center + width));
            contrib[i].Init(right - left + 1);
            for( int j = left, k = 0; j <= right; ++j, ++k )
            { double weight = center - j;
              weight = filter(weight / fScale) / fScale;
              int n = j;
              if( n < 0 )
              { n = -n;
              } 
              else if ( n >= src.Width() ) 
              { n = (src.Width() - n) + src.Width() - 1;
				      }
              contrib[i][k].Set(n, weight);
            }
          }
        }
        else 
        { for( int i = 0; i < dst.Width(); ++i ) 
          { double center = double(i) / xScale;
            int left = int(::ceil(center - filter.Width()));
            int right = int(::floor(center + filter.Width()));
            contrib[i].Init(right - left + 1);
            for( int j = left, k = 0; j <= right; ++j, ++k ) 
            { double weight = center - j;
              weight = filter(weight);
              int n = j;
              if ( n < 0 )
              { n = -n;
              } 
              else if ( n >= src.Width() ) 
              { n = (src.Width() - n) + src.Width() - 1;
              }
              contrib[i][k].Set(n, weight);
            }
          }
        }
  
        // Apply filter to zoom horizontally from src to temp
        SArr<unsigned char> buf(new unsigned char[3 * src.Width()]);
        unsigned char* row[3] = {&buf[0], &buf[src.Width()], &buf[2 * src.Width()]};
        for( int k = 0; k < temp.Height(); ++k ) 
        { src.DeinterleaveRow(row, k);
          for( int i = 0; i < temp.Width(); ++i )
          { double bWeight = 0.0;
            double gWeight = 0.0;
            double rWeight = 0.0;
            for ( int j = 0; j < contrib[i].Size(); ++j ) 
            { bWeight += row[0][contrib[i][j].Pixel()] * contrib[i][j].Weight();
              gWeight += row[1][contrib[i][j].Pixel()] * contrib[i][j].Weight();
              rWeight += row[2][contrib[i][j].Pixel()] * contrib[i][j].Weight();
			      }
            temp.SetPixel(i, k, Clamp(rWeight), Clamp(gWeight), Clamp(bWeight));
          }
        }
      }

      {
        // pre-calculate filter contributions for a column
        SArr<ContribList> contrib( new ContribList[dst.Height()] );
        if ( yScale < 1.0 ) 
        { double width = filter.Width() / yScale;
          double fScale = 1.0 / yScale;
          for ( int i = 0; i < dst.Height(); ++i ) 
          { double center = i / yScale;
            int left = int(::ceil(center - width));
            int right = int(::floor(center + width));
            contrib[i].Init(right - left + 1);
            for ( int j = left, k = 0; j <= right; ++j, ++k ) 
            { double weight = center - j;
              weight = filter(weight / fScale) / fScale;
              int  n = j;
              if ( n < 0 ) 
              { n = -n;
              } 
              else if ( n >= temp.Height() ) 
              { n = (temp.Height() - n) + temp.Height() - 1;
				      }
              contrib[i][k].Set(n, weight);
            }
          }
        } 
        else 
        { for ( int i = 0; i < dst.Height(); ++i ) 
          { double center = i / yScale;
            int left = int(::ceil(center - filter.Width()));
            int right = int(::floor(center + filter.Width()));
            contrib[i].Init(right - left + 1);
            for ( int j = left, k = 0; j <= right; ++j, ++k ) 
            { double weight = center - j;
              weight = filter(weight);
              int n = j;
              if ( n < 0 ) 
              { n = -n;
              }
              else if ( n >= temp.Height() ) 
              { n = (temp.Height() - n) + temp.Height() - 1;
              }
              contrib[i][k].Set(n, weight);
            }
          }
        }
  
        // Apply filter to zoom vertically from temp to dst
        SArr<unsigned char> buf(new unsigned char[3 * temp.Height()]);
        unsigned char* col[3] = {&buf[0], &buf[temp.Height()], &buf[2 * temp.Height()]};
        for ( int k = 0; k < dst.Width(); ++k ) 
        { temp.DeinterleaveCol(col, k);
          for ( int i = 0; i < dst.Height(); ++i ) 
          { double bWeight = 0.0;
            double gWeight = 0.0;
            double rWeight = 0.0;
            for ( int j = 0; j < contrib[i].Size(); ++j ) 
            { bWeight += col[0][contrib[i][j].Pixel()] * contrib[i][j].Weight();
              gWeight += col[1][contrib[i][j].Pixel()] * contrib[i][j].Weight();
              rWeight += col[2][contrib[i][j].Pixel()] * contrib[i][j].Weight();
            }
            dst.SetPixel(k, i, Clamp(rWeight), Clamp(gWeight), Clamp(bWeight));
          }
        }
      }
      return dst;
    }

    template <class _Filter>
    DibSection24 MakeThumbnail( int maxWidth, int maxHeight, _Filter& filter, bool enlargeSmall = true )
    { double wRatio = double(Width()) / maxWidth;
      double hRatio = double(Height()) / maxHeight;
      double factor = (wRatio > hRatio ? wRatio : hRatio);
      if ( !enlargeSmall && factor <= 1.0 )
      { return *this;
      }      
      int newWidth = int(Width() / factor);
      int newHeight = int(Height() / factor);
      return Resample(newWidth, newHeight, filter);
    }

    static unsigned char Clamp( double val )
    { return unsigned char(val < 0.0 ? 0 : (val > 255.0 ? 255 : val));
    }

  //
  // Some resampling filters
  // ~~~~~~~~~~~~~~~~~~~~~~~
  public: // Class
    class SimpleFilter
    {
      public:
        double Width( void ) const { return _width; }
    
      protected: // Methods
        explicit SimpleFilter( double width )
        : _width(width)
        {}

      protected: // Data members
        double _width;
    };

  public: // Class
    class Filter : public SimpleFilter
    {
      public:
        Filter( void )
        : SimpleFilter(1.0)
        {}
    
        double operator()( double t ) const
        { // f(t) = 2|t|^3 - 3|t|^2 + 1, -1 <= t <= 1
          if ( t < 0.0 ) t = -t;
          if ( t < 1.0 )
          { return (2.0 * t - 3.0) * t * t + 1.0;
          }
          return 0.0;
        }
    };

  public: // Class
    class BoxFilter : public SimpleFilter
    {
      public:
        BoxFilter( void )
        : SimpleFilter(0.5)
        {}

        double operator()( double t ) const
        { if ( t > -0.5 && t <= 0.5 ) return 1.0;
	        return 0.0;
        }
    };

  public: // Class
    class TriangleFilter : public SimpleFilter
    {
      public:
        TriangleFilter( void )
        : SimpleFilter(1.0)
        {}
    
        double operator()( double t ) const
        { if ( t < 0.0 ) t = -t;
          if ( t < 1.0 ) return 1.0 - t;
          return 0.0;
        }
    };

  public: // Class
    class BellFilter : public SimpleFilter
    {
      public:
        BellFilter( void )
        : SimpleFilter(1.0)
        {}
    
        double operator()( double t ) const
        { if ( t < 0.0 ) t = -t;
          if ( t < 0.5 ) return 0.75 - (t * t);
          if ( t < 1.5 ) 
          { t -= 1.5;
            return 0.5 * (t * t);
	        }
          return 0.0;
        }
    };

  public: // Class
    class BSplineFilter : public SimpleFilter
    {
      public:
        BSplineFilter( void )
        : SimpleFilter(2.0)
        {}
    
        double operator()( double t ) const
        { if ( t < 0.0 ) t = -t;
          if ( t < 1.0 ) 
          { double t2 = t * t;
            return (0.5 * t2 * t) - t2 + (2.0 / 3.0);
          } 
          else if ( t < 2.0 ) 
          { t = 2.0 - t;
            return (1.0 / 6.0) * (t * t * t);
          }
          return 0.0;
        }
    };

  public: // Class
    class Lanczos3Filter : public SimpleFilter
    {
      public:
        Lanczos3Filter( void )
        : SimpleFilter(3.0)
        {}
    
        double operator()( double t ) const
        { if ( t < 0.0 ) t = -t;
          if ( t < 3.0 )
          { return sinc(t) * sinc(t / 3.0);
          }
          return 0.0;
        }

      private: // Methods
        static double sinc( double x )
        { if ( x != 0.0 )
          { x *= 3.14159265358979;
            return ::sin(x) / x;
          }
          return 1.0;
        }
    };

  public: // Class
    class MitchellFilter : public SimpleFilter
    {
      public:
        MitchellFilter( void )
        : SimpleFilter(2.0)
        {}
    
        double operator()( double t ) const
        { static double const B = 1.0 / 3.0;
          static double const C = 1.0 / 3.0;
      
          double t2 = t * t;
          if ( t < 0.0 ) t = -t;
          if ( t < 1.0 ) 
          { t = (((12.0 - 9.0 * B - 6.0 * C) * (t * t2))
                + ((-18.0 + 12.0 * B + 6.0 * C) * t2)
                + (6.0 - 2.0 * B));
             return t / 6.0;
          } 
          else if ( t < 2.0 )
          { t = (((-1.0 * B - 6.0 * C) * (t * t2))
                + ((6.0 * B + 30.0 * C) * t2)
                + ((-12.0 * B - 48.0 * C) * t)
                + (8.0 * B + 24.0 * C));
            return t / 6.0;
          }
          return 0.0;
        }
    };

  private: // Class
    class Contrib
    {
      public:
        Contrib( int pixel = 0, double weight = 0.0 )
        : _pixel(pixel),
          _weight(weight)
        {}

        void Set( int pixel, double weight )
        { _pixel = pixel; 
          _weight = weight; 
        }

        int Pixel( void ) const { return _pixel; }
        double Weight( void ) const { return _weight; }

      private:
        int    _pixel;
        double _weight;
    };

  private: // Class
    class ContribList
    {
      public:
        ContribList( void )
        : _size(0)
        {}

        void Init( int n )
        { _size = n;
          _arr = SArr<Contrib>(new Contrib[_size]);
        }

        Contrib& operator[]( int i )
        { assert(_arr != 0);
          assert(i >= 0);
          assert(i < _size);
          return _arr[i];
        }

        Contrib const& operator[]( int i ) const
        { assert(_arr != 0);
          assert(i >= 0);
          assert(i < _size);
          return _arr[i];
        }

        int Size( void ) const { return _size; }

      public:
        int           _size;
        SArr<Contrib> _arr;
    };
};

/*

  There seems to be no way of duplicating a Device Dependent Bitmap
  so following class has only Transfer Semantic and is not ref counted

*/
class DdBitmap : public SimpleBitmap
{
  public:
    DdBitmap( HBITMAP hBitmap = 0 )
    : SimpleBitmap(hBitmap),
      _w(0),
      _h(0)
    { GetSize();
    }

    DdBitmap( DdBitmap& b )
    : _w(b._w),
      _h(b._h)
    { _hBitmap = b.Release();
    }

    ~DdBitmap( void )
    { Destroy();
    }

    DdBitmap& operator=( DdBitmap& b )
    { if ( this != &b )
      { Destroy();
        _w = b._w;
        _h = b._h;
        _hBitmap = b.Release();
      }
    }

    void Destroy( void )
    { if ( _hBitmap != 0 )
      { ::DeleteObject(reinterpret_cast<HGDIOBJ>(_hBitmap));
      }
      _hBitmap = 0;
      _w = 0;
      _h = 0;
    }

    void CreateCompatible( int w, int h, HDC hdc = DisplayCanvas() )
    { Destroy();
      _hBitmap = ::CreateCompatibleBitmap(hdc, w, h);
      if ( _hBitmap == 0 )
      { throw Win::Exception(_T("::CreateCompatibleBitmap failed"));
      }
      _w = w;
      _h = h;
    }

    void CreateFromDib( DibSection24 const& dib, HDC hdc = DisplayCanvas() )
    { Destroy();
      _hBitmap = ::CreateDIBitmap(hdc, reinterpret_cast<BITMAPINFOHEADER const*>(dib.GetBitmapInfo()), CBM_INIT, dib.GetBits(), dib.GetBitmapInfo(), DIB_RGB_COLORS);
      if ( _hBitmap == 0 )
      { throw Win::Exception(_T("::CreateDIBitmap failed"));
      }
      _w = dib.Width();
      _h = dib.Height();
    }

    void Load( HINSTANCE hInst, int resId )
    { Destroy();
      _hBitmap = reinterpret_cast<HBITMAP>(::LoadImage(hInst, MAKEINTRESOURCE(resId), IMAGE_BITMAP, 0, 0, LR_DEFAULTCOLOR));
      if ( _hBitmap == 0 )
      { throw Win::Exception(_T("::LoadImage failed"));
      }
      GetSize();
    }

    void Load( TCHAR const path[] )
    { Destroy();
      _hBitmap = reinterpret_cast<HBITMAP>(::LoadImage(0, path, IMAGE_BITMAP, 0, 0, LR_DEFAULTCOLOR|LR_LOADFROMFILE));
      if ( _hBitmap == 0 )
      { throw Win::Exception(_T("::LoadImage failed"));
      }
      GetSize();
    }

    // Caller is then responsible for freeing the bitmap !!
    HBITMAP Release( void )
    { HBITMAP hTemp = _hBitmap;
      _hBitmap = 0;
      _w = 0;
      _h = 0;
      return hTemp;
    }

    int Width( void ) const { return _w; }
    int Height( void ) const { return _h; }

  protected:
    void GetSize( void )
    { if ( _hBitmap != 0 )
      { BITMAP bmp;
        ::GetObject(reinterpret_cast<HGDIOBJ>(_hBitmap), sizeof(bmp), &bmp);
        _w = bmp.bmWidth;
        _h = bmp.bmHeight;
      }
    }

  protected:
    int _w;
    int _h;
};

/*

  But if we know that the Device Context is always Display then
  we can duplicate it by using ::BitBlt function and make it ref counted

*/
class DisplayBitmapRep : public SimpleBitmap, public RefCounted
{
  public:
    DisplayBitmapRep( HBITMAP hBitmap = 0 )
    : SimpleBitmap(hBitmap),
      _w(0),
      _h(0)
    { GetSize();
    }

    DisplayBitmapRep( int w, int h )
    : _w(0),
      _h(0)
    { Create(w, h);
    }

    DisplayBitmapRep( DisplayBitmapRep const& b )
    : _w(0),
      _h(0)
    { Duplicate(b);
    }

    ~DisplayBitmapRep( void )
    { Destroy();
    }

    void Destroy( void )
    { if ( _hBitmap != 0 )
      { ::DeleteObject(reinterpret_cast<HGDIOBJ>(_hBitmap));
      }
      _hBitmap = 0;
      _w = 0;
      _h = 0;
    }

    void Duplicate( DisplayBitmapRep const& b )
    { if ( b.IsOk() )
      { Create(b._w, b._h);
        BitmapCanvas dest(DisplayCanvas(), _hBitmap);
        Blitter blitter(dest, 0, 0, _w, _h);
        blitter.BlitFrom(BitmapCanvas(DisplayCanvas(), b._hBitmap));
      }
      else
      { Destroy();
      }
    }

    void Create( int w, int h )
    { Destroy();
      _hBitmap = ::CreateCompatibleBitmap(DisplayCanvas(), w, h);
      if ( _hBitmap == 0 )
      { throw Win::Exception(_T("::CreateCompatibleBitmap failed"));
      }
      _w = w;
      _h = h;
    }

    void CreateFromDib( DibSection24 const& dib )
    { Destroy();
      _hBitmap = ::CreateDIBitmap(DisplayCanvas(), reinterpret_cast<BITMAPINFOHEADER const*>(dib.GetBitmapInfo()), CBM_INIT, dib.GetBits(), dib.GetBitmapInfo(), DIB_RGB_COLORS);
      if ( _hBitmap == 0 )
      { throw Win::Exception(_T("::CreateDIBitmap failed"));
      }
      _w = dib.Width();
      _h = dib.Height();
    }

    void Load( HINSTANCE hInst, int resId )
    { Destroy();
      _hBitmap = reinterpret_cast<HBITMAP>(::LoadImage(hInst, MAKEINTRESOURCE(resId), IMAGE_BITMAP, 0, 0, LR_DEFAULTCOLOR));
      if ( _hBitmap == 0 )
      { throw Win::Exception(_T("::LoadImage failed"));
      }
      GetSize();
    }

    void Load( TCHAR const path[] )
    { Destroy();
      _hBitmap = reinterpret_cast<HBITMAP>(::LoadImage(0, path, IMAGE_BITMAP, 0, 0, LR_DEFAULTCOLOR|LR_LOADFROMFILE));
      if ( _hBitmap == 0 )
      { throw Win::Exception(_T("::LoadImage failed"));
      }
      GetSize();
    }

    int Width( void ) const { return _w; }
    int Height( void ) const { return _h; }

  protected:
    void GetSize( void )
    { if ( _hBitmap != 0 )
      { BITMAP bmp;
        ::GetObject(reinterpret_cast<HGDIOBJ>(_hBitmap), sizeof(bmp), &bmp);
        _w = bmp.bmWidth;
        _h = bmp.bmHeight;
      }
    }

  protected:
    int _w;
    int _h;
};

/*

  Ref counted Display bitmap

*/
class DisplayBitmap : public RefPtr<DisplayBitmapRep>
{
  public:
    DisplayBitmap( HBITMAP hBitmap = 0 )
    : RefPtr<DisplayBitmapRep>(new DisplayBitmapRep) 
    {}

    DisplayBitmap( int w, int h )
    : RefPtr<DisplayBitmapRep>(new DisplayBitmapRep(w, h)) 
    {}

    DisplayBitmap( DisplayBitmap const& b )
    : RefPtr<DisplayBitmapRep>(b)
    {}

    void Destroy( void )
    { Release();
      _p = new DisplayBitmapRep;
    }

    void Duplicate( DisplayBitmap const& b )
    { Destroy();
      _p->Duplicate(*_p);
    }

    void Create( int w, int h )
    { Destroy();
      _p->Create(w, h);
    }

    void CreateFromDib( DibSection24 const& dib )
    { Destroy();
      _p->CreateFromDib(dib);
    }

    void Load( HINSTANCE hInst, int resId )
    { Destroy();
      _p->Load(hInst, resId);
    }

    void Load( TCHAR const path[] )
    { Destroy();
      _p->Load(path);
    }

    int Width( void ) const { return _p->Width(); }
    int Height( void ) const { return _p->Height(); }

    bool IsOk( void ) const { return _p->IsOk(); }

    operator HBITMAP( void ) const { return static_cast<HBITMAP>(*_p); }
};

} // namespace Win

#endif // WINBITMAP_H