#ifndef WINVERSION_H
#define WINVERSION_H

#include <windows.h>

namespace Win
{

class Version
{
  public:
    Version( void )
    { _ver.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
      ::GetVersionEx(&_ver);
    }

    bool Win9x( void ) const { return _ver.dwPlatformId == VER_PLATFORM_WIN32_WINDOWS; }
    bool WinNT( void ) const { return _ver.dwPlatformId == VER_PLATFORM_WIN32_NT; }

  private:
    OSVERSIONINFO _ver;
};

} // namespace Win

#endif // WINVERSION_H