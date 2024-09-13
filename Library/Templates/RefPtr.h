#ifndef REFERENCE_POINTER_H
#define REFERENCE_POINTER_H

template <class T>
class RefPtr
{
  public:
    RefPtr( RefPtr<T> const& p )
    : _p(p._p)
    { 
      _p->IncRefCount();
    }

    ~RefPtr( void )
    { 
      Release();
    }

    RefPtr<T> const& operator=( RefPtr<T> const& p )
    {
      if ( this != &p )
      {
        Release();
        _p = p._p;
        _p->IncRefCount ();
      }
      return *this;
    }
    
  protected: // Methods
    RefPtr( T* p ) 
    : _p(p)
    {}

    void Release( void )
    {
      if ( _p->DecRefCount() == 0 )
      {
        delete _p;
        _p = 0;
      }
    }

    void Clone( void )
    { 
      if ( _p->GetRefCount() > 1 )
      { // clone it
        T* rep = new T(*_p);
        Release();
        _p = rep;
      }
    }

  protected: // Data members
    T* _p;
};

class RefCounted
{
  public:
    RefCounted( void )
    : _count(1)
    {}

    int GetRefCount( void ) const { return _count; }
    void IncRefCount( void ) const { ++_count; }
    int DecRefCount( void ) const { return --_count; }
    
  private:
    mutable int _count;
};

#endif // REFERENCE_POINTER_H