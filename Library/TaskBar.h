#ifndef WINTASKBAR_H
#define WINTASKBAR_H

#include "Shell.h"
#include "TaskBar.h"

namespace Win
{
/*
class TaskBar : public SIFacePtr<ITaskbarList>
{
  public:
    TaskBar( void )
    { HRESULT hr = ::CoCreateInstance(CLSID_TaskbarList, 0, CLSCTX_SERVER, IID_ITaskbarList, (void**)&_p);
      if ( FAILED(hr) )
      { throw XCom(hr, TEXT("Cannot create the TaskBar object"));
      }
      hr = _p->HrInit();
      if ( FAILED(hr) )
      { Free();
        throw XCom(hr, TEXT("TaskBar initialization failed"));
      }
    }
};
*/
class StatusArea
{
  public:
    static void AddIcon( HICON hIcon, HWND hwnd, UINT id, TCHAR const tip[] = TEXT("") )
    { IconData data(hIcon, hwnd, id, tip);
      ::Shell_NotifyIcon(NIM_ADD, &data);
    }

    static void AddIcon( HICON hIcon, HWND hwnd, UINT id, TCHAR const tip[], UINT callbackId )
    { IconData data(hIcon, hwnd, id, tip, callbackId);
      ::Shell_NotifyIcon(NIM_ADD, &data);
    }


    static void DeleteIcon( HWND hwnd, UINT id )
    { IconData data(hwnd, id);
      ::Shell_NotifyIcon(NIM_DELETE, &data);
    }

    static void SetTip( HWND hwnd, UINT id, TCHAR const tip[] )
    { IconData data(hwnd, id, tip);
      ::Shell_NotifyIcon(NIM_MODIFY, &data);
    }

    static void SetIcon( HWND hwnd, UINT id, HICON hIcon )
    { IconData data(hIcon, hwnd, id);
      ::Shell_NotifyIcon(NIM_MODIFY, &data);
    }

  private: // Class
    class IconData : public NOTIFYICONDATA
    {
      public:
        IconData( void )
        { ::ZeroMemory(this, sizeof(NOTIFYICONDATA));
          cbSize = sizeof(NOTIFYICONDATA); 
          uID = 0xFFFFFFFF; 
        }

        IconData( HWND hwnd, UINT id )
        { ::ZeroMemory(this, sizeof(NOTIFYICONDATA));
          cbSize = sizeof(NOTIFYICONDATA); 
          hWnd = hwnd; 
          uID = id; 
        }

        IconData( HWND hwnd, UINT id, TCHAR const tip[] )
        { ::ZeroMemory(this, sizeof(NOTIFYICONDATA));
          cbSize = sizeof(NOTIFYICONDATA); 
          hWnd = hwnd; 
          uID = id; 
          uFlags = NIF_TIP; 
          ::_tcsncpy(szTip, tip, 63);
        }

        IconData( HICON icon, HWND hwnd, UINT id )
        { ::ZeroMemory(this, sizeof(NOTIFYICONDATA));
          cbSize = sizeof(NOTIFYICONDATA); 
          hWnd = hwnd; 
          uID = id; 
          uFlags = NIF_ICON; 
          hIcon = icon;
        }

        IconData( HICON icon, HWND hwnd, UINT id, TCHAR const tip[] )
        { ::ZeroMemory(this, sizeof(NOTIFYICONDATA));
          cbSize = sizeof(NOTIFYICONDATA); 
          hWnd = hwnd; 
          uID = id; 
          uFlags = NIF_ICON|NIF_TIP; 
          hIcon = icon;
          ::_tcsncpy(szTip, tip, 63);
        }

        IconData( HICON icon, HWND hwnd, UINT id, TCHAR const tip[], UINT callbackId )
        { ::ZeroMemory(this, sizeof(NOTIFYICONDATA));
          cbSize = sizeof(NOTIFYICONDATA); 
          hWnd = hwnd; 
          uID = id; 
          uFlags = NIF_ICON|NIF_TIP|NIF_MESSAGE;
          uCallbackMessage = callbackId; 
          hIcon = icon;
          ::_tcsncpy(szTip, tip, 63);
        }
    };
};

} // namespace Win

#endif // WINTASKBAR_H