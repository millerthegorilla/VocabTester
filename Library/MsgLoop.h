#ifndef WINMESSAGELOOP_H
#define WINMESSAGELOOP_H

#include <list>
#include "Window.h"

namespace Win
{

class MsgLoop
{
  public:
    MsgLoop( void )
    : _hAccel(0),
      _translateAccel(true),
      _topWin(0),
      _mdiClient(0)
    {}

    void SetAccelerator( HWND topWin, int resId ) 
    { _topWin.Init(topWin);
      _hAccel = ::LoadAccelerators(_topWin.GetInstance(), MAKEINTRESOURCE(resId));
    }

    void SetAccelerator( HWND topWin, HACCEL hAccel )
    { _topWin.Init(topWin);
      _hAccel = hAccel;
    }

    void EnableAccelerator( bool enable = true )
    { _translateAccel = enable;
    }

    void DisableAccelerator( void )
    { EnableAccelerator(false);
    }

    void SetMDIClient( HWND mdiClient )
    { _mdiClient.Init(mdiClient); 
    }

		// Use with modeless dialogs
    void AddDialogFilter( HWND dlg ) 
    { _dialogs.push_back(dlg);
    }

    void RemoveDialogFilter( HWND dlg )
    { _dialogs.remove(dlg);
    }

    WPARAM Pump( void );

  private:
    std::list<HWND> _dialogs;
    HACCEL          _hAccel;
    bool            _translateAccel;
    Win::Dow        _topWin;
    Win::Dow        _mdiClient;
};

} // namespace Win

#endif // WINMESSAGELOOP_H
