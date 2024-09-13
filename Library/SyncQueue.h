#ifndef WINSYNCQUEUE_H
#define WINSYNCQUEUE_H

#include "Templates/Queue.h"
#include "Active.h"

namespace Win
{

template <class T>
class SyncQueue
{
  public:
    void PushBack( T& t )
    { Win::Lock lock(_mutex);
      _queue.PushBack(t);
    }

    T PopHead( void )
    { Win::Lock lock(_mutex);
      return _queue.PopHead();
    }

    bool IsEmpty( void )
    { Win::Lock lock(_mutex);
      return _queue.IsEmpty();
    }

    void Empty( void )
    { Win::Lock lock(_mutex);
      _queue.Empty();
    }

    int GetCount( void )
    { Win::Lock lock(_mutex);
      return _queue.GetCount();
    }

  private:
    Win::Mutex _mutex;
    Queue<T>   _queue;
};


template <class T>
class BlockingQueue
{
  public:
    T PopHead( void )
    { if ( _queue.IsEmpty() )
      { _push.Wait();
      }
      else
      { _push.Reset();
      }
      return _queue.PopHead();
    }

    void PushBack( T t )
    { _queue.PushBack(t);
      _push.Set();
    }

    void Empty( void )
    { _queue.Empty();
    }

    int GetCount( void )
    { return _queue.GetCount();
    }

  private:
    Win::Event   _push;
    SyncQueue<T> _queue;
};

} // namespace Win

#endif // WINSYNCQUEUE_H