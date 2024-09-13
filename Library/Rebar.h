#ifndef WINREBAR_H
#define WINREBAR_H

#include "Window.h"
#include "NotifyHandler.h"
#include "Maker.h"
#include <commctrl.h>

namespace Win
{

class Rebar : public Win::Dow
{
  public: // Class
    class BandInfo : public REBARBANDINFO
    {
      public:
        BandInfo( HWND child, int childMinCx, int childMinCy, int width )
        { ::memset(this, 0, sizeof(REBARBANDINFO));
          cbSize = sizeof(REBARBANDINFO);
          fMask |= RBBIM_CHILD|RBBIM_CHILDSIZE;
          hwndChild = child;
          cxMinChild = childMinCx;
          cyMinChild = childMinCy;
          cx = width;
        }

        BandInfo( HWND child, int childMinCx, int childMinCy, int width, TCHAR const text[] )
        { ::memset(this, 0, sizeof(REBARBANDINFO));
          cbSize = sizeof(REBARBANDINFO);
          fMask |= RBBIM_CHILD|RBBIM_SIZE|RBBIM_CHILDSIZE|RBBIM_TEXT;
          hwndChild = child;
          cxMinChild = childMinCx;
          cyMinChild = childMinCy;
          cx = width;
          lpText = const_cast<TCHAR*>(text);
        }

        void SetStyle( unsigned style )
        { fMask |= RBBIM_STYLE;
          fStyle |= style;
        }
    };

  public:
    explicit Rebar( HWND hwnd = 0 )
    : Win::Dow(hwnd)
    {}

    int GetHeight( void ) const
    { return SendMessage(RB_GETBARHEIGHT);
    }

    void InsertBand( REBARBANDINFO const& info, int idx = -1 )
    { SendMessage(RB_INSERTBAND, idx, reinterpret_cast<LPARAM>(&info));
    }

    void ShowBand( int bandIdx, bool show = true )
    { SendMessage(RB_SHOWBAND, bandIdx, show);
    }

    void HideBand( int bandIdx )
    { ShowBand(bandIdx, false);
    }

    void MaximizeBand( int bandIdx, bool ideal = true )
    { SendMessage(RB_MAXIMIZEBAND, bandIdx, ideal);
    }

    void MinimizeBand( int bandIdx )
    { SendMessage(RB_MINIMIZEBAND, bandIdx);
    }
};

class RebarNotifyHandler : public CommonNotifyHandler
{
  public:
    // Overriden methods
    virtual bool OnNotify( NMHDR* hdr, LRESULT& result );

    // New methods
    virtual bool OnAutoSize( HWND hwnd, NMRBAUTOSIZE const& info ) { return false; }
};

class RebarMaker : public Win::ControlMakerHandler<RebarNotifyHandler>
{
  public:
    RebarMaker( Win::Dow parent, int id, DWORD style = WS_VISIBLE|RBS_AUTOSIZE, DWORD exStyle = WS_EX_TOOLWINDOW, int x = 0, int y = 0, int w = 0, int h = 0 )
    : ControlMakerHandler<RebarNotifyHandler>(REBARCLASSNAME, parent, id, style, exStyle, x, y, w, h)
    {}
};

} // namespace Win

#endif // WINREBAR_H