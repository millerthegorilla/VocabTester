#ifndef WINDEFERPOS_H
#define WINDEFERPOS_H

#include <windows.h>

namespace Win
{

class DeferPos
{
  public:
    explicit DeferPos( int nWins )
    : _h(::BeginDeferWindowPos(nWins))
    {}

    ~DeferPos( void )
    { if ( _h != 0 )
      { ::EndDeferWindowPos(_h);
      }
    }

    void SetWinPos( HWND win, int x, int y, int w, int h, unsigned flags = SWP_NOZORDER, HWND insertAfter = 0 )
    { if ( _h != 0 )
      { _h = ::DeferWindowPos(_h, win, insertAfter, x, y, w, h, flags); 
      }
      else
      { ::SetWindowPos(win, insertAfter, x, y, w, h, flags);
      }
    }

  private:
    HDWP _h;
};

} // namespace Win

#endif // WINDEFERPOS_H