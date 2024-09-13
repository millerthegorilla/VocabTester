#ifndef WINACTIVE_H
#define WINACTIVE_H

#include "XWin.h"

namespace Win
{

class Thread
{
  public:
    Thread( DWORD (WINAPI *pFun)(void* arg), void* pArg )
    : _handle(::CreateThread(0, 0, pFun, pArg, CREATE_SUSPENDED, &_tid))
    {}
   
    ~Thread( void )
    { if ( _handle != 0 )
      { ::CloseHandle(_handle);
      }
    }

    void Resume( void )
    { ::ResumeThread(_handle); 
    }

    void Suspend( void )
    { ::SuspendThread(_handle); 
    }
    
    bool WaitForDeath( DWORD timeOut )
    { return ::WaitForSingleObject(_handle, timeOut) == WAIT_OBJECT_0; 
    }
    
    operator HANDLE( void ) const
    { return _handle;
    }

  private:
    HANDLE  _handle;
    DWORD   _tid;
};


class ActiveObject
{
  public:
    ActiveObject( void )
    : _isDying(0),
#pragma warning(disable: 4355) // 'this' used before initialized
      _thread(ThreadEntry, this)
#pragma warning(default: 4355)
    {}
    
    virtual ~ActiveObject( void )
    {}

    void Resume( void )
    { _thread.Resume();
    }
    
    bool Kill( DWORD timeOut = 2000 )
    { ++_isDying;
      FlushThread();
      return _thread.WaitForDeath(timeOut);
    }

    HANDLE GetThreadHandle( void )
    { return _thread; 
    }

  protected: // Methods
    virtual void InitThread( void ) = 0;
    virtual void Run( void ) = 0;
    virtual void FlushThread( void ) = 0;

    static DWORD WINAPI ThreadEntry( void* pArg )
    { ActiveObject* pActive = reinterpret_cast<ActiveObject*>(pArg);
      pActive->InitThread();
      pActive->Run();
      return 0;
    }

  protected: // Data members
    int    _isDying;
    Thread _thread;
};

class Mutex
{
  public:
    Mutex( void )
    { ::InitializeCriticalSection(&_critSection); 
    }

    ~Mutex( void )
    { ::DeleteCriticalSection(&_critSection); 
    }

  private: // Methods
    void Acquire( void )
    { ::EnterCriticalSection(&_critSection); 
    }
 
    void Release( void )
    { ::LeaveCriticalSection(&_critSection);
    }

  private: // Data members
    CRITICAL_SECTION _critSection;

  // Friends
  friend class Lock;
  friend class TransferLock;
};


// You have to put Locks inside all the methods
// of your ActiveObject that access data shared
// with the captive thread.
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
class Lock
{
  public:
    Lock( Mutex& mutex )
    : _mutex(mutex)
    { _mutex.Acquire(); 
    }
    
    ~Lock( void )
    { _mutex.Release(); 
    }
 
  private:
    Mutex& _mutex;
};

class TransferLock
{
  public:
    TransferLock( void )
    : _mutex(0)
    {}

    TransferLock( Mutex& mutex )
    : _mutex(&mutex)
    { _mutex->Acquire();
    }

    TransferLock( TransferLock& lock )
    : _mutex(lock.Release())
    {}

    ~TransferLock( void )
    { if ( _mutex != 0 )
      { _mutex->Release();
      }
    }

    void operator=( TransferLock& lock )
    { if ( this != &lock )
      { if ( _mutex != 0 )
        { _mutex->Release();
        }
        _mutex = lock.Release();
      }
    }

    Mutex* Release( void )
    { Mutex* temp = _mutex;
      _mutex = 0;
      return temp;
    }

  private:
    Mutex* _mutex;
};

class InterProcessMutex
{
  public:
    InterProcessMutex( TCHAR const name[] )
    : _h(::CreateMutex(0, FALSE, name))
    {
    }

    ~InterProcessMutex( void )
    { if ( _h != 0 )
      { ::CloseHandle(_h);
      }
    }

  private: // Methods
    void Acquire( void )
    { ::WaitForSingleObject(_h, INFINITE);
    }

    void Release( void )
    { ::ReleaseMutex(_h);
    }

  private: // Data members
    HANDLE _h;

  // Friends
  friend class InterProcessLock;
};

class InterProcessLock
{
  public:
    InterProcessLock( InterProcessMutex& mutex )
    : _mutex(mutex)
    { _mutex.Acquire();
    }

    ~InterProcessLock( void )
    { _mutex.Release();
    }

  private:
    InterProcessMutex& _mutex;
};

class Event
{
  public: // Class
    class Setter
    {
      public:
        Setter( Event& event )
        : _event(event)
        {}

        ~Setter( void )
        { _event.Set(); 
        }

      private:
        Event& _event;
    };

  public: // Methods
    Event( BOOL manualReset = FALSE, BOOL initialState = FALSE )
    // Start in non-signaled state (red light) - by default
    // Auto reset after every wait - by default
    : _handle(::CreateEvent(0, manualReset, initialState, 0))
    {}
    
    ~Event( void )
    { ::CloseHandle(_handle); 
    }

    // Put into signaled state
    void Set( void )
    { ::SetEvent(_handle); 
    }
    
    // Put into non-signaled state
    void Reset( void )
    { ::ResetEvent(_handle);
    }

    bool Wait( DWORD ms = INFINITE )
    { // Wait until event is in signaled state
      return ::WaitForSingleObject(_handle, ms) == WAIT_OBJECT_0;
    }
    
    operator HANDLE( void ) const
    { return _handle; 
    }

  private:
    HANDLE _handle;
};

} // namespace Win

#endif // WINACTIVE_H