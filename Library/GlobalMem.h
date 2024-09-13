#ifndef WINGLOBALMEM_H
#define WINGLOBALMEM_H

#include <windows.h>
#include "XWin.h"

namespace Win
{

class GlobalMem
{
  public:
    GlobalMem( DWORD nBytes, UINT flags = 0 )
    : _hGlobal(::GlobalAlloc(flags, nBytes))
    { if ( _hGlobal == 0 )
      { throw Win::Exception(TEXT("::GlobalAlloc failed"));
      }
    }

    ~GlobalMem( void )
    { if ( _hGlobal != 0 )
      { ::GlobalFree(_hGlobal);
      }
    }

    operator HGLOBAL( void ) const
    { return _hGlobal;
    }

  private:
    HGLOBAL _hGlobal;
};

template <class T>
class GlobalMemLock
{
  public:
    GlobalMemLock( HGLOBAL hMem )
    : _hMem(hMem),
      _p(reinterpret_cast<T*>(::GlobalLock(_hMem)))
    {}

    ~GlobalMemLock( void )
    { ::GlobalUnlock(_hMem);
    }
/*
    T* GetPtr( void )
    { return _p;
    }
*/
    operator T*( void )
    { return _p;
    }

  private:
    HGLOBAL _hMem;
    T*      _p;
};

} // namespace Win

#endif // WINGLOBALMEM_H