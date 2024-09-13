#ifndef WINMAKER_H
#define WINMAKER_H

#include "Window.h"
#include "XWin.h"

namespace Win
{

class Controller;

class CtrlFactory
{
  public:
    virtual Controller* MakeController( HWND hwnd ) = 0;
};

template <class ActualCtrl>
class ControllerFactory : public CtrlFactory
{
  public:
    ControllerFactory( void ) {}

    virtual Controller* MakeController( HWND hwnd )
    { return new ActualCtrl(hwnd);
    }
};

template <class ActualCtrl, class ActualArg>
class ControllerFactoryArg : public CtrlFactory
{
  public:
    ControllerFactoryArg( ActualArg arg )
    : _arg(arg)
    {}

    virtual Controller* MakeController( HWND hwnd )
    { return new ActualCtrl(hwnd, _arg);
    }

  private:
    ActualArg _arg;
};

class SimpleMaker
{
  public: // Methods
    void SetStyle( DWORD style ) { _style |= style; }
    void ResetStyle( DWORD style ) { _style &= ~style; }
    DWORD GetStyle( void ) const { return _style; }

    void SetExStyle( DWORD exStyle ) { _exStyle |= exStyle; }
    void ResetExStyle( DWORD exStyle ) { _exStyle &= ~exStyle; }
    DWORD GetExStyle( void ) const { return _exStyle; }

    void SetPosition( int x, int y ) { _x = x; _y = y; }
    void SetSize( int width, int height ) { _width = width; _height = height; }

    void SetParent( HWND parent ) { _parent = parent; }

  protected: // Methods
    SimpleMaker( TCHAR const className[], HINSTANCE hInst )
    : _hInst(hInst),          // program instance
      _className(className),  // name of Window class
      _style(WS_VISIBLE),     // window style
      _exStyle(0),            // window extended style
      _x(CW_USEDEFAULT),      // horizontal position of window
      _y(CW_USEDEFAULT),      // vertical position of window
      _width(CW_USEDEFAULT),  // window width 
      _height(CW_USEDEFAULT), // window height
      _parent(0),             // handle to parent or owner window
      _hMenu(0)               // handle to menu, or child-window id
    {}

  protected: // Data members
    HINSTANCE    _hInst;        // program instance
    TCHAR const* _className;    // name of Window class
    DWORD        _style;        // window style
    DWORD        _exStyle;      // window extended style
    int          _x;            // horizontal position of window
    int          _y;            // vertical position of window
    int          _width;        // window width 
    int          _height;       // window height
    HWND         _parent;       // handle to parent or owner window
    HMENU        _hMenu;        // handle to menu, or child-window id
};


class TopMaker : public SimpleMaker
{
  public:
    TopMaker( TCHAR const className[], HINSTANCE hInst )
    : SimpleMaker(className, hInst)
    { _style |= WS_OVERLAPPEDWINDOW;
    }

    HWND Create( TCHAR const title[], CtrlFactory& factory )
    {
      HWND hwnd = ::CreateWindowEx( _exStyle,
                                    _className,
                                    title,
                                    _style,
                                    _x, _y,
                                    _width, _height,
                                    _parent,
                                    _hMenu,
                                    _hInst,
                                    &factory );
      if ( hwnd == 0 )
      { throw Win::Exception(TEXT("Top window creation failed"));
      }
      return hwnd;
    }
};

class ControlParams
{
  public:
    int   id;
    DWORD style;
    DWORD exStyle;
    int   x;
    int   y;
    int   w;
    int   h;
};


class ControlMaker : public SimpleMaker
{
  public: // Methods
    ControlMaker( TCHAR const className[], Win::Dow parent, int id, DWORD style = WS_VISIBLE, DWORD exStyle = 0, int x = 0, int y = 0, int w = 0, int h = 0 )
    : SimpleMaker(className, parent.GetInstance())
    { _style = WS_CHILD|style;
      _exStyle = exStyle;
      _x = x;
      _y = y;
      _width = w;
      _height = h;
      _parent = parent;
      _hMenu = reinterpret_cast<HMENU>(id);
    }

    HWND Create( TCHAR const title[] = TEXT("") )
    { 
      HWND hwnd = ::CreateWindowEx( _exStyle,
                                    _className,
                                    title,
                                    _style,
                                    _x, _y,
                                    _width, _height,
                                    _parent,
                                    _hMenu,
                                    _hInst,
                                    0 );
      if ( hwnd == 0 )
      { throw Win::Exception(TEXT("Control window creation failed"));
      }
      return hwnd;
    }
};

template <class ActualNotifyHandler>
class ControlMakerHandler : public ControlMaker
{
  public:
    ControlMakerHandler( TCHAR const className[], Win::Dow parent, int id, DWORD style = WS_VISIBLE, DWORD exStyle = 0, int x = 0, int y = 0, int w = 0, int h = 0 )
    : ControlMaker(className, parent, id, style, exStyle, x, y, w, h)
    {}

    HWND Create( TCHAR const title[], ActualNotifyHandler* handler )
    { 
      HWND hwnd = ::CreateWindowEx( _exStyle,
                                    _className,
                                    title,
                                    _style,
                                    _x, _y,
                                    _width, _height,
                                    _parent,
                                    _hMenu,
                                    _hInst,
                                    0 );
      if ( hwnd == 0 )
      { throw Win::Exception(TEXT("Control window creation failed"));
      }
      Win::SetLong<ActualNotifyHandler*>(hwnd, handler);
      return hwnd;
    }
};

template <class ActualCtrl, TCHAR const ActualClassName[], class ActualNotifyHandler = Win::NotifyHandler>
class UserControlMaker : public Win::SimpleMaker
{
  public:
    UserControlMaker( Win::Dow parent, int id, DWORD style = WS_VISIBLE, DWORD exStyle = 0, int x = 0, int y = 0, int w = 0, int h = 0 )
    : SimpleMaker(ActualClassName, parent.GetInstance())
    { _style = WS_CHILD|style;
      _exStyle = exStyle;
      _x = x;
      _y = y;
      _width = w;
      _height = h;
      _parent = parent;
      _hMenu = reinterpret_cast<HMENU>(id);
    }

    // DO NOT DELETE
    ActualCtrl* Create( TCHAR const title[] = TEXT(""), ActualNotifyHandler* handler = 0 )
    { 
      ControllerFactory<ActualCtrl> factory;
      HWND hwnd = ::CreateWindowEx( _exStyle,
                                    _className,
                                    title,
                                    _style,
                                    _x, _y,
                                    _width, _height,
                                    _parent,
                                    _hMenu,
                                    _hInst,
                                    &factory );
      if ( hwnd == 0 )
      { throw Win::Exception(TEXT("Control window creation failed"));
      }
      Win::SetLong<ActualNotifyHandler*>(hwnd, handler);
      return Win::GetLong<ActualCtrl*>(hwnd, 0);
    }
};

} // namespace Win

#endif // WINMAKER_H