#ifndef WINTIMER_H
#define WINTIMER_H

#include <windows.h>

namespace Win
{

class Timer
{
  public:
    Timer( HWND hwnd = 0, UINT id = 0 )
    : _hwnd(hwnd),
      _id(id),
      _ID(0),
      _set(false)
    {}

    ~Timer( void )
    { Kill(); 
    }

    void Init( HWND hwnd, UINT id )
    { _hwnd = hwnd; 
      _id = id; 
    }

    void Set( UINT ms )
    { Kill();
      _ID = ::SetTimer(_hwnd, _id, ms, 0); 
      _set = true;
    }

    void Kill( void )
    { if ( _ID != 0 )
      { ::KillTimer(_hwnd, _ID);
      }
      _ID = 0;
      _set = false;
    }

    bool IsSet( void ) const { return _set; }

    bool operator==( UINT id ) const { return _ID == id; }

  private:
    HWND _hwnd;
    UINT _id;
    UINT _ID;
    bool _set;
};

} // namespace Win

#endif // WINTIMER_H