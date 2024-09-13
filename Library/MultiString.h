#ifndef WINMULTISTRING_H
#define WINMULTISTRING_H

#include "Templates/SmartPtr.h"
#include "stdstring"
#include <windows.h>
#include <tchar.h>

namespace Win
{

class MultiString
{
  public:
    MultiString( void )
    : _size(0),
      _length(0),
      _count(0)
    {}

    MultiString& AddString( TCHAR const str[] )
    { 
      return AddString(str, ::_tcslen(str));
    }

    MultiString& operator<<( TCHAR const str[] )
    { 
      return AddString(str);
    }

    MultiString& AddString( stdstring const& str )
    {
      return AddString(str.c_str(), str.length());
    }

    MultiString& operator<<( stdstring const& str )
    {
      return AddString(str);
    }

    MultiString& AddString( TCHAR const str[], unsigned long len )
    { 
      if ( _length + len + 2 > _size )
      { 
        Grow(_length + len + 2);
      }
      
      ::memcpy(&_str[_length], str, len * sizeof(TCHAR));
      
      _length += len;
      
      _str[_length] = _T('\0'); // first zero
      
      ++_length;
      
      _str[_length] = _T('\0'); // second zero

      // We don't increment the _length here because we want the last zero be
      // overwritten by the first character of the next string !!
      
      ++_count;
      
      return *this;
    }
    
    void Clear( void )
    { 
      _size = _length = _count = 0;
      _str = SArr<TCHAR>();
    }

    unsigned long GetStringCount( void ) const
    { 
      return _count;
    }
 
    // With all zeros and in CHARACTERS (not bytes) !!
    unsigned long GetStringSize( void ) const
    { 
      return (_length == 0 ? 2 : _length + 1);
    }

    TCHAR const* GetString( void ) const
    {
      return (_str == 0 ? Nullstr() : _str); 
    }

    operator TCHAR const*( void ) const
    { 
      return GetString(); 
    }

  private: // Enums
    enum { GROWBY = 256 };

  private: // Methods
    // No copy please
    MultiString( MultiString const& b );
    MultiString& operator=( MultiString const& b );


    static TCHAR const* Nullstr( void )
    {
      static TCHAR const nullstr[2] = {_T('\0'), _T('\0')};
      return nullstr;
    }

    void Grow( unsigned long reqSize )
    { 
      unsigned long newSize = _size + GROWBY;
      
      if ( newSize < reqSize ) newSize = reqSize;

      SArr<TCHAR> newStr(new TCHAR[newSize]);
      
      ::memcpy(newStr, GetString(), GetStringSize() * sizeof(TCHAR));
      
      _str = newStr; // Transfer semantic !!
      
      _size = newSize;
    }

  private: // Data members
    unsigned long _size;   // Size of the whole _str buffer (in characters)
    unsigned long _length; // Size of the occupied _str buffer (in characters)
    SArr<TCHAR>   _str;    // String buffer
    unsigned long _count;  // Number of strings in the buffer
};

} // namespace Win

#endif // WINMULTISTRING_H