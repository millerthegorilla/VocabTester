#ifndef WINFOLDERWATCHER_H
#define WINFOLDERWATCHER_H

#include "Templates/SmartPtr.h"
#include "Active.h"

namespace Win
{

class FolderWatcher
{
  public: // Class
    class NotifySink
    {
      public:
        virtual void FolderChanged( int id ) = 0;
    };

  public: // Class
    class Lock
    {
      public:
        Lock( FolderWatcher& w )
        :_w(w)
        { _w.lock(); }

        ~Lock( void )
        { _w.unlock(); }

      private:
        FolderWatcher& _w;
    };

  public:
    FolderWatcher( int id, NotifySink* sink )
    : _lock(0),
      _id(id),
      _sink(sink),
      _hwnd(0),
      _msg(0)
    { ::ZeroMemory(&_hdr, sizeof(_hdr));
    }

    FolderWatcher( int id, HWND hwnd, UINT msg )
    : _lock(0),
      _id(id),
      _sink(0),
      _hwnd(hwnd),
      _msg(msg)
    { ::ZeroMemory(&_hdr, sizeof(_hdr));
    }

    FolderWatcher( HWND hwnd, NMHDR const& hdr )
    : _lock(0),
      _id(0),
      _sink(0),
      _hwnd(hwnd),
      _msg(WM_COMMAND)
    { _hdr = hdr;
    }

    void WatchFolder( TCHAR const folder[], bool subFolders = false, unsigned long flags = FILE_NOTIFY_CHANGE_FILE_NAME|FILE_NOTIFY_CHANGE_DIR_NAME )
    { _watcher = SPtr<Watcher>();
      if ( folder != 0 )
      { _watcher = SPtr<Watcher>( new Watcher(folder,subFolders,flags,this) );
      }
    }

  private: // Methods
    void lock( void ) { ++_lock; }
    void unlock( void ) { --_lock; }
    
    void FolderChanged( void );

  private: // Class
    class Watcher : public Win::ActiveObject
    {
      public:
        Watcher( TCHAR const folder[], bool subFolders, unsigned long flags, FolderWatcher* sink )
        : _handle(::FindFirstChangeNotification(folder, subFolders, flags)),
          _sink(sink)
        { _thread.Resume();
        }

        ~Watcher( void )
        { Kill(INFINITE);
          if ( _handle != INVALID_HANDLE_VALUE )
          { ::FindCloseChangeNotification(_handle);
          }
        }

      protected:
        virtual void InitThread( void ) {};
        virtual void Run( void );
        virtual void FlushThread( void ) 
        { _event.Set(); 
        }

      private: // Data members
        Win::Event     _event;
        HANDLE         _handle;
        FolderWatcher* _sink;
    };

  private: // Data members
    int           _lock;
    int           _id;
    NotifySink*   _sink;
    HWND          _hwnd;
    UINT          _msg;
    NMHDR         _hdr;
    SPtr<Watcher> _watcher;

  friend class Lock;
  friend class Watcher;
};

} // namespace Win

#endif // WINFOLDERWATCHER_H