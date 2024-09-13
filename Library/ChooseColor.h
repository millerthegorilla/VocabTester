#ifndef WINCHOOSECOLOR_H
#define WINCHOOSECOLOR_H

#include <windows.h>

namespace Win
{

class PickColor
{
  public:
    PickColor( HWND hwnd )
    : _result(FALSE),
      _color(RGB(0,0,0))
    { CHOOSECOLOR c;
      ::ZeroMemory(&c, sizeof(c));
      c.lStructSize = sizeof(c);
      c.hwndOwner = hwnd;
      c.lpCustColors = _custom;
      c.Flags = CC_ANYCOLOR|CC_FULLOPEN;
      _result = ::ChooseColor(&c);
      _color = c.rgbResult;
    }

    PickColor( HWND hwnd, COLORREF initColor )
    : _result(FALSE),
      _color(initColor)
    { CHOOSECOLOR c;
      ::ZeroMemory(&c, sizeof(c));
      c.lStructSize = sizeof(c);
      c.hwndOwner = hwnd;
      c.lpCustColors = _custom;
      c.Flags = CC_ANYCOLOR|CC_FULLOPEN|CC_RGBINIT;
      c.rgbResult = _color;
      _result = ::ChooseColor(&c);
      _color = c.rgbResult;
    }

    bool IsOk( void ) const { return _result == TRUE; }
    COLORREF GetColor( void ) const { return _color; }

  private:
    BOOL            _result;
    COLORREF        _color;
    static COLORREF _custom[16];
};

} // namespace Win

#endif // WINCHOOSECOLOR_H