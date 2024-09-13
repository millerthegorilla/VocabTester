#if !defined(CYCLICREDUNDANCYCHECK_H)
#define CYCLICREDUNDANCYCHECK_H

#include <cassert>

//
// Possible keys:
// 0xA001 := x^16 + x^15 + x^2 + 1
// 0x1021
// 0xFFFF := all '1' polynomial

class Crc16
{
  public: // Typedefs
    typedef unsigned short Type;

  public: // Methods
    Crc16( Type key );

    void PutByte( unsigned char byte );
    Type Done( void );

  private: // Data members
    Type _register;
    Type _table[256];
};

// ~~~ Inlines ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
inline Crc16::Crc16( Type key )
: _register(0)
{
  assert(key != 0);
  
  // For all possible byte values
  for ( unsigned i = 0; i < 256; ++i )
  {
    Type reg = i << 8; // 24 -> 8 (16 bit crc)

    // For all bits in a byte
    for ( int j = 0; j < 8; ++j )
    {
      bool topBit = (reg & 0x8000) != 0; // 0x80000000 -> 0x8000 (16 bit crc)
      reg <<= 1;
      
      if ( topBit )
      {
        reg ^= key;
      }
    }
    
    _table [i] = reg;
  }
}

inline void Crc16::PutByte( unsigned char byte )
{
  unsigned top = _register >> 8; // 24 -> 8 (16 bit crc)
  top ^= byte;
  _register = (_register << 8) ^ _table[top];
}

inline Crc16::Type Crc16::Done( void )
{
  Type temp = _register;
  _register = 0;
  return temp;
}

#endif // CYCLICREDUNDANCYCHECK_H