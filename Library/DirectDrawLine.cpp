#include "DirectDraw.h"

namespace Win
{
namespace Direct
{

void DrawSurfaceBuffer::Line( int x1, int y1, int x2, int y2, COLORREF c )
{
  switch ( _format.Bpp() )
  { case 16: Line16(x1, y1, x2, y2, c); break;
    case 24: Line24(x1, y1, x2, y2, c); break;
    case 32: Line32(x1, y1, x2, y2, c); break;
  }
}

void DrawSurfaceBuffer::Line16( int x1, int y1, int x2, int y2, COLORREF c )
{
  // Bresenham's algorithm
  // ~~~~~~~~~~~~~~~~~~~~~

  // ranges
  int dx = x2 - x1;
  int dy = y2 - y1;

  // making sure dx and dy > 0
  int inc_xh, inc_yh, inc_xl, inc_yl;

  if ( dx < 0 )
  { dx = -dx;
    inc_xh= -2;
    inc_xl= -2;
  }
  else
  { inc_xh = 2;
    inc_xl = 2;
  }
  
  if ( dy < 0 )
  { dy = -dy;
    inc_yh = -_pitch;
    inc_yl = -_pitch;
  }
  else
  { inc_yh = _pitch;
    inc_yl = _pitch;
  }

  // long range, making sure either x or y is changed in L case
  int long_d, short_d;
  if ( dx > dy )
  { long_d = dx;
    short_d = dy;
    inc_yl = 0;
  }
  else
  { long_d = dy;
    short_d = dx;
    inc_xl = 0;
  }

  // increments for point address
  register int inc_ah = inc_xh + inc_yh;
  register int inc_al = inc_xl + inc_yl;

  // address of first point
  register unsigned char* addr = &_buf[y1 * _pitch + 2 * x1];
  
  // initial value of d
  int d = 2 * short_d - long_d;
  
  // d adjustment for H case
  int add_dl = 2 * short_d;             
  
  // d adjustment for L case      
  int add_dh = 2 * short_d - 2 * long_d;
  
  // for all points in longer range
  unsigned short color = static_cast<unsigned short>(_format.ColorValue16(c));
  for ( register int i = 0; i <= long_d ; ++i )                     
  {
    { // rendering
      unsigned short* p = reinterpret_cast<unsigned short*>(addr);
      *p &= _format.Mask();
      *p |= color;
    }

    if ( d >= 0 )
    { // previous point was H type
      addr += inc_ah; 
      d += add_dh;
    }
    else
    { // previous point was L type
      addr += inc_al;
      d += add_dl;
    }
  }
}

void DrawSurfaceBuffer::Line24( int x1, int y1, int x2, int y2, COLORREF c )
{
  // Bresenham's algorithm
  // ~~~~~~~~~~~~~~~~~~~~~

  // ranges
  int dx = x2 - x1;
  int dy = y2 - y1;

  // making sure dx and dy > 0
  int inc_xh, inc_yh, inc_xl, inc_yl;

  if ( dx < 0 )
  { dx = -dx;
    inc_xh= -3;
    inc_xl= -3;
  }
  else
  { inc_xh = 3;
    inc_xl = 3;
  }
  
  if ( dy < 0 )
  { dy = -dy;
    inc_yh = -_pitch;
    inc_yl = -_pitch;
  }
  else
  { inc_yh = _pitch;
    inc_yl = _pitch;
  }

  // long range, making sure either x or y is changed in L case
  int long_d, short_d;
  if ( dx > dy )
  { long_d = dx;
    short_d = dy;
    inc_yl = 0;
  }
  else
  { long_d = dy;
    short_d = dx;
    inc_xl = 0;
  }

  // increments for point address
  register int inc_ah = inc_xh + inc_yh;
  register int inc_al = inc_xl + inc_yl;

  // address of first point
  register unsigned char* addr = &_buf[y1 * _pitch + 3 * x1];
  
  // initial value of d
  int d = 2 * short_d - long_d;
  
  // d adjustment for H case
  int add_dl = 2 * short_d;             
  
  // d adjustment for L case      
  int add_dh = 2 * short_d - 2 * long_d;
  
  // for all points in longer range
  unsigned long color = _format.ColorValue24(c);
  for ( register int i = 0; i <= long_d ; ++i )                     
  {
    { // rendering
      unsigned long* p = reinterpret_cast<unsigned long*>(addr);
      *p &= _format.Mask();
      *p |= color;
    }

    if ( d >= 0 )
    { // previous point was H type
      addr += inc_ah; 
      d += add_dh;
    }
    else
    { // previous point was L type
      addr += inc_al;
      d += add_dl;
    }
  }
}

void DrawSurfaceBuffer::Line32( int x1, int y1, int x2, int y2, COLORREF c )
{
  // Bresenham's algorithm
  // ~~~~~~~~~~~~~~~~~~~~~

  // ranges
  int dx = x2 - x1;
  int dy = y2 - y1;

  // making sure dx and dy > 0
  int inc_xh, inc_yh, inc_xl, inc_yl;

  if ( dx < 0 )
  { dx = -dx;
    inc_xh= -4;
    inc_xl= -4;
  }
  else
  { inc_xh = 4;
    inc_xl = 4;
  }
  
  if ( dy < 0 )
  { dy = -dy;
    inc_yh = -_pitch;
    inc_yl = -_pitch;
  }
  else
  { inc_yh = _pitch;
    inc_yl = _pitch;
  }

  // long range, making sure either x or y is changed in L case
  int long_d, short_d;
  if ( dx > dy )
  { long_d = dx;
    short_d = dy;
    inc_yl = 0;
  }
  else
  { long_d = dy;
    short_d = dx;
    inc_xl = 0;
  }

  // increments for point address
  register int inc_ah = inc_xh + inc_yh;
  register int inc_al = inc_xl + inc_yl;

  // address of first point
  register unsigned char* addr = &_buf[y1 * _pitch + 4 * x1];
  
  // initial value of d
  int d = 2 * short_d - long_d;
  
  // d adjustment for H case
  int add_dl = 2 * short_d;             
  
  // d adjustment for L case      
  int add_dh = 2 * short_d - 2 * long_d;
  
  // for all points in longer range
  unsigned long color = _format.ColorValue32(c);
  for ( register int i = 0; i <= long_d ; ++i )                     
  {
    { // rendering
      unsigned long* p = reinterpret_cast<unsigned long*>(addr);
      *p &= _format.Mask();
      *p |= color;
    }

    if ( d >= 0 )
    { // previous point was H type
      addr += inc_ah; 
      d += add_dh;
    }
    else
    { // previous point was L type
      addr += inc_al;
      d += add_dl;
    }
  }
}

} // namespace Direct
} // namespace Win
