#ifndef WINDOUBLEZEROSTRING_H
#define WINDOUBLEZEROSTRING_H

#include "Templates/SmartPtr.h"
#include <windows.h>
#include <tchar.h>

namespace Win
{

class DoubleZeroString
{
  public:
    DoubleZeroString( void )
    : _size(INITIAL_SIZE),
      _length(0),
      _str(new TCHAR[_size]),
      _count(0)
    { _str[0] = _str[1] = TEXT('\0');
    }

    TCHAR const* AddString( TCHAR const str[] )
    { int len = ::_tcslen(str);
      if ( _length + len + 2 > _size )
      { Grow(_length + len + 2);
      }
      ::memcpy(&_str[_length], str, len * sizeof(TCHAR));
      _length += len;
      _str[_length] = TEXT('\0'); // first zero
      ++_length;
      _str[_length] = TEXT('\0'); // second zero
      ++_count;
      return _str;
    }
    
    void Clear( void )
    { _length = _count = 0;
      _str[0] = _str[1] = TEXT('\0');
    }

    int GetStringCount( void ) const
    { return _count;
    }
 
    // With all zeros !!
    int GetStringSize( void ) const
    { return _length == 0 ? 2 : _length + 1;
    }

    operator TCHAR const*( void ) const
    { return _str; 
    }

  private: // Enums
    enum { INITIAL_SIZE = 256 };

  private: // Methods
    void Grow( int reqSize )
    { int newSize = 2 * _size;
      newSize = (newSize < reqSize ? reqSize : newSize);
      SArr<TCHAR> newStr(new TCHAR[newSize]);
      ::memcpy(newStr, _str, _size * sizeof(TCHAR));
      _str = newStr; // Transfer semantic !!
      _size = newSize;
    }

  private: // Data members
    int         _size;
    int         _length;
    SArr<TCHAR> _str;
    int         _count;
};

} // namespace Win

#endif // WINDOUBLEZEROSTRING_H