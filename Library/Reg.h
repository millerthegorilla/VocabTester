#ifndef WINREGISTRY_H
#define WINREGISTRY_H

#include <windows.h>
#include <shlwapi.h>
#include <tchar.h>
#include "stdstring"
#include <cassert>
#include "Templates/SmartPtr.h"

namespace Win
{

class SimpleKey
{
  public:
    bool OpenedExistingKey( void ) const { return _disp == REG_OPENED_EXISTING_KEY; }
    bool CreatedNewKey( void ) const { return _disp == REG_CREATED_NEW_KEY; }

    bool DeleteSubKey( TCHAR const subKey[] )
    { assert(_hKey != 0);
      return ::SHDeleteKey(_hKey, subKey) == ERROR_SUCCESS;
    }

    bool DeleteSubKey( stdstring const& subKey )
    { return DeleteSubKey(subKey.c_str());
    }

    bool QueryValue( TCHAR const name[], DWORD* type = 0, BYTE* data = 0, DWORD* dataSize = 0 )
    { assert(_hKey != 0);
      return ::RegQueryValueEx(_hKey, const_cast<TCHAR*>(name), 0, type, data, dataSize) == ERROR_SUCCESS;
    }

    bool SetValue( TCHAR const name[], DWORD type, BYTE const* data, DWORD dataSize )
    { assert(_hKey != 0);
      return ::RegSetValueEx(_hKey, name, 0, type, data, dataSize) == ERROR_SUCCESS;
    }

    bool DeleteValue( TCHAR const name[] )
    { assert(_hKey != 0);
      return ::RegDeleteValue(_hKey, name) == ERROR_SUCCESS; 
    }

    bool DeleteValue( stdstring const& name )
    { return DeleteValue(name.c_str());
    }

    bool SetString( TCHAR const name[], TCHAR const str[], int len = -1 )
    { return SetValue(name, REG_SZ, (BYTE CONST*)str, ((len < 0 ? ::_tcslen(str) : len) + 1) * sizeof(TCHAR));
    }

    bool SetString( TCHAR const name[], stdstring const& str )
    { return SetString(name, str.c_str(), str.length());
    }

    bool SetString( stdstring const& name, stdstring const& str )
    { return SetString(name.c_str(), str.c_str(), str.length());
    }

    bool GetString( TCHAR const name[], stdstring& str )
    { DWORD size = 0;
      DWORD type = 0;
      if ( QueryValue(name, &type, 0, &size) && size > 0 && type == REG_SZ )
      { str.reserve(size / sizeof(TCHAR));
        str.resize(size / sizeof(TCHAR) - 1); // size includes terminating null
        return QueryValue(name, &type, (BYTE*)&str[0], &size);
      }
      return false;
    }

    bool GetString( stdstring const& name, stdstring& str )
    { return GetString(name.c_str(), str);
    }

    bool SetLong( TCHAR const name[], long value )
    { return SetValue(name, REG_DWORD, (BYTE CONST*)&value, sizeof(value));
    }

    bool SetLong( stdstring const& name, long value )
    { return SetLong(name.c_str(), value);
    }

    bool GetLong( TCHAR const name[], long& value )
    { DWORD type = REG_DWORD;
      DWORD size = sizeof(value);
      return QueryValue(name, &type, (BYTE*)(&value), &size);
    }

    bool GetLong( stdstring const& name, long& value )
    { return GetLong(name.c_str(), value);
    }

    unsigned GetNoValues( void ) const
    { assert(_hKey != 0);
      DWORD nValues = 0;
      ::RegQueryInfoKey(_hKey, 0, 0, 0, 0, 0, 0, &nValues, 0, 0, 0, 0);
      return nValues;
    }

    unsigned GetNoSubKeys( void ) const
    { assert(_hKey != 0);
      DWORD nSubKeys = 0;
      ::RegQueryInfoKey(_hKey, 0, 0, 0, &nSubKeys, 0, 0, 0, 0, 0, 0, 0);
      return nSubKeys;
    }

    unsigned GetMaxSubKeyLen( void ) const
    { assert(_hKey != 0);
      DWORD maxLen = 0;
      ::RegQueryInfoKey(_hKey, 0, 0, 0, 0, &maxLen, 0, 0, 0, 0, 0, 0);
      return maxLen;
    }

    unsigned GetMaxValueNameLen( void ) const
    { assert(_hKey != 0);
      DWORD maxLen = 0;
      ::RegQueryInfoKey(_hKey, 0, 0, 0, 0, 0, 0, 0, &maxLen, 0, 0, 0);
      return maxLen;
    }

    unsigned GetMaxValueLen( void ) const
    { assert(_hKey != 0);
      DWORD maxLen = 0;
      ::RegQueryInfoKey(_hKey, 0, 0, 0, 0, 0, 0, 0, 0, &maxLen, 0, 0);
      return maxLen;
    }


    operator HKEY( void ) const { return _hKey; }

  public: // Class
    class EnumSubKeys
    {
      public:
        EnumSubKeys( SimpleKey const& key )
        : _key(key),
          _cur(0),
          _bufSize(0),
          _buf(),
          _res(ERROR_NO_MORE_ITEMS)
        { 
          if ( key != 0 && key.GetNoSubKeys() > 0 )
          { 
            _bufSize = _key.GetMaxSubKeyLen() + 1;
            _buf = SArr<TCHAR>(new TCHAR[_bufSize]);
            _buf[0] = TEXT('\0');
            Advance();
          }
        }

        bool AtEnd( void ) const
        { return _res != ERROR_SUCCESS;
        }

