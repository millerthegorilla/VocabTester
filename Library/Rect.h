#ifndef WINRECT_H
#define WINRECT_H

#include <windows.h>

namespace Win
{

class Rect : public RECT
{
  public:
    Rect( long l = 0, long t = 0, long r = 0, long b = 0 )
    { left = l;
      right = r;
      top = t;
      bottom = b;
    }

    long Left( void ) const { return left; }
    long Right( void ) const { return right; }
    long Top( void ) const { return top; }
    long Bottom( void ) const { return bottom; }
    long Width( void ) const { return right - left; }
    long Height( void ) const { return bottom - top; }
    
    void Set( long l, long t, long r, long b )
    { left = l; 
      top = t; 
      right = r; 
      bottom = b; 
    }

    void SetLeft( long l ) { left = l; }
    void SetRight( long r ) { right = r; }
    void SetTop( long t ) { top = t; }
    void SetBottom( long b ) { bottom = b; }

    bool HasPoint( long x, long y )
    { return left <= x && x < right && top <= y && y < bottom; 
    }

    bool HasPoint( POINT const& pt )
    { return HasPoint(pt.x, pt.y);
    }
};

class DowRect : public Rect
{
  public:
    DowRect( HWND hwnd )
    { ::GetWindowRect(hwnd, this);
    }
};

class ClientRect : public Rect
{
  public:
    ClientRect( HWND hwnd )
    { ::GetClientRect(hwnd, this);
    }
};

class ClientScreenRect : public Rect
{
  public:
    ClientScreenRect( HWND hwnd )
    {
      POINT pt = {0,0};
      ::ClientToScreen(hwnd, &pt);
      ::GetClientRect(hwnd, this);
      left += pt.x;
      right += pt.x;
      top += pt.y;
      bottom += pt.y;
    }
};

class Point : public POINT
{
  public:
    Point( long lx = 0, long ly = 0 )
    { x = lx;
      y = ly;
    }

    long X( void ) const { return x; }
    long Y( void ) const { return y; }

    void Set( long lx, long ly )
    { x = lx;
      y = ly;
    }

    void SetX( long lx ) { x = lx; }
    void SetY( long ly ) { y = ly; }
};

class CursorPos : public Point
{
  public:
    CursorPos( void )
    { ::GetCursorPos(this);
    }
};

} // namespace Win

#endif // WINRECT_H