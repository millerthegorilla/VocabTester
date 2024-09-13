#ifndef WINANIMATION_H
#define WINANIMATION_H

#include "Window.h"
#include "NotifyHandler.h"
#include "Maker.h"

namespace Win
{

class Animation : public Win::Dow
{
  public: // Methods
    explicit Animation( HWND hwnd = 0 )
    : Win::Dow(hwnd)
    {}

    Animation( HWND dlg, int id )
    : Win::Dow(::GetDlgItem(dlg, id))
    {}

    void Open( int resId )
    { SendMessage(ACM_OPEN, 0, reinterpret_cast<LPARAM>(MAKEINTRESOURCE(resId)));
    }

    void Close( void )
    { SendMessage(ACM_OPEN);
    }

    void Play( int nTimes = -1, int from = 0, int to = -1 )
    { SendMessage(ACM_PLAY, nTimes, MAKELONG(from, to));
    }

    void Stop( void )
    { SendMessage(ACM_STOP);
    }
};

typedef CommonNotifyHandler AnimationNotifyHandler;

class AnimationMaker : public ControlMakerHandler<AnimationNotifyHandler>
{
  public:
    AnimationMaker( Win::Dow parent, int id, DWORD style = WS_VISIBLE|ACS_AUTOPLAY|ACS_TRANSPARENT, DWORD exStyle = 0, int x = 0, int y = 0, int w = 0, int h = 0 )
    : ControlMakerHandler<AnimationNotifyHandler>(ANIMATE_CLASS, parent, id, style, exStyle, x, y, w, h)
    {}
};

} // namespace Win

#endif // WINANIMATION_H