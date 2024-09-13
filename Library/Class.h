#ifndef WINCLASS_H
#define WINCLASS_H

#include <windows.h>
#include "XWin.h"

namespace Win
{

class SimpleClass
{
  public:
    void Register( void )
    { if ( ::RegisterClassEx(&_class) == 0 )
      { throw Win::Exception(TEXT("::RegisterClassEx failed"));
      }
    }

    void SetStyle( UINT style )
    { _class.style |= style;
    }

    void ResetStyle( UINT style )
    { _class.style &= ~style;
    }

    void SetIcons( int resId )
    { _class.hIcon = reinterpret_cast<HICON>(::LoadImage(_class.hInstance,
                                                         MAKEINTRESOURCE(resId),
                                                         IMAGE_ICON,
                                                         ::GetSystemMetrics(SM_CXICON),
                                                         ::GetSystemMetrics(SM_CYICON),
                                                         LR_DEFAULTCOLOR));
      _class.hIconSm = reinterpret_cast<HICON>(::LoadImage(_class.hInstance,
                                                           MAKEINTRESOURCE(resId),
                                                           IMAGE_ICON,
                                                           ::GetSystemMetrics(SM_CXSMICON),
                                                           ::GetSystemMetrics(SM_CYSMICON),
                                                           LR_DEFAULTCOLOR));
    }

    void SetCursor( int resId )
    { _class.hCursor = ::LoadCursor(_class.hInstance, MAKEINTRESOURCE(resId));
    }

    void SetSysCursor( TCHAR* id )
    { _class.hCursor = ::LoadCursor(0, id);
    }

    void SetBkColor( COLORREF c )
    { _class.hbrBackground = ::CreateSolidBrush(c);
    }

    void SetSysBkColor( int id )
    { _class.hbrBackground = reinterpret_cast<HBRUSH>(id + 1);
    }

    void SetMenu( int resId )
    { _class.lpszMenuName = MAKEINTRESOURCE(resId);
    }

    HWND GetRunningWindow( void )
    { HWND hwnd = ::FindWindow(_class.lpszClassName , 0);
      if ( ::IsWindow(hwnd) )
      { HWND hwndPopup = ::GetLastActivePopup(hwnd);
        if ( ::IsWindow(hwndPopup) ) hwnd = hwndPopup;
      }
      else
      { hwnd = 0;
      }
      return hwnd;
    }

  protected: // Methods
    SimpleClass( TCHAR const className[], HINSTANCE hInst, WNDPROC winProc )
    {
      _class.lpfnWndProc = winProc;      // window procedure: mandatory
      _class.hInstance = hInst;          // owner of the class: mandatory
      _class.lpszClassName = className;  // mandatory
      _class.cbSize = sizeof(WNDCLASSEX);
      _class.hCursor = ::LoadCursor(0, IDC_ARROW);
      _class.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1);
      _class.style = 0;
      _class.cbClsExtra = 0;
      _class.cbWndExtra = 4; // 4 bytes for Controllers' pointer
      _class.hIcon = 0;
      _class.hIconSm = 0;
      _class.lpszMenuName = 0;
    }

  protected: // Data members
    WNDCLASSEX _class;
};

class Class : public SimpleClass
{
  public:
    Class( TCHAR const className[], HINSTANCE hInst )
    : SimpleClass(className, hInst, WndProc)
    {}

  private:
    static LRESULT CALLBACK WndProc( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam );
};

} // namespace Win

#endif // WINCLASS_H