        void Advance( void )
        { DWORD bufSize = _bufSize;
          _res = ::RegEnumKeyEx(_key, _cur, _buf, &bufSize, 0, 0, 0, 0);
          ++_cur;
        }

        TCHAR const* GetName( void ) const { return _buf; }

      private: // Data members
        SimpleKey const& _key;
        DWORD            _cur;
        DWORD            _bufSize;
        SArr<TCHAR>      _buf;
        long             _res;
    };

  public: // Class
    class EnumValues
    {
      public:
        EnumValues( SimpleKey const& key )
        : _key(key),
          _cur(0),
          _bufSize(0),
          _buf(),
          _type(REG_NONE),
          _res(ERROR_NO_MORE_ITEMS)
        { if ( key.GetNoValues() > 0 )
          { _bufSize = _key.GetMaxValueNameLen() + 1;
            _buf = SArr<TCHAR>(new TCHAR[_bufSize]);
            _buf[0] = TEXT('\0');
            Advance();
          }
        }

        bool AtEnd( void ) const
        { return _res == ERROR_NO_MORE_ITEMS;
        }

        void Advance( void )
        { DWORD bufSize = _bufSize;
          _res = ::RegEnumValue(_key, _cur, _buf, &bufSize, 0, &_type, 0, 0);
          ++_cur;
        }

        TCHAR const* GetName( void ) const { return _buf; }
        DWORD GetType( void ) const { return _type; }

      private: // Data members
        SimpleKey const& _key;
        DWORD            _cur;
        DWORD            _bufSize;
        SArr<TCHAR>      _buf;
        DWORD            _type;
        long             _res;
    };

  protected:
    SimpleKey( HKEY hKey = 0, DWORD disp = 0 )
    : _hKey(hKey),
      _disp(disp)
    {}

  protected:
    HKEY  _hKey;
    DWORD _disp;
};

class Key : public SimpleKey
{
  public:
    Key( void ) {}

    Key( Key& b )
    { b.Release(_hKey, _disp);
    }

    ~Key( void )
    { Close();
    }

    Key& operator=( Key& b )
    { if ( this != &b )
      { Close();
        b.Release(_hKey, _disp);
      }
      return *this;
    }

    void Close( void )
    { if ( _hKey != 0 )
      { ::RegCloseKey(_hKey);
      }
      _hKey = 0;
      _disp = 0;
    }

    Key OpenSubKey( TCHAR const subKey[], REGSAM sam = KEY_ALL_ACCESS )
    { HKEY hKey = 0;
      if ( ::RegOpenKeyEx(_hKey, subKey, 0, sam, &hKey) == ERROR_SUCCESS )
      { return Key(hKey, REG_OPENED_EXISTING_KEY);
      }
      return Key();
    }

    Key CreateSubKey( TCHAR const subKey[], REGSAM sam = KEY_ALL_ACCESS )
    { HKEY key = 0;
      DWORD disp = 0;
      if ( ::RegCreateKeyEx(_hKey, subKey, 0, TEXT(""), 0, sam, 0, &key, &disp) == ERROR_SUCCESS )
      { return Key(key, disp);
      }
      return Key();
    }

  private:
    Key( HKEY hKey, DWORD disp )
    : SimpleKey(hKey, disp)
    {}

    void Release( HKEY& hKey, DWORD& disp )
    { hKey = _hKey;
      disp = _disp;
      _hKey = 0;
      _disp = 0;
    }

  // Friends
  friend class PredefinedKey;
};

class PredefinedKey : public SimpleKey
{
  public:
    Key OpenSubKey( TCHAR const subKey[], REGSAM sam = KEY_ALL_ACCESS )
    { HKEY hKey = 0;
      if ( ::RegOpenKeyEx(_hKey, subKey, 0, sam, &hKey) == ERROR_SUCCESS )
      { return Key(hKey, REG_OPENED_EXISTING_KEY);
      }
      return Key();
    }

    Key CreateSubKey( TCHAR const subKey[], REGSAM sam = KEY_ALL_ACCESS )
    { HKEY key = 0;
      DWORD disp = 0;
      if ( ::RegCreateKeyEx(_hKey, subKey, 0, TEXT(""), 0, sam, 0, &key, &disp) == ERROR_SUCCESS )
      { return Key(key, disp);
      }
      return Key();
    }

  protected:
    PredefinedKey( HKEY hKey, DWORD disp )
    : SimpleKey(hKey, disp)
    {}
};

class ClassesRoot : public PredefinedKey
{
  public:
    ClassesRoot( void )
    : PredefinedKey(HKEY_CLASSES_ROOT, REG_OPENED_EXISTING_KEY)
    {}
};

class CurrentUser : public PredefinedKey
{
  public:
    CurrentUser( void )
    : PredefinedKey(HKEY_CURRENT_USER, REG_OPENED_EXISTING_KEY)
    {}
};

class LocalMachine : public PredefinedKey
{
  public:
    LocalMachine( void )
    : PredefinedKey(HKEY_LOCAL_MACHINE, REG_OPENED_EXISTING_KEY)
    {}
};

class Users : public PredefinedKey
{
  public:
    Users( void )
    : PredefinedKey(HKEY_USERS, REG_OPENED_EXISTING_KEY)
    {}
};

class DynData : public PredefinedKey
{
  public:
    DynData( void )
    : PredefinedKey(HKEY_DYN_DATA, REG_OPENED_EXISTING_KEY)
    {}
};


class PerformanceData : public PredefinedKey
{
  public:
    PerformanceData( void )
    : PredefinedKey(HKEY_PERFORMANCE_DATA, REG_OPENED_EXISTING_KEY)
    {}
};

} // namespace Win

#endif // WINREGISTRY_H