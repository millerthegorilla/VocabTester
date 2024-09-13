#include "XWin.h"
#include <tchar.h>

using namespace Win;

class ErrorMessage
{
  public:
    ErrorMessage( DWORD code )
    : _buf(0),
      _isOk(false)
    { 
      _isOk = ::FormatMessage(
                  FORMAT_MESSAGE_ALLOCATE_BUFFER|FORMAT_MESSAGE_FROM_SYSTEM,
                  0,
                  code,
                  0,
                  reinterpret_cast<TCHAR*>(&_buf),
                  0,
                  0) != 0;
                  
      if ( IsOk() )
      { // Remove line breaks - yes they are there :-(
        int last = ::_tcslen(_buf) - 1;
        while ( last >= 0 && (_buf[last] == _T('\n') || _buf[last] == _T('\r')) )
        { _buf[last] = _T('\0');
          --last;
        }
      }
    }
    
    ~ErrorMessage( void )
    {
      if ( IsOk() )
      { ::LocalFree(_buf);
      }
    }
    
    bool IsOk( void ) const { return _isOk; }

    TCHAR const* Message( void ) const { return _buf; }
    
  private: // Data members
    TCHAR* _buf;
    bool   _isOk;
};

stdstring Exception::MakeMessage( DWORD code, TCHAR const msg[] )
{
  stdstring message(msg);

  // To protect against following error message:
  // ~~~
  // Some horrible error (The operation completed successfuly)
  // ~~~
  // Some functions just don't set the error code ...
  if ( code == NOERROR )
  { return message;
  }

  ErrorMessage errMsg(code);
  if ( errMsg.IsOk() )
  { message += _T(" (");
    message += errMsg.Message();
    message += _T(")");
  }
  else
  { message += _T(" (and ::FormatMessage failed too)");
  }

  return message;
}