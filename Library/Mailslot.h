#ifndef WINMAILSLOT_H
#define WINMAILSLOT_H

#include <windows.h>
#include <tchar.h>

namespace Win
{
namespace Mailslot
{

class Server
{
  public:
    Server( TCHAR const name[] );
    ~Server( void );

    bool IsOk( void ) const { return _h != INVALID_HANDLE_VALUE; }

    // Returns true if there is a message pending
    bool HasMessage( unsigned long& msgSize ) const;

    // Returns amount of bytes actually read from a mailslot
    unsigned long ReadMessage( void* buf, unsigned long bufSize );

  private:
    HANDLE _h;
};

class Client
{
  public:
    Client( TCHAR const name[] );
    ~Client( void );

    bool IsOk( void ) const { return _h != INVALID_HANDLE_VALUE; }
    
    unsigned long WriteMessage( void const* buf, unsigned long bufSize );

  private:
    HANDLE _h;
};

} // namespace Mailslot

} // namespace Win

#endif // WINMAILSLOT_H