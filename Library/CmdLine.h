#ifndef WINCMDLINE_H
#define WINCMDLINE_H

#include "Templates/Vector.h"
#include <windows.h>
#include "stdstring"

namespace Win
{

class CmdLine
{
  public:
    CmdLine( void );

	stdstring GetCmdLine( void ) const { return _cmdLine; }
    operator TCHAR const*( void ) const { return _cmdLine.c_str(); }

  private:
	stdstring _cmdLine;
};

class CommandLine
{
  public:
    CommandLine( void );

	stdstring const& operator[]( unsigned long i ) const
    {
      assert(i < _args.Size());
      return _args[i];
    }

    unsigned long NoArgs( void ) const throw () { return _args.Size(); }

  private:
	  Vector<stdstring> _args;
};



} // namespace Win

#endif // WINCMDLINE_H