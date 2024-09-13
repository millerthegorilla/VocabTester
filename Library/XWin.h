#ifndef WINEXCEPTION_H
#define WINEXCEPTION_H

#include "stdstring"
#include <windows.h>

namespace Win
{

class Exception
{
  public:
    Exception( TCHAR const msg[] )
    : _msg(MakeMessage(::GetLastError(), msg))
    {}

    virtual ~Exception( void ) {}

    TCHAR const* Message( void ) const { return _msg.c_str(); }

  protected: // Methods
    Exception( void ) {}
    
    stdstring MakeMessage( DWORD code, TCHAR const msg[] );

  protected: // Data members
    stdstring _msg;
};

class XCom : public Exception
{
  public:
    XCom( HRESULT hr, TCHAR const msg[] )
    { _msg = MakeMessage(hr, msg);
    }
};

} // namespace Win

#endif // WINEXCEPTION_H