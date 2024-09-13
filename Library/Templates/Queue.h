#ifndef QUEUE_H
#define QUEUE_H

#include "Templates/SmartPtr.h"
#include <cassert>

//
// ~~~ Queue ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
template <typename T>
class Queue
{
  public:
    Queue( void );
   ~Queue( void );
   
    void PushBack( T const& t );
    void PushHead( T const& t );
    T PopHead( void );
    
    T& PeekHead( void );
    T const& PeekHead( void ) const;
    
    void Clear( void );

    unsigned long Size( void ) const;
    bool IsEmpty( void ) const;

  private:
    class Item
    {
      public:
        Item( T const& t )
        : _val(t),
          _next(0)
        {}

        T& Value( void ) { return _val; }
        T const& Value( void ) const { return _val; }

        Item* Next( void ) const { return _next; }
        void SetNext( Item* next ) { _next = next; }


      private:
        T     _val;
        Item* _next;
    };

  private:
    Item*         _head;
    Item*         _tail;
    unsigned long _count;
};

template <typename T>
inline Queue<T>::Queue( void )
: _head(0),
  _tail(0),
  _count(0)
{}

template <typename T>
inline Queue<T>::~Queue( void )
{ 
  Clear(); 
}

template <typename T>
void Queue<T>::PushBack( T const& t )
{
  Item* item = new Item(t);
  if ( _head == 0 )
  { 
    _head = item;
    _tail = _head;
  }
  else
  { _tail->SetNext(item);
    _tail = item;
  }
  ++_count;
}

template <typename T>
void Queue<T>::PushHead( T const& t )
{
  Item* item = new Item(t);
  if ( _head == 0 )
  { 
    _head = item;
    _tail = _head;
  }
  else
  { 
    item->SetNext(_head);
    _head = item;
  }
  ++_count;
}


template <typename T>
T Queue<T>::PopHead( void )
{
  assert(_head != 0);
  T val = _head->Value();
  SPtr<Item> item(_head); // Destructor will delete it
  if ( _head == _tail )
  { 
    _head = _head->Next();
    _tail = _head;
  }
  else
  { 
    _head = _head->Next();
  }
  --_count;
  return val;
}

template <typename T>
inline T& Queue<T>::PeekHead( void )
{ 
  return _head->Value();
}

template <typename T>
inline T const& Queue<T>::PeekHead( void ) const 
{ 
  return _head->Value();
}

template <typename T>
void Queue<T>::Clear( void )
{
  Item* item = _head;
  while ( item != 0 )
  { 
    Item* j = item->Next();
    delete item;
    item = j;
  }
  _head = _tail = 0;
  _count = 0;
}

template <typename T>
inline unsigned long Queue<T>::Size( void ) const 
{ 
  return _count;
}

template <typename T>
inline bool Queue<T>::IsEmpty( void ) const 
{ 
  return _head == 0;
}

//
// ~~~ SPtrQueue ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
template <typename T>
class SPtrQueue
{
  public:
    SPtrQueue( void );
   ~SPtrQueue( void );
   
    void PushBack( SPtr<T>& t );
    void PushHead( SPtr<T>& t );
    SPtr<T> PopHead( void );
    
    T* PeekHead( void );
    T const* PeekHead( void ) const;
    
    void Clear( void );

    unsigned long Size( void ) const;
    bool IsEmpty( void ) const;

  private:
    class Item
    {
      public:
        Item( SPtr<T>& t )
        : _ptr(t),
          _next(0)
        {}

        T* Ptr( void ) { return _ptr; }
        T const* Ptr( void ) const { return _ptr; }

        Item* Next( void ) const { return _next; }
        void SetNext( Item* next ) { _next = next; }
        
        SPtr<T> Release( void ) { return _ptr; }

      private:
        SPtr<T> _ptr;
        Item*   _next;
    };

  private:
    Item*         _head;
    Item*         _tail;
    unsigned long _count;
};

template <typename T>
inline SPtrQueue<T>::SPtrQueue( void )
: _head(0),
  _tail(0),
  _count(0)
{
}

template <typename T>
inline SPtrQueue<T>::~SPtrQueue( void )
{ 
  Clear(); 
}

template <typename T>
void SPtrQueue<T>::PushBack( SPtr<T>& t )
{
  Item* item = new Item(t);
  if ( _head == 0 )
  { 
    _head = item;
    _tail = _head;
  }
  else
  { 
    _tail->SetNext(item);
    _tail = item;
  }
  ++_count;
}

template <typename T>
void SPtrQueue<T>::PushHead( SPtr<T>& t )
{
  Item* item = new Item(t);
  if ( _head == 0 )
  { 
    _head = item;
    _tail = _head;
  }
  else
  { 
    item->SetNext(_head);
    _head = item;
  }
  ++_count;
}


template <typename T>
SPtr<T> SPtrQueue<T>::PopHead( void )
{
  assert(_head != 0);
  SPtr<T> ptr = _head->Release();
  SPtr<Item> item(_head); // Destructor will delete it
  if ( _head == _tail )
  { 
    _head = _head->Next();
    _tail = _head;
  }
  else
  { 
    _head = _head->Next();
  }
  --_count;
  return ptr;
}

template <typename T>
inline T* SPtrQueue<T>::PeekHead( void )
{ 
  return _head->Ptr();
}

template <typename T>
inline T const* SPtrQueue<T>::PeekHead( void ) const 
{ 
  return _head->Ptr();
}

template <typename T>
void SPtrQueue<T>::Clear( void )
{
  Item* item = _head;
  while ( item != 0 )
  { 
    Item* j = item->Next();
    delete item;
    item = j;
  }
  _head = _tail = 0;
  _count = 0;
}

template <typename T>
inline unsigned long SPtrQueue<T>::Size( void ) const 
{ 
  return _count;
}

template <typename T>
inline bool SPtrQueue<T>::IsEmpty( void ) const 
{ 
  return _head == 0;
}

#endif // QUEUE_H