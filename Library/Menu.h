#ifndef WINMENU_H
#define WINMENU_H

#include <windows.h>

namespace Win
{

class SimpleMenu
{
  public: // Methods
    HMENU GetSubMenu( int nPos )
    { return ::GetSubMenu(_hMenu, nPos);
    }
    
    void AddString( TCHAR const itemText[], UINT itemId )
    { ::AppendMenu(_hMenu, MF_STRING, itemId, itemText);
    }
    
    void AddSeparator( void )
    { ::AppendMenu(_hMenu, MF_SEPARATOR, 0, 0);
    }

    void GrayItem( UINT itemId, UINT uFlags = MF_BYCOMMAND )
    { ::EnableMenuItem(_hMenu, itemId, uFlags|MF_GRAYED);
    }

    void EnableItem( UINT itemId, bool enable = true, UINT uFlags = MF_BYCOMMAND )
    { ::EnableMenuItem(_hMenu, itemId, uFlags|(enable ? MF_ENABLED : MF_GRAYED));
    }

    void DisableItem( UINT itemId, UINT uFlags = MF_BYCOMMAND )
    { EnableItem(itemId, false, uFlags);
    }

    void CheckItem( UINT itemId, bool check = true, UINT uFlags = MF_BYCOMMAND )
    { ::CheckMenuItem(_hMenu, itemId, uFlags|(check ? MF_CHECKED : MF_UNCHECKED));
    }

    void UnCheckItem( UINT itemId, UINT uFlags = MF_BYCOMMAND )
    { CheckItem(itemId, false, uFlags);
    }
    
    operator HMENU( void ) const
    { return _hMenu;
    }

  protected: // Methods
    SimpleMenu( HMENU hMenu = 0 )
    : _hMenu(hMenu)
    {}

  protected: // Data members
    HMENU _hMenu;
};

class Menu : public SimpleMenu
{
  public: // Methods
    ~Menu( void )
    { Destroy();
    }

    void Destroy( void )
    { if ( _hMenu != 0 )
      { ::DestroyMenu(_hMenu);
      }
      _hMenu = 0;
    }

  protected:
    Menu( HMENU hMenu = 0 )
    : SimpleMenu(hMenu)
    {}
};

class PopupMenu : public Menu
{
  public:
    PopupMenu( void )
    : Menu(::CreatePopupMenu())
    {}

    int TrackMenu( HWND hwnd, POINT pt, UINT uFlags = 0 )
    { return ::TrackPopupMenu(_hMenu, uFlags|TPM_LEFTALIGN|TPM_TOPALIGN, pt.x, pt.y, 0, hwnd, 0);
    }

    int TrackMenu( HWND hwnd, int x, int y, UINT uFlags = 0 )
    { return ::TrackPopupMenu(_hMenu, uFlags|TPM_LEFTALIGN|TPM_TOPALIGN, x, y, 0, hwnd, 0);
    }
};

class StrangerMenu : public SimpleMenu
{
  public:
    StrangerMenu( HMENU hMenu )
    : SimpleMenu(hMenu)
    {}
};

class StrangerPopupMenu : public StrangerMenu
{
  public:
    StrangerPopupMenu( HMENU hMenu )
    : StrangerMenu(hMenu)
    {}

    int TrackMenu( HWND hwnd, POINT pt, UINT uFlags = 0 )
    { return ::TrackPopupMenu(_hMenu, uFlags|TPM_LEFTALIGN|TPM_TOPALIGN, pt.x, pt.y, 0, hwnd, 0);
    }

    int TrackMenu( HWND hwnd, int x, int y, UINT uFlags = 0 )
    { return ::TrackPopupMenu(_hMenu, uFlags|TPM_LEFTALIGN|TPM_TOPALIGN, x, y, 0, hwnd, 0);
    }
};


} // namespace Win

#endif // WINMENU_H