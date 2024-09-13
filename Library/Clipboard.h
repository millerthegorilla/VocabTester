#ifndef WINCLIPBOARD_H
#define WINCLIPBOARD_H

#include "XWin.h"
#include <cassert>

namespace Win
{

class Clipboard
{
  public:
    Clipboard( HWND hwnd )
    : _success(::OpenClipboard(hwnd))
    {}

    ~Clipboard( void )
    { if ( _success )
      { ::CloseClipboard();
      }
    }

    bool IsOk( void ) const { return _success != 0; }

    void Empty( void )
    { assert(_success);
      if ( !::EmptyClipboard() )
      { throw Win::Exception(_T("Couldn't empty the clipboard"));
      }
    }

    void SetData( UINT format, HANDLE hData )
    { assert(_success);
      if ( ::SetClipboardData(format, hData) == 0 )
      { throw Win::Exception(_T("Couldn't transfer data to the clipboard"));
      }
    }

  private:
    BOOL _success;
};

} // namespace Win

#endif // WINCLIPBOARD_H