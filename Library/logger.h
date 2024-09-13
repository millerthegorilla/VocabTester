#ifndef LOGGER_H
#define LOGGER_H

#ifdef LOGGING

#include "stdstring"
#include "Templates/SmartPtr.h"
#include "Mailslot.h"

class LogDevice
{
  public:
    virtual void Log( stdstring const& msg ) = 0;
};

class SimpleLogger
{
  public:
    SimpleLogger( LogDevice& dev )
    : _dev(dev)
    {}

    template <typename T>
    SimpleLogger& operator<<( T t )
    { 
      try { _os << t; } catch ( ... ) {}
      return *this;
    }

    void EndMsg( void )
    { 
      try
      {
        _dev.Log(_os.str());
        _os.str(stdstring()); // Clear the output stream
      }
      catch ( ... )
      {}
    }

  protected:
    LogDevice&       _dev;
    stdostringstream _os;
};

class MailslotLogDevice : public LogDevice
{
  public:
    MailslotLogDevice( TCHAR const name[] = _T("deblogger") )
    : _client(name)
    {}

    virtual void Log( stdstring const& msg )
    {
      if ( _client.IsOk() )
      {
#ifdef UNICODE
        unsigned long size = sizeof(TCHAR) * msg.length() + 1;
        SArr<char> buf(new char[size]);
        buf[0] = 'U'; // UNICODE string follows
#else // ANSI
        unsigned long size = msg.length() + 1;
        SArr<char> buf(new char[size]);
        buf[0] = 'A'; // ANSI string follows
#endif // UNICODE
        ::memcpy(&buf[1], &msg[0], size - 1);
        _client.WriteMessage(buf, size);
      }
    }

  private:
    Win::Mailslot::Client _client;
};

class MailslotLogger : public SimpleLogger
{
  public:
    MailslotLogger( void )
    : SimpleLogger(_dev)
    {}

    ~MailslotLogger( void )
    {
      if ( !_os.str().empty() ) EndMsg();
    }

  private:
    MailslotLogDevice _dev;
};

typedef MailslotLogger Logger;

#endif // LOGGING

#endif // LOGGER_H