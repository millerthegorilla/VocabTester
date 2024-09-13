#include "FolderWatcher.h"

using namespace Win;

void FolderWatcher::FolderChanged( void )
{ 
  if ( _lock == 0 )
  { if ( _sink != 0 )
    { _sink->FolderChanged(_id);
    }
    else if ( _hwnd != 0 && _hdr.hwndFrom != 0 )
    { ::PostMessage(_hwnd, _msg, MAKELPARAM(_hdr.idFrom,_hdr.code), reinterpret_cast<LPARAM>(_hdr.hwndFrom));
    }
    else if ( _hwnd != 0 )
    { ::PostMessage(_hwnd, _msg, _id, 0);
    }
  }
}

void FolderWatcher::Watcher::Run( void )
{
  ::SetThreadPriority(::GetCurrentThread(), THREAD_PRIORITY_IDLE);
  for ( ;; )
  {
    HANDLE const handles[] = { _handle, _event };
    DWORD result = ::WaitForMultipleObjects(2, handles, FALSE, INFINITE);
    ::FindNextChangeNotification(_handle);
    if ( _isDying )
    { return;
    }
    if ( result - WAIT_OBJECT_0 != 0 )
    { continue;
    }
    do
    { result = ::WaitForMultipleObjects(2, handles, FALSE, 500);
      ::FindNextChangeNotification(_handle);
      if ( _isDying )
      { return;
      }
    } while ( result - WAIT_OBJECT_0 == 0 );
    _sink->FolderChanged();
  }
}
