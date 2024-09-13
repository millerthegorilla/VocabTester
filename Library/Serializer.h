#ifndef SERIALIZER_H
#define SERIALIZER_H

#include "Templates/SmartPtr.h"
#include "tstring.h"
#include "Crc.h"

//
// XSerializer
// ~~~~~~~~~~~
// This exception class is used by both, Serializer and DeSerializer
class XSerializer
{
  public:
    XSerializer( TCHAR const msg[] );

    TCHAR const* Message( void ) const;

  private: // Enums
    enum { MAXMSGLEN = 255 };

  private: // Data members
    TCHAR _msg[MAXMSGLEN + 1];
};


//
// SerializerBase
// ~~~~~~~~~~~~~~
class SerializerBase
{
  protected:
    enum
    {
      TruePattern  = 0xFAB1FAB2,
      FalsePattern = 0xBAD1BAD2,
    };
};


//
// Serializer
// ~~~~~~~~~~
class Serializer : public SerializerBase
{
  public:
    virtual ~Serializer( void );

    virtual unsigned long Write( void const* buf, unsigned long bufSize ) = 0;

    void WriteX( void const* buf, unsigned long bufSize, TCHAR const xMsg[] = _T("Serializer::WriteX: failed to write data") );
    
    void PutLong( long val,                        TCHAR const xMsg[] = _T("Serializer::PutLong: failed to write data") );
		void PutShort( short val,                      TCHAR const xMsg[] = _T("Serializer::PutShort: failed to write data") );
		void PutChar( char val,                        TCHAR const xMsg[] = _T("Serializer::PutChar: failed to write data") );
    void PutDouble( double val,                    TCHAR const xMsg[] = _T("Serializer::PutDouble: failed to write data") );
    void PutFloat( float val,                      TCHAR const xMsg[] = _T("Serializer::PutFloat: failed to write data") );
    void PutBool( bool val,                        TCHAR const xMsg[] = _T("Serializer::PutBool: failed to write data") );
    void PutString( TCHAR const str[], int len,    TCHAR const xMsg[] = _T("Serializer::PutString: failed to write data") );
    void PutString( TCHAR const str[],             TCHAR const xMsg[] = _T("Serializer::PutString: failed to write data") );
		void PutString( tstring const& str,            TCHAR const xMsg[] = _T("Serializer::PutString: failed to write data") );
    void PutStringA( char const str[], int len,    TCHAR const xMsg[] = _T("Serializer::PutStringA: failed to write data") );
    void PutStringA( char const str[],             TCHAR const xMsg[] = _T("Serializer::PutStringA: failed to write data") );
    void PutStringA( stdstring str,		           TCHAR const xMsg[] = _T("Serializer::PutStringA: failed to write data") );
    void PutStringW( wchar_t const str[], int len, TCHAR const xMsg[] = _T("Serializer::PutStringW: failed to write data") );
    void PutStringW( wchar_t const str[],          TCHAR const xMsg[] = _T("Serializer::PutStringW: failed to write data") );
    void PutStringW( std::wstring str,             TCHAR const xMsg[] = _T("Serializer::PutStringW: failed to write data") );
};


//
// DeSerializer
// ~~~~~~~~~~~~
class DeSerializer : public SerializerBase
{
  public:
    virtual ~DeSerializer( void );

    virtual unsigned long Read( void* buf, unsigned long bufSize ) = 0;

    void ReadX( void* buf, unsigned long bufSize, TCHAR const xMsg[] = _T("DeSerializer::ReadX: failed to read data") );
    
