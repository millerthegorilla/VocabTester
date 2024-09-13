#include "Mailslot.h"
#include "XWin.h"
#include "stdstring"
#include <cassert>

namespace Win
{
namespace Mailslot
{

//
// Class Server
// ~~~~~~~~~~~~
Server::Server( TCHAR const name[] )
{
  stdstring mailslot(_T("\\\\.\\mailslot\\"));
  mailslot += name;
  _h = ::CreateMailslot(mailslot.c_str(), 0, 0, 0);
}

Server::~Server( void )
{
  if ( IsOk() ) ::CloseHandle(_h);
}

// Returns true if there is a message pending
bool Server::HasMessage( unsigned long& msgSize ) const
{
  assert(IsOk());
  if ( ::GetMailslotInfo(_h, 0, &msgSize, 0, 0) == 0 )
  { throw Win::Exception(_T("Win::Mailslot::Server::HasMessage: couldn't obtain mailslot information"));
  }
  if ( msgSize == MAILSLOT_NO_MESSAGE )
  { msgSize = 0;
    return false;
  }
  return true;
}

// Returns amount of bytes actually read from a mailslot
unsigned long Server::ReadMessage( void* buf, unsigned long bufSize )
{
  assert(IsOk());
  unsigned long bytesRead;
  if ( ::ReadFile(_h, buf, bufSize, &bytesRead, 0) == 0 )
  { throw Win::Exception(_T("Win::Mailslot::Server::ReadMessage: couldn't read next message from a mailslot"));
  }
  return bytesRead;
}

//
// Class Client
// ~~~~~~~~~~~~
Client::Client( TCHAR const name[] )
{
  stdstring mailslot(_T("\\\\.\\mailslot\\"));
  mailslot += name;
  _h = ::CreateFile(mailslot.c_str(), GENERIC_WRITE, FILE_SHARE_READ, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0); 
}

Client::~Client( void )
{
  if ( IsOk() ) ::CloseHandle(_h);
}

unsigned long Client::WriteMessage( void const* buf, unsigned long bufSize )
{
  assert(IsOk());
  unsigned long bytesWritten;
  if ( ::WriteFile(_h, buf, bufSize, &bytesWritten, 0) == 0 )
  { throw Win::Exception(_T("Win::Mailslot::Client::WriteMessage: couldn't put a message to a mailslot"));
  }
  return bytesWritten;
}

} // namespace Mailslot
} // namespace Win