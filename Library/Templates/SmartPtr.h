#if !defined(SMARTPOINTER_H)
#define SMARTPOINTER_H

#include <cstdlib> // rand()
 
//
// SPtr
// ~~~~
template <class T>
class SPtr
{
  public:
    SPtr( void )
    : _p(0) 
    {}
    
    explicit SPtr( T* p )
    : _p(p)
    {}

    template <class U>
    SPtr( SPtr<U>& ptr )
    : _p(ptr.Release())
    {}

    SPtr( SPtr<T>& ptr )
    : _p(ptr.Release())
    {}

    ~SPtr( void )
    { 
      delete _p;
    }

    template <class U>
    SPtr<T>& operator=( SPtr<U>& ptr )
    {
      if ( _p != ptr._p )
      {
        delete _p;
        _p = ptr.Release();
      }

      return *this;
    }

    SPtr<T>& operator=( SPtr<T>& ptr )
    {
      if ( _p != ptr._p )
      {
        delete _p;
        _p = ptr.Release();
      }

      return *this;
    }

    T* Release( void )
    {
      T* temp = _p;
      _p = 0;
      return temp;
    }

    T* operator->( void ) { return _p; }
    T const* operator->( void ) const { return _p; }
    
    T& operator*( void ){ return *_p; }
    T const& operator*( void ) const { return *_p; }

    operator T*( void ) { return _p; }
    operator T const*( void ) const { return _p; }

  private:
    T* _p;
};

//
// SArr
// ~~~~
template <class T>
class SArr
{
  public:
    SArr( void )
    : _p(0) 
    {}
    
    explicit SArr( T* p )
    : _p(p)
    {}
    
    explicit SArr( unsigned long size )
    : _p(new T[size])
    {}

    SArr( SArr<T>& ptr )
    : _p(ptr.Release())
    {}

    ~SArr( void )
    { 
      delete[] _p;
    }

    SArr<T>& operator=( SArr<T>& ptr )
    {
      if ( _p != ptr._p )
      {
        delete[] _p;
        _p = ptr.Release();
      }
      
      return *this;
    }

    T* Release( void )
    {
      T* temp = _p;
      _p = 0;
      return temp;
    }

    T& operator*( void ){ return _p[0]; }
    T const& operator*( void ) const { return _p[0]; }

    operator T*( void ) { return _p; }
    operator T const*( void ) const { return _p; }

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
    void Swap( unsigned long i, unsigned long j )
    {
      T temp = _p[i];
      _p[i] = _p[j];
      _p[j] = temp;
    }

  protected: // Data members
    T* _p;
};

#endif // SMARTPOINTER_H