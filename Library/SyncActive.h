#ifndef SYNCACTIVEOBJECT_H
#define SYNCACTIVEOBJECT_H

#include "Templates/SmartPtr.h"
#include "SyncQueue.h"
#include <tchar.h>

namespace Win
{

class SyncActiveObject : public Win::ActiveObject
{
  public:
    SyncActiveObject( void )
    : _abort(false)
    {}

    virtual ~SyncActiveObject( void )
    {}

  protected: // Methods
    virtual void InitThread( void ) {}
    
    virtual void Run( void )
    { for ( ;; )
      { try
        { _event.Wait();
          if ( _isDying ) return;

          _abort = false;
          while ( !_queue.IsEmpty() && !_abort )
          { SPtr<SimpleCommand> cmd = _queue.PopHead();
            cmd->Perform();
          }
        }
        catch ( Win::Exception& e )
        { ::MessageBox(0, e.Message(), _T("SyncActiveObject::Run: Windows exception"), MB_ICONEXCLAMATION|MB_OK);
        }
        catch ( ... )
        { ::MessageBox(0, _T("Unknown exception has been caught in SyncActiveObject::Run."), _T("SyncActiveObject::Run: unknown exception"), MB_ICONEXCLAMATION|MB_OK);
        }
      }
    }

    virtual void FlushThread( void )
    { _abort = true;
      _event.Set();
    }

  protected: // Class
    class SimpleCommand
    {
      public:
        virtual void Perform( void ) = 0;
    };

  protected: // Methods
    void PostCommand( SPtr<SimpleCommand>& cmd )
    { _queue.PushBack(cmd);
      _event.Set();
    }

  protected: // Data members
    Win::Event                            _event;
    Win::SyncQueue< SPtr<SimpleCommand> > _queue;
    volatile bool                         _abort;
};

} // namespace Win

#endif // SYNCACTIVEOBJECT_H