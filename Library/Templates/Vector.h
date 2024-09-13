#ifndef VECTOR_H
#define VECTOR_H

#include <cassert>

template <typename T>
class Vector
{
  public:
    Vector( void );
    explicit Vector( unsigned long size );
    Vector( unsigned long size, T const& init );
    
    ~Vector( void );

    Vector( Vector<T> const& b );
    Vector<T>& operator=( Vector<T> const& b );

    unsigned long PushBack( T const& t );
    void Insert( unsigned long pos, T const& t );
    T Remove( unsigned long pos );
    void Clear( void );

    T const& operator[]( unsigned long i ) const;
    T& operator[]( unsigned long i );

    unsigned long Size( void ) const;
    
    template <class Pr>
    void Sort( Pr pr )
    {
      if ( _size == 0 )
        return;
        
      Sort(pr, 0, _size - 1);
    }
    
    template <class Pr>
    void Sort( Pr pr, unsigned long from, unsigned long to )
    { 
      if ( from >= to )
        return;
        
      assert(to < _size);
      
      unsigned long i = ::rand() % (to - from + 1) + from; // a random integer between from and to
      
      Swap(from, i);
      unsigned long mid = from;
      for ( i = from + 1; i <= to; ++i )
      {  
        if ( pr(_arr[i], _arr[from]) )
        {  
          ++mid;
          Swap(mid, i);
        }
      }
      
      if ( mid > from )
      {
        Swap(from, mid);
       	Sort(pr, from, mid - 1);
      }
      
      Sort(pr, mid + 1, to);
    }

  private: // Methods
    void Grow( void );
    void Swap( unsigned long i, unsigned long j );

  private: // Data members
    T*            _arr;
    unsigned long _size;  // Number of elements in the vector
    unsigned long _cap;   // Capacity
};

template <typename T>
Vector<T>::Vector( void )
: _arr(0),
  _size(0),
  _cap(0)
{}

template <typename T>
Vector<T>::Vector( unsigned long size )
: _arr(new T[size]),
  _size(size),
  _cap(size)
{}

template <typename T>
Vector<T>::Vector( unsigned long size, T const& init )
: _arr(new T[size]),
  _size(size),
  _cap(size)
{
  for ( unsigned long i = 0; i < _size; ++i ) _arr[i] = init;
}

template <typename T>
Vector<T>::~Vector( void )
{
  Clear();
}

template <typename T>
Vector<T>::Vector( Vector<T> const& b )
: _arr(0),
  _size(b._size),
  _cap(b._cap)
{
  if ( _size > 0 )
  {
    _arr = new T[_cap];
    for ( unsigned long i = 0; i < _size; ++i ) _arr[i] = b._arr[i];
  }
}

template <typename T>
Vector<T>& Vector<T>::operator=( Vector<T> const& b )
{
  if ( this != &b )
  {
    delete[] _arr;
    _arr = 0;
    _size = b._size;
    _cap = b._cap;

    if ( _size > 0 )
    {
      _arr = new T[_cap];
      for ( unsigned long i = 0; i < _size; ++i ) _arr[i] = b._arr[i];
    }
  }
  return *this;
}

template <typename T>
unsigned long Vector<T>::PushBack( T const& t )
{
  if ( _size >= _cap ) Grow();
  _arr[_size] = t;
  return _size++;
}

template <typename T>
void Vector<T>::Insert( unsigned long pos, T const& t )
{
  assert(pos <= _size);

  if ( pos == _size )
  {
    PushBack(t);
  }
  else
  {
    if ( _size >= _cap ) Grow();
    for ( unsigned long i = _size - 1; i > pos; --i ) _arr[i + 1] = _arr[i];
    _arr[pos + 1] = _arr[pos];
    _arr[pos] = t;
    ++_size;
  }
}

template <typename T>
T Vector<T>::Remove( unsigned long pos )
{
  assert(pos < _size);
  T temp = _arr[pos];
  for ( unsigned i = pos; i < _size - 1; ++i ) _arr[i] = _arr[i + 1];
  --_size;
  return temp;
}

template <typename T>
void Vector<T>::Clear( void )
{
  delete[] _arr;
  _arr = 0;
  _size = 0;
  _cap = 0;
}

template <typename T>
inline T const& Vector<T>::operator[]( unsigned long i ) const
{
  assert(i < _size);
  return _arr[i];
}

template <typename T>
inline T& Vector<T>::operator[]( unsigned long i )
{
  assert(i < _size);
  return _arr[i];
}

template <typename T>
inline unsigned long Vector<T>::Size( void ) const
{
  return _size;
}

template <typename T>
void Vector<T>::Grow( void )
{
  unsigned long newCap = (4 * _cap + _cap) / 4; // 5/4 * _cap
  if ( newCap == 0 ) newCap = 4;

  T* newArr = new T[newCap];
  for ( unsigned long i = 0; i < _size; ++i ) newArr[i] = _arr[i];
  delete[] _arr;
  _arr = newArr;
  _cap = newCap;
}

template <typename T>
inline void Vector<T>::Swap( unsigned long i, unsigned long j )
{
  T temp = _arr[i];
  _arr[i] = _arr[j];
  _arr[j] = temp;
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ~~~ Class StrongVector
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
template <typename Strong, typename Weak, typename ConstWeak = const Weak>
class StrongVector
{
  public:
    StrongVector( void );
    explicit StrongVector( unsigned long size );
    ~StrongVector( void );

    unsigned long PushBack( Strong& t );
    void Insert( unsigned long pos, Strong& t );
    Strong Remove( unsigned long pos );
    Strong Replace( unsigned long pos, Strong& t );
    void Clear( void );

    ConstWeak operator[]( unsigned long i ) const;
    Weak operator[]( unsigned long i );

    unsigned long Size( void ) const;

  private: // Methods
    // No copy, please
    StrongVector( StrongVector<Strong, Weak, ConstWeak> const& b );
    StrongVector<Strong, Weak, ConstWeak>& operator=( StrongVector<Strong, Weak, ConstWeak> const& b );

    void Grow( void );

  private: // Data members
    Strong*       _arr;
    unsigned long _size;  // Number of elements in the vector
    unsigned long _cap;   // Capacity
};

template <typename Strong, typename Weak, typename ConstWeak>
StrongVector<Strong, Weak, ConstWeak>::StrongVector( void )
: _arr(0),
  _size(0),
  _cap(0)
{}

template <typename Strong, typename Weak, typename ConstWeak>
StrongVector<Strong, Weak, ConstWeak>::StrongVector( unsigned long size )
: _arr(new Strong[size]),
  _size(size),
  _cap(size)
{}

template <typename Strong, typename Weak, typename ConstWeak>
StrongVector<Strong, Weak, ConstWeak>::~StrongVector( void )
{
  Clear();
}

template <typename Strong, typename Weak, typename ConstWeak>
unsigned long StrongVector<Strong, Weak, ConstWeak>::PushBack( Strong& t )
{
  if ( _size >= _cap ) Grow();
  _arr[_size] = t;
  return _size++;
}

template <typename Strong, typename Weak, typename ConstWeak>
void StrongVector<Strong, Weak, ConstWeak>::Insert( unsigned long pos, Strong& t )
{
  assert(pos <= _size);

  if ( pos == _size )
  {
    PushBack(t);
  }
  else
  {
    if ( _size >= _cap ) Grow();
    for ( unsigned long i = _size - 1; i > pos; --i ) _arr[i + 1] = _arr[i];
    _arr[pos + 1] = _arr[pos];
    _arr[pos] = t;
    ++_size;
  }
}

template <typename Strong, typename Weak, typename ConstWeak>
Strong StrongVector<Strong, Weak, ConstWeak>::Remove( unsigned long pos )
{
  assert(pos < _size);
  Strong temp = _arr[pos];
  for ( unsigned i = pos; i < _size - 1; ++i ) _arr[i] = _arr[i + 1];
  --_size;
  return temp;
}

template <typename Strong, typename Weak, typename ConstWeak>
Strong StrongVector<Strong, Weak, ConstWeak>::Replace( unsigned long pos, Strong& t )
{
  assert(pos < _size);
  Strong temp = _arr[pos];
  _arr[pos] = t;
  return temp;
}

template <typename Strong, typename Weak, typename ConstWeak>
void StrongVector<Strong, Weak, ConstWeak>::Clear( void )
{
  delete[] _arr;
  _arr = 0;
  _size = 0;
  _cap = 0;
}

template <typename Strong, typename Weak, typename ConstWeak>
inline ConstWeak StrongVector<Strong, Weak, ConstWeak>::operator[]( unsigned long i ) const
{
  assert(i < _size);
  return _arr[i];
}

template <typename Strong, typename Weak, typename ConstWeak>
inline Weak StrongVector<Strong, Weak, ConstWeak>::operator[]( unsigned long i )
{
  assert(i < _size);
  return _arr[i];
}

template <typename Strong, typename Weak, typename ConstWeak>
inline unsigned long StrongVector<Strong, Weak, ConstWeak>::Size( void ) const
{
  return _size;
}

template <typename Strong, typename Weak, typename ConstWeak>
void StrongVector<Strong, Weak, ConstWeak>::Grow( void )
{
  unsigned long newCap = (4 * _cap + _cap) / 4; // 5/4 * _cap
  if ( newCap == 0 ) newCap = 4;

  Strong* newArr = new Strong[newCap];
  for ( unsigned long i = 0; i < _size; ++i ) newArr[i] = _arr[i];
  delete[] _arr;
  _arr = newArr;
  _cap = newCap;
}

#endif // VECTOR_H