    long         GetLong(    TCHAR const xMsg[] = _T("DeSerializer::GetLong: failed to read data") );
		short        GetShort(   TCHAR const xMsg[] = _T("DeSerializer::GetShort: failed to read data") );
		char         GetChar(    TCHAR const xMsg[] = _T("DeSerializer::GetChar: failed to read data") );
    double       GetDouble(  TCHAR const xMsg[] = _T("DeSerializer::GetDouble: failed to read data") );
    double       GetFloat(   TCHAR const xMsg[] = _T("DeSerializer::GetFloat: failed to read data") );
    bool         GetBool(    TCHAR const xMsg[] = _T("DeSerializer::GetBool: failed to read data") );
		tstring      GetString(  TCHAR const xMsg[] = _T("DeSerializer::GetString: failed to read data") );
    std::string  GetStringA( TCHAR const xMsg[] = _T("DeSerializer::GetStringA: failed to read data") );
    std::wstring GetStringW( TCHAR const xMsg[] = _T("DeSerializer::GetStringW: failed to read data") );
};


//
// Serializable
// ~~~~~~~~~~~~
class Serializable
{
  public:
    virtual void Serialize( Serializer& out ) const = 0;
    virtual void DeSerialize( DeSerializer& in ) = 0;
};


//
// CountingSerializer
// ~~~~~~~~~~~~~~~~~~
class CountingSerializer : public Serializer
{
  public:
    CountingSerializer( void );

    virtual unsigned long Write( void const* buf, unsigned long bufSize );

  private: // Data members
    unsigned long _size;
};


//
// BufferedSerializer
// ~~~~~~~~~~~~~~~~~~
class BufferedSerializer : public Serializer
{
  public:
    BufferedSerializer( Serializer& out, unsigned long bufSize = 4096 );

    virtual unsigned long Write( void const* buf, unsigned long bufSize );
    
    void Flush( TCHAR const xMsg[] = _T("BufferedSerializer::Flush: failed to write data") );

  private: // Data mambers
    Serializer&         _out;
    unsigned long const _bufSize;
    unsigned long       _len;
    SArr<unsigned char> _buf;
};


//
// BufferedDeSerializer
// ~~~~~~~~~~~~~~~~~~~~
class BufferedDeSerializer : public DeSerializer
{
  public:
    BufferedDeSerializer( DeSerializer& in, unsigned long bufSize = 4096 );

    virtual unsigned long Read( void* buf, unsigned long bufSize );

  private: // Data mambers
    DeSerializer&       _in;
    unsigned long const _bufSize;
    unsigned long       _bufLen;
    unsigned long       _ptr;
    SArr<unsigned char> _buf;
};


//
// RC5SerizalizerBase
// ~~~~~~~~~~~~~~~~~~
class RC5SerializerBase
{
  protected: // Methods
    RC5SerializerBase( char const key[] );

    static unsigned long Rol( unsigned long val, unsigned char n );
    static unsigned long Ror( unsigned long val, unsigned char n );

  protected: // Enums
    enum { BUFSIZE = 8 };

  protected: // Data members
    unsigned char       _buf[BUFSIZE];
    int                 _w;
    int                 _r;
    SArr<unsigned long> _s;
    Crc16               _crc;
};

//
// RC5Serializer
// ~~~~~~~~~~~~~
class RC5Serializer : public RC5SerializerBase, public Serializer
{
  public:
    RC5Serializer( Serializer& out, char const key[] );

    virtual unsigned long Write( void const* buf, unsigned long bufSize );
    
    void Flush( TCHAR const xMsg[] = _T("RC5Serializer::Flush: failed to write data") );

  private: // Methods
    bool Encrypt( void );

  private: // Data members
    Serializer& _out;
    int         _bytesInBuf;
};


//
// RC5DeSerializer
// ~~~~~~~~~~~~~~~
class RC5DeSerializer : public RC5SerializerBase, public DeSerializer
{
  public:
    RC5DeSerializer( DeSerializer& in, char const key[] );

    virtual unsigned long Read( void* buf, unsigned long bufSize );

  private: // Methods
    bool Decrypt( void );

  private: // Data members
    DeSerializer& _in;
    int           _bytesRead;
};

// ~~~ Inlines ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//
// XSerializer
// ~~~~~~~~~~~
inline XSerializer::XSerializer( TCHAR const msg[] )
{
  ::_tcsncpy(_msg, msg, MAXMSGLEN);
  _msg[MAXMSGLEN] = _T('\0');
}

inline TCHAR const* XSerializer::Message( void ) const
{ 
  return _msg;
}

//
// Serializer
// ~~~~~~~~~~
inline Serializer::~Serializer( void )
{
}

inline void Serializer::WriteX( void const* buf, unsigned long bufSize, TCHAR const xMsg[] )
{
  if ( Write(buf, bufSize) != bufSize )
  {
    throw XSerializer(xMsg);
  }
}

inline void Serializer::PutLong( long val, TCHAR const xMsg[] )
{
  WriteX(&val, sizeof(val), xMsg);
}

inline void Serializer::PutShort( short val, TCHAR const xMsg[] )
{
  WriteX(&val, sizeof(val), xMsg);
}

inline void Serializer::PutChar( char val, TCHAR const xMsg[] )
{
  WriteX(&val, sizeof(val), xMsg);
}

inline void Serializer::PutDouble( double val, TCHAR const xMsg[] )
{
  WriteX(&val, sizeof(val), xMsg);
}

inline void Serializer::PutFloat( float val, TCHAR const xMsg[] )
{
  WriteX(&val, sizeof(val), xMsg);
}

inline void Serializer::PutBool( bool val, TCHAR const xMsg[] )
{
  PutLong(val ? TruePattern : FalsePattern, xMsg);
}

inline void Serializer::PutString( TCHAR const str[], int len, TCHAR const xMsg[])
{
  // The string is written WITH the terminating null character!!
  PutLong(len + 1, xMsg);
  WriteX(str, (len + 1) * sizeof(TCHAR), xMsg);
}

inline void Serializer::PutString( TCHAR const str[], TCHAR const xMsg[] )
{
  PutString(str, ::_tcslen(str), xMsg);
}

inline void Serializer::PutString( tstring const& str, TCHAR const xMsg[] )
{
  PutString(str.c_str(), str.length(), xMsg);
}

inline void Serializer::PutStringA( char const str[], int len, TCHAR const xMsg[] )
{
  // The string is written WITH the terminating null character!!
  PutLong(len + 1, xMsg);
  WriteX(str, len + 1, xMsg);
}

inline void Serializer::PutStringA( char const str[], TCHAR const xMsg[] )
{
  PutStringA(str, ::strlen(str), xMsg);
}

inline void Serializer::PutStringA( std::string str, TCHAR const xMsg[] )
{
  PutStringA(str.c_str(), str.length(), xMsg);
}

inline void Serializer::PutStringW( wchar_t const str[], int len, TCHAR const xMsg[] )
{
  // The string is written WITH the terminating null character!!
  PutLong(len + 1, xMsg);
  WriteX(str, (len + 1) * sizeof(wchar_t), xMsg);
}

inline void Serializer::PutStringW( wchar_t const str[], TCHAR const xMsg[] )
{
  PutStringW(str, ::wcslen(str), xMsg);
}

inline void Serializer::PutStringW( std::wstring str, TCHAR const xMsg[] )
{
  PutStringW(str.c_str(), str.length(), xMsg);
}


//
// DeSerializer
// ~~~~~~~~~~~~
inline DeSerializer::~DeSerializer( void )
{
}

inline void DeSerializer::ReadX( void* buf, unsigned long bufSize, TCHAR const xMsg[] )
{
  if ( Read(buf, bufSize) != bufSize )
  {
    throw XSerializer(xMsg);
  }
}
    
inline long DeSerializer::GetLong( TCHAR const xMsg[] )
{
  long val;
  ReadX(&val, sizeof(val), xMsg);
  return val;
}

inline short DeSerializer::GetShort( TCHAR const xMsg[] )
{
  short val;
  ReadX(&val, sizeof(val), xMsg);
  return val;
}

inline char DeSerializer::GetChar( TCHAR const xMsg[] )
{
  char val;
  ReadX(&val, sizeof(val), xMsg);
  return val;
}

inline double DeSerializer::GetDouble( TCHAR const xMsg[] )
{
  double val;
  ReadX(&val, sizeof(val), xMsg);
  return val;
}

inline double DeSerializer::GetFloat( TCHAR const xMsg[] )
{
  float val;
  ReadX(&val, sizeof(val), xMsg);
  return val;
}

inline bool DeSerializer::GetBool( TCHAR const xMsg[] )
{
  long val;
  ReadX(&val, sizeof(val), xMsg);

  switch ( val )
  {
    case TruePattern:  return true;
    case FalsePattern: return false;

    default: throw XSerializer(_T("DeSerializer::GetBool: corrupted data"));
  }

  return false;
}

inline tstring DeSerializer::GetString( TCHAR const xMsg[] )
{
  // The string is written WITH the terminating null character!!
  long size = GetLong(xMsg);
  tstring str;
  str.reserve(size);
  str.resize(size - 1);
  ReadX(&str[0], size * sizeof(TCHAR), xMsg);
  return str;
}

inline std::string DeSerializer::GetStringA( TCHAR const xMsg[] )
{
  // The string is written WITH the terminating null character!!
  long size = GetLong(xMsg);
  std::string str;
  str.reserve(size);
  str.resize(size - 1);
  ReadX(&str[0], size, xMsg);
  return str;
}

inline std::wstring DeSerializer::GetStringW( TCHAR const xMsg[] )
{
  // The string is written WITH the terminating null character!!
  long size = GetLong(xMsg);
  std::wstring str;
  str.reserve(size);
  str.resize(size - 1);
  ReadX(&str[0], size * sizeof(wchar_t), xMsg);
  return str;
}


//
// CountingSerializer
// ~~~~~~~~~~~~~~~~~~
inline CountingSerializer::CountingSerializer( void )
: _size(0)
{}

inline unsigned long CountingSerializer::Write( void const* buf, unsigned long bufSize )
{
  _size += bufSize;
  return bufSize;
}


//
// BufferedSerializer
// ~~~~~~~~~~~~~~~~~~~
inline BufferedSerializer::BufferedSerializer( Serializer& out, unsigned long bufSize )
: _out(out),
  _bufSize(bufSize),
  _len(0),
  _buf(new unsigned char[_bufSize])
{}

inline unsigned long BufferedSerializer::Write( void const* buf, unsigned long bufSize )
{
  if ( _len + bufSize <= _bufSize )
  {
    ::memcpy(&_buf[_len], buf, bufSize);
    _len += bufSize;

    return bufSize;
  }
  else
  {
    if ( _len > 0 )
    {
      if ( _out.Write(_buf, _len) != _len )
      {
        return 0;
      }

      _len = 0;
    }
  
    if ( bufSize <= _bufSize )
    {
      ::memcpy(&_buf[_len], buf, bufSize);
      _len += bufSize;

      return bufSize;
    }
    else
    {
      return _out.Write(buf, bufSize);
    }

  }
}

inline void BufferedSerializer::Flush( TCHAR const xMsg[] )
{
  if ( _len > 0 )
  {
    _out.WriteX(_buf, _len, xMsg);
    _len = 0;
  }
}


//
// BufferedDeSerializer
// ~~~~~~~~~~~~~~~~~~~~
inline BufferedDeSerializer::BufferedDeSerializer( DeSerializer& in, unsigned long bufSize )
: _in(in),
  _bufSize(bufSize),
  _bufLen(0), // Force _in.Read on first Read
  _ptr(0),
  _buf(new unsigned char[_bufSize])
{
}

inline unsigned long BufferedDeSerializer::Read( void* buf, unsigned long bufSize )
{
  if ( _ptr + bufSize <= _bufLen )
  {
    ::memcpy(buf, &_buf[_ptr], bufSize);
    _ptr += bufSize;

    return bufSize;
  }
  else
  {
    unsigned long avail = _bufLen - _ptr;
    ::memcpy(buf, &_buf[_ptr], avail);
    _ptr += avail;

    unsigned char* buf1 = reinterpret_cast<unsigned char*>(buf) + avail;
    bufSize -= avail;
    

    if ( bufSize <= _bufSize )
    {
      _ptr = 0;
      _bufLen = _in.Read(&_buf[_ptr], _bufSize);

      if ( _bufLen >= bufSize )
      {
        ::memcpy(buf1, &_buf[_ptr], bufSize);
        _ptr += bufSize;

        return avail + bufSize;
      }
      else
      {
        ::memcpy(buf1, &_buf[_ptr], _bufLen);
        _ptr += _bufLen;

        return avail + _bufLen;
      }
    }
    else
    {
      return avail + _in.Read(buf1, bufSize);
    }
  }
}


//
// RC5SerializerBase
// ~~~~~~~~~~~~~~~~~
inline RC5SerializerBase::RC5SerializerBase( char const key[] )
: _w(32),
  _r(36),
  _s(new unsigned long[2 * (_r + 1)]),
  _crc(0x1021)
{ 
  int b = ::strlen(key);
  int t = 2 * (_r + 1);
  int c = b / (_w / 8) + 1;
  
  SArr<unsigned long> l(new unsigned long[c]);
  ::memset(l, 0, c * sizeof(unsigned long));
  ::memcpy(l, key, b);

  { 
    unsigned long p = 0xB7E15163;
    unsigned long q = 0x9E3779B9;
    
    _s[0] = p;
    
    for ( int i = 1; i < t; ++i )
    { 
      _s[i] = _s[i - 1] + q;
    }
  }

  int nLoops = 3 * (c > t ? c : t);

  unsigned long a[2] = {0,0};

  for ( int i = 0, m = 0, n = 0; i < nLoops; ++i )
  { 
    a[0] = _s[m] = Rol(_s[m] + a[0] + a[1], 3);
    a[1] = l[n] = Rol(l[n] + a[0] + a[1], a[0] + a[1]);
    m = (m + 1) % t;
    n = (n + 1) % c;
  }
}

inline unsigned long RC5SerializerBase::Rol( unsigned long val, unsigned char n )
{
  __asm
  { 
    mov cl, n;
    rol val, cl;
  }

  return val;
}

inline unsigned long RC5SerializerBase::Ror( unsigned long val, unsigned char n )
{
  __asm
  { 
    mov cl, n;
    ror val, cl;
  }

  return val;
}


//
// RC5Serializer
inline RC5Serializer::RC5Serializer( Serializer& out, char const key[] )
: RC5SerializerBase(key),
  _out(out),
  _bytesInBuf(0)
{
}

inline unsigned long RC5Serializer::Write( void const* buffer, unsigned long bufSize )
{
  unsigned char const* buf = reinterpret_cast<unsigned char const*>(buffer);
  unsigned long bytesWritten = 0; // For return value
  
  do
  { 
    int bytesNeeded = BUFSIZE - _bytesInBuf;
  
    if ( bufSize >= bytesNeeded )
    { 
      ::memcpy(&_buf[_bytesInBuf], buf, bytesNeeded);
    
      if ( !Encrypt() )
      {
        return bytesWritten;
      }

      bytesWritten += BUFSIZE;
    
      _bytesInBuf = 0;
      buf += bytesNeeded;
      bufSize -= bytesNeeded;
    }
    else
    { 
      ::memcpy(&_buf[_bytesInBuf], buf, bufSize);

      bytesWritten += bufSize;

      _bytesInBuf += bufSize;
      bufSize = 0;
    }

  } while ( bufSize > 0 );

  return bytesWritten;
}
    
inline void RC5Serializer::Flush( TCHAR const xMsg[] )
{
  if ( _bytesInBuf > 0 )
  { 
    // Pad with zeros
    ::memset(&_buf[_bytesInBuf], 0, BUFSIZE - _bytesInBuf);
    
    if ( !Encrypt() )
    {
      throw XSerializer(xMsg);
    }
    
    _bytesInBuf = 0;
  }
}

inline bool RC5Serializer::Encrypt( void )
{
  // Compute crc16
  _crc.PutByte(_buf[0]); _crc.PutByte(_buf[1]);
  _crc.PutByte(_buf[2]); _crc.PutByte(_buf[3]);
  _crc.PutByte(_buf[4]); _crc.PutByte(_buf[5]);
  _crc.PutByte(_buf[6]); _crc.PutByte(_buf[7]);

  // Encrypt
  unsigned long* a = reinterpret_cast<unsigned long*>(_buf);
  
  a[0] += _s[0];
  a[1] += _s[1];
  
  for ( int j = 1; j <= _r; ++j )
  { 
    a[0] = Rol(a[0] ^ a[1], a[1]) + _s[2 * j];
    a[1] = Rol(a[1] ^ a[0], a[0]) + _s[2 * j + 1];
  }

  if ( _out.Write(_buf, BUFSIZE) != BUFSIZE ) return false;
  
  Crc16::Type crc = _crc.Done();
  return _out.Write(&crc, sizeof(crc)) == sizeof(crc);
}


//
// RC5DeSerializer
// ~~~~~~~~~~~~~~~
inline RC5DeSerializer::RC5DeSerializer( DeSerializer& in, char const key[] )
: RC5SerializerBase(key),
  _in(in),
  _bytesRead(BUFSIZE) // Force Decrypt on the first read
{
}

inline unsigned long RC5DeSerializer::Read( void* buffer, unsigned long bufSize )
{
  unsigned char* buf = reinterpret_cast<unsigned char*>(buffer);
  unsigned long bytesRead = 0; // For return value

  do
  { 
    int bytesAvail = BUFSIZE - _bytesRead;
  
    if ( bytesAvail == 0 )
    { 
      if ( !Decrypt() )
      {
        return bytesRead;
      }

      _bytesRead = 0;
      bytesAvail = BUFSIZE;
    }
  
    if ( bytesAvail <= bufSize )
    { 
      ::memcpy(buf, &_buf[_bytesRead], bytesAvail);
    
      buf += bytesAvail;
      _bytesRead += bytesAvail;
      bytesRead += bytesAvail;
      bufSize -= bytesAvail;
    }
    else
    { 
      ::memcpy(buf, &_buf[_bytesRead], bufSize);
    
      buf += bufSize;
      _bytesRead += bufSize;
      bytesRead += bufSize;
      bufSize = 0;
    }

  } while ( bufSize > 0 );

  return bytesRead;
}

inline bool RC5DeSerializer::Decrypt( void )
{
  Crc16::Type crc;
  if ( _in.Read(_buf, BUFSIZE) == BUFSIZE && _in.Read(&crc, sizeof(crc)) == sizeof(crc) )
  {
    // Decrypt
    unsigned long* a = reinterpret_cast<unsigned long*>(_buf);
  
    for ( int j = _r; j >= 1; --j )
    { 
      a[1] = Ror((a[1] - _s[2 * j + 1]), a[0]) ^ a[0];
      a[0] = Ror((a[0] - _s[2 * j]), a[1]) ^ a[1];
    }

    a[1] -= _s[1];
    a[0] -= _s[0];

    // Compute crc16
    _crc.PutByte(_buf[0]); _crc.PutByte(_buf[1]);
    _crc.PutByte(_buf[2]); _crc.PutByte(_buf[3]);
    _crc.PutByte(_buf[4]); _crc.PutByte(_buf[5]);
    _crc.PutByte(_buf[6]); _crc.PutByte(_buf[7]);

    if ( _crc.Done() != crc )
    {
      throw XSerializer(_T("RC5DeSerializer::Decrypt: corrupted data (CRC mismatch)"));
    }
    
    return true;
  }
  else
  {
    return false;
  }
}


#endif // SERIALIZER_H