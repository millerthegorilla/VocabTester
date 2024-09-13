#ifndef WINPROGRESSBAR_H
#define WINPROGRESSBAR_H

#include "Window.h"
#include "NotifyHandler.h"
#include "Maker.h"
#include <commctrl.h>

namespace Win
{

class ProgressBar : public Win::Dow
{
  public:
    explicit ProgressBar( HWND hwnd = 0 )
    : Win::Dow(hwnd)
    {}

    void Advance( int nSteps )
    { SendMessage(PBM_DELTAPOS, nSteps);
    }

    void Advance( void )
    { SendMessage(PBM_STEPIT);
    }

    int GetPosition( void ) const
    { return SendMessage(PBM_GETPOS);
    }

    int GetLow( void ) const
    { return SendMessage(PBM_GETRANGE, TRUE);
    }

    int GetHigh( void ) const
    { return SendMessage(PBM_GETRANGE, FALSE);
    }

    void SetBarColor( COLORREF color )
    { SendMessage(PBM_SETBARCOLOR, 0, color);
    }

    void SetBkColor( COLORREF color )
    { SendMessage(PBM_SETBKCOLOR, 0, color);
    }

    void SetPosition( int pos )
    { SendMessage(PBM_SETPOS, pos);
    }

    void SetRange( int low = 0, int high = 100 )
    { SendMessage(PBM_SETRANGE, 0, MAKELPARAM(low, high));
    }

    void SetStep( int step )
    { SendMessage(PBM_SETSTEP, step);
    }
};

class ProgressBarMaker : public Win::ControlMakerHandler<NotifyHandler>
{
  public:
    ProgressBarMaker( Win::Dow parent, int id, DWORD style = WS_VISIBLE|PBS_SMOOTH, DWORD exStyle = 0, int x = 0, int y = 0, int w = 0, int h = 0 )
    : ControlMakerHandler<NotifyHandler>(PROGRESS_CLASS, parent, id, style, exStyle, x, y, w, h)
    {}
};

} // namespace Win

#endif // WINPROGRESSBAR_H