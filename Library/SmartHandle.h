#ifndef WINSMARTHANDLE_H
#define WINSMARTHANDLE_H

#include <windows.h>

namespace Win
{

class SHandle
{
  public:
    SHandle( HANDLE h = INVALID_HANDLE_VALUE )
    : _h(h)
    {}

    SHandle( SHandle& b )
    : _h(b.Release())
    {}

    SHandle& operator=( SHandle& b )
    { if ( this != &b )
      { Close();
        _h = b.Release();
      }
      return *this;
    }

    ~SHandle( void )
    { Close(); 
    }

    bool IsOk( void ) const
    { return _h != INVALID_HANDLE_VALUE; 
    }

    void Close( void )
    { if ( _h != INVALID_HANDLE_VALUE )
      { ::CloseHandle(_h);
      }
      _h = INVALID_HANDLE_VALUE;
    }

    HANDLE Release( void )
    { HANDLE hTemp = _h;
      _h = INVALID_HANDLE_VALUE;
      return hTemp;
    }

    operator HANDLE( void ) const
    { return _h; 
    }

  protected:
    HANDLE _h;
};
 
} // namespace Win

#endif // WINSMARTHANDLE_H