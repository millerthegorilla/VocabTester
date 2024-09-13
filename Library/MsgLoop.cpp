#include "MsgLoop.h"
#include "XWin.h"

using namespace Win;

WPARAM MsgLoop::Pump( void )
{
  MSG  msg;
  int status;
  while ( (status = ::GetMessage(&msg, 0, 0, 0)) != 0 )
  {
    if ( status == -1 )
    { throw Win::Exception(TEXT("::GetMessage failed"));
    }
    
    std::list<HWND>::iterator it = _dialogs.begin();
		for ( ; it != _dialogs.end(); ++it )
			if ( ::IsDialogMessage(*it, &msg) )
      { break;
      }

		if ( it == _dialogs.end() )
		{ if ( _mdiClient != 0 && ::TranslateMDISysAccel(_mdiClient, &msg) )
      { continue;
      }

      if ( _translateAccel )
      { if ( _hAccel != 0 && ::TranslateAccelerator(_topWin, _hAccel, &msg) )
        { continue;
        }
      }

      ::TranslateMessage(&msg);
      ::DispatchMessage(&msg);
    }
  }
  return msg.wParam;
}
