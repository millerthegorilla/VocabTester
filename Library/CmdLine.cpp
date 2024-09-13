#include "CmdLine.h"
#include <cctype>
#include <tchar.h>

namespace Win
{

CmdLine::CmdLine( void )
: _cmdLine(::GetCommandLine())
{
  stdstring::size_type pos = 0;
  if ( _cmdLine[0] == _T('\"') )
  { _cmdLine.erase(0, 1);
    pos = _cmdLine.find(_T('\"'));
  }
  else
  { pos = _cmdLine.find(_T(' '));
  }

  if ( pos != stdstring::npos )
  { _cmdLine.erase(0, pos + 1);
  }
  else
  { _cmdLine.replace(0, stdstring::npos, _T(""));
  }
  
  while ( _cmdLine[0] != _T('\0') && _cmdLine[0] == _T(' ') )
  { _cmdLine.erase(0, 1);
  }
}

CommandLine::CommandLine( void )
{
  TCHAR const* cmdLine = ::GetCommandLine();
  assert(cmdLine != 0);

  stdstring arg;
  bool inQuotes = false;
  for ( unsigned long i = 0; cmdLine[i] != _T('\0'); ++i )
  {
    if ( ::_istspace(cmdLine[i]) )
    {
      if ( inQuotes )
      {
        arg.append(1, cmdLine[i]);
      }
      else if ( arg.length() > 0 )
      {
        _args.PushBack(arg);
        arg.erase();
      }
    }
    else if ( cmdLine[i] == _T('\"') )
    {
      inQuotes = !inQuotes;
    }
    else
    {
      arg.append(1, cmdLine[i]);
    }
  }

  if ( arg.length() > 0 )
  {
    _args.PushBack(arg);
  }
}

} // namespace Win