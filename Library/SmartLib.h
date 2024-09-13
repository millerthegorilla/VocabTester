#ifndef WINSMARTLIB_H
#define WINSMARTLIB_H

#include <windows.h>

namespace Win
{

class SLib
{
  public:
    SLib( void )
    : _hInst(0)
    {}

    SLib( TCHAR const path[] )
    : _hInst(0)
    { Load(path);
    }
    
    ~SLib( void )
    { Free();
    }

    // Transfer semantic
    SLib( SLib& b )
    : _hInst(b.Release())
    {}

    SLib& operator=( SLib& b )
    { if ( this != &b )
      { Free();
        _hInst = b.Release();
      }
      return *this;
    }

    HINSTANCE Release( void )
    { HINSTANCE hTemp = _hInst;
      _hInst = 0;
      return hTemp;
    }

    bool IsOk( void ) const
    { return _hInst != 0; 
    }

    FARPROC GetProcAddr( char const name[] ) const
    { return ::GetProcAddress(_hInst, name);
    }

    void Load( TCHAR const path[] )
    { Free();
      _hInst = ::LoadLibrary(path);
    }

    void Free( void )
    { if ( _hInst != 0 )
      { ::FreeLibrary(_hInst);
      }
      _hInst = 0;
    }
    
    HINSTANCE GetInst( void ) const
    { return _hInst; 
    }

    operator HINSTANCE( void ) const
    { return _hInst; 
    }

  protected: // Data members
    HINSTANCE _hInst;
};

template < class T >
class SProc
{
  public:
    SProc( void )
    : _proc(0)
    {}

    SProc( T proc )
    : _proc(proc)
    {}

    SProc( FARPROC proc )
    : _proc(reinterpret_cast<T>(proc))
    {}
        
    // Transfer semantic
    SProc( SProc<T>& b )
    : _proc(b.Release())
    {}

    SProc<T>& operator=( SProc<T>& b )
    { if ( this != &b )
      { _proc = b.Release();
      }
      return *this;
    }

    T Release( void )
    { T temp = _proc;
      _proc = 0;
      return temp;
    }

    bool IsOk( void ) const
    { return _proc != 0; 
    }

    T operator*( void ) const
    { return _proc; 
    }

    operator T( void ) const
    { return _proc; 
    }

  private:
    T _proc;
};

} // namespace Win

#endif // WINSMARTLIB